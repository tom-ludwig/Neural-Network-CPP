#include "MainWindow.h"
#include "NetworkScene.h"
#include "Net.h"
#include "Neuron.h"
#include "TrainingData.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include <QScrollArea>
#include <QFrame>
#include <QSplitter>
#include <QApplication>
#include <QTimer>
#include <QMetaObject>
#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Neural Network Visualizer"));
    resize(1000, 700);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    m_mainLayout = new QVBoxLayout(m_centralWidget);

    auto *splitter = new QSplitter(Qt::Horizontal);

    // Left panel: controls
    auto *leftPanel = new QFrame;
    leftPanel->setFrameShape(QFrame::StyledPanel);
    leftPanel->setMinimumWidth(280);
    leftPanel->setMaximumWidth(350);
    auto *leftLayout = new QVBoxLayout(leftPanel);

    auto *topologyGroup = setupTopologyPanel(leftPanel);
    auto *trainingGroup = setupTrainingPanel(leftPanel);
    leftLayout->addWidget(topologyGroup);
    leftLayout->addWidget(trainingGroup);
    leftLayout->addStretch();

    splitter->addWidget(leftPanel);

    // Right panel: network visualization
    setupNetworkView(m_centralWidget);
    splitter->addWidget(m_networkView);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    m_mainLayout->addWidget(splitter);

}

MainWindow::~MainWindow() = default;

QWidget *MainWindow::setupTopologyPanel(QWidget *parent) {
    auto *group = new QGroupBox(tr("Topology"), parent);
    auto *layout = new QVBoxLayout(group);

    m_topologyList = new QListWidget;
    m_topologyList->setMaximumHeight(150);
    m_topologyList->setEditTriggers(QAbstractItemView::DoubleClicked);
    layout->addWidget(m_topologyList);

    auto *btnLayout = new QHBoxLayout;
    auto *addBtn = new QPushButton(tr("Add layer"));
    auto *removeBtn = new QPushButton(tr("Remove layer"));
    auto *loadBtn = new QPushButton(tr("Load from file"));
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(removeBtn);
    btnLayout->addWidget(loadBtn);
    layout->addLayout(btnLayout);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddLayer);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveLayer);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadTopologyFromFile);

    // Default topology for XOR
    for (int v : {2, 4, 1}) {
        auto *item = new QListWidgetItem(QString::number(v));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        m_topologyList->addItem(item);
    }

    return group;
}

