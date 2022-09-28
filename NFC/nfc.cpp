#include "ui_nfc.h"
#include "nfc.h"
#include "utils.h"
#include <QByteArray>
#include <QDebug>
#include <QKeyEvent>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QtAlgorithms>
#include <QThread>
#include <QTimer>
#include <QMessageBox>

static QSharedPointer<NFC> self = nullptr;
NFC::NFC(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NFC)
{
    ui->setupUi(this);
    setWindowTitle("黑格NFC读写器");
    setStyleSheet("font-size: 16px;");
    setWindowIcon(QIcon(":/Icon/logo/logo.ico"));
    init_log();
    init_connect();
    init_config();
    self.reset(this);
}

NFC::~NFC()
{
    if (serial->isOpen())
    {
        serial->close();
        qDebug() << "close serial";
    }

    if (read_timer->isActive())
    {
        read_timer->stop();
        qDebug() << "stop read_timer";
    }

    if (write_timer->isActive())
    {
        write_timer->stop();
        qDebug() << "stop write_timer";
    }

    delete ui;
}

void NFC::init_connect()
{
    serial.reset(new QSerialPort);
    connect(serial.data(), &QSerialPort::readyRead, this, &NFC::read_data);
    connect(serial.data(), &QSerialPort::aboutToClose, this, [=]()
    {
        ui->textBrowser->append(QString("%1:%2关闭").arg(QDateTime::currentDateTime().toString("yyyy~MM~dd hh:mm:ss zzz")).arg(ui->serialPortCombo->currentText()));
    });
    connect(serial.data(), &QSerialPort::errorOccurred, this, [=]()
    {
        QString msg = serial->errorString();
        if (serial->error() == QSerialPort::NoError)
        {
            msg.clear();
            msg.append(QString("打开%1成功").arg(ui->serialPortCombo->currentText()));
        }

        ui->textBrowser->append(QString("%1:%2").arg(QDateTime::currentDateTime().toString("yyyy~MM~dd hh:mm:ss zzz")).arg(msg));
    });

    read_timer.reset(new QTimer(this));
    connect(read_timer.data(), &QTimer::timeout, this, [=]()
    {
        qint8 key = static_cast<qint8>(0xFF);
        static int i = 0;
        qint8 sector =  static_cast<qint8>(i+1);
        QVector<qint8> params = {sector,key,key,key,key,key,key};
        QByteArray array = QByteArray::fromRawData(reinterpret_cast<const char*>(params.data()),params.size());
        QByteArray data = GenRawCmd(sys::Command::sector_read, sys::Sate::normal, array);
        send(data);
        qDebug() << "send:" << data.toHex(' ').toUpper() << " data'size: " << data.size();

        if(++i == recv_sector_num)
        {
            i = 0;
            read_timer->stop();
        }
    });

    write_timer.reset(new QTimer(this));
    connect(write_timer.data(), &QTimer::timeout, this, [=]()
    {
        qint8 key = static_cast<qint8>(0xFF);
        static uint8_t i = 0;
        qint8 sector = static_cast<qint8>(i+1);
        QVector<qint8>params = {sector, key,key,key,key,key,key};
        QByteArray array;
        array.resize(7+ONE_WRITE_SIZE);
        array.fill(0);
        array = QByteArray::fromRawData(reinterpret_cast<const char*>(params.data()),params.size());

        if(i == send_sector_num-1)
        {
            QByteArray temp;
            temp.resize(ONE_WRITE_SIZE-sendbyteArray.size()%ONE_WRITE_SIZE);
            temp.fill(0);
            array += sendbyteArray.mid(i*48, sendbyteArray.size()%ONE_WRITE_SIZE);
            array += temp;
        }
        else
        {
            array += sendbyteArray.mid(i*48, 48);
        }

        QByteArray data = GenRawCmd(sys::Command::sector_write, sys::Sate::normal, array);
        qDebug() << "send "<< data.toHex(' ').toUpper();
        send(data);
        if(++i == send_sector_num)
        {
            i = 0;
            uint8_t id = static_cast<uint8_t>(sendbyteArray[0]);
            write_timer->stop();
            sendbyteArray.clear();

            QMessageBox::about(this, "提示", QString("写入%1完毕！！！").arg(id == sys::WhoUI::bolttle ? BOTTLE_NFC : MO_NFC));
        }
    });

    connect(this, &NFC::sigRefreshUI, this, &NFC::refresh_ui);
}

