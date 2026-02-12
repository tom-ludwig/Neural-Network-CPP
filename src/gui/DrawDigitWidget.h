#ifndef DRAWDIGITWIDGET_H
#define DRAWDIGITWIDGET_H

#include <QWidget>
#include <vector>

class DrawDigitWidget : public QWidget {
    Q_OBJECT

public:
    explicit DrawDigitWidget(QWidget *parent = nullptr);

    std::vector<double> getValues() const;  // 64 values, row-major, 0.0-1.0
    void setValues(const std::vector<double> &values);
    void clear();

signals:
    void valuesChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setCellAt(const QPoint &pos, double value);
    int cellAt(const QPoint &pos) const;  // -1 if out of bounds

    static constexpr int GRID_SIZE = 8;
    static constexpr int CELL_PIXELS = 32;
    double m_cells[64];
};

#endif // DRAWDIGITWIDGET_H
