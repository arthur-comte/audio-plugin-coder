#pragma once

#include <visage/ui.h>
#include <visage/graphics.h>
#include "BinaryData.h"

#include <algorithm>

// NewMood Visage UI - Chase Bliss inspired design
class VisageMainView : public visage::Frame {
public:
    VisageMainView() = default;

    void init() override {
        updateFonts();
    }

    void dpiChanged() override {
        updateFonts();
    }

    void resized() override {
        // Layout child frames here as the view resizes.
        // 600x400 window with 20px padding
        const float padding = 20.0f;
        const float w = width() - 2 * padding;
        const float h = height() - 2 * padding;
        
        // Section widths (3-column layout)
        float colWidth = w / 3.0f;
        float sectionHeight = h - 40; // Leave room for header
        
        // Header
        headerBounds_ = {padding, padding, w, 30};
        
        // Left column (Core controls)
        leftColBounds_ = {padding, padding + 40, colWidth - 8, sectionHeight};
        
        // Center column (Recording controls)
        centerColBounds_ = {padding + colWidth, padding + 40, colWidth - 8, sectionHeight};
        
        // Right column (Fine-tune controls)
        rightColBounds_ = {padding + 2 * colWidth, padding + 40, colWidth - 8, sectionHeight};
    }

    void draw(visage::Canvas& canvas) override {
        // Background - warm dark brown
        canvas.setColor(0xFF2D2A26);
        canvas.fill(0, 0, width(), height());
        
        // Draw header
        drawHeader(canvas);
        
        // Draw sections
        drawLeftSection(canvas);
        drawCenterSection(canvas);
        drawRightSection(canvas);
    }

private:
    void updateFonts() {
        const float dpi = std::max(1.0f, dpiScale());
        // Embedded font (Lato) for consistent text in VST hosts.
        const auto* font_data = reinterpret_cast<const unsigned char*>(newmood_BinaryData::LatoRegular_ttf);
        title_font_ = visage::Font(24.0f, font_data, newmood_BinaryData::LatoRegular_ttfSize, dpi);
        label_font_ = visage::Font(14.0f, font_data, newmood_BinaryData::LatoRegular_ttfSize, dpi);
        value_font_ = visage::Font(11.0f, font_data, newmood_BinaryData::LatoRegular_ttfSize, dpi);
    }
    
    void drawHeader(visage::Canvas& canvas) {
        // Plugin title
        canvas.setColor(0xFFE8E0D5);
        canvas.setFont(title_font_);
        canvas.drawText("NEWMOOD", static_cast<int>(headerBounds_.x), static_cast<int>(headerBounds_.y), 
                       static_cast<int>(headerBounds_.w), static_cast<int>(headerBounds_.h),
                       visage::Canvas::Justification::left);
        
        // Accent line under title
        canvas.setColor(0xFFD4A574);
        canvas.fill(headerBounds_.x, headerBounds_.y + 32, headerBounds_.w, 2);
    }
    
    void drawLeftSection(visage::Canvas& canvas) {
        // Section background
        canvas.setColor(0xFF3D3833);
        canvas.fill(leftColBounds_.x, leftColBounds_.y, leftColBounds_.w, leftColBounds_.h);
        
        // Section title
        canvas.setColor(0xFFD4A574);
        canvas.setFont(label_font_);
        canvas.drawText("CORE", static_cast<int>(leftColBounds_.x + 10), static_cast<int>(leftColBounds_.y + 10), 
                       100, 20, visage::Canvas::Justification::left);
        
        // Draw knobs: Time, Speed, Mix
        float knobSize = 50.0f;
        float startX = leftColBounds_.x + leftColBounds_.w / 2 - knobSize / 2;
        float startY = leftColBounds_.y + 40;
        float gap = 80.0f;
        
        drawKnob(canvas, startX, startY, "TIME", 0.5f);
        drawKnob(canvas, startX, startY + gap, "SPEED", 0.5f);
        drawKnob(canvas, startX, startY + 2 * gap, "MIX", 0.5f);
    }
    
    void drawCenterSection(visage::Canvas& canvas) {
        // Section background
        canvas.setColor(0xFF3D3833);
        canvas.fill(centerColBounds_.x, centerColBounds_.y, centerColBounds_.w, centerColBounds_.h);
        
        // Section title
        canvas.setColor(0xFFD4A574);
        canvas.setFont(label_font_);
        canvas.drawText("RECORD", static_cast<int>(centerColBounds_.x + 10), static_cast<int>(centerColBounds_.y + 10), 
                       100, 20, visage::Canvas::Justification::left);
        
        // Draw buttons: Record, Overdub, Reverse, Clear
        float btnW = 50.0f;
        float btnH = 35.0f;
        float startX = centerColBounds_.x + 15;
        float startY = centerColBounds_.y + 40;
        float gap = 60.0f;
        
        drawButton(canvas, startX, startY, btnW, btnH, "REC", false);
        drawButton(canvas, startX + gap, startY, btnW, btnH, "OVR", false);
        drawButton(canvas, startX + 2 * gap, startY, btnW, btnH, "REV", false);
        drawButton(canvas, startX, startY + 50, btnW, btnH, "CLR", false);
        
        // Sync controls
        canvas.setColor(0xFFE8E0D5);
        canvas.setFont(value_font_);
        canvas.drawText("SYNC", static_cast<int>(startX), static_cast<int>(startY + 100), 
                       40, 20, visage::Canvas::Justification::left);
        
        // Subdivision selector
        drawSelector(canvas, startX + 50, startY + 95, 100, 25, "1/8");
    }
    
