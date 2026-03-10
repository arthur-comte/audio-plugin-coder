#pragma once

#include <visage/ui.h>
#include <visage/graphics.h>
#include "BinaryData.h"

#include <algorithm>

// Minimal Visage root view. Replace with real layout and widgets.

class MoodsKnob : public visage::Frame {
public:
    void init() override {
        updateFont();
    }

    void dpiChanged() override {
        updateFont();
    }

    void updateFont() {
        const float dpi = std::max(1.0f, dpiScale());
        font_ = visage::Font(12.0f, reinterpret_cast<const unsigned char*>(BinaryData::LatoRegular_ttf), BinaryData::LatoRegular_ttfSize, dpi);
    }

    void draw(visage::Canvas& canvas) override {
        const float r = std::min(width(), height()) * 0.35f;
        const float cx = width() * 0.5f;
        const float cy = height() * 0.55f;

        // Label
        canvas.setColor(0xffF0F0F0);
        canvas.text(label_, font_, visage::Font::kCenter, 0, 0, width(), 20);

        // Background circle (Dark metallic)
        canvas.setColor(0xff111111);
        canvas.circle(cx - r, cy - r, r * 2.0f);

        // Value arc
        canvas.setColor(color_);
        float start_angle = 0.75f * visage::kPi;
        float sweep_angle = 1.5f * value_ * visage::kPi;
        canvas.arc(cx - r + 3, cy - r + 3, (r - 3) * 2.0f, 4.5f, start_angle + sweep_angle * 0.5f, sweep_angle, true);

        // Pointer
        const float angle = (0.75f + 1.5f * value_) * visage::kPi;
        canvas.segment(cx, cy, cx + std::cos(angle) * r, cy + std::sin(angle) * r, 2.5f, true);
    }

    void mouseDown(const visage::MouseEvent& e) override { last_y_ = e.position.y; }
    void mouseDrag(const visage::MouseEvent& e) override {
        float delta = (last_y_ - e.position.y) * 0.005f;
        value_ = std::clamp(value_ + delta, 0.0f, 1.0f);
        last_y_ = e.position.y;
        if (on_change) on_change(value_);
        redraw();
    }

    void setValue(float v) { value_ = v; redraw(); }
    void setColor(uint32_t c) { color_ = c; redraw(); }
    void setLabel(const std::string& l) { label_ = l; redraw(); }

    std::function<void(float)> on_change;

private:
    float value_ = 0.5f;
    float last_y_ = 0.0f;
    uint32_t color_ = 0xffFF1493; // Deep Pink
    std::string label_ = "KNOB";
    visage::Font font_;
};

class WaveformView : public visage::Frame {
public:
    void draw(visage::Canvas& canvas) override {
        // Dark metallic "LCD" background
        canvas.setColor(0xff080808);
        canvas.fill(0, 0, (float)width(), (float)height());
        
        // Subtle grid or border
        canvas.setColor(0x2298FF98);
        canvas.rectangleBorder(0, 0, (float)width(), (float)height(), 1.0f);

        canvas.setColor(0xff98FF98); // Mint Green
        const float midY = height() * 0.5f;
        const float w = (float)width();
        const float step = w / 512.0f;

        for (int i = 0; i < 511; ++i)
        {
            float y1 = midY - data[i] * midY * 0.8f;
            float y2 = midY - data[i+1] * midY * 0.8f;
            canvas.segment(i * step, y1, (i + 1) * step, y2, 1.5f, true);
        }
    }

    float data[512] = { 0.0f };
};

class VisageMainView : public visage::Frame {
public:
    VisageMainView() {
    }

    void init() override {
        // Setup initial properties
        clock_knob.setLabel("CLOCK");
        clock_knob.setColor(0xffFFD700); // Gold
        
        mix_knob.setLabel("MIX");
        mix_knob.setColor(0xffFFD700); // Gold
        
        wet_time.setLabel("TIME");
        wet_time.setColor(0xffFF1493); // Deep Pink
        
        wet_mod.setLabel("MODIFY");
        wet_mod.setColor(0xffFF1493); // Deep Pink
        
        loop_len.setLabel("LENGTH");
        loop_len.setColor(0xff98FF98); // Mint Green
        
        loop_mod.setLabel("MODIFY");
        loop_mod.setColor(0xff98FF98); // Mint Green

        // Add children
        addChild(clock_knob);
        addChild(mix_knob);
        addChild(wet_time);
        addChild(wet_mod);
        addChild(loop_len);
        addChild(loop_mod);
        addChild(waveform);

        updateFont();
        visage::Frame::init();
    }

    void dpiChanged() override {
        updateFont();
    }

    void updateFont() {
        const float dpi = std::max(1.0f, dpiScale());
        font_ = visage::Font(14.0f, reinterpret_cast<const unsigned char*>(BinaryData::LatoRegular_ttf), BinaryData::LatoRegular_ttfSize, dpi);
    }

    void draw(visage::Canvas& canvas) override {
        // Main Background
        canvas.setColor(0xff1E1E24);
        canvas.fill(0, 0, (float)width(), (float)height());

        // Section Backgrounds
        float margin = 10.0f;
        float halfW = width() * 0.5f;
        float bottomH = 100.0f;
        float topH = height() - bottomH;

        // Wet Section
        canvas.setColor(0xff2A2A35);
        canvas.roundedRectangle(margin, margin, halfW - margin * 1.5f, topH - margin * 1.5f, 6.0f);
        canvas.setColor(0xffFF1493);
        canvas.text("WET", font_, visage::Font::kCenter, margin, margin, halfW - margin * 1.5f, 30);

        // Looper Section
        canvas.setColor(0xff2A2A35);
        canvas.roundedRectangle(halfW + margin * 0.5f, margin, halfW - margin * 1.5f, topH - margin * 1.5f, 6.0f);
        canvas.setColor(0xff98FF98);
        canvas.text("LOOP", font_, visage::Font::kCenter, halfW + margin * 0.5f, margin, halfW - margin * 1.5f, 30);
    }

    void resized() override {
        float margin = 10.0f;
        float bottomH = 100.0f;
        float topH = height() - bottomH;
        float halfW = width() * 0.5f;

        // Bottom Strip
        clock_knob.setBounds(margin + 20, topH + margin, 80, 80);
        mix_knob.setBounds(width() - 80 - margin - 20, topH + margin, 80, 80);

        // Wet Section (Left)
        wet_time.setBounds(margin + 20, 50, 100, 100);
        wet_mod.setBounds(margin + 20, 160, 100, 100);

        // Looper Section (Right)
        waveform.setBounds(halfW + margin + 10, 50, halfW - margin * 3, 60);
        loop_len.setBounds(halfW + margin + 10, 120, 80, 80);
        loop_mod.setBounds(halfW + margin + 100, 120, 80, 80);
    }

    MoodsKnob clock_knob, mix_knob;
    MoodsKnob wet_time, wet_mod;
    MoodsKnob loop_len, loop_mod;
    WaveformView waveform;
    visage::Font font_;
};
