#include "DopaChessGui.h"
#include <QtWidgets/QApplication>


//#pragma comment(linker, "/SUBSYSTEM:console")

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DopaChessGui w;
	w.show();

	return a.exec();
}
