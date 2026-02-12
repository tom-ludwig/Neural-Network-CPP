//
// Created by Tommy Ludwig on 23.09.23.
//

#ifndef XORGATE_NEURALNETWORK_TRAININGDATA_H
#define XORGATE_NEURALNETWORK_TRAININGDATA_H


#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class TrainingData {
public:
    TrainingData(const std::string filename);

    bool isEof(void);

    void getTopology(std::vector<unsigned> &topology);

    unsigned getNextInputs(std::vector<double> &inputVals);

    unsigned getTargetOutputs(std::vector<double> &targetOutputVals);

    void reset();

    // Parse topology from file without consuming (for UI preview)
    static bool peekTopology(const std::string &filename, std::vector<unsigned> &topology);

private:
    std::ifstream m_trainingDataFile;
    std::string m_filename;
};


#endif //XORGATE_NEURALNETWORK_TRAININGDATA_H
