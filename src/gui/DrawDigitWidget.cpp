#include "DrawDigitWidget.h"
#include <QPainter>
#include <QMouseEvent>

DrawDigitWidget::DrawDigitWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(CELL_PIXELS * GRID_SIZE + 2, CELL_PIXELS * GRID_SIZE + 2);
    setMouseTracking(false);
    clear();
}

std::vector<double> DrawDigitWidget::getValues() const {
    std::vector<double> v(64);
    for (int i = 0; i < 64; ++i) v[i] = m_cells[i];
    return v;
}

void DrawDigitWidget::setValues(const std::vector<double> &values) {
    for (size_t i = 0; i < 64 && i < values.size(); ++i) {
        m_cells[i] = qBound(0.0, values[i], 1.0);
    }
    update();
}

void DrawDigitWidget::clear() {
    for (int i = 0; i < 64; ++i) m_cells[i] = 0.0;
    update();
}

void DrawDigitWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.fillRect(rect(), Qt::white);
    p.setPen(Qt::lightGray);
    for (int i = 0; i <= GRID_SIZE; ++i) {
        int pos = i * CELL_PIXELS + 1;
        p.drawLine(pos, 1, pos, height() - 1);
        p.drawLine(1, pos, width() - 1, pos);
    }
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            int idx = r * GRID_SIZE + c;
            double v = m_cells[idx];
            if (v > 0.01) {
                int gray = static_cast<int>(255 * (1.0 - v));
                p.fillRect(c * CELL_PIXELS + 2, r * CELL_PIXELS + 2,
                           CELL_PIXELS - 1, CELL_PIXELS - 1,
                           QColor(gray, gray, gray));
            }
        }
    }
}

void DrawDigitWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        setCellAt(event->pos(), 1.0);
    } else if (event->button() == Qt::RightButton) {
        setCellAt(event->pos(), 0.0);
    }
}

void DrawDigitWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        setCellAt(event->pos(), 1.0);
    } else if (event->buttons() & Qt::RightButton) {
        setCellAt(event->pos(), 0.0);
    }
}

void DrawDigitWidget::setCellAt(const QPoint &pos, double value) {
    int idx = cellAt(pos);
    if (idx >= 0 && m_cells[idx] != value) {
        m_cells[idx] = value;
        update();
        emit valuesChanged();
    }
}

int DrawDigitWidget::cellAt(const QPoint &pos) const {
    int c = (pos.x() - 1) / CELL_PIXELS;
    int r = (pos.y() - 1) / CELL_PIXELS;
    if (r >= 0 && r < GRID_SIZE && c >= 0 && c < GRID_SIZE) {
        return r * GRID_SIZE + c;
    }
    return -1;
}
