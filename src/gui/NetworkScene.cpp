#include "NetworkScene.h"
#include "NeuronItem.h"
#include "Net.h"
#include "Neuron.h"
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QGraphicsLineItem>
#include <vector>

namespace {
constexpr double NeuronRadius = 20.0;
constexpr double LayerSpacing = 120.0;
constexpr double MinLayerHeight = 80.0;
}

NetworkScene::NetworkScene(QObject *parent) : QGraphicsScene(parent) {
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

void NetworkScene::setNet(const Net *net) {
    m_net = net;
    paintNetwork();
}

void NetworkScene::paintNetwork() {
    clear();

    if (!m_net || m_net->getLayerCount() == 0) {
        return;
    }

    const size_t numLayers = m_net->getLayerCount();

    // Find max neurons in any layer for vertical centering
    size_t maxNeurons = 0;
    for (size_t l = 0; l < numLayers; ++l) {
        size_t count = m_net->getLayer(l).size();
        if (count > maxNeurons) maxNeurons = count;
    }

    double totalHeight = qMax(MinLayerHeight * static_cast<double>(maxNeurons - 1), MinLayerHeight);
    double neuronSpacing =
        maxNeurons > 1 ? totalHeight / static_cast<double>(maxNeurons - 1) : 0.0;

    // Get weight range for connection coloring
    double minWeight = 0, maxWeight = 0;
    bool firstWeight = true;
    for (size_t l = 0; l < numLayers - 1; ++l) {
        const auto &layer = m_net->getLayer(l);
        for (const auto & n : layer) {
            const auto &weights = n.getOutputWeights();
            for (const auto &conn : weights) {
                if (firstWeight) {
                    minWeight = maxWeight = conn.weight;
                    firstWeight = false;
                } else {
                    minWeight = qMin(minWeight, conn.weight);
                    maxWeight = qMax(maxWeight, conn.weight);
                }
            }
        }
    }
    double weightRange = maxWeight - minWeight;
    if (weightRange < 0.001) weightRange = 1.0;

    struct Pos { double x, y; };
    std::vector<std::vector<Pos>> positions(numLayers);

    for (size_t l = 0; l < numLayers; ++l) {
        size_t neuronCount = m_net->getLayer(l).size();
        const double layerHeight =
            (neuronCount > 1) ? static_cast<double>(neuronCount - 1) * neuronSpacing : 0.0;
        const double startY = -layerHeight / 2.0;

        for (size_t n = 0; n < neuronCount; ++n) {
            const double x = static_cast<double>(l) * LayerSpacing;
            const double y = startY + static_cast<double>(n) * neuronSpacing;
            positions[l].push_back({x, y});
        }
    }

    // Draw connections first (behind neurons)
    for (size_t l = 0; l < numLayers - 1; ++l) {
        const auto &layer = m_net->getLayer(l);
        const auto &nextPositions = positions[l + 1];

        for (size_t n = 0; n < layer.size(); ++n) {
            const auto &weights = layer[n].getOutputWeights();
            for (size_t c = 0; c < weights.size() && c < nextPositions.size(); ++c) {
                const double w = weights[c].weight;
                const double t = (w - minWeight) / weightRange;
                const int r = static_cast<int>(t * 255);
                constexpr int g = 100;
                const int b = static_cast<int>((1 - t) * 255);
                QColor color(qBound(0, r, 255), qBound(0, g, 255), qBound(0, b, 255));
                const double thickness = 0.5 + 2.0 * (0.5 + 0.5 * (w - minWeight) / weightRange);

                auto *line = addLine(
                    positions[l][n].x + NeuronRadius,
                    positions[l][n].y,
                    nextPositions[c].x - NeuronRadius,
                    nextPositions[c].y,
                    QPen(color, static_cast<qreal>(thickness))
                );
                line->setZValue(-1);
            }
        }
    }

    // Draw neurons
    for (size_t l = 0; l < numLayers; ++l) {
        const auto &layer = m_net->getLayer(l);
        bool isInput = (l == 0);
        bool isOutput = (l == numLayers - 1);

        for (size_t n = 0; n < layer.size(); ++n) {
            bool isBias = (n == layer.size() - 1);  // last neuron in each layer is bias
            double outVal = layer[n].getOutputVal();
            QColor fillColor;
            if (isBias) {
                fillColor = QColor(180, 180, 200);
            } else if (isInput) {
                fillColor = QColor(100, 180, 255);
            } else if (isOutput) {
                fillColor = QColor(255, 150, 100);
            } else {
                double t = (outVal + 1.0) / 2.0;
                fillColor = QColor(
                    static_cast<int>(255 * (1 - t)),
                    200,
                    static_cast<int>(255 * t)
                );
            }

            auto *item = new NeuronItem(l, n, outVal, isInput, isBias, fillColor, NeuronRadius);
            item->setPos(positions[l][n].x, positions[l][n].y);
            item->setZValue(1);
            addItem(item);

            if (isInput && !isBias) {
                connect(item, &NeuronItem::inputClicked, this, [this](size_t index) {
                    emit inputNeuronClicked(index);
                });
            }
        }
    }
}
