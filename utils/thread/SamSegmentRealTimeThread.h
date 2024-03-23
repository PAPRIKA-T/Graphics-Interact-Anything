#pragma once
#include <QThread>
class GraphicsScene;

class SamSegmentRealTimeThread : public QThread
{
    Q_OBJECT
public:
    SamSegmentRealTimeThread(GraphicsScene* s);
    ~SamSegmentRealTimeThread();
signals:
    void finishSamSegmentRealTime();
protected:
    void run() override;
public slots:
    void startSamSegmentRealTime();

private:
    GraphicsScene* m_scene = nullptr;
};

