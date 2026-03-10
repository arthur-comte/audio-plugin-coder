# Style Guide v1 - NewMood

## Color Palette

### Primary Colors
| Name | Hex Code | Usage |
|------|----------|-------|
| Background | #2D2A26 | Main window background |
| Panel | #3D3833 | Section backgrounds, panels |
| Primary Text | #E8E0D5 | Labels, values |
| Accent | #D4A574 | Active indicators, highlights |
| Secondary Accent | #8B7355 | Borders, inactive states |

### Functional Colors
| Name | Hex Code | Usage |
|------|----------|-------|
| LED Active | #7CB342 | Recording active, engaged states |
| LED Inactive | #4A4A4A | Inactive indicators |
| Button Pressed | #C4956A | Pressed button state |
| Knob Body | #4A4540 | Rotary knob background |
| Knob Indicator | #D4A574 | Knob position marker |

## Typography
- **Plugin Title:** Custom handwritten style, 24px, #E8E0D5
- **Section Headers:** Bold, 14px, #D4A574
- **Control Labels:** Regular, 11px, #E8E0D5
- **Control Values:** Monospace, 10px, #8B7355

## Spacing System
- **Window Padding:** 20px
- **Section Gap:** 16px
- **Control Spacing:** 12px
- **Knob Size:** 50px diameter
- **Button Size:** 40x30px

## Visual Style

### Hand-Drawn Aesthetic
- Slightly irregular borders (1-2px variance)
- Soft shadows for depth
- Textured panel surfaces (optional grain)
- Organic curves on decorative elements

### Control Styles

#### Rotary Knobs
- Circular body with subtle gradient
- Center indicator line
- Value arc showing position
- Smooth rotation animation
- Label below, value above

#### Buttons
- Rounded rectangle shape
- Raised appearance (3D effect)
- Pressed state: darker, inset shadow
- Toggle buttons: LED indicator
- Momentary buttons: flash on press

#### Toggle Switches
- Horizontal slider style
- LED indicator for on state
- Clear on/off visual distinction

#### Selector/Dropdown
- Horizontal button row for subdivisions
- Active state highlighted with accent color
- Options: 1/4, 1/4T, 1/8, 1/8T, 1/16, 1/16T, 1/32, 1/32T

## Layout Structure

```
+--------------------------------------------------+
|  NEWMOOD                              [Logo]     |  <- Header (40px)
+--------------------------------------------------+
|  TIME    SPEED    MIX    |  REC  OVR  REV  CLR  |  <- Main Area
|  [knob]  [knob]  [knob]  |  [btn][btn][btn][btn]|
|                          |                        |
|  FEEDBACK  GRAIN  DRIFT |  SYNC  [toggle]       |
|  [knob]    [knob] [knob]|  SUB   [selector]    |
|                          |                        |
|  SPREAD   TONE   LEVEL  |                        |
|  [knob]   [knob] [knob]|                        |
+--------------------------------------------------+
```

## Interaction States

### Knobs
- **Default:** Standard appearance
- **Hover:** Subtle glow, cursor change
- **Dragging:** Value tooltip appears
- **Double-click:** Reset to default

### Buttons
- **Default:** Raised appearance
- **Hover:** Slight highlight
- **Pressed:** Inset, darker
- **Active (toggle):** LED lit

## Inspiration
- Chase Bliss Mood mkII pedal aesthetic
- Warm, analog hardware feel
- Hand-crafted, unique character
- Professional studio equipment vibe
