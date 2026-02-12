#ifndef NETWORKSCENE_H
#define NETWORKSCENE_H

#include <QGraphicsScene>

class Net;
class NeuronItem;

class NetworkScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit NetworkScene(QObject *parent = nullptr);

    void setNet(const Net *net);
    void paintNetwork();

signals:
    void inputNeuronClicked(size_t index);

private:
    const Net *m_net = nullptr;
};

#endif // NETWORKSCENE_H
