#pragma once
#include <JuceHeader.h>

/**
 * A basic modulated delay line for the Wet Channel.
 */
class MoodsDelay
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        delayLine.prepare(spec);
        delayLine.setMaximumDelayInSamples(spec.sampleRate * 2.0); // 2 seconds max
        
        feedback = 0.5f;
        timeInSeconds = 0.5f;
    }

    void setParameters(float time, float fb)
    {
        timeInSeconds = time;
        feedback = fb;
    }

    void process(juce::AudioBuffer<float>& buffer)
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < numSamples; ++sample)
            {
                float input = channelData[sample];
                float delayed = delayLine.popSample(channel);
                
                float output = input + delayed * feedback;
                delayLine.pushSample(channel, output);
                
                channelData[sample] = delayed;
            }
        }
    }

    void updateTime(double sampleRate)
    {
        delayLine.setDelay(timeInSeconds * sampleRate);
    }

private:
    juce::dsp::DelayLine<float> delayLine { 96000 };
    float feedback = 0.0f;
    float timeInSeconds = 0.5f;
};

/**
 * A lush algorithmic reverb.
 */
class MoodsReverb
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        reverb.prepare(spec);
        
        juce::Reverb::Parameters params;
        params.roomSize = 0.8f;
        params.damping = 0.5f;
        params.wetLevel = 1.0f;
        params.dryLevel = 0.0f;
        params.width = 1.0f;
        params.freezeMode = 0.0f;
        reverb.setParameters(params);
    }
    
    void setParameters(float size, float damp)
    {
        auto params = reverb.getParameters();
        params.roomSize = size;
        params.damping = damp;
        reverb.setParameters(params);
    }

    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        reverb.process(context);
    }

private:
    juce::dsp::Reverb reverb;
};

/**
 * The Slip engine: Real-time pitch shifting/time-stretching buffer.
 * For now, implemented as a simple variable-speed read/write buffer.
 */
class MoodsSlip
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        bufferSize = (int)(spec.sampleRate * 2.0);
        internalBuffer.setSize(spec.numChannels, bufferSize);
        internalBuffer.clear();
        writePos = 0;
        readPos = 0;
    }

    void process(juce::AudioBuffer<float>& buffer, float pitchFactor)
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        for (int sample = 0; sample < numSamples; ++sample)
        {
            for (int channel = 0; channel < numChannels; ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                float input = channelData[sample];

                // Write to buffer
                internalBuffer.setSample(channel, writePos, input);

                // Read from buffer with pitch factor
                float out = internalBuffer.getSample(channel, (int)readPos % bufferSize);
                channelData[sample] = out;
            }

            writePos = (writePos + 1) % bufferSize;
            readPos += pitchFactor;
            if (readPos >= bufferSize) readPos -= bufferSize;
        }
    }

private:
    juce::AudioBuffer<float> internalBuffer;
    int bufferSize = 0;
    int writePos = 0;
    double readPos = 0;
};
