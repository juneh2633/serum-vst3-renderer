cmake_minimum_required(VERSION 3.15)
project(SerumRenderer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE framework
add_subdirectory(external/JUCE)

# Common utilities library
add_library(serum_common STATIC
    src/common/Hash.cpp
    src/common/Paths.cpp
    src/common/Log.cpp
)

target_include_directories(serum_common PUBLIC src)
target_link_libraries(serum_common PUBLIC
    juce::juce_core
)

# VST plugin management library
add_library(serum_vst STATIC
    src/vst/PluginScanner.cpp
    src/vst/PluginFactory.cpp
    src/vst/PresetStateIO.cpp
)

target_include_directories(serum_vst PUBLIC src)
target_link_libraries(serum_vst PUBLIC
    serum_common
    juce::juce_audio_processors
)

# MIDI processing library
add_library(serum_midi STATIC
    src/midi/SyntheticMidiGenerator.cpp
)

target_include_directories(serum_midi PUBLIC src)
target_link_libraries(serum_midi PUBLIC
    serum_common
    juce::juce_audio_basics
)

# Rendering library
add_library(serum_render STATIC
    src/render/OfflineRenderer.cpp
    src/render/WavWriter.cpp
    src/render/AudioStats.cpp
)

target_include_directories(serum_render PUBLIC src)
target_link_libraries(serum_render PUBLIC
    serum_common
    serum_midi
    juce::juce_audio_formats
    juce::juce_audio_processors
)

# StateCapturer GUI application
juce_add_gui_app(StateCapturer
    PRODUCT_NAME "Serum State Capturer"
)

target_sources(StateCapturer PRIVATE
    src/apps/StateCapturerMain.cpp
)

target_link_libraries(StateCapturer PRIVATE
    serum_common
    serum_vst
    juce::juce_gui_basics
)

# BatchRenderer console application
juce_add_console_app(BatchRenderer
    PRODUCT_NAME "Serum Batch Renderer"
)

target_sources(BatchRenderer PRIVATE
    src/apps/BatchRendererMain.cpp
)

target_link_libraries(BatchRenderer PRIVATE
    serum_common
    serum_vst
    serum_midi
    serum_render
)

# Compiler warnings
if(MSVC)
    target_compile_options(serum_common PRIVATE /W4)
    target_compile_options(serum_vst PRIVATE /W4)
    target_compile_options(serum_midi PRIVATE /W4)
    target_compile_options(serum_render PRIVATE /W4)
endif()
