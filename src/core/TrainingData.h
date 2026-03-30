//
// Created by Tommy Ludwig on 23.09.23.
//

#ifndef XORGATE_NEURALNETWORK_TRAININGDATA_H
#define XORGATE_NEURALNETWORK_TRAININGDATA_H


#include <cstddef>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class TrainingData {
public:
    explicit TrainingData(const std::string &filename);

    static bool isEof();

    void getTopology(std::vector<unsigned> &topology);

    std::size_t getNextInputs(std::vector<double> &inputVals);

    std::size_t getTargetOutputs(std::vector<double> &targetOutputVals);

    void reset();

    // Parse topology from file without consuming (for UI preview)
    static bool peekTopology(const std::string &filename, std::vector<unsigned> &topology);

private:
    std::ifstream m_trainingDataFile;
    std::string m_filename;
};


#endif //XORGATE_NEURALNETWORK_TRAININGDATA_H
