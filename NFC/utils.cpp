#include "utils.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>

utils::utils()
{

}

QVariantMap utils::readini(const QString &ini)
{
    QVariantMap map;
    QSettings set(ini, QSettings::IniFormat);
    for (auto key : set.allKeys()) {
        map.insert(key, set.value(key));
    }
    return map;
}

QVariantMap utils::readini(const QString &ini, const QString &group)
{
    QVariantMap map;
    QSettings set(ini, QSettings::IniFormat);
    set.beginGroup(group);
    for (auto key : set.allKeys()) {
        map.insert(key, set.value(key));
    }
    set.endGroup();
    return map;
}

QStringList utils::readtxt(const QString &txt)
{
    QStringList bottle_ens;

    QFile file(txt);
    QTextStream read(&file);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = read.readAll().simplified();
    file.close();
    bottle_ens = content.split(',', QString::SkipEmptyParts);
    bottle_ens.sort(Qt::CaseSensitive);
    return bottle_ens;
}
