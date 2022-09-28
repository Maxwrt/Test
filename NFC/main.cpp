#include "nfc.h"
#include "logger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logger::outputMessage);
    QApplication a(argc, argv);
    NFC nfc;
    nfc.show();

    return a.exec();
}
