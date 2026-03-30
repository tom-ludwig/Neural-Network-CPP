//
// Generates XOR gate training data in the format expected by TrainingData.
// Usage: ./GenerateXorData > ../data/xor.txt
//
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    cout << "topology: 2 4 1" << endl;
    for (int i = 10000; i >= 0; --i) {
        int n1 = static_cast<int>(2.0 * rand() / static_cast<double>(RAND_MAX));
        int n2 = static_cast<int>(2.0 * rand() / static_cast<double>(RAND_MAX));
        const int t = n1 ^ n2;  // bitwise XOR
        cout << "in: " << n1 << ".0 " << n2 << ".0 " << endl;
        cout << "out: " << t << ".0" << endl;
    }
}
