# UI Specification v1

## Layout
- **Window:** 600x400px (Standard Size)
- **Sections:**
  - **Left Half:** Wet Channel Controls
  - **Right Half:** Micro-Looper Controls (including Audio Waveform visualizer)
  - **Bottom Strip:** Global Controls (Clock and Mix)
- **Grid:** A 2x2 main area for the left and right halves, with a dedicated horizontal strip along the bottom.

## Controls
| Parameter | Type | Position | Range | Default |
|-----------|------|----------|-------|---------|
| `clock` | Rotary Knob | Bottom Left | 0.0 - 1.0 | 0.5 |
| `mix` | Rotary Knob | Bottom Right | 0.0 - 1.0 | 0.5 |
| `wet_mode` | Toggle Button Group | Top Left | 0 - 2 | 0 |
| `wet_time` | Rotary Knob | Mid Left | 0.0 - 1.0 | 0.5 |
| `wet_modify` | Rotary Knob | Mid Left | 0.0 - 1.0 | 0.5 |
| `loop_mode` | Toggle Button Group | Top Right | 0 - 2 | 0 |
| `loop_length` | Rotary Knob | Mid Right | 0.0 - 1.0 | 0.5 |
| `loop_modify` | Rotary Knob | Mid Right | 0.0 - 1.0 | 0.5 |
| `routing` | 2-Way Switch | Center | 0 - 1 | 0 |

## Visualizations
- **Looper Buffer Display:** A waveform display situated in the top-right section (above or alongside the Looper mode toggles), providing real-time visual feedback of the "always-listening" audio buffer.

## Color Palette
- **Background:** Dark slate / charcoal (to make pastels pop)
- **Primary (Global/Wet Baseline):** Deep Pink
- **Accent (Loop/Global Focus):** Mint Green
- **Text:** Off-White / Light Grey

## Style Notes
- Boutique pedal aesthetic with dark metallic knobs.
- Wet section uses Deep Pink accents and outlines.
- Looper section uses a subtle frame with the Audio Waveform drawn in Mint Green.
- Global Clock and Mix knobs are styled distinctly (perhaps larger or with Mint Green caps/rings) to denote their global impact.
