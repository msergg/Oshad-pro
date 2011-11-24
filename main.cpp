#include <QtGui/QApplication>
#include <QTextCodec>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.show();

    //set codecs for dos text reading
     QTextCodec* tc = QTextCodec::codecForName("CP1251");
     QTextCodec::setCodecForTr(tc);
     QTextCodec::setCodecForCStrings(tc);
     QTextCodec::setCodecForLocale(tc);

    return a.exec();
}
