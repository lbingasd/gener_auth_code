#include "pdd_auth_code.h"
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QClipboard>

#define KEY "morankeji"

PDDAuthCode::PDDAuthCode(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	ui.lb_loacl_mac->setText(GetMacAddress());
	ui.dateTimeEdit->setDateTime(QDateTime::currentDateTime());
	connect(ui.pb_genery, SIGNAL(clicked()), SLOT(GeneryCode()));
	connect(ui.pb_copy, SIGNAL(clicked()), SLOT(Copy()));
}

PDDAuthCode::~PDDAuthCode()
{}

void PDDAuthCode::GeneryCode()
{
	QString in_mac_address = ui.lineEdit->text();
	qint64 time = ui.dateTimeEdit->dateTime().toMSecsSinceEpoch();
	QString to_enc = in_mac_address + ";" + QString::number(time);
	QString enc_code = Encrypt(to_enc, KEY);
	ui.lb_auth_code->setText(enc_code);
}

void PDDAuthCode::Copy()
{
	QClipboard* clipboard = QGuiApplication::clipboard();

	// 将内容复制到剪贴板
	clipboard->setText(ui.lb_auth_code->text());
}

QString PDDAuthCode::GetMacAddress()
{
	QString macAddress;
	QList<QNetworkInterface> interfaces =
		QNetworkInterface::allInterfaces();

	foreach(QNetworkInterface interface, interfaces)
	{
		// 过滤掉虚拟接口和无效接口
		if (interface.flags().testFlag(QNetworkInterface::IsRunning) &&
			!interface.flags().testFlag(
				QNetworkInterface::IsLoopBack)) {
			macAddress = interface.hardwareAddress();
			break; // 获取第一个有效接口的MAC地址
		}
	}

	return macAddress;
}

QString PDDAuthCode::Encrypt(const QString& input, const QString& key)
{
	QByteArray data = input.toUtf8();
	QByteArray keyData = key.toUtf8();

	// 使用哈希函数对密钥进行处理
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(keyData);
	QByteArray keyHash = hash.result();

	// 使用异或运算进行简单加密
	for (int i = 0; i < data.size(); ++i) {
		data[i] = data[i] ^ keyHash.at(i % keyHash.size());
	}

	// 返回加密后的数据
	return QString(data.toBase64());
}

QString PDDAuthCode::Decrypt(const QString& input, const QString& key)
{
	QByteArray data = QByteArray::fromBase64(input.toUtf8());
	QByteArray keyData = key.toUtf8();

	// 使用哈希函数对密钥进行处理
	QCryptographicHash hash(QCryptographicHash::Sha256);
	hash.addData(keyData);
	QByteArray keyHash = hash.result();

	// 使用异或运算进行解密
	for (int i = 0; i < data.size(); ++i) {
		data[i] = data[i] ^ keyHash.at(i % keyHash.size());
	}

	// 返回解密后的数据
	return QString(data);
}
