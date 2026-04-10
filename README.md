# Neural Network in C++

A neural network with Qt GUI for training and visualization. Includes XOR gate and support for more datasets (e.g. handwritten digits).

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/f6b7086d-3243-4b37-a50a-6d042522bb12" width="800"></td>
    <td><img src="https://github.com/user-attachments/assets/5155181d-dd9b-4179-b87b-f00e0bd19a7d" width="800"></td>
  </tr>
</table>

## Usage (GUI)

(Run `./NeuralNetworkGUI`)

1. **Topology** – Configure layers (default: 2-4-1 for XOR)
2. **Training Data** – Browse to select a `.txt` file (e.g. `data/xor.txt` or `data/digits.txt`)
3. **Load from file** – Load topology from the training file
4. **Create Network** – Build network from topology
5. **Train** – Train on the selected data (Do not set the epchos to high on big data sets or many neurons since the training is running on your CPU it will likely freez the application)
6. **Test / Predict** – Enter inputs and run a forward pass
7. **Click input neurons** – Edit values directly in the visualization

With the digits dataset a small 8×8 drawing canvas is available for live prediction. Accuracy hovers around 90%; not ideal, but impressive for a from scratch implementation. Recognition is especially strong for digits like `8`, even with distorted or incomplete inputs.

## Requirements

- `cmake` and `make`
- `Qt` for GUI

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

**GUI only (Release):**

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target NeuralNetworkGUI
```

**macOS (Qt):**

```sh
cmake -B build -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
cmake --build build
```

## Run

**GUI (recommended):**

```sh
# macOS (.app bundle)
open build/NeuralNetworkGUI.app

# Linux / other (plain executable)
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

**Handwritten digits (8×8, 10 classes):**

```sh
./build/GenerateDigitsData > data/digits.txt
```

Uses embedded synthetic patterns (500 samples). For the full 1,797-sample UCI dataset, download `optdigits.tes` from [UCI](https://archive.ics.uci.edu/ml/machine-learning-databases/optdigits/), place in `data/`, and run:

```sh
./build/GenerateDigitsData data/optdigits.tes > data/digits.txt
```

## Data Format

```
topology: <input_count> <hidden_1> <hidden_2> ... <output_count>
in: <value1> <value2> ...
out: <target1> <target2> ...
```
