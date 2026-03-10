/*
  ==============================================================================
    VisageJuceHost.h
    Bridge for JUCE 8 + Visage (Fixed Rendering Pipeline)
  ==============================================================================
*/
#pragma once
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include "visage/app.h"
#include "visage/ui.h"
#include "visage/graphics.h"

// Crash Handler
static void npsCrashHandler(void*) {
    auto logFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                   .getChildFile("APC_CRASH_REPORT.txt");
    juce::String report = "TIME: " + juce::Time::getCurrentTime().toString(true, true) + "\n";
    report += juce::SystemStats::getStackBacktrace();
    logFile.replaceWithText(report);
}

/**
 * VisagePluginEditor - A JUCE AudioProcessorEditor that hosts Visage UI
 * 
 * This class properly integrates Visage's rendering pipeline with JUCE's OpenGL context.
 * 
 * Key concepts:
 * 1. Visage uses a Frame hierarchy where each Frame has a Region
 * 2. The Canvas manages rendering and needs regions added to it
 * 3. Frames must be initialized and have their event handlers set up
 * 4. The redraw() mechanism triggers actual drawing via drawToRegion()
 */
class VisagePluginEditor : public juce::AudioProcessorEditor,
                           private juce::Timer
{
public:
    VisagePluginEditor(juce::AudioProcessor& p) : AudioProcessorEditor(&p) {
        static bool crashHandlerSet = false;
        if (!crashHandlerSet) {
            juce::SystemStats::setApplicationCrashHandler(npsCrashHandler);
            crashHandlerSet = true;
        }
        
        setOpaque(true);
        startTimerHz(60);
    }

    ~VisagePluginEditor() override {
        stopTimer();
        teardownVisage();
    }

    void paint(juce::Graphics& g) override { 
        g.fillAll(juce::Colours::darkblue);

        if (windowless_ && backbuffer_.isValid()) {
            g.drawImageWithin(backbuffer_, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
        }
    }

    void mouseDown(const juce::MouseEvent& e) override {
        dispatchMouse(e, MouseDispatch::Down);
    }

    void mouseDrag(const juce::MouseEvent& e) override {
        dispatchMouse(e, MouseDispatch::Drag);
    }

    void mouseUp(const juce::MouseEvent& e) override {
        dispatchMouse(e, MouseDispatch::Up);
    }

    void mouseMove(const juce::MouseEvent& e) override {
        dispatchMouse(e, MouseDispatch::Move);
    }

    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override {
        dispatchWheel(wheel, e);
    }

    void resized() override { 
        onResize(getWidth(), getHeight()); 
        if (canvas_) {
            const float scale = (float)getApproximateScaleFactorForComponent(this);
            const int physicalW = juce::roundToInt(getWidth() * scale);
            const int physicalH = juce::roundToInt(getHeight() * scale);

            if (windowless_) {
                canvas_->setWindowless(physicalW, physicalH);
            } else {
                canvas_->setDimensions(physicalW, physicalH);
            }
        }
    }

    void timerCallback() override {
        if (!rendererInitialized_) {
            tryInitialize();
            return;
        }

        if (!canvas_)
            return;

        onRender();
        drawStaleFrames();
        canvas_->submit();

        if (windowless_) {
            updateBackbufferFromScreenshot(canvas_->takeScreenshot());
            repaint();
        }
    }

    // Override these in your subclass
    virtual void onInit() {}
    virtual void onRender() {}
    virtual void onDestroy() {}
    virtual void onResize(int w, int h) {}

protected:
    visage::Canvas& getCanvas() { return *canvas_; }
    visage::FrameEventHandler& getEventHandler() { return eventHandler_; }
    void setEventRoot(visage::Frame* root) { event_root_ = root; }
    
    /**
     * Add a frame to the canvas for rendering.
     * This sets up the frame's region and event handler.
     */
    void addFrameToCanvas(visage::Frame* frame) {
        if (!canvas_ || !frame)
            return;
            
        // Add the frame's region to the canvas
        canvas_->addRegion(frame->region());
        
        // Set up the event handler so redraw() works
        frame->setEventHandler(&eventHandler_);
        
        // Set DPI scale
        float scale = (float)getApproximateScaleFactorForComponent(this);
        frame->setDpiScale(scale);
        
        // Initialize the frame
        frame->init();
        
        // Trigger initial redraw
        frame->redrawAll();
    }
    
    /**
     * Remove a frame from the canvas.
     */
    void removeFrameFromCanvas(visage::Frame* frame) {
        if (!frame)
            return;
            
        // Clear event handler
        frame->setEventHandler(nullptr);
        
        // Remove from stale list
        auto pos = std::find(staleFrames_.begin(), staleFrames_.end(), frame);
        if (pos != staleFrames_.end())
            staleFrames_.erase(pos);
    }
    
    /**
     * Draw all frames that need redrawing.
     * This is called automatically in renderOpenGL().
     */
    void drawStaleFrames() {
        if (!canvas_)
            return;
            
        // Swap stale list to avoid issues if redraw() is called during draw
        std::vector<visage::Frame*> drawing;
        std::swap(staleFrames_, drawing);
        
        for (visage::Frame* frame : drawing) {
            if (frame && frame->isDrawing())
                frame->drawToRegion(*canvas_);
        }
        
        // Handle any frames that were added during drawing
        for (auto it = staleFrames_.begin(); it != staleFrames_.end();) {
            visage::Frame* frame = *it;
            if (std::find(drawing.begin(), drawing.end(), frame) == drawing.end()) {
                if (frame && frame->isDrawing())
                    frame->drawToRegion(*canvas_);
                it = staleFrames_.erase(it);
            } else {
                ++it;
            }
        }
    }

private:
    enum class MouseDispatch {
        Down,
        Drag,
        Up,
        Move
    };

    void dispatchMouse(const juce::MouseEvent& e, MouseDispatch type) {
        if (!event_root_)
            return;

        // window_position = cursor position relative to this component (top-left = 0,0)
        // This matches how visage's WindowEventHandler sets window_position from the native cursor x,y
        const visage::Point window_pos = { static_cast<float>(e.position.x), static_cast<float>(e.position.y) };
        
        visage::MouseEvent me;
        me.window_position = window_pos;
        me.relative_position = window_pos - last_mouse_window_pos_;
        last_mouse_window_pos_ = window_pos;

        int mods = visage::kModifierNone;
        if (e.mods.isShiftDown()) mods |= visage::kModifierShift;
        if (e.mods.isCtrlDown())  mods |= visage::kModifierRegCtrl;
        if (e.mods.isAltDown())   mods |= visage::kModifierAlt;
        if (e.mods.isCommandDown()) mods |= visage::kModifierCmd;
        me.modifiers = mods;

        int buttons = visage::kMouseButtonNone;
        if (e.mods.isLeftButtonDown())   buttons |= visage::kMouseButtonLeft;
        if (e.mods.isMiddleButtonDown()) buttons |= visage::kMouseButtonMiddle;
        if (e.mods.isRightButtonDown())  buttons |= visage::kMouseButtonRight;
        me.button_state = buttons;

        if (type == MouseDispatch::Down) {
            last_button_id_ = e.mods.isLeftButtonDown() ? visage::kMouseButtonLeft :
                              e.mods.isRightButtonDown() ? visage::kMouseButtonRight :
                              e.mods.isMiddleButtonDown() ? visage::kMouseButtonMiddle :
                              visage::kMouseButtonLeft;
            
            captured_frame_ = event_root_->frameAtPoint(window_pos);
            me.button_id = last_button_id_;
            me.is_down = true;

            if (captured_frame_) {
                me.event_frame = captured_frame_;
                me.position = window_pos - captured_frame_->positionInWindow();
                captured_frame_->processMouseDown(me);
            }
            return;
        }

        if (type == MouseDispatch::Up) {
            me.button_id = last_button_id_;
            me.is_down = false;

            visage::Frame* up_target = captured_frame_ ? captured_frame_ : event_root_->frameAtPoint(window_pos);
            visage::Frame* new_hover = event_root_->frameAtPoint(window_pos);
            bool exited = new_hover != captured_frame_;

            if (captured_frame_) {
                me.event_frame = captured_frame_;
                me.position = window_pos - captured_frame_->positionInWindow();
                auto* was_captured = captured_frame_;
                captured_frame_ = nullptr;
                was_captured->processMouseUp(me);
                if (exited)
                    was_captured->processMouseExit(me);
            }

            hovered_frame_ = new_hover;
            if (exited && hovered_frame_) {
                me.event_frame = hovered_frame_;
                me.position = window_pos - hovered_frame_->positionInWindow();
                hovered_frame_->processMouseEnter(me);
            }
            return;
        }

        // Move or Drag
        if (type == MouseDispatch::Drag && captured_frame_) {
            me.button_id = last_button_id_;
            me.is_down = true;
            me.event_frame = captured_frame_;
            me.position = window_pos - captured_frame_->positionInWindow();
            captured_frame_->processMouseDrag(me);
            return;
        }

        // Mouse move - update hover
        visage::Frame* new_hover = event_root_->frameAtPoint(window_pos);
        if (new_hover != hovered_frame_) {
            if (hovered_frame_) {
                me.event_frame = hovered_frame_;
                me.position = window_pos - hovered_frame_->positionInWindow();
                hovered_frame_->processMouseExit(me);
            }
            if (new_hover) {
                me.event_frame = new_hover;
                me.position = window_pos - new_hover->positionInWindow();
                new_hover->processMouseEnter(me);
            }
            hovered_frame_ = new_hover;
        } else if (hovered_frame_) {
            me.event_frame = hovered_frame_;
            me.position = window_pos - hovered_frame_->positionInWindow();
            hovered_frame_->processMouseMove(me);
        }
    }

    void dispatchWheel(const juce::MouseWheelDetails& wheel, const juce::MouseEvent& e) {
        if (!event_root_)
            return;

        const visage::Point window_pos = { static_cast<float>(e.position.x), static_cast<float>(e.position.y) };

        visage::MouseEvent me;
        me.window_position = window_pos;
        me.wheel_delta_x = wheel.deltaX;
        me.wheel_delta_y = wheel.deltaY;
        me.wheel_momentum = wheel.isInertial;

        hovered_frame_ = event_root_->frameAtPoint(window_pos);
        visage::Frame* temp = hovered_frame_;
        while (temp) {
            if (!temp->ignoresMouseEvents()) {
                me.event_frame = temp;
                me.position = window_pos - temp->positionInWindow();
                if (temp->processMouseWheel(me))
                    break;
            }
            temp = temp->parent();
        }
    }

    void tryInitialize() {
        if (rendererInitialized_)
            return;

        auto* peer = getPeer();
        if (!peer)
            return;

        void* nativeWindow = peer->getNativeHandle();
        if (!nativeWindow)
            return;

        visage::Renderer::instance().initialize(nativeWindow, nullptr);

        canvas_ = std::make_unique<visage::Canvas>();

        // TEMP: Swap-chain path is unstable in plugin hosting. Use windowless render for preview.
        constexpr bool kForceWindowless = true;
        const float scale = (float)getApproximateScaleFactorForComponent(this);
        const int physicalW = juce::roundToInt(getWidth() * scale);
        const int physicalH = juce::roundToInt(getHeight() * scale);

        if (kForceWindowless || !visage::Canvas::swapChainSupported()) {
            windowless_ = true;
            canvas_->setWindowless(physicalW, physicalH);
        } else {
            windowless_ = false;
            canvas_->pairToWindow(nativeWindow, physicalW, physicalH);
        }

        canvas_->setDpiScale(scale);

        eventHandler_.request_redraw = [this](visage::Frame* frame) {
            if (std::find(staleFrames_.begin(), staleFrames_.end(), frame) == staleFrames_.end())
                staleFrames_.push_back(frame);
        };

        eventHandler_.remove_from_hierarchy = [this](visage::Frame* frame) {
            auto pos = std::find(staleFrames_.begin(), staleFrames_.end(), frame);
            if (pos != staleFrames_.end())
                staleFrames_.erase(pos);
        };

        rendererInitialized_ = true;
        onInit();
    }

    void teardownVisage() {
        rendererInitialized_ = false;
        staleFrames_.clear();
        onDestroy();
        if (canvas_) {
            canvas_->removeFromWindow();
            canvas_.reset();
        }
        windowless_ = false;
        backbuffer_ = juce::Image();
    }

    void updateBackbufferFromScreenshot(const visage::Screenshot& shot) {
        if (shot.width() <= 0 || shot.height() <= 0)
            return;

        if (!backbuffer_.isValid() || backbuffer_.getWidth() != shot.width() || backbuffer_.getHeight() != shot.height()) {
            backbuffer_ = juce::Image(juce::Image::ARGB, shot.width(), shot.height(), true);
        }

        juce::Image::BitmapData data(backbuffer_, juce::Image::BitmapData::writeOnly);
        const uint8_t* src = shot.data();
        for (int y = 0; y < shot.height(); ++y) {
            auto* dst = reinterpret_cast<juce::PixelARGB*>(data.getLinePointer(y));
            const uint8_t* row = src + (y * shot.width() * 4);
            for (int x = 0; x < shot.width(); ++x) {
                const uint8_t r = row[x * 4 + 0];
                const uint8_t g = row[x * 4 + 1];
                const uint8_t b = row[x * 4 + 2];
                const uint8_t a = row[x * 4 + 3];
                dst[x].setARGB(a, r, g, b);
            }
        }
    }

    std::unique_ptr<visage::Canvas> canvas_;
    visage::FrameEventHandler eventHandler_;
    std::vector<visage::Frame*> staleFrames_;
    bool rendererInitialized_ = false;
    bool windowless_ = false;
    juce::Image backbuffer_;
    visage::Frame* event_root_ = nullptr;
    visage::Frame* captured_frame_ = nullptr;
    visage::Frame* hovered_frame_ = nullptr;
    visage::Point last_mouse_window_pos_ = { 0.0f, 0.0f };
    visage::MouseButton last_button_id_ = visage::kMouseButtonLeft;
};
