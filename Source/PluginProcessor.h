#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_formats/juce_audio_formats.h>

class PluginProcessor : public juce::AudioProcessor
{
public: 
    PluginProcessor();
    ~PluginProcessor() override;


    void prepareToPlay (double sampleRate, int samplePerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;


    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;


    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;
    

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;




    static constexpr int numTracks = 4;
    static constexpr int numSteps = 16;


    bool grid[numTracks][numSteps] = {};

    juce::AudioBuffer<float> samples[numTracks];

    std::atomic<bool> playing { false };
    std::atomic<double> bpm { 120.0 };
    std::atomic<int> currentStep { -1 };

    private:
        void loadSamples();
        juce::AudioFormatManager formatManager;
        
        double sampleRate = 44100.0;
        int stepIndex = -1;
        double samplesIntoStep = 0.0;
        int playPosition[numTracks] = { -1, -1, -1, -1 };
        static constexpr float trackGain = 0.25f;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};