    void drawRightSection(visage::Canvas& canvas) {
        // Section background
        canvas.setColor(0xFF3D3833);
        canvas.fill(rightColBounds_.x, rightColBounds_.y, rightColBounds_.w, rightColBounds_.h);
        
        // Section title
        canvas.setColor(0xFFD4A574);
        canvas.setFont(label_font_);
        canvas.drawText("FINE-TUNE", static_cast<int>(rightColBounds_.x + 10), static_cast<int>(rightColBounds_.y + 10), 
                       100, 20, visage::Canvas::Justification::left);
        
        // Draw knobs: Feedback, GrainSize, Drift, Spread, Tone, Level
        float knobSize = 45.0f;
        float startX = rightColBounds_.x + rightColBounds_.w / 2 - knobSize / 2;
        float startY = rightColBounds_.y + 40;
        float gap = 55.0f;
        
        drawKnob(canvas, startX, startY, "FEED", 0.3f);
        drawKnob(canvas, startX, startY + gap, "GRAIN", 0.3f);
        drawKnob(canvas, startX, startY + 2 * gap, "DRIFT", 0.0f);
        drawKnob(canvas, startX, startY + 3 * gap, "SPREAD", 0.5f);
        drawKnob(canvas, startX, startY + 4 * gap, "TONE", 0.5f);
        drawKnob(canvas, startX, startY + 5 * gap, "LEVEL", 0.75f);
    }
    
    void drawKnob(visage::Canvas& canvas, float x, float y, const char* label, float value) {
        float knobSize = 45.0f;
        
        // Knob body
        canvas.setColor(0xFF4A4540);
        canvas.fillCircle(x + knobSize/2, y + knobSize/2, knobSize/2 - 2);
        
        // Knob indicator (value arc)
        canvas.setColor(0xFFD4A574);
        float angle = -135.0f + value * 270.0f; // -135 to 135 degrees
        float rad = angle * 3.14159f / 180.0f;
        float indicatorLen = knobSize / 2 - 8;
        float cx = x + knobSize / 2;
        float cy = y + knobSize / 2;
        float ex = cx + std::cos(rad) * indicatorLen;
        float ey = cy + std::sin(rad) * indicatorLen;
        canvas.setLineWidth(3);
        canvas.drawLine(cx, cy, ex, ey);
        
        // Label
        canvas.setColor(0xFFE8E0D5);
        canvas.setFont(value_font_);
        canvas.drawText(label, static_cast<int>(x), static_cast<int>(y + knobSize + 5), 
                       static_cast<int>(knobSize), 15, visage::Canvas::Justification::center);
        
        // Value
        canvas.setColor(0xFF8B7355);
        canvas.drawText(std::to_string(static_cast<int>(value * 100)), static_cast<int>(x), static_cast<int>(y - 15), 
                       static_cast<int>(knobSize), 15, visage::Canvas::Justification::center);
    }
    
    void drawButton(visage::Canvas& canvas, float x, float y, float w, float h, const char* label, bool active) {
        // Button background
        canvas.setColor(active ? 0xFFC4956A : 0xFF4A4540);
        canvas.fillRect(x, y, w, h);
        
        // Button border
        canvas.setColor(0xFF8B7355);
        canvas.setLineWidth(1);
        canvas.drawRect(x, y, w, h);
        
        // Label
        canvas.setColor(0xFFE8E0D5);
        canvas.setFont(value_font_);
        canvas.drawText(label, static_cast<int>(x), static_cast<int>(y + 10), 
                       static_cast<int>(w), static_cast<int>(h - 20), visage::Canvas::Justification::center);
        
        // LED indicator
        canvas.setColor(active ? 0xFF7CB342 : 0xFF4A4A4A);
        canvas.fillCircle(x + w - 8, y + 6, 4);
    }
    
    void drawSelector(visage::Canvas& canvas, float x, float y, float w, float h, const char* value) {
        // Selector background
        canvas.setColor(0xFF4A4540);
        canvas.fillRect(x, y, w, h);
        
        // Selector border
        canvas.setColor(0xFF8B7355);
        canvas.setLineWidth(1);
        canvas.drawRect(x, y, w, h);
        
        // Value text
        canvas.setColor(0xFFE8E0D5);
        canvas.setFont(value_font_);
        canvas.drawText(value, static_cast<int>(x), static_cast<int>(y + 5), 
                       static_cast<int>(w), static_cast<int>(h - 10), visage::Canvas::Justification::center);
    }

    visage::Font title_font_;
    visage::Font label_font_;
    visage::Font value_font_;
    
    // Bounds
    visage::Rect headerBounds_;
    visage::Rect leftColBounds_;
    visage::Rect centerColBounds_;
    visage::Rect rightColBounds_;
};
