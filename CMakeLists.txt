cmake_minimum_required(VERSION 3.26)
project(XORGate_NeuralNetwork)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Shared neural network sources
set(NN_SOURCES
    Net.cpp Net.h
    Connection.cpp Connection.h
    Neuron.cpp Neuron.h
    TrainingData.cpp TrainingData.h
)

# CLI executable
add_executable(XORGate_NeuralNetwork main.cpp ${NN_SOURCES})

# GUI executable
add_executable(NeuralNetworkGUI main_gui.cpp
    MainWindow.cpp MainWindow.h
    NetworkScene.cpp NetworkScene.h
    NeuronItem.cpp NeuronItem.h
    ${NN_SOURCES}
)
target_link_libraries(NeuralNetworkGUI PRIVATE Qt6::Widgets)
