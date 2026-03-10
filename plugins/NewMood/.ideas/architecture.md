# DSP Architecture Specification - NewMood

## Core Components

### 1. Circular Buffer Manager
- Fixed-size audio buffer for loop recording
- Write pointer management
- Read pointer with variable speed
- Overdub functionality (mix new input with existing buffer)

### 2. Granular Processor
- Grain extraction from recorded buffer
- Variable grain size control
- Grain windowing (envelope shaping)
- Pitch shifting via speed control
- Drift modulation for organic feel

### 3. Time Sync Engine
- DAW tempo detection (from JUCE transport)
- Subdivision calculation (1/4, 1/8, 1/16, triplets)
- Manual tap tempo implementation
- Sample-accurate sync

### 4. Feedback Processor
- Feedback gain control
- Optional filtering in feedback path
- Stability monitoring (prevent runaway)

### 5. Output Processor
- Wet/dry mix control
- Tone control (lowpass/highpass filter)
- Stereo spread processing
- Output level control

## Processing Chain

```
Input
  │
  ├─→ [Record Buffer] ← Time/Sync Control
  │         │
  │         ├─→ [Grain Extractor] ← Speed, GrainSize, Drift
  │         │
  │         └─→ [Feedback Loop] ← Feedback
  │
  └─→ [Dry Path]
              │
              ↓
        [Mixer] ← Mix Control
              │
              ├─→ [Tone] ← Tone
              │
              ├─→ [Spread] ← Spread
              │
              └─→ [Level] ← Level
                    │
                    ↓
              Output
```

## Parameter Mapping

| Parameter | Component | Function | Range |
|-----------|-----------|----------|-------|
| `time` | Buffer Manager | Sets loop length in samples | 0.0 - 1.0 (maps to 0.1s - 10s) |
| `speed` | Granular Processor | Playback rate / pitch | 0.0 - 1.0 (0.25x - 4x) |
| `mix` | Mixer | Wet/dry balance | 0.0 - 1.0 |
| `feedback` | Feedback Processor | Regeneration amount | 0.0 - 1.0 |
| `grainSize` | Granular Processor | Individual grain duration | 0.0 - 1.0 (10ms - 500ms) |
| `drift` | Granular Processor | Pitch variation amount | 0.0 - 1.0 |
| `spread` | Output Processor | Stereo width | 0.0 - 1.0 |
| `tone` | Output Processor | Brightness filter | 0.0 - 1.0 |
| `level` | Output Processor | Output gain | 0.0 - 1.0 |
| `syncEnabled` | Time Sync | Enable DAW sync | 0/1 |
| `subdivision` | Time Sync | Musical subdivision | 0-7 |
| `record` | Buffer Manager | Recording toggle | 0/1 |
| `overdub` | Buffer Manager | Overdub mode | 0/1 |
| `clear` | Buffer Manager | Clear buffer | trigger |
| `reverse` | Granular Processor | Reverse playback | 0/1 |

## Complexity Assessment

**Score: 4 (Expert)**

**Rationale:**
- Granular processing requires careful buffer management and real-time grain extraction
- DAW clock sync requires sample-accurate timing
- Multiple modulation sources (drift, speed)
- State management for recording/overdub/reverse modes
- Stereo processing for spread effect
- Complex parameter interactions (feedback stability, sync behavior)

This is a sophisticated effect that requires careful implementation of:
1. Circular buffer with variable-length playback
2. Granular engine with windowing and pitch control
3. Tempo-sync logic with multiple subdivisions
4. Multi-mode recording (record, overdub, clear)
5. Stereo enhancement processing
