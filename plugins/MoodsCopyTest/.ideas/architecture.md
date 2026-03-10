# DSP Architecture Specification

## Core Components
- **Global Clock Engine**: Manages the effective sample rate and modulates the read/write speeds of the entire DSP processing chain to simulate clock degradation and pitch shifts.
- **Wet Channel DSP**:
  - **Reverb Tank**: Lush, modulated algorithmic reverb.
  - **Delay Line**: Variable delay with feedback and time modulation.
  - **Pitch/Slip Engine**: Real-time pitch shifting and time-stretching buffer.
- **Micro-Looper DSP**:
  - **Always-Listening Buffer**: A continuous circular buffer capturing the last N seconds of audio.
  - **Playback Engine**: Handles granular stretching (Stretch), variable speed/direction playback (Tape), and triggered slice playback (Env).
- **Mix & Routing Stage**: Blends Dry, Wet, and Looper signals with interactive routing capabilities (Wet into Looper, or Looper into Wet).

## Processing Chain
```text
Input ──┬─────────────────────────────> Dry Mute/Gain ─────────────────┬──> Output
        │                                                              │
        ├─> [Wet Channel DSP] (Reverb/Delay/Slip) ──> Level / Mix ─────┤
        │                                                              │
        └─> [Micro-Looper DSP] (Tape/Stretch/Env) ──> Level / Mix ─────┘
                                                                       ^
                                                                 (Routing toggles
                                                                  can feed outputs
                                                                  into each other)
```

## Parameter Mapping
| Parameter | Component | Function | Range |
|-----------|-----------|----------|-------|
| `clock` | Global Clock | Sets effective sample rate / pitch shift | 0.0 - 1.0 |
| `mix` | Mix Stage | Blends Dry and Wet/Loop signals | 0.0 - 1.0 (0-100%) |
| `wet_mode` | Wet Channel | Selects between Reverb (0), Delay (1), Slip (2) | 0 - 2 |
| `wet_time` | Wet Channel | Decay (Reverb), Time (Delay), Refresh (Slip) | 0.0 - 1.0 |
| `wet_modify` | Wet Channel | Amount/Modulation (Reverb), Feedback (Delay), Pitch/Direction (Slip) | 0.0 - 1.0 |
| `loop_mode` | Micro-Looper | Selects between Env (0), Tape (1), Stretch (2) | 0 - 2 |
| `loop_length` | Micro-Looper | Slice size (Env/Stretch), Loop Length (Tape) | 0.0 - 1.0 |
| `loop_modify` | Micro-Looper | Varies by mode (e.g., speed, overdub amount, trigger threshold) | 0.0 - 1.0 |
| `routing` | Routing Stage | Defines if Wet feeds Looper or Looper feeds Wet | 0 - 1 |

## Complexity Assessment
**Score: 4**
**Rationale:** The complexity is high due to the requirement of real-time pitch shifting (Slip), a continuous 'always-listening' micro-looper with variable playback engines (granular stretch, tape manipulation, envelope slicing), and a global clock that interactively scales the DSP processing rate across multiple complex modules simultaneously. State management of the looper buffer and ensuring artifact-free transitions between modes add significant challenge.
