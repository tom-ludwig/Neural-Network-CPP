#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>

class Net;
class NetworkScene;
class DrawDigitDialog;
class TrainingData;
class QGraphicsView;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QListWidget;
class QVBoxLayout;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onBrowseTrainingData();
    void onLoadTopologyFromFile();
    void onAddLayer();
    void onRemoveLayer();
    void onTrain();
    void onCreateNetwork();
    void onPredict();
    void onInputNeuronClicked(size_t index);
    void onDrawDigit();
    void onDrawDigitApply(const std::vector<double> &values);

private:
    QWidget *setupTopologyPanel(QWidget *parent);
    QWidget *setupTrainingPanel(QWidget *parent);
    void setupNetworkView(QWidget *parent);
    void refreshNetworkVisualization();
    void refreshPredictInputs();
    std::vector<unsigned> getTopologyFromUi() const;
    bool validateAndPrepareTraining();

    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;
    NetworkScene *m_networkScene;
    QGraphicsView *m_networkView;

    QLineEdit *m_trainingDataPath;
    QListWidget *m_topologyList;
    QSpinBox *m_epochsSpin;
    QDoubleSpinBox *m_etaSpin;
    QDoubleSpinBox *m_alphaSpin;
    QPushButton *m_trainButton;
    QPushButton *m_browseButton;
    QPushButton *m_createNetButton;
    QPushButton *m_predictButton;
    QPushButton *m_drawDigitButton;
    QLabel *m_statusLabel;
    QLabel *m_errorLabel;
    QLabel *m_outputLabel;
    QWidget *m_predictInputsContainer;
    std::vector<QDoubleSpinBox *> m_inputSpins;

    std::unique_ptr<Net> m_net;
    std::unique_ptr<TrainingData> m_trainingData;
};

#endif // MAINWINDOW_H