void NFC::init_config()
{
    QString ini = qApp->applicationDirPath() + "/config/config.ini";
    if (!QFile::exists(ini))
        QMessageBox::information(this, "提示", QString("config.ini文件不存在"), QMessageBox::Ok);

    QVariantMap map = utils::readini(ini);
    QStringList bottleTypes = map.value("bottle/type").toStringList();
    for(auto type: bottleTypes)
    {
        ui->bottleTypeComboBox->addItem(type);
    }

    QStringList moTypes = map.value("mo/type").toStringList();
    for(auto type: moTypes)
    {
        ui->moTypeComboBox->addItem(type);
    }

    QString txt = qApp->applicationDirPath() + "/data/bottle_en.txt";
    if (!QFile::exists(txt))
        QMessageBox::information(this, "提示", QString("bottle_en.txt文件不存在"), QMessageBox::Ok);

    QStringList bottleEns =  utils::readtxt(txt);
    for(auto en: bottleEns)
    {
        ui->bottleMaterialEnComboBox->addItem(en.trimmed());
    }
}

void NFC::init_log()
{
    log.reset(new logger());
}

void NFC::on_read_clicked()
{
    if(card_ready)
    {
        size_t size = 0;
        size = sizeof (BOTTLE_INFO) >= sizeof (MO_INFO) ? sizeof (BOTTLE_INFO) : sizeof (MO_INFO);
        qDebug() << "sizeof(T): " << size;
        if(size % ONE_WRITE_SIZE)
        {
            recv_sector_num = static_cast<uint8_t>(size/ONE_WRITE_SIZE+1);
        }
        else
        {
            recv_sector_num = static_cast<uint8_t>(size/ONE_WRITE_SIZE);
        }

        read_timer->start(100);
    }
    else
    {
        QMessageBox::warning(this, "警告", "请放置标签", QMessageBox::Ok);
    }
}

void farme_stream_decode_cb_(farme_t* precv_farme)
{
    self.data()->farme_stream_decode_cb(precv_farme);
}

void NFC::str_to_uint8(uint8_t *src, QByteArray str)
{
    memcpy(src, str.data(), static_cast<size_t>(str.size()));
}

void NFC::set_write_bottle_info()
{
    QByteArray bottleSn = ui->bottleSnLineEdit->text().toLatin1();
    QByteArray bottleEn = ui->bottleMaterialEnComboBox->currentText().toLatin1();
    QByteArray bottleType = ui->bottleTypeComboBox->currentText().toLatin1();
    uint32_t   bottleProduceDate = ui->bottleProduceDateLineEdit->text().toUInt();
    uint32_t   bottleWeight = ui->bottleWeightLineEdit->text().toUInt();
    uint32_t   bottleOpen = ui->bottleOpenLineEdit->text().toUInt();
    uint32_t   bottleUnopen = ui->bottleUnOpenLineEdit->text().toUInt();
    uint32_t   bottleOpened = ui->bottleOpenedLineEdit->text().toUInt();

    memset(&write_bottle_info, 0, sizeof(BOTTLE_INFO));
    write_bottle_info.id = sys::WhoUI::bolttle;
    str_to_uint8(write_bottle_info.sn, bottleSn);
    str_to_uint8(write_bottle_info.en, bottleEn);
    str_to_uint8(write_bottle_info.type, bottleType);

    write_bottle_info.produceDate = bottleProduceDate;
    write_bottle_info.weight = bottleWeight;
    write_bottle_info.openDate = bottleOpen;
    write_bottle_info.unOpenDate = bottleUnopen;
    write_bottle_info.openedDate = bottleOpened;

    sendbyteArray.append(reinterpret_cast<char *>(&write_bottle_info), sizeof(BOTTLE_INFO));
    qDebug() << "sizeof(BOTTLE_INFO): " << sizeof(BOTTLE_INFO) << "sendbyteArray'size: "<< sendbyteArray.size();
}

void NFC::set_mo_bottle_info()
{
    QByteArray moSn = ui->moSnLineEdit->text().toLatin1();
    QByteArray moType = ui->moTypeComboBox->currentText().toLatin1();
    uint32_t   moExpireDate = ui->moExpireDateLineEdit->text().toUInt();
    uint32_t   moProduceDate = ui->moProduceDateLineEdit->text().toUInt();
    uint32_t   moPeelLeftCount = ui->moPeelLeftCountLineEdit->text().toUInt();

    memset(&write_mo_info, 0, sizeof(MO_INFO));
    write_mo_info.id = sys::WhoUI::mo;
    str_to_uint8(write_mo_info.sn, moSn);
    str_to_uint8(write_mo_info.type, moType);

    write_mo_info.produceDate = moProduceDate;
    write_mo_info.expireDate = moExpireDate;
    write_mo_info.peelLeftCount = moPeelLeftCount;

    sendbyteArray.append(reinterpret_cast<char *>(&write_mo_info), sizeof(MO_INFO));
    qDebug() << "sizeof(MO_INFO): " << sizeof(MO_INFO) << "sendbyteArray'size: "<< sendbyteArray.size();
}

