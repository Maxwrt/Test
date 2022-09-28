#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>

class logger
{
public:
    logger();
    ~logger();
    static void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QFile file;
};

#endif // LOGGER_H
