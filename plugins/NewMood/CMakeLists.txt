cmake_minimum_required(VERSION 3.15)

# Plugin definition
juce_add_plugin(newmood
    COMPANY_NAME "APC"
    PLUGIN_MANUFACTURER_CODE "APCM"
    PLUGIN_CODE "NMWD"
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "NewMood"
)

# Source files
target_sources(newmood
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

# Include paths
target_include_directories(newmood
    PRIVATE
        Source
)

# Required JUCE modules + WebView
target_link_libraries(newmood
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(newmood
    PUBLIC
        JUCE_VST3_CAN_REPLACE_VST2=0
        _USE_MATH_DEFINES=1
)