void NFC::on_write_clicked()
{
    if(card_ready)
    {
        sys::WhoUI who = ui->tabWidget->currentIndex() == 0 ? sys::WhoUI::bolttle : sys::WhoUI::mo;
        switch(who)
        {
        case sys::WhoUI::bolttle:
            set_write_bottle_info();
            break;
        case sys::WhoUI::mo:
            set_mo_bottle_info();
            break;
        }

        if(sendbyteArray.size()%ONE_WRITE_SIZE)
        {
            send_sector_num = static_cast<uint8_t>(sendbyteArray.size()/ONE_WRITE_SIZE+1);
        }
        else
        {
            send_sector_num = static_cast<uint8_t>(sendbyteArray.size()/ONE_WRITE_SIZE);
        }

        qDebug() << "NFC write data:" << sendbyteArray.toHex(' ').toUpper();
        qDebug() << "send_sector_num:" << send_sector_num;
        write_timer->start(100);
    }
    else
    {
        QMessageBox::warning(this, "警告", "请放置标签", QMessageBox::Ok);
    }
}

void NFC::show_message()
{

}

void NFC::on_clear_clicked()
{
    if (ui->tabWidget->currentIndex() == 0)
    {
        refresh_bottle(true);
    }
    else {
        refresh_mo(true);
    }
}

void NFC::on_connect_clicked()
{
    if(serial != nullptr)
    {
        if(serial->isOpen())
        {
            if (QMessageBox::information(this, "提示", QString("确定关闭串口%1").arg(ui->serialPortCombo->currentText()), QMessageBox::Ok) == QMessageBox::Ok)
            {
                serial->close();
                ui->serialPortCombo->setEnabled(true);
                ui->scanSerialPort->setEnabled(true);
                ui->connect->setText("打开串口");
            }
        }
        else
        {
            QString portname = ui->serialPortCombo->currentText();
            serial->setPortName(portname);
            serial->setBaudRate(QSerialPort::Baud115200);
            serial->setParity(QSerialPort::NoParity);
            serial->setDataBits(QSerialPort::Data8);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            if(!serial->open(QIODevice::ReadWrite))
            {
                ui->serialPortCombo->setEnabled(true);
                ui->scanSerialPort->setEnabled(true);
                QMessageBox::information(this, "提示", QString("打开串口失败，原因：%1").arg(serial->errorString()), QMessageBox::Ok);
            }
            else
            {
                ui->serialPortCombo->setEnabled(false);
                ui->scanSerialPort->setEnabled(false);
                ui->connect->setText("关闭串口");
            }
        }
    }
}

void NFC::on_scanSerialPort_clicked()
{
    ui->serialPortCombo->clear();
    for(auto info : QSerialPortInfo::availablePorts())
    {
        ui->serialPortCombo->addItem(info.portName());
        portname = info.portName();
    }
}

quint16 NFC::CRC16_Dn_Cal(const QByteArray &data)
{
    static const uint16_t crc16Table[] =
    {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
    };
    uint8_t  buf;
    uint16_t crc16 = 0xFFFF;

    for ( auto i = 0; i < data.size(); ++i )
    {
        buf = static_cast<uint8_t>(data.at(i) ^ crc16);
        crc16 >>= 8;
        crc16 ^= crc16Table[buf];
    }
    return crc16;
}

QByteArray NFC::GenRawCmd(quint8 command, quint8 state, const QByteArray &data)
{
    QByteArray outBuf;
    outBuf.fill(0);
    {
        QDataStream stream(&outBuf, QIODevice::WriteOnly | QIODevice::Append);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << PROTOCOL_HEAD
               << command
               << state
               << static_cast<quint8>(data.length());
    }

    outBuf += data;
    {
        QDataStream stream(&outBuf, QIODevice::WriteOnly | QIODevice::Append);
        stream.setByteOrder(QDataStream::BigEndian);
        quint16 cal_crc = CRC16_Dn_Cal(outBuf);
        stream << cal_crc
               << PROTOCOL_TAIL;
    }

    return outBuf;
}

void NFC::send(QByteArray buff)
{
    if (serial != nullptr)
    {
        tx_byte += buff.size();
        tx_cnt++;
        buff = byte_encrypt(buff);
        qDebug() << "send encrypt:"<< buff.toHex(' ').toUpper();
        serial->write(buff);
        serial->flush();
    }
}

