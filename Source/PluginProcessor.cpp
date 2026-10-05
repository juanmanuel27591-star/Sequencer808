#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

PluginProcessor::PluginProcessor()
    : AudioProcessor (BusesProperties()
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
        {
            formatManager.registerBasicFormats();
            loadSamples();

            for (int s = 0; s <numSteps; s += 4)
            grid [0][s] = true;
            grid [1][12] = true;
            for (int s = 0; s <numSteps; s += 2)
            grid [2][s] = true;

        }

        PluginProcessor::~PluginProcessor()
        {
        }
        void PluginProcessor::loadSamples()
        {
            const char* data[numTracks] = { BinaryData::kick_wav, BinaryData::snare_wav, BinaryData::hihat_closed_wav, BinaryData::hihat_open_wav };
            const int sizes[numTracks] = { BinaryData::kick_wavSize, BinaryData::snare_wavSize, BinaryData::hihat_closed_wavSize, BinaryData::hihat_open_wavSize };
            
            for (int t = 0; t < numTracks; ++t)
            {
                auto stream = std::make_unique<juce::MemoryInputStream> (data[t], (size_t) sizes[t], false);
                std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (std::move (stream)));

                if (reader == nullptr)
                    continue;

                samples[t].setSize ((int) reader->numChannels, (int) reader->lengthInSamples);
                reader->read (&samples[t], 0, (int) reader->lengthInSamples, 0, true, true);
            }
        }
void PluginProcessor::prepareToPlay (double newSampleRate, int)
{
    sampleRate = newSampleRate;
}
        void PluginProcessor::releaseResources()
        {            
        }
                void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)

        {
            buffer.clear();

        const double stepLength = sampleRate * 60.0 / bpm / 4.0;

        if (! playing)
        {
            stepIndex = -1;
            currentStep = -1;
            samplesIntoStep = stepLength;
            for (int t = 0; t < numTracks; ++t)
                playPosition[t] = -1;
            return;
        }

        auto* left = buffer.getWritePointer (0);
        auto* right = buffer.getNumChannels() > 1
            ? buffer.getWritePointer (1) : nullptr;
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            if (samplesIntoStep >= stepLength)
            {
                samplesIntoStep -= stepLength;
                stepIndex = (stepIndex + 1) % numSteps;
                currentStep = stepIndex;

                for (int t = 0; t < numTracks; ++t)
                    if (grid[t][stepIndex] && samples[t].getNumSamples() > 0)
                    playPosition[t] = 0;
            }
            samplesIntoStep += 1.0;
            float out = 0.0f;

            for (int t = 0; t < numTracks; ++t)
            {
                if (playPosition[t] < 0)
                continue;

                out += samples[t].getSample (0, playPosition[t])
                * trackGain;
                ++playPosition[t];
                if (playPosition[t] >= samples[t].getNumSamples())
                playPosition[t] = -1;
            }

            left[i] = out;
            if (right != nullptr)
                right[i] = out;
        }  
}

        juce::AudioProcessorEditor* PluginProcessor::createEditor()
        {
            return new PluginEditor (*this);
        }

        bool PluginProcessor::hasEditor() const
        {
            return true;
        }

        const juce::String PluginProcessor::getName() const
        {
            return JucePlugin_Name;
        }

        bool PluginProcessor::acceptsMidi() const
        {
            return false;
        }

        bool PluginProcessor::producesMidi() const
        {
            return false;
        }

        double PluginProcessor::getTailLengthSeconds() const
        {
            return 0.0;
        }

        int PluginProcessor::getNumPrograms()
        {
            return 1;
        }

        int PluginProcessor::getCurrentProgram()
        {
            return 0;
        }

        void PluginProcessor::setCurrentProgram (int)
        {
        }

        const juce::String PluginProcessor::getProgramName (int)
        {
            return {};
        }

        void PluginProcessor::changeProgramName (int, const juce::String&)
        {
        }

        void PluginProcessor::getStateInformation (juce::MemoryBlock&)
        {            
        }

        void PluginProcessor::setStateInformation (const void*, int)
        {            
        }

        juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
        {
            return new PluginProcessor();
        }