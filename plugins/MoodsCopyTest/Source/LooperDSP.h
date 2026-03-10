#pragma once
#include <JuceHeader.h>

/**
 * The Always-Listening Micro-Looper.
 */
class MoodsLooper
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        maxLoopSeconds = 30.0;
        bufferSize = (int)(sampleRate * maxLoopSeconds);
        
        loopBuffer.setSize(spec.numChannels, bufferSize);
        loopBuffer.clear();
        
        writePos = 0;
        playbackPos = 0;
    }

    void write(const juce::AudioBuffer<float>& input)
    {
        const int numChannels = input.getNumChannels();
        const int numSamples = input.getNumSamples();

        for (int sample = 0; sample < numSamples; ++sample)
        {
            for (int channel = 0; channel < numChannels; ++channel)
            {
                loopBuffer.setSample(channel, writePos, input.getSample(channel, sample));
            }
            writePos = (writePos + 1) % bufferSize;
        }
    }

    void process(juce::AudioBuffer<float>& output, int mode, float length, float modify, double clockScale)
    {
        const int numChannels = output.getNumChannels();
        const int numSamples = output.getNumSamples();
        
        // length parameter determines loop size (up to maxLoopSeconds)
        int currentLoopSize = (int)(length * 0.95f * bufferSize);
        if (currentLoopSize < 100) currentLoopSize = 100;

        float playbackSpeed = 1.0f;
        
        for (int sampleIdx = 0; sampleIdx < numSamples; ++sampleIdx)
        {
            float currentSampleValue = 0.0f;

            if (mode == 0) // Env Mode: Triggered slicing / windowing
            {
                // Simple version: playbackPos cycles within length, modify controls 'jitter' or window
                playbackSpeed = (float)clockScale;
                int readIdx = ((int)playbackPos) % currentLoopSize;
                currentSampleValue = loopBuffer.getSample(0, (writePos - currentLoopSize + readIdx + bufferSize) % bufferSize);
            }
            else if (mode == 1) // Tape mode: Variable speed
            {
                playbackSpeed = (modify - 0.5f) * 4.0f;
                if (std::abs(playbackSpeed) < 0.05f) playbackSpeed = 0.05f;
                playbackSpeed *= (float)clockScale;

                int readIdx = (int)playbackPos % currentLoopSize;
                currentSampleValue = loopBuffer.getSample(0, (writePos - currentLoopSize + readIdx + bufferSize) % bufferSize);
            }
            else if (mode == 2) // Stretch mode: Granular-ish time stretch
            {
                // Simplistic stretch: playback cycles fast but offset moves slow
                playbackSpeed = (float)clockScale;
                float stretchFactor = modify * 4.0f + 0.1f;
                
                int grainPos = (int)playbackPos % currentLoopSize;
                int stretchOffset = (int)(playbackPos / stretchFactor) % currentLoopSize;
                
                int readIdx = (grainPos + stretchOffset) % currentLoopSize;
                currentSampleValue = loopBuffer.getSample(0, (writePos - currentLoopSize + readIdx + bufferSize) % bufferSize);
            }

            for (int channel = 0; channel < numChannels; ++channel)
            {
                output.setSample(channel, sampleIdx, currentSampleValue);
            }
            
            playbackPos += playbackSpeed;
        }
    }

    void getWaveformSummary(float* dest, int numPoints)
    {
        if (bufferSize <= 0) return;
        
        const int samplesPerPoint = bufferSize / numPoints;
        for (int i = 0; i < numPoints; ++i)
        {
            float maxVal = 0;
            for (int s = 0; s < samplesPerPoint; ++s)
            {
                int idx = i * samplesPerPoint + s;
                maxVal = std::max(maxVal, std::abs(loopBuffer.getSample(0, idx % bufferSize)));
            }
            dest[i] = maxVal;
        }
    }

private:
    juce::AudioBuffer<float> loopBuffer;
    int bufferSize = 0;
    int writePos = 0;
    double playbackPos = 0;
    double sampleRate = 44100.0;
    double maxLoopSeconds = 30.0;
};
