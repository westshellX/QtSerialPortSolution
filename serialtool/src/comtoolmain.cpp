#include <QApplication>
#include <QtCore>
#include <QTextCodec>

#include "serialtool.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	Q_INIT_RESOURCE(myresource);
	
	MyComTool tool;
	tool.show();
	
	return app.exec();	
}
