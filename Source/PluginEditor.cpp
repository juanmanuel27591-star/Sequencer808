#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
addAndMakeVisible (playButton); 
addAndMakeVisible (stopButton);
addAndMakeVisible (bpmSlider);

playButton.onClick = [this] { audioProcessor.playing = true; };
stopButton.onClick = [this] { audioProcessor.playing = false; };

bpmSlider.setRange (60.0, 200.0, 1.0);
bpmSlider.setValue (audioProcessor.bpm, juce::dontSendNotification);
bpmSlider.setTextValueSuffix (" BPM");
bpmSlider.onValueChange = [this] { audioProcessor.bpm = bpmSlider.getValue(); };

    setSize (800, 320);
    startTimerHz (30);
}

PluginEditor::~PluginEditor()
{    
}

juce::Rectangle<int> PluginEditor::cellBounds (int track, int step) const
{
    auto area = getLocalBounds().reduced (20);
    area.removeFromLeft (80);
    area.removeFromTop (60);

    const int cellW = area.getWidth() / PluginProcessor::numSteps;
    const int cellH = area.getHeight() / PluginProcessor::numTracks;

    return { area.getX() + step * cellW,
            area.getY() + track * cellH, cellW, cellH };      
}

void PluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff1a1a1a));
    g.setFont (14.0f);

    const char* names[PluginProcessor::numTracks]
        = { "KICK", "SNARE", "CL HAT", "OP HAT" };
for (int s = 0; s < PluginProcessor::numSteps; ++s)
{
    auto cell = cellBounds (0, s);
    g.setColour (s % 4 == 0 ? juce::Colours::yellow : juce::Colours::lightblue);
    g.drawText (juce::String (s + 1), cell.getX(), cell.getY() - 20, cell.getWidth(), 20, juce::Justification::centred);
}

        for (int t = 0; t < PluginProcessor::numTracks; ++t)
    {
            auto first = cellBounds (t, 0);
            g.setColour (juce::Colours::white);
            g.drawText (names[t], 20, first.getY(), 70, first.getHeight(), juce::Justification::centredLeft);
        for (int s = 0; s < PluginProcessor::numSteps; ++s)                
        {
            auto cell = cellBounds (t, s).reduced (2);

            if (audioProcessor.grid[t][s])
                g.setColour (juce::Colours::orange);
            else if ((s / 4) % 2 == 0)
                g.setColour (juce::Colour (0xff3c3c3c));
            else    
                g.setColour (juce::Colour (0xff2a2a2a));

            g.fillRoundedRectangle (cell.toFloat(), 4.0f);    

            if (s == audioProcessor.currentStep)
            {
                g.setColour (juce::Colours::white.withAlpha (0.35f));
                g.fillRoundedRectangle (cell.toFloat(), 4.0f);
            }
        }    
    }
}
void PluginEditor::mouseDown (const juce::MouseEvent& e)
{
    for (int t = 0; t < PluginProcessor::numTracks; ++t)
        for (int s = 0; s <PluginProcessor::numSteps; ++s)
        if (cellBounds (t, s).contains (e.getPosition()))
        {
            audioProcessor.grid[t][s]
                = ! audioProcessor.grid[t][s];
                repaint();
                return;
        }
}

void PluginEditor::timerCallback()
{
    repaint();
}

void PluginEditor::resized()
{
    auto area = getLocalBounds().reduced (20);
    auto strip = area.removeFromTop (30);

    playButton.setBounds (strip.removeFromLeft (80));
    strip.removeFromLeft (10);
    stopButton.setBounds (strip.removeFromLeft (80));
    strip.removeFromLeft (20);
    bpmSlider.setBounds (strip);
}