#ifndef UTILS_H
#define UTILS_H

#include <QSettings>
#include <QVariantMap>
#include <QStringList>

class utils
{
public:
    utils();
    static QVariantMap readini(const QString &ini);
    static QVariantMap readini(const QString &ini, const QString &group);
    static QStringList readtxt(const QString &txt);
};

#endif // UTILS_H