QWidget *MainWindow::setupTrainingPanel(QWidget *parent) {
    auto *dataGroup = new QGroupBox(tr("Training Data"), parent);
    auto *dataLayout = new QFormLayout(dataGroup);
    m_trainingDataPath = new QLineEdit;
    m_trainingDataPath->setPlaceholderText(tr("Select a training data file..."));
    m_browseButton = new QPushButton(tr("Browse"));
    auto *pathRow = new QHBoxLayout;
    pathRow->addWidget(m_trainingDataPath);
    pathRow->addWidget(m_browseButton);
    dataLayout->addRow(tr("File:"), pathRow);
    connect(m_browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseTrainingData);

    auto *paramsGroup = new QGroupBox(tr("Training Parameters"), parent);
    auto *paramsLayout = new QFormLayout(paramsGroup);
    m_epochsSpin = new QSpinBox;
    m_epochsSpin->setRange(1, 1000000);
    m_epochsSpin->setValue(1000);
    m_epochsSpin->setSingleStep(100);
    paramsLayout->addRow(tr("Epochs:"), m_epochsSpin);

    m_etaSpin = new QDoubleSpinBox;
    m_etaSpin->setRange(0.001, 1.0);
    m_etaSpin->setValue(0.15);
    m_etaSpin->setDecimals(3);
    m_etaSpin->setSingleStep(0.01);
    paramsLayout->addRow(tr("Learning rate (η):"), m_etaSpin);

    m_alphaSpin = new QDoubleSpinBox;
    m_alphaSpin->setRange(0.0, 1.0);
    m_alphaSpin->setValue(0.5);
    m_alphaSpin->setDecimals(3);
    m_alphaSpin->setSingleStep(0.05);
    paramsLayout->addRow(tr("Momentum (α):"), m_alphaSpin);

    m_trainButton = new QPushButton(tr("Train"));
    m_createNetButton = new QPushButton(tr("Create Network"));
    m_statusLabel = new QLabel(tr("Ready"));
    m_errorLabel = new QLabel;

    connect(m_trainButton, &QPushButton::clicked, this, &MainWindow::onTrain);
    connect(m_createNetButton, &QPushButton::clicked, this, &MainWindow::onCreateNetwork);

    auto *predictGroup = new QGroupBox(tr("Test / Predict"), parent);
    auto *predictLayout = new QVBoxLayout(predictGroup);
    m_predictInputsContainer = new QWidget(predictGroup);
    auto *inputsLayout = new QVBoxLayout(m_predictInputsContainer);
    inputsLayout->setContentsMargins(0, 0, 0, 0);
    predictLayout->addWidget(m_predictInputsContainer);
    m_predictButton = new QPushButton(tr("Run"));
    m_predictButton->setEnabled(false);
    connect(m_predictButton, &QPushButton::clicked, this, &MainWindow::onPredict);
    predictLayout->addWidget(m_predictButton);
    m_outputLabel = new QLabel(tr("Output: —"));
    predictLayout->addWidget(m_outputLabel);

    auto *container = new QWidget(parent);
    auto *layout = new QVBoxLayout(container);
    layout->addWidget(dataGroup);
    layout->addWidget(paramsGroup);
    layout->addWidget(m_createNetButton);
    layout->addWidget(m_trainButton);
    layout->addWidget(m_statusLabel);
    layout->addWidget(m_errorLabel);
    layout->addWidget(predictGroup);

    return container;
}

void MainWindow::setupNetworkView(QWidget *parent) {
    m_networkScene = new NetworkScene(this);
    connect(m_networkScene, &NetworkScene::inputNeuronClicked, this, &MainWindow::onInputNeuronClicked);
    m_networkView = new QGraphicsView(m_networkScene, parent);
    m_networkView->setRenderHint(QPainter::Antialiasing);
    m_networkView->setRenderHint(QPainter::SmoothPixmapTransform);
    m_networkView->setDragMode(QGraphicsView::ScrollHandDrag);
    m_networkView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_networkView->setBackgroundBrush(QColor(245, 245, 250));
    m_networkView->setSceneRect(-50, -200, 500, 400);
}

void MainWindow::onBrowseTrainingData() {
    QString path = QFileDialog::getOpenFileName(
        this,
        tr("Select Training Data"),
        QString(),
        tr("Text files (*.txt);;All files (*)")
    );
    if (!path.isEmpty()) {
        m_trainingDataPath->setText(path);
    }
}

void MainWindow::onLoadTopologyFromFile() {
    QString path = m_trainingDataPath->text().trimmed();
    if (path.isEmpty()) {
        QMessageBox::information(this, tr("Load Topology"),
            tr("Please select a training data file first."));
        return;
    }
    std::vector<unsigned> topology;
    if (!TrainingData::peekTopology(path.toStdString(), topology)) {
        QMessageBox::warning(this, tr("Load Topology"),
            tr("Could not read topology from file."));
        return;
    }
    m_topologyList->clear();
    for (unsigned n : topology) {
        auto *item = new QListWidgetItem(QString::number(n));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        m_topologyList->addItem(item);
    }
}

void MainWindow::onAddLayer() {
    int count = m_topologyList->count();
    int value = (count > 0) ? 4 : 2;
    auto *item = new QListWidgetItem(QString::number(value));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    m_topologyList->addItem(item);
}

void MainWindow::onRemoveLayer() {
    if (m_topologyList->count() > 1) {
        delete m_topologyList->takeItem(m_topologyList->count() - 1);
    }
}

