#ifndef INTTEST_H
#define INTTEST_H

#include <QObject>
#include <QMetaEnum>
#include <QObject>
#include <QThread>

class Work: public QObject
{
    Q_OBJECT
public:
    Work(int ar = 1, QObject *parent = nullptr);

public slots:
    void setResult(const QString &mark);

public:
    int ar_;
};

class INTTEST: public QObject
{
    Q_OBJECT
public:
    INTTEST(QObject *parent = nullptr);
    ~INTTEST();
    enum  Test{
        Test0,
        Test1,
        Test2,
        Test3,
        Test4
    };
    Q_ENUM(Test)

    void callResult();
    void testEnum2Str();
    void testRelativeFile(const QString &exec_path);

private:
#ifdef ASYNC
    void callASyncResut();
#else
    void callSyncResut();
#endif

signals:
    void getResult(const QString &mark);

public:
    QString name;

private:
#ifdef THREAD
    QThread thread;
#endif
    Work work;
};

#endif // INTTEST_H