void NFC::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_F1:
        ui->textBrowser->clear();
        break;
    }
}

void NFC::farme_stream_decode_cb(farme_t *precv_farme)
{
    if (precv_farme->state == FARME_STATE_NORMAL)
    {
        switch (precv_farme->cmd)
        {
        case FARME_CMD_ECHO:
        {
        }
            break;
        case FARME_CMD_REPORT_ACTIVE:
        {
            if (precv_farme->data[0] == 1)
            {
                ui->cardStatus->setText("有卡");
                ui->cardStatus->setStyleSheet("color: green; font-size: 16px;");
                card_ready = true;
            } else
            {
                ui->cardStatus->setText("无卡");
                ui->cardStatus->setStyleSheet("color: black; font-size: 16px;");
                card_ready = false;
            }
        }
            break;
        case FARME_CMD_REQ_SECTOR_READ:
        {
        }
            break;
        case FARME_CMD_RESP_SECTOR_READ:
        {
            if(precv_farme->state == FARME_STATE_ERR_WRITE)
            {
                qDebug() << QString("扇区%1读取失败").arg(precv_farme->data[0]);
            }
            else if(precv_farme->state == FARME_STATE_NORMAL)
            {
                qDebug() << QString("扇区%1读取成功").arg(precv_farme->data[0]);
                parse_cmd_resp_sector_read(precv_farme);
            }
        }
            break;
        case FARME_CMD_REQ_SECTOR_WRITE:
        {
        }
            break;
        case FARME_CMD_RESP_SECTOR_WRITE:
        {
            if(precv_farme->state == FARME_STATE_ERR_WRITE)
            {
                QMessageBox::warning(this, "警告", QString("扇区%1写入失败！请重新写入").arg(precv_farme->data[0]));
                if(write_timer->isActive())
                {
                    write_timer->stop();
                }
            }
            else if(precv_farme->state == FARME_STATE_NORMAL)
            {
                qDebug() << QString("扇区%1写入成功").arg(precv_farme->data[0]);
            }
        }
            break;
        case FARME_CMD_REQ_SECTOR_VERIFY_KEY:
        {
        }
            break;
        case FARME_CMD_RESP_SECTOR_VERIFY_KEY:
        {
            if(precv_farme->state == FARME_STATE_ERR_KEY)
            {
                QMessageBox::warning(this, "警告", QString("扇区%1密钥校验失败！无法写入").arg(precv_farme->data[0]));
                if(write_timer->isActive())
                {
                    write_timer->stop();
                }
            }
        }
            break;
        case FARME_CMD_REQ_SECTOR_MODIFY_KEY:
        {
        }
            break;
        case FARME_CMD_RESP_SECTOR_MODIFY_KEY:
        {
        }
            break;
        default:
        {
        }
            break;
        }
    }
}

void NFC::parse_cmd_resp_sector_read(farme_t* precv_farme)
{
    static uint8_t i = 0;
    recvbyteArray.append(reinterpret_cast<char *>(&precv_farme->data[1]), precv_farme->size-1);
    i++;
    if(i == recv_sector_num)
    {
        qDebug() << "NFC read data:" << recvbyteArray.toHex(' ').toUpper();
        uint8_t id = static_cast<uint8_t>(recvbyteArray[0]);
        i = 0;
        QString id_str;
        if (id == sys::WhoUI::bolttle)
        {
            memset(&read_bottle_info, 0, sizeof (BOTTLE_INFO));
            memcpy(reinterpret_cast<uint8_t *>(&read_bottle_info), recvbyteArray.data(), sizeof(BOTTLE_INFO));
            id_str = BOTTLE_NFC;
        }
        else if (id == sys::WhoUI::mo)
        {
            memset(&read_mo_info, 0, sizeof (MO_INFO));
            memcpy(reinterpret_cast<uint8_t *>(&read_mo_info), recvbyteArray.data(), sizeof(MO_INFO));
            id_str = MO_NFC;
        }
        else
        {
            qDebug() << "未识别的NFC标签: " << id;
        }

        if (id == sys::WhoUI::bolttle || id == sys::WhoUI::mo)
        {
            emit sigRefreshUI(id);
            QMessageBox::about(this, "提示", QString("读取%1完毕").arg(id_str));
        }
        recvbyteArray.clear();
    }
}

void NFC::parse_recv(QByteArray buff)
{
    farme_err_t ret;
    ret = farme_stream_decode(reinterpret_cast<uint8_t*>(buff.data()), static_cast<uint32_t>(buff.size()), farme_stream_decode_cb_);
    if (ret != FERR_OK)
    {
        qDebug() << "farme_stream_decode: " << ret;
    }
}

