cmake_minimum_required(VERSION 3.15)

# Plugin definition with Visage support
juce_add_plugin(newmood
    COMPANY_NAME "APC"
    PLUGIN_MANUFACTURER_CODE "APC"
    PLUGIN_CODE "NMWD"
    FORMATS VST3 Standalone
    PRODUCT_NAME "NewMood"
)

# Binary assets (fonts)
juce_add_binary_data(newmood_Assets
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../_tools/visage/visage_graphics/fonts/Lato-Regular.ttf
    NAMESPACE newmood_BinaryData
)

# Source files
target_sources(newmood
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/VisageControls.h
)

# Include paths
target_include_directories(newmood
    PRIVATE
        Source
        ${CMAKE_CURRENT_SOURCE_DIR}/../../common
)

# Required JUCE modules + Visage
target_link_libraries(newmood
    PRIVATE
        newmood_Assets
        visage::visage
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
