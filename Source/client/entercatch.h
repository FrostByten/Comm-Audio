#ifndef ENTERCATCH_H
#define ENTERCATCH_H

#include <QObject>
#include <QKeyEvent>
#include "mainwindow.h"

class EnterCatch : public QObject
{
		Q_OBJECT
	public:
		explicit EnterCatch(QObject *parent = 0);
		~EnterCatch();
	protected:
		bool eventFilter(QObject *obj, QEvent *event);

	signals:

	public slots:
};

#endif // ENTERCATCH_H

