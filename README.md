# Neural Network in C++

A neural network with Qt GUI for training and visualization. Includes XOR gate and support for more datasets (e.g. handwritten digits).

## Project Structure

```
Neural-Network-CPP/
├── src/
│   ├── core/           # Neural network (Net, Neuron, Connection, TrainingData)
│   └── gui/            # Qt UI (MainWindow, NetworkScene, NeuronItem)
├── tools/              # Training data generators
│   └── generateXorData.cpp
├── data/               # Training data files
│   └── xor.txt
├── main.cpp            # CLI entry point
├── main_gui.cpp        # GUI entry point
└── CMakeLists.txt
```

## Build

```sh
mkdir -p build && cd build
cmake ..
cmake --build .
```

**macOS (Qt):**
```sh
cmake -B build -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
cmake --build build
```

## Run

**GUI (recommended):**
```sh
./build/NeuralNetworkGUI
```

**CLI:**
```sh
# Run from project root so data/xor.txt is found
./build/NeuralNetCLI
```

## Generate Training Data

**XOR gate:**
```sh
./build/GenerateXorData > data/xor.txt
```

Or use the built-in generator `tools/generateXorData.cpp`.

## Usage (GUI)

1. **Topology** – Configure layers (default: 2-4-1 for XOR)
2. **Training Data** – Browse to select a `.txt` file (e.g. `data/xor.txt`)
3. **Load from file** – Load topology from the training file
4. **Create Network** – Build network from topology
5. **Train** – Train on the selected data
6. **Test / Predict** – Enter inputs and run a forward pass
7. **Click input neurons** – Edit values directly in the visualization

## Data Format

```
topology: <input_count> <hidden_1> <hidden_2> ... <output_count>
in: <value1> <value2> ...
out: <target1> <target2> ...
```
