#include "entercatch.h"

EnterCatch::EnterCatch(QObject *parent) : QObject(parent)
{}

EnterCatch::~EnterCatch()
{}

bool EnterCatch::eventFilter(QObject *obj, QEvent *event)
{
	if(event->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Return)
	{
		((MainWindow*)this->parent())->send_control();
		return true;
	}
	else
		return QObject::eventFilter(obj, event);
}
