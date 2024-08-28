# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

# Set name of project (as PROJECT_NAME)
project(main VERSION 1.0.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} src/main.cpp)

target_sources(${PROJECT_NAME} PRIVATE 
                src/main.cpp
                src/BlinkLed/BlinkLed.cpp
                src/Clock/Clock.cpp
                src/LedControl/LedControl.cpp 
                src/LedControl/NeoPixelLed/NeoPixelLed.cpp
                src/LedControl/NeoPixelLed/NeoPio/NeoPio.cpp
                src/LedControl/Settings/Settings.cpp
                src/AnalogRead/AnalogRead.cpp
                src/Rotary/Button/Button.cpp
                src/Rotary/Rotary.cpp
                src/LedControl/NeoPixelLed/Color/Color.cpp
                src/SerialIn/SerialIn.cpp
                )

target_link_libraries(${PROJECT_NAME} PRIVATE
        pico_stdlib
        hardware_pio
        hardware_dma
        hardware_flash
        hardware_adc
        pico_multicore
        )
# Create pio header files
pico_generate_pio_header(${PROJECT_NAME} 
                         ${CMAKE_CURRENT_LIST_DIR}/src/LedControl/NeoPixelLed/NeoPio/NeoPio.pio)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})


