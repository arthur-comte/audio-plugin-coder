# NewMood - Granular Looper Plugin

A Chase Bliss Mood mkII inspired granular looper with real-time processing for VST3/AU.

## Overview

NewMood is a granular looper that processes audio in real-time using a circular buffer and granular synthesis techniques. It captures incoming audio and plays it back as small grains with various manipulations.

## Controls

### Core Parameters

| Control | Range | Description |
|---------|-------|-------------|
| **Time** | 0-100% | Controls the loop length (how much audio is held in the buffer). Higher = longer loops. |
| **Speed** | 0-100% | Granular playback speed. 50% = normal speed, lower = slower, higher = faster. |
| **Mix** | 0-100% | Dry/wet blend. 0% = dry signal, 100% = fully processed. |
| **Feedback** | 0-100% | Regeneration amount. Higher values create more repeats and buildup. |

### Fine-Tune Parameters

| Control | Range | Description |
|---------|-------|-------------|
| **GrainSize** | 0-100% | Size of audio grains. Lower = shorter grains (glitchy), Higher = longer grains (smooth). |
| **Drift** | 0-100% | Random pitch variation. Adds organic, unstable character. |
| **Spread** | 0-100% | Stereo width of the granular output. |
| **Tone** | 0-100% | Tone control (high-pass filter). |
| **Level** | 0-100% | Output volume. |

## How It Works

1. **Circular Buffer**: Audio is continuously written to a circular buffer (up to 10 seconds).
2. **Granular Playback**: The buffer is read back in small "grains" that can vary in size, speed, and position.
3. **Real-Time Processing**: No need to record first - audio passes through immediately with effects.

## Tips

- Start with **Mix** at 50% and **Feedback** at 30%
- Use **Time** to control how much audio is in the loop
- **Drift** adds pitch instability for experimental textures
- High **Feedback** with long **Time** creates ambient pads
- Low **GrainSize** + high **Drift** = glitchy textures

## Installation

1. Download the VST3 from GitHub Actions
2. Copy to `/Library/Audio/Plug-Ins/VST3/`
3. Authorize if needed (Right-click > Open, or System Settings > Privacy & Security)
4. Restart your DAW

## Technical Details

- **Sample Rate**: 44.1kHz / 48kHz
- **Buffer**: 10 seconds max circular buffer
- **Formats**: VST3, AU (macOS)
