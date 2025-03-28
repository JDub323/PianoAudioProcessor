cmake_minimum_required(VERSION 3.29)
project(PianoAudioProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(PianoAudioProcessor tests/UnitTests.cpp
        MyAudio/AudioFinder.cpp
        MyAudio/AudioFinder.h
        NoteLogic/PianoLogic.cpp
        NoteLogic/PianoLogic.h
        NoteLogic/MyFilter.cpp
        MyAudio/CallbackFunctions.cpp
        MyAudio/CallbackFunctions.h
        noteLogic/SpectroHandler.cpp
        noteLogic/SpectroHandler.h
        tests/UnitTests.cpp
        tests/UnitTests.cpp
        myGui/GuiHandler.cpp
        myGui/GuiHandler.h
        myGui/SpectroImageMaker.cpp
        myGui/SpectroImageMaker.h
        noteLogic/NoteData.cpp
        noteLogic/NoteData.h
        noteLogic/NoteHistory.cpp
        noteLogic/NoteHistory.h
        noteLogic/NoteFileHandler.cpp
        noteLogic/NoteFileHandler.h

        #        imgui/imconfig.h
#        imgui/imgui.cpp
#        imgui/imgui_demo.cpp
#        imgui/imgui_draw.cpp
#        imgui/imgui_tables.cpp
#        imgui/imgui_widgets.cpp
#        imgui/backends/imgui_impl_dx11.cpp
#        imgui/backends/imgui_impl_win32.cpp
#        implot/implot.cpp
#        implot/implot_items.cpp
)

find_package(FFTW3 CONFIG REQUIRED)
target_link_libraries(PianoAudioProcessor PRIVATE FFTW3::fftw3)
find_package(portaudio CONFIG REQUIRED)
target_link_libraries(PianoAudioProcessor PRIVATE portaudio)
#target_link_libraries(PianoAudioProcessor PRIVATE
#        d3d11         # DirectX 11
#        #dxgi          # DirectX Graphics Infrastructure
#        d3dcompiler   # DirectX Shader Compiler
#        user32        # Core Win32 API
#        #gdi32         # Graphics Device Interface (optional but common for Win32)
#        #winmm         # Multimedia timers (optional for Win32)
#        dwmapi        # Desktop Window Manager API
#)