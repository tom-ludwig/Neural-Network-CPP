#include "DrawDigitDialog.h"
#include "DrawDigitWidget.h"
#include "Net.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

DrawDigitDialog::DrawDigitDialog(Net *net, QWidget *parent)
    : QDialog(parent)
    , m_net(net)
{
    setWindowTitle(tr("Draw Digit"));
    setModal(false);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Draw a digit (left=draw, right=erase):")));
    m_grid = new DrawDigitWidget(this);
    layout->addWidget(m_grid, 0, Qt::AlignCenter);

    m_resultLabel = new QLabel(tr("Result: —"));
    layout->addWidget(m_resultLabel, 0, Qt::AlignCenter);

    auto *btnLayout = new QHBoxLayout;
    auto *clearBtn = new QPushButton(tr("Clear"));
    auto *predictBtn = new QPushButton(tr("Predict"));
    auto *applyBtn = new QPushButton(tr("Use in main"));
    btnLayout->addWidget(clearBtn);
    btnLayout->addWidget(predictBtn);
    btnLayout->addWidget(applyBtn);
    layout->addLayout(btnLayout);

    connect(clearBtn, &QPushButton::clicked, this, &DrawDigitDialog::onClear);
    connect(predictBtn, &QPushButton::clicked, this, &DrawDigitDialog::onPredict);
    connect(applyBtn, &QPushButton::clicked, this, &DrawDigitDialog::onApply);

    predictBtn->setEnabled(m_net != nullptr);
    applyBtn->setEnabled(m_net != nullptr);
}

std::vector<double> DrawDigitDialog::getDrawnValues() const {
    return m_grid->getValues();
}

void DrawDigitDialog::onPredict() {
    if (!m_net || m_net->getLayerCount() == 0) return;
    auto vals = m_grid->getValues();
    if (vals.size() != 64) return;
    m_net->feedForward(vals);
    std::vector<double> results;
    m_net->getResults(results);
    int best = 0;
    for (size_t i = 1; i < results.size(); ++i) {
        if (results[i] > results[best]) best = static_cast<int>(i);
    }
    m_resultLabel->setText(tr("Result: Digit %1 (confidence: %2)")
        .arg(best).arg(results[best], 0, 'f', 2));
}

void DrawDigitDialog::onApply() {
    emit applyRequested(m_grid->getValues());
}

void DrawDigitDialog::onClear() {
    m_grid->clear();
    m_resultLabel->setText(tr("Result: —"));
}
