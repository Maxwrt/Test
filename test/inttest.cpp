#include "inttest.h"
#include <QString>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

Work::Work(int ar, QObject *parent):QObject (parent)
{
    ar_ = ar;
}

void Work::setResult(const QString &mark)
{
    qDebug() << QString("%1 filename:%2 line:%3 function:%4").arg(mark).arg(QFileInfo(__FILE__).fileName()).arg(__LINE__).arg(__FUNCTION__);
    qDebug() << "work thread'id: " << QThread::currentThreadId();
}

INTTEST::INTTEST(QObject *parent):QObject (parent)
{
    name = typeid (*this).name();
#ifdef ASYNC
    auto ret = connect(this, SIGNAL(getResult(const QString&)), &work, SLOT(setResult(const QString&)), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
#else
#ifdef DIFF_THREAD_SYNC_BLOCK
    auto ret = connect(this, SIGNAL(getResult(const QString&)), &work, SLOT(setResult(const QString&)), Qt::ConnectionType(Qt::BlockingQueuedConnection | Qt::UniqueConnection));
#else
    auto ret = connect(this, SIGNAL(getResult(const QString&)), &work, SLOT(setResult(const QString&)), Qt::ConnectionType(Qt::DirectConnection | Qt::UniqueConnection));
#endif
#endif
    if (ret)
        qDebug() << "connect is valid";
    else
        qDebug() << "connect is invalid";
#ifdef THREAD
    work.moveToThread(&thread);
    thread.start();
#endif
    qDebug() << "main thread'id: " << QThread::currentThreadId();
}

INTTEST::~INTTEST()
{
#ifdef THREAD
    if (thread.isRunning())
    {
        thread.wait();
        thread.quit();
    }
#endif
}

void INTTEST::callResult()
{
#ifdef ASYNC
    callASyncResut();
#else
    callSyncResut();
#endif
}

#ifdef ASYNC
void INTTEST::callASyncResut()
{
#ifdef THREAD
    if (!thread.isRunning())
        thread.isRunning();
    else
        qDebug() << "work is running";
    QString mark{"diff thread async"};
#else
    QString mark{"same thread async"};
#endif
    qDebug() << "callASyncResut emit start";
    emit getResult(mark);
    qDebug() << "callASyncResut emit end";
}
#else
void INTTEST::callSyncResut()
{
#ifdef THREAD
    if (!thread.isRunning())
        thread.start();
    else
        qDebug() << "work is running";
    QString mark{"new thread sync"};
#else
    QString mark{"same thread sync"};
#endif
    qDebug() << "callSyncResut emit getResult start";
    emit getResult(mark);
    qDebug() << "callSyncResut emit getResult end";
}
#endif


void INTTEST::testEnum2Str()
{
    QVariantMap env;
    QMetaEnum e = QMetaEnum::fromType<INTTEST::Test>();
    env[e.valueToKey(INTTEST::Test1)] = QVariant("Model");
    env[e.valueToKey(INTTEST::Test2)] = QVariant("Slave");
    env[e.valueToKey(INTTEST::Test3)] = QVariant("Platform");
    env[e.valueToKey(INTTEST::Test0)] = QVariant("Projector");
    qDebug() << "env: " << env;
}

void INTTEST::testRelativeFile(const QString &exec_path)
{
    qDebug() << "exec_path: "<< exec_path;
    QStringList fileterlist;
    fileterlist << "*";
    QDir dir(exec_path);
    int i=0;
    foreach(auto item, dir.entryList(fileterlist))
    {
        qDebug() << "item" << ++i << " " << item;
        if (QDir::isRelativePath(item))
        {
            qDebug() << "isRelative: " << dir.filePath(exec_path);
        }
        else
        {
            qDebug() << "isAbsolute: " << dir.filePath(exec_path);
        }
    }
}
