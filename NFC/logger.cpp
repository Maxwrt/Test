#include "logger.h"
#include <QDateTime>
#include <QMutex>
#include <QDir>
#include <QTextStream>
#include <QMessageLogger>
#include <QApplication>

static QString logDir;
QFile logger::file;
void logger::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    switch(type)
    {
    case QtDebugMsg:
        text = QString("[%1 %2 %3 Debug]:%4").arg(current_date_time).arg(context.file).arg(context.line).arg(msg);
        break;
    case QtWarningMsg:
        text = QString("[%1 %2 %3 Warning]:%4").arg(current_date_time).arg(context.file).arg(context.line).arg(msg);
        break;
    case QtCriticalMsg:
        text = QString("[%1 %2 %3 Critical]:%4").arg(current_date_time).arg(context.file).arg(context.line).arg(msg);
        break;
    case QtFatalMsg:
        text = QString("[%1 %2 %3 Fatal]:%4").arg(current_date_time).arg(context.file).arg(context.line).arg(msg);
        break;
    case QtInfoMsg:
        text = QString("[%1 %2 %3 Info]:%4").arg(current_date_time).arg(context.file).arg(context.line).arg(msg);
        break;
    }

    QString filename = file.fileName();
    if(file.fileName() != QDateTime::currentDateTime().toString("yyyyMMdd"))
    {
        if (file.isOpen())
            file.close();
        file.setFileName(logDir + QDateTime::currentDateTime().toString("yyyyMMdd") + ".txt");
    }

    if (!file.isOpen())
        file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << text << "\r\n";

    file.flush();
    mutex.unlock();
}

logger::logger()
{
    qInstallMessageHandler(outputMessage);

    logDir = qApp->applicationDirPath() + "/log/";
    QDir dir(logDir);
    if(!dir.exists())
    {
        dir.mkdir(logDir);
    }
}

logger::~logger()
{
    if (file.isOpen())
    {
        QTextStream text_stream(&file);
        text_stream << "文件关闭" << "\r\n";
        file.flush();
        file.close();
    }
}

