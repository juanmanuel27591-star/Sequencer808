#pragma once

#include "PluginProcessor.h"

class PluginEditor : public juce::AudioProcessorEditor,
                    private juce::Timer
{
    public:
        PluginEditor (PluginProcessor&);
        ~PluginEditor() override;

        void paint (juce::Graphics& override);
        void resized() override;
        void mouseDown (const juce::MouseEvent&) override;

private:
    void timerCallback() override;

    juce::Rectangle<int> cellBounds
    (int track, int step) const;

    PluginProcessor& audioProcessor;
    juce::TextButton playButton { "PLAY" };
    juce::TextButton stopButton { "STOP" };
    juce::Slider bpmSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};