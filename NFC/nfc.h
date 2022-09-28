#ifndef NFC_H
#define NFC_H

#include "logger.h"
#include "Module/farme.h"
#include <QJsonObject>
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <QDebug>
#include <QSharedPointer>


//S01B2112186A1
class QTimer;
class logger;
#define ONE_WRITE_SIZE  48
#define BOTTLE_NFC QString("树脂瓶标签信息")
#define MO_NFC  QString("离型膜标签信息")
const quint8   PROTOCOL_HEAD  = 0xAA;
const quint8   PROTOCOL_TAIL  = 0x55;
const quint8   key = 0xFF;
namespace sys {
enum Command:quint8{
    card_notify = 0x01,
    sector_read = 0x02,
    sector_write = 0x04,
};

enum Sate:quint8{
    normal = 0x01,
    write_error = 0x05,
    read_error = 0x07,
};

enum WhoUI:quint8{
    bolttle = 0x00,
    mo = 0x01,
};
}

struct MO_INFO
{
    uint8_t id = sys::WhoUI::mo;
    uint8_t sn[32] = {0};
    uint8_t type[16] = {0};
    uint32_t produceDate = 0;
    uint32_t expireDate = 0;
    uint32_t peelLeftCount = 0;
};

struct BOTTLE_INFO
{
    uint8_t id = sys::WhoUI::bolttle;
    uint8_t sn[32] = {0};
    uint8_t en[32] = {0};
    uint8_t type[16] = {0};
    uint32_t produceDate = 0;
    uint32_t weight = 0;
    uint32_t openDate = 0;
    uint32_t unOpenDate = 0;
    uint32_t openedDate = 0;
};

QT_BEGIN_NAMESPACE
namespace Ui { class NFC; }
QT_END_NAMESPACE

class NFC : public QWidget
{
    Q_OBJECT

public:
    NFC(QWidget *parent = nullptr);
    ~NFC();

private slots:
    void on_read_clicked();
    void on_write_clicked();
    void on_connect_clicked();
    void on_clear_clicked();
    void on_scanSerialPort_clicked();
    void show_message();

public slots:
    void read_data(void);
    void refresh_ui(uint8_t id);

public:
    quint16 CRC16_Dn_Cal(const QByteArray &data);
    QByteArray  GenRawCmd(quint8 command,quint8 state,const QByteArray &data);
    void send(QByteArray buff);
    void open();
    QString portname;
    void farme_stream_decode_cb(farme_t* precv_farme);
    BOTTLE_INFO write_bottle_info;
    BOTTLE_INFO read_bottle_info;
    MO_INFO write_mo_info;
    MO_INFO read_mo_info;

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    void parse_recv(QByteArray buff);
    QByteArray byte_encrypt(QByteArray data);
    QByteArray byte_decrypt(QByteArray data);
    void parse_cmd_resp_sector_read(farme_t* precv_farme);
    void str_to_uint8(uint8_t *src, QByteArray str);
    void refresh_bottle(bool hand_clear = false);
    void refresh_mo(bool hand_clear = false);
    void set_write_bottle_info();
    void set_mo_bottle_info();
    void init_connect();
    void init_config();
    void init_log();

signals:
    void sigRefreshUI(uint8_t id);

private:
    Ui::NFC *ui;
    QSharedPointer<QSerialPort> serial;
    QSharedPointer<QTimer> read_timer;
    QSharedPointer<QTimer> write_timer;
    QSharedPointer<logger> log;

    QByteArray sendbyteArray;
    QByteArray recvbyteArray;

    int rx_cnt = 0;
    int tx_cnt = 0;
    int rx_byte = 0;
    int tx_byte = 0;
    int card_yes_cnt = 0;
    int card_no_cnt = 0;
    uint8_t send_sector_num = 0;
    uint8_t recv_sector_num = 0;
    bool card_ready = false;
};
#endif // NFC_H
