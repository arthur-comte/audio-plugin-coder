# NewMood - Parameter Specification

## Core Parameters

| ID | Name | Type | Range | Default | Unit | Description |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `time` | Time | Float | 0.0 - 1.0 | 0.5 | - | Loop length (0 = short, 1 = long). When synced, controls subdivision multiplier. |
| `speed` | Speed | Float | 0.0 - 1.0 | 0.5 | - | Granular playback rate. 0.5 = normal, <0.5 = slower/darker, >0.5 = faster/brighter |
| `mix` | Mix | Float | 0.0 - 1.0 | 0.5 | - | Wet/dry mix. 0 = dry, 1 = fully wet |

## Additional Parameters

| ID | Name | Type | Range | Default | Unit | Description |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `feedback` | Feedback | Float | 0.0 - 1.0 | 0.3 | - | Amount of feedback/regeneration |
| `grainSize` | Grain Size | Float | 0.0 - 1.0 | 0.3 | - | Size of individual grains |
| `drift` | Drift | Float | 0.0 - 1.0 | 0.0 | - | Pitch drift/variation for organic feel |
| `spread` | Spread | Float | 0.0 - 1.0 | 0.5 | - | Stereo width/spread |
| `tone` | Tone | Float | 0.0 - 1.0 | 0.5 | - | Tone control for brightness |
| `level` | Level | Float | 0.0 - 1.0 | 0.75 | - | Output level |

## Clock & Sync Parameters

| ID | Name | Type | Range | Default | Unit | Description |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `syncEnabled` | Sync | Bool | 0/1 | 0 | - | Enable DAW tempo sync |
| `subdivision` | Subdivision | Int | 0-7 | 2 | - | 0=1/4, 1=1/4T, 2=1/8, 3=1/8T, 4=1/16, 5=1/16T, 6=1/32, 7=1/32T |
| `tapTempo` | Tap | Float | - | - | - | Tap tempo button (trigger only) |

## Recording Controls

| ID | Name | Type | Range | Default | Unit | Description |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `record` | Record | Bool | 0/1 | 0 | - | Record enable toggle |
| `overdub` | Overdub | Bool | 0/1 | 0 | - | Overdub mode toggle |
| `clear` | Clear | Trigger | - | - | - | Clear buffer (trigger) |
| `reverse` | Reverse | Bool | 0/1 | 0 | - | Reverse playback |

## Parameter Groups

### Primary (Main Knobs)
- Time, Speed, Mix

### Secondary (Fine-Tune)
- Feedback, GrainSize, Drift

### Output
- Tone, Level, Spread

### Global
- SyncEnabled, Subdivision
