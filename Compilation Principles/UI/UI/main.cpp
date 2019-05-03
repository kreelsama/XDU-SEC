#include "ui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UI w;
	w.setWindowTitle(QStringLiteral("JYH's compiler"));
	w.show();
	return a.exec();
}
