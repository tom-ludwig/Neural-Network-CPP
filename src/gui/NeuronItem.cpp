#include "NeuronItem.h"
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>

NeuronItem::NeuronItem(size_t layer, size_t index, double value, bool isInput, bool isBias,
                       const QColor &fillColor, double radius, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_layer(layer)
    , m_index(index)
    , m_value(value)
    , m_isInput(isInput)
    , m_isBias(isBias)
    , m_fillColor(fillColor)
    , m_radius(radius)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    if (isInput && !isBias) {
        setCursor(Qt::PointingHandCursor);
    }
}

void NeuronItem::setValue(double value) {
    if (qAbs(m_value - value) > 1e-9) {
        m_value = value;
        update();
    }
}

QRectF NeuronItem::boundingRect() const {
    return QRectF(-m_radius - 2, -m_radius - 2, (m_radius + 2) * 2, (m_radius + 2) * 2);
}

void NeuronItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::darkGray, 2));
    painter->setBrush(QBrush(m_fillColor));
    painter->drawEllipse(QPointF(0, 0), m_radius, m_radius);

    QString valueStr = QString::number(m_value, 'f', 2);
    painter->setPen(Qt::black);
    QFont font = painter->font();
    font.setPointSize(8);
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, valueStr);
}

void NeuronItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isInput && !m_isBias && event->button() == Qt::LeftButton) {
        event->accept();
        emit inputClicked(m_index);
    } else {
        QGraphicsObject::mousePressEvent(event);
    }
}

void NeuronItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event);
    if (m_isInput && !m_isBias) {
        update();
    }
}

void NeuronItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event);
    if (m_isInput && !m_isBias) {
        update();
    }
}
