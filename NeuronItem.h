#ifndef NEURONITEM_H
#define NEURONITEM_H

#include <QGraphicsObject>
#include <QPen>
#include <QBrush>

class NeuronItem : public QGraphicsObject {
    Q_OBJECT

public:
    NeuronItem(size_t layer, size_t index, double value, bool isInput, bool isBias,
               const QColor &fillColor, double radius, QGraphicsItem *parent = nullptr);

    void setValue(double value);
    QRectF boundingRect() const override;

signals:
    void inputClicked(size_t index);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    size_t m_layer;
    size_t m_index;
    double m_value;
    bool m_isInput;
    bool m_isBias;
    QColor m_fillColor;
    double m_radius;
};

#endif // NEURONITEM_H