QByteArray NFC::byte_encrypt(QByteArray data)
{
    farme_err_t ret;
    QByteArray byte;
    if (!data.isEmpty())
    {
        ret = farme_data_encrypt(reinterpret_cast<uint8_t*>(data.data()), static_cast<uint32_t>(data.size()));
        if (ret != FERR_OK)
        {
            qDebug()<<"encrypt error";
        }
        else
        {
            byte = data;
            return byte;
        }
    }
    return byte;
}

QByteArray NFC::byte_decrypt(QByteArray data)
{
    farme_err_t ret;
    QByteArray byte;
    if (!data.isEmpty())
    {
        ret = farme_data_decrypt(reinterpret_cast<uint8_t*>(data.data()), static_cast<uint32_t>(data.size()));
        if (ret != FERR_OK)
        {
            qDebug()<<"decrypt fail";
        }
        else
        {
            byte = data;
            return byte;
        }
    }
    return byte;
}

void NFC::read_data()
{
    qDebug() << "read_data";
    if (serial != nullptr)
    {
        QByteArray buf;
        buf = serial->readAll();
        if (!buf.isEmpty())
        {
            /* 更新计数 */
            rx_cnt++;
            rx_byte += buf.size();
            buf = byte_decrypt(buf);
            parse_recv(buf);
        }
    }
}

void NFC::refresh_bottle(bool hand_clear)
{
    ui->bottleSnLineEdit->clear();
    ui->bottleWeightLineEdit->clear();
    ui->bottleProduceDateLineEdit->clear();
    ui->bottleOpenLineEdit->clear();
    ui->bottleUnOpenLineEdit->clear();
    ui->bottleOpenedLineEdit->clear();

    if (!hand_clear)
    {
        ui->bottleSnLineEdit->setText(reinterpret_cast<char*>(read_bottle_info.sn));
        ui->bottleWeightLineEdit->setText(QString::number(read_bottle_info.weight));
        ui->bottleProduceDateLineEdit->setText(QString::number(read_bottle_info.produceDate));
        ui->bottleOpenLineEdit->setText(QString::number(read_bottle_info.openDate));
        ui->bottleUnOpenLineEdit->setText(QString::number(read_bottle_info.unOpenDate));
        ui->bottleOpenedLineEdit->setText(QString::number(read_bottle_info.openedDate));

        {
            QByteArray array(reinterpret_cast<char*>(read_bottle_info.type));
            for(auto index = 0; index < ui->bottleTypeComboBox->count(); index++)
            {
                if (QString(array.constData()) == ui->bottleTypeComboBox->itemText(index))
                {
                    ui->bottleTypeComboBox->setCurrentIndex(index);
                }
            }
        }

        {
            QByteArray array(reinterpret_cast<char*>(read_bottle_info.en));
            for(auto index = 0; index < ui->bottleMaterialEnComboBox->count(); index++)
            {
                if (QString(array.constData()) == ui->bottleMaterialEnComboBox->itemText(index))
                {
                    ui->bottleMaterialEnComboBox->setCurrentIndex(index);
                }
            }
        }
    }
    else {
        ui->bottleTypeComboBox->setCurrentIndex(0);
        ui->bottleMaterialEnComboBox->setCurrentIndex(0);
    }
}

void NFC::refresh_mo(bool hand_clear)
{
    ui->moSnLineEdit->clear();
    ui->moExpireDateLineEdit->clear();
    ui->moProduceDateLineEdit->clear();
    ui->moPeelLeftCountLineEdit->clear();

    if (!hand_clear)
    {
        ui->moSnLineEdit->setText(reinterpret_cast<char*>(read_mo_info.sn));
        ui->moExpireDateLineEdit->setText(QString::number(read_mo_info.expireDate));
        ui->moProduceDateLineEdit->setText(QString::number(read_mo_info.produceDate));
        ui->moPeelLeftCountLineEdit->setText(QString::number(read_mo_info.peelLeftCount));

        QByteArray array(reinterpret_cast<char*>(read_mo_info.type));
        for(auto index = 0; index < ui->moTypeComboBox->count(); index++)
        {
            if (QString(array.constData()) == ui->moTypeComboBox->itemText(index))
            {
                ui->moTypeComboBox->setCurrentIndex(index);
            }
        }
    }
    else {
        ui->moTypeComboBox->setCurrentIndex(0);
    }
}

void NFC::refresh_ui(uint8_t id)
{
    if (id == sys::WhoUI::bolttle)
    {
        refresh_bottle();
    }
    else
    {
        refresh_mo();
    }
}
