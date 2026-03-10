# UI Specification v1 - NewMood

## Design Source
- **Style:** Chase Bliss / Analog hardware inspired
- **Framework:** Visage (Native C++)
- **Custom:** Hand-drawn aesthetic, organic feel

## Layout
- **Window:** 600x400px (Standard)
- **Sections:** Multi-section layout with grouped controls
- **Grid:** 3-column layout with sections for:
  - Top: Header with plugin name and branding
  - Left: Core controls (Time, Speed, Mix)
  - Center: Recording controls (Record, Overdub, Clear, Reverse)
  - Right: Fine-tune controls (Feedback, GrainSize, Drift, Spread, Tone, Level)
  - Bottom: Sync controls (Sync toggle, Subdivision)

## Controls

| Parameter | Type | Position | Range | Default |
|-----------|------|----------|-------|---------|
| `time` | Rotary Knob | Left Section | 0.0 - 1.0 | 0.5 |
| `speed` | Rotary Knob | Left Section | 0.0 - 1.0 | 0.5 |
| `mix` | Rotary Knob | Left Section | 0.0 - 1.0 | 0.5 |
| `feedback` | Rotary Knob | Right Section | 0.0 - 1.0 | 0.3 |
| `grainSize` | Rotary Knob | Right Section | 0.0 - 1.0 | 0.3 |
| `drift` | Rotary Knob | Right Section | 0.0 - 1.0 | 0.0 |
| `spread` | Rotary Knob | Right Section | 0.0 - 1.0 | 0.5 |
| `tone` | Rotary Knob | Right Section | 0.0 - 1.0 | 0.5 |
| `level` | Rotary Knob | Right Section | 0.0 - 1.0 | 0.75 |
| `syncEnabled` | Toggle Button | Bottom Section | 0/1 | 0 |
| `subdivision` | Selector | Bottom Section | 0-7 | 2 |
| `record` | Momentary Button | Center Section | trigger | 0 |
| `overdub` | Toggle Button | Center Section | 0/1 | 0 |
| `clear` | Momentary Button | Center Section | trigger | 0 |
| `reverse` | Toggle Button | Center Section | 0/1 | 0 |

## Color Palette
- **Background:** #2D2A26 (Warm dark brown)
- **Panel Background:** #3D3833 (Slightly lighter brown)
- **Primary Text:** #E8E0D5 (Warm off-white)
- **Accent:** #D4A574 (Warm amber/gold)
- **Secondary Accent:** #8B7355 (Muted brown)
- **LED Active:** #7CB342 (Green)
- **LED Inactive:** #4A4A4A (Dark gray)
- **Knob Body:** #4A4540 (Dark brown)
- **Knob Indicator:** #D4A574 (Amber)

## Style Notes
- Hand-drawn aesthetic with slightly imperfect borders
- Organic, tactile feel reminiscent of hardware pedals
- Warm earth tones throughout
- Visible texture/grain on panels (optional)
- LED indicators for active states
- Chunky, tactile buttons for recording controls
- Smooth, high-quality knob rendering
- Custom font for plugin name (handwritten style if available)
