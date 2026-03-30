//
// Created by Tommy Ludwig on 23.09.23.
//

#include "Neuron.h"
#include "Connection.h"
#include <cmath>
#include <cstddef>

Neuron::Neuron(const unsigned int numOutputs, const unsigned int myIndex) {
    for (unsigned connection = 0; connection < numOutputs; ++connection) {
        m_outputWeights.push_back(Connection()); // a random weight is assigned in the Connection constructor
    }
    m_myIndex = myIndex;
}

double Neuron::eta = 0.15; // overall net learning rate, [0.0..1.0]; 0.0 means: no learning, 1.0 means: learn at full
double Neuron::alpha = 0.5; // momentum, multiplier of last deltaWeight, [0.0..n]; 0.0 means: no momentum

void Neuron::setEta(const double value) { eta = value; }
void Neuron::setAlpha(const double value) { alpha = value; }
double Neuron::getEta() { return eta; }
double Neuron::getAlpha() { return alpha; }

void Neuron::setOutputVal(const double value) {
    m_outputVal = value;
}

double Neuron::getOutputVal() const {
    return m_outputVal;
}

// this function is used for calculating the output of the neurons
void Neuron::feedForward(const vector<Neuron> &prevLayer) {
    double sum = 0.0;

    for (const auto & neuron : prevLayer) {
        sum += neuron.getOutputVal() * neuron.m_outputWeights[m_myIndex].weight;
    }

    m_outputVal = Neuron::transferFunction(sum);
    //m_outputVal = Neuron::sigmoid(sum);
    //std::cout << "Neuron " << m_myIndex << " output: " << m_outputVal << std::endl;
}

double Neuron::sigmoid(const double x) {
    //sigmoid function
    return 1 / (1 + exp(-x));
}

double Neuron::ReLu(const double x) {
    return x > 0 ? x : 0;
}

double Neuron::transferFunction(const double x) {
    return tanh(x);
}

void Neuron::calculateOutputGradients(const double targetValue) {
    const double delta = targetValue - m_outputVal;
    m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::calculateHiddenGradients(const vector<Neuron> &nextLayer) {
    const double dow = sumDOW(nextLayer);
    m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::updateInputWeights(vector<Neuron> &prevLayer) const {
    // n stands for the index of the neuron
    for (auto & neuron : prevLayer) {
        const double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

        // TODO: Implement quicker method;
        const double newDeltaWeight = eta * neuron.getOutputVal() * m_gradient + alpha * oldDeltaWeight;

        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;

        //cout << "New weight: " << neuron.m_outputWeights[m_myIndex].weight << endl;
        //cout << "New delta weight: " << neuron.m_outputWeights[m_myIndex].deltaWeight << endl;
    }
}

vector<Connection> Neuron::getOutputWeights() const {
    return m_outputWeights;
}

double Neuron::transferFunctionDerivative(const double x) {
    return 1 - x * x;
}

double Neuron::sumDOW(const vector<Neuron> &nextLayer) const {
    double sum = 0.0;

    for (std::size_t neuron = 0; neuron < nextLayer.size() - 1; ++neuron) {
        sum += m_outputWeights[neuron].weight * nextLayer[neuron].m_gradient;
    }

    return sum;
}
