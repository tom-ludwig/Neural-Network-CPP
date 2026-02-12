# XOR Gate Neural Network in Cpp
This repository showcases a basic neural network designed to learn and predict the output of an XOR-Gate. It serves as an excellent introductory project for anyone looking to dive into the world of neural networks. Start your journey here with this simple yet fundamental 'Hello, World' example in neural networking.

<img width="457" alt="Screenshot 2023-09-22 at 5 05 50 PM" src="https://github.com/activcoding/XOR-Gate-Neural-Network-in-Cpp/assets/83090745/c4f6738b-8c86-4e2a-b9be-dd9079b7a72c">

<img width="457" alt="NeuralNet" src="https://github.com/activcoding/XOR-Gate-Neural-Network-in-Cpp/assets/83090745/ed81b4b4-7806-45c8-aae7-fee69d442b5f">


## Usage
1. **Fork** and **clone** the repository.
2. Follow the steps to generate the training data.
3. Execute the code using the following instructions.

## Generate Training Data for a XOR-Gate
#### 1. C++ file:
```cpp
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;
  
int main() {
  cout << "topology: 2 4 1" << endl;
  for(int i = 10000; i >= 0; --i) {
    int n1 = (int) (2.0 * rand() / double(RAND_MAX));
    int n2 = (int) (2.0 * rand() / double(RAND_MAX));
    int t = n1 ^ n2; // bitwise XOR
    cout << "in: " << n1 << ".0 " << n2 << ".0 " << endl;
    cout << "out: " << t << ".0" << endl;
  }
}
```
#### 2. Compile the file:
```sh
g++ yourFileName.cpp -o makeTraingSamples
```
#### 3. Run the compiled executable:
```sh
./makeTraingSamples > trainingData.txt
```

#### 4. Move the trainingData.txt file into the project's folder.

## Running the XOR Gate Neural Network

#### 1. Navigate to the project directory:
```sh
cd Neural-Network-CPP
```

#### 2. Install Qt (required for GUI, on macOS with Homebrew):
```sh
brew install qt
```

#### 3. Build the project:
```sh
mkdir -p build && cd build
cmake ..
cmake --build .
```

#### 4. Run the project:

**GUI (recommended):**
```sh
./NeuralNetworkGUI
```
Launch the visual interface to configure topology, select training data, adjust learning rate and momentum, and watch the network train in real time.

**CLI:**
```sh
./XORGate_NeuralNetwork
```
Uses `shortTrainingData.txt` by default. Ensure the file exists in the current working directory.

If the training file cannot be found, please ensure the file is in the same directory from which you are executing the command. For example, if you are using CLion, you may need to move the file into the build folder.

Feel free to reach out if you have any questions or encounter any issues. Happy coding!
