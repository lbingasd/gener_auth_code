#pragma once

#include <QtWidgets/QWidget>
#include "ui_pdd_auth_code.h"

class PDDAuthCode : public QWidget
{
    Q_OBJECT

public:
    PDDAuthCode(QWidget *parent = nullptr);
    ~PDDAuthCode();

protected slots:
    void GeneryCode();
    void Copy();
protected:
	QString GetMacAddress();
	QString Encrypt(const QString& input, const QString& key);
	QString Decrypt(const QString& input, const QString& key);

private:
    Ui::PDDAuthCodeClass ui;
};
