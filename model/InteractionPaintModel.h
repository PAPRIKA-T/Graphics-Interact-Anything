#pragma once

#include <QObject>

class InteractionPaintModel  : public QObject
{
	Q_OBJECT

public:
	InteractionPaintModel(QObject *parent);
	~InteractionPaintModel();
};
