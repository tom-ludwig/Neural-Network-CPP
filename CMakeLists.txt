cmake_minimum_required(VERSION 3.26)
project(NeuralNetwork CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Include paths for all targets
include_directories(
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/gui
)

# Core neural network library
set(CORE_SOURCES
    src/core/Net.cpp
    src/core/Net.h
    src/core/Neuron.cpp
    src/core/Neuron.h
    src/core/Connection.cpp
    src/core/Connection.h
    src/core/TrainingData.cpp
    src/core/TrainingData.h
)

# CLI executable (run from project root: data/xor.txt)
add_executable(NeuralNetCLI main.cpp ${CORE_SOURCES})

# GUI executable
find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_executable(NeuralNetworkGUI
    main_gui.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/gui/NetworkScene.cpp
    src/gui/NetworkScene.h
    src/gui/NeuronItem.cpp
    src/gui/NeuronItem.h
    ${CORE_SOURCES}
)
target_link_libraries(NeuralNetworkGUI PRIVATE Qt6::Widgets)

# Training data generators
add_executable(GenerateXorData tools/generateXorData.cpp)
