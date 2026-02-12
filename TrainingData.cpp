//
// Created by Tommy Ludwig on 23.09.23.
//

#include "TrainingData.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

TrainingData::TrainingData(const std::string filename) : m_filename(filename) {
    m_trainingDataFile.open(filename.c_str());
    if (!m_trainingDataFile.is_open()) {
        std::cerr << "Error: training data file not found: " << filename << std::endl;
        throw std::runtime_error("Training data file not found");
    }
}

bool TrainingData::peekTopology(const std::string &filename, std::vector<unsigned> &topology) {
    topology.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string line;
    if (!std::getline(file, line)) return false;
    std::stringstream ss(line);
    std::string label;
    ss >> label;
    if (label != "topology:") return false;
    unsigned n;
    while (ss >> n) topology.push_back(n);
    return !topology.empty();
}

void TrainingData::reset() {
    m_trainingDataFile.close();
    m_trainingDataFile.open(m_filename);
    if (!m_trainingDataFile.is_open()) {
        throw std::runtime_error("Failed to reset training data file");
    }
    std::vector<unsigned> dummy;
    getTopology(dummy);
}

void TrainingData::getTopology(std::vector<unsigned> &topology) {
    topology.clear();
    std::string line;
    std::string label;

    if (!std::getline(m_trainingDataFile, line)) {
        return;
    }
    std::stringstream ss(line);
    ss >> label;

    if (label != "topology:") {
        return;
    }
    unsigned n;
    while (ss >> n) {
        topology.push_back(n);
    }
}

bool TrainingData::isEof(void) {
    return false;
}

unsigned TrainingData::getNextInputs(std::vector<double> &inputVals) {
    inputVals.clear();

    std::string line;
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("in:") == 0) {
        double oneValue;
        while (ss >> oneValue) {
            inputVals.push_back(oneValue);
        }
    }

    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(std::vector<double> &targetOutputVals) {
    targetOutputVals.clear();

    std::string line;
    std::getline(m_trainingDataFile, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("out:") == 0) {
        double oneValue;
        while (ss >> oneValue) {
            targetOutputVals.push_back(oneValue);
        }
    }
    return targetOutputVals.size();
}