#include "SamSegmentRealTimeThread.h"
#include "graphs/GraphicsScene.h"
#include "graphs/GraphicsView.h"

SamSegmentRealTimeThread::SamSegmentRealTimeThread(GraphicsScene* s)
    : m_scene(s)
{
}

SamSegmentRealTimeThread::~SamSegmentRealTimeThread()
{
}

void SamSegmentRealTimeThread::run()
{
    m_scene->samSegmentRealTime();
}

void SamSegmentRealTimeThread::startSamSegmentRealTime()
{
    start();
}