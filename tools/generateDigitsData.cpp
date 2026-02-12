//
// Generates handwritten digits training data (pure C++, no Python).
// Uses embedded synthetic 8x8 digit patterns + optional UCI file.
//
// Usage:
//   ./GenerateDigitsData                    # embedded data only (~500 samples)
//   ./GenerateDigitsData data/optdigits.tes # convert UCI file (full 1797 samples)
//
// UCI format: comma-separated, 64 pixel values (0-16) + class (0-9).
// Download from: https://archive.ics.uci.edu/ml/machine-learning-databases/optdigits/
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>

namespace {

// Canonical 8x8 digit patterns (0-16 grayscale). One pattern per digit 0-9.
// Each is 64 values, row-major. Stylized but trainable.
const unsigned char DIGIT_PATTERNS[10][64] = {
    // 0 - ring
    { 0, 0,12,14,14,12, 0, 0,  0,10,16, 0, 0,16,10, 0,  0,14, 0, 0, 0, 0,14, 0,
      0,14, 0, 0, 0, 0,14, 0,  0,10,16, 0, 0,16,10, 0,  0, 0,12,14,14,12, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0 },
    // 1 - vertical line
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 8,16, 0, 0, 0,  0, 0, 0, 8,16, 0, 0, 0,
      0, 0, 0, 8,16, 0, 0, 0,  0, 0, 0, 8,16, 0, 0, 0,  0, 0, 0, 8,16, 0, 0, 0,  0, 0, 0, 8,16, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0 },
    // 2
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,12,14,14,14, 0, 0,  0, 0, 0, 0, 0,14, 8, 0,  0, 0, 0, 0, 0,14, 4, 0, 0,  0, 0, 0, 8,14, 0, 0, 0,  0, 0, 4,14, 0, 0, 0, 0,  0,12,14,14,14,14,14, 0,  0, 0, 0, 0, 0, 0, 0 },
    // 3
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,10,14,14,12, 0, 0,  0, 0, 0, 0, 0,14, 8, 0,  0, 0, 0, 6,14,14, 4, 0, 0,  0, 0, 0, 0, 0,14, 8, 0,  0, 0, 0, 0, 0,14, 8, 0,  0, 0,10,14,14,12, 0, 0,  0, 0, 0, 0, 0, 0, 0 },
    // 4
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 6,14, 0, 0,  0, 0, 0, 4,14,14, 0, 0,  0, 0, 0,10, 8,14, 0, 0, 0,  0, 0,12, 0,14, 0, 0, 0,  0, 8,14,14,14,14,14, 0,  0, 0, 0, 0,14, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0 },
    // 5
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,12,14,14,14,14, 0,  0, 0,12, 0, 0, 0, 0, 0,  0, 0,10,14,14,12, 0, 0, 0,  0, 0, 0, 0, 0,14, 8, 0,  0, 0, 0, 0, 0,14, 8, 0,  0, 0,10,14,14,12, 0, 0,  0, 0, 0, 0, 0, 0, 0 },
    // 6
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 6,14, 8, 0, 0,  0, 0, 4,14, 0, 0, 0, 0,  0, 0,10,14,14,12, 0, 0, 0,  0, 0,14, 0, 0,14, 8, 0,  0, 0,14, 0, 0,14, 8, 0,  0, 0,10,14,14,12, 0, 0,  0, 0, 0, 0, 0, 0, 0 },
    // 7
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,12,14,14,14,14, 0,  0, 0, 0, 0, 0,14, 8, 0,  0, 0, 0, 0, 4,14, 0, 0, 0,  0, 0, 0, 0,14, 4, 0, 0,  0, 0, 0, 4,14, 0, 0, 0,  0, 0, 0, 8,14, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0 },
    // 8
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,10,14,14,12, 0, 0,  0, 0,14, 0, 0,14, 0, 0,  0, 0,10,14,14,12, 0, 0, 0,  0, 0,14, 0, 0,14, 0, 0,  0, 0,14, 0, 0,14, 0, 0,  0, 0,10,14,14,12, 0, 0,  0, 0, 0, 0, 0, 0, 0 },
    // 9
    { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,10,14,14,12, 0, 0,  0, 0,14, 0, 0,14, 0, 0,  0, 0,10,14,14,14, 0, 0, 0,  0, 0, 0, 0, 0,14, 0, 0,  0, 0, 0, 0, 6,14, 0, 0,  0, 0, 0, 4,14, 4, 0, 0,  0, 0, 0, 0, 0, 0, 0 },
};

void emitSample(const double pixels[64], int label) {
    std::cout << "in:";
    for (int i = 0; i < 64; ++i) {
        std::cout << " " << pixels[i];
    }
    std::cout << "\nout:";
    for (int d = 0; d < 10; ++d) {
        std::cout << " " << (d == label ? "1.0" : "0.0");
    }
    std::cout << "\n";
}

void generateEmbedded(int samplesPerDigit) {
    std::cout << "topology: 64 32 10\n";
    for (int digit = 0; digit < 10; ++digit) {
        for (int s = 0; s < samplesPerDigit; ++s) {
            double pixels[64];
            for (int i = 0; i < 64; ++i) {
                int base = DIGIT_PATTERNS[digit][i];
                // Add small random perturbation (±2)
                int noise = (std::rand() % 5) - 2;
                int val = base + noise;
                if (val < 0) val = 0;
                if (val > 16) val = 16;
                pixels[i] = val / 16.0;  // normalize to 0-1
            }
            emitSample(pixels, digit);
        }
    }
}

bool readUciFile(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;

    std::cout << "topology: 64 32 10\n";
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::vector<int> vals;
        std::stringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, ',')) {
            int v = 0;
            for (char c : tok) {
                if (std::isdigit(static_cast<unsigned char>(c)))
                    v = v * 10 + (c - '0');
            }
            vals.push_back(v);
        }
        if (vals.size() < 65) continue;
        int label = vals[64];
        if (label < 0 || label > 9) continue;
        double pixels[64];
        for (int i = 0; i < 64; ++i) {
            int v = vals[i];
            if (v > 16) v = 16;
            pixels[i] = v / 16.0;
        }
        emitSample(pixels, label);
    }
    return true;
}

}  // namespace

int main(int argc, char* argv[]) {
    std::srand(42);
    if (argc >= 2) {
        std::string path = argv[1];
        if (readUciFile(path)) {
            std::cerr << "Generated from " << path << "\n";
            return 0;
        }
        std::cerr << "Could not open " << path << ", using embedded data.\n";
    }
    generateEmbedded(50);  // 50 samples per digit = 500 total
    std::cerr << "Generated 500 embedded digit samples.\n";
    return 0;
}