std::vector<unsigned> MainWindow::getTopologyFromUi() const {
    std::vector<unsigned> topo;
    for (int i = 0; i < m_topologyList->count(); ++i) {
        bool ok;
        int v = m_topologyList->item(i)->text().toInt(&ok);
        if (ok && v > 0) topo.push_back(static_cast<unsigned>(v));
    }
    return topo;
}

bool MainWindow::validateAndPrepareTraining() {
    auto topology = getTopologyFromUi();
    if (topology.size() < 2) {
        QMessageBox::warning(this, tr("Training"),
            tr("Topology must have at least 2 layers (input and output)."));
        return false;
    }
    for (size_t i = 0; i < topology.size(); ++i) {
        if (topology[i] == 0) {
            QMessageBox::warning(this, tr("Training"),
                tr("Each layer must have at least 1 neuron (layer %1 has 0).").arg(static_cast<int>(i) + 1));
            return false;
        }
    }
    QString path = m_trainingDataPath->text().trimmed();
    if (path.isEmpty()) {
        QMessageBox::warning(this, tr("Training"),
            tr("Please select a training data file."));
        return false;
    }
    try {
        std::vector<unsigned> fileTopology;
        if (TrainingData::peekTopology(path.toStdString(), fileTopology) &&
            !fileTopology.empty() && (fileTopology[0] != topology[0] || fileTopology.back() != topology.back())) {
            QMessageBox::StandardButton r = QMessageBox::question(this, tr("Topology Mismatch"),
                tr("The training file's topology differs from yours. Continue anyway?"),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (r == QMessageBox::No) return false;
        }
        m_trainingData = std::make_unique<TrainingData>(path.toStdString());
        std::vector<unsigned> dummy;
        m_trainingData->getTopology(dummy);
        m_net = std::make_unique<Net>(topology);
        Neuron::setEta(m_etaSpin->value());
        Neuron::setAlpha(m_alphaSpin->value());
        m_trainingData->reset();
        return true;
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Error"), QString::fromStdString(e.what()));
        return false;
    }
}

void MainWindow::onTrain() {
    if (!validateAndPrepareTraining()) return;

    auto topology = getTopologyFromUi();
    int epochs = m_epochsSpin->value();

    refreshPredictInputs();
    refreshNetworkVisualization();
    m_trainButton->setEnabled(false);
    m_statusLabel->setText(tr("Training..."));

    std::thread worker([this, topology, epochs]() {
        int currentEpoch = 0;
        std::vector<double> inputVals, targetVals, resultVals;
        const int updateInterval = 100;

        while (currentEpoch < epochs) {
            unsigned count = m_trainingData->getNextInputs(inputVals);
            if (count != topology[0]) {
                currentEpoch++;
                if (currentEpoch >= epochs) break;
                m_trainingData->reset();
                count = m_trainingData->getNextInputs(inputVals);
                if (count != topology[0]) break;
            }
            m_net->feedForward(inputVals);
            m_net->getResults(resultVals);
            m_trainingData->getTargetOutputs(targetVals);
            if (targetVals.size() == topology.back()) {
                m_net->backPropagate(targetVals);
            }

            if (currentEpoch > 0 && currentEpoch % updateInterval == 0) {
                double err = m_net->getRecentAverageError();
                int epoch = currentEpoch;
                QMetaObject::invokeMethod(this, [this, epoch, epochs, err]() {
                    m_statusLabel->setText(tr("Training... (epoch %1 / %2)").arg(epoch).arg(epochs));
                    m_errorLabel->setText(tr("Recent error: %1").arg(err));
                    refreshNetworkVisualization();
                }, Qt::QueuedConnection);
            }
        }

        double finalError = m_net->getRecentAverageError();
        QMetaObject::invokeMethod(this, [this, finalError]() {
            m_trainButton->setEnabled(true);
            m_statusLabel->setText(tr("Training complete"));
            m_errorLabel->setText(tr("Final error: %1").arg(finalError));
            refreshPredictInputs();
            refreshNetworkVisualization();
        }, Qt::QueuedConnection);
    });
    worker.detach();
}

void MainWindow::refreshNetworkVisualization() {
    if (m_net) {
        m_networkScene->setNet(m_net.get());
    } else {
        m_networkScene->setNet(nullptr);
    }
}

void MainWindow::refreshPredictInputs() {
    for (auto *spin : m_inputSpins) {
        spin->deleteLater();
    }
    m_inputSpins.clear();
    if (!m_net || m_net->getLayerCount() == 0) return;
    const size_t numInputs = m_net->getLayer(0).size() - 1;  // exclude bias
    auto *layout = m_predictInputsContainer->layout();
    for (size_t i = 0; i < numInputs; ++i) {
        auto *spin = new QDoubleSpinBox(m_predictInputsContainer);
        spin->setRange(-10.0, 10.0);
        spin->setDecimals(4);
        spin->setSingleStep(0.1);
        spin->setValue(m_net->getLayer(0)[i].getOutputVal());
        spin->setPrefix(tr("In %1: ").arg(static_cast<int>(i) + 1));
        layout->addWidget(spin);
        m_inputSpins.push_back(spin);
    }
    m_predictButton->setEnabled(true);
}

void MainWindow::onCreateNetwork() {
    auto topology = getTopologyFromUi();
    if (topology.size() < 2) {
        QMessageBox::warning(this, tr("Create Network"),
            tr("Topology must have at least 2 layers (input and output)."));
        return;
    }
    for (size_t i = 0; i < topology.size(); ++i) {
        if (topology[i] == 0) {
            QMessageBox::warning(this, tr("Create Network"),
                tr("Each layer must have at least 1 neuron (layer %1 has 0).").arg(static_cast<int>(i) + 1));
            return;
        }
    }
    Neuron::setEta(m_etaSpin->value());
    Neuron::setAlpha(m_alphaSpin->value());
    m_net = std::make_unique<Net>(topology);
    refreshPredictInputs();
    refreshNetworkVisualization();
    m_statusLabel->setText(tr("Network created (untrained)"));
}

void MainWindow::onInputNeuronClicked(size_t index) {
    if (!m_net || m_net->getLayerCount() == 0) return;
    const size_t numInputs = m_net->getLayer(0).size() - 1;
    if (index >= numInputs) return;
    double currentVal = m_net->getLayer(0)[index].getOutputVal();
    bool ok;
    double newVal = QInputDialog::getDouble(this, tr("Set Input Value"),
        tr("Enter value for input neuron %1:").arg(static_cast<int>(index) + 1),
        currentVal, -10.0, 10.0, 2, &ok);
    if (!ok) return;
    std::vector<double> inputVals;
    for (size_t i = 0; i < numInputs; ++i) {
        inputVals.push_back(i == index ? newVal : m_net->getLayer(0)[i].getOutputVal());
    }
    m_net->feedForward(inputVals);
    std::vector<double> resultVals;
    m_net->getResults(resultVals);
    QString outText = tr("Output: ");
    for (size_t i = 0; i < resultVals.size(); ++i) {
        if (i > 0) outText += QStringLiteral(", ");
        outText += QString::number(resultVals[i], 'f', 4);
    }
    m_outputLabel->setText(outText);
    refreshPredictInputs();
    refreshNetworkVisualization();
}

void MainWindow::onPredict() {
    if (!m_net) return;
    std::vector<double> inputVals;
    for (auto *spin : m_inputSpins) {
        inputVals.push_back(spin->value());
    }
    if (inputVals.size() != m_net->getLayer(0).size() - 1) {
        m_outputLabel->setText(tr("Output: (input count mismatch)"));
        return;
    }
    m_net->feedForward(inputVals);
    std::vector<double> resultVals;
    m_net->getResults(resultVals);
    QString outText = tr("Output: ");
    for (size_t i = 0; i < resultVals.size(); ++i) {
        if (i > 0) outText += QStringLiteral(", ");
        outText += QString::number(resultVals[i], 'f', 4);
    }
    m_outputLabel->setText(outText);
    refreshNetworkVisualization();
}
