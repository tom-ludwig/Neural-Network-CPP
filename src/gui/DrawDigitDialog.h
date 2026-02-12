#ifndef DRAWDIGITDIALOG_H
#define DRAWDIGITDIALOG_H

#include <QDialog>
#include <vector>

class Net;
class DrawDigitWidget;
class QLabel;

class DrawDigitDialog : public QDialog {
    Q_OBJECT

public:
    explicit DrawDigitDialog(Net *net, QWidget *parent = nullptr);

    std::vector<double> getDrawnValues() const;

signals:
    void applyRequested(const std::vector<double> &values);

private:
    void onPredict();
    void onApply();
    void onClear();

    Net *m_net;
    DrawDigitWidget *m_grid;
    QLabel *m_resultLabel;
};

#endif // DRAWDIGITDIALOG_H
