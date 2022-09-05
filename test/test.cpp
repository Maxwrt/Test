#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QMap>
#include <QSharedPointer>
#include <QObject>
#include <QMetaEnum>
#include <QVariantMap>
#include <QVariant>

#include "inttest.h"

using Address = QString;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString exec_path = a.applicationDirPath();

    INTTEST *p_int1 = new INTTEST();
    QString key = p_int1->name;
    QMap<Address,QSharedPointer<INTTEST>> objects_;

    if (objects_.contains(key))
    {
        qDebug() << "p_int already exists" << p_int1->name;
    }

     p_int1->callResult();
//    p_int1->testEnum2Str();
//    p_int1->testRelativeFile();

    objects_[key].reset(p_int1, &QObject::deleteLater);
    qDebug() << "container size: " << objects_.count();
    objects_.clear();

    return a.exec();
}
