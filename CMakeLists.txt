cmake_minimum_required(VERSION 3.22)

project(Sequencer808 VERSION 3.22)

set(CMAKE_CXX_STANDARS 17)
set(CMAKE_CXX_STANDARS_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)


juce_add_plugin(Sequencer808
COMPANY_NAME "Portfolio"
IS_SYNTH TRUE
NEEDS_MIDI_INPUT FALSE
NEEDS_MIDI_OUTPUT FALSE
PLUGIN_MANUFACTURER_CODE port 
PLUGIN_CODE Sq08 
FORMATS Standalone VST3
PRODUCT_NAME "Sequencer808"
)


juce_add_binary_data(DrumSamples SOURCES
Resources/kick.wav
Resources/snare.wav
Resources/hihat_closed.wav
Resources/hihat_open.wav
)

target_sources(Sequencer808 PRIVATE
Source/PluginProcessor.cpp
Source/PluginProcessor.h
Source/PluginEditor.cpp
Source/PluginEditor.h
)

target_compile_definitions(Sequencer808 PUBLIC
JUCE_WEB_BROWSER=0
JUCE_USE_CURL=0
JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(Sequencer808
PRIVATE
    DrumSamples
    juce::juce_audio_utils
PUBLIC  
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)