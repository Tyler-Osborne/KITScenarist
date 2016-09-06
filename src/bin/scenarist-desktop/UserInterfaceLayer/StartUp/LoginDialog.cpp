#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include "QRegExpValidator"
#include <3rd_party/Widgets/TabBarExpanded/TabBarExpanded.h>

using UserInterface::LoginDialog;

LoginDialog::LoginDialog(QWidget* _parent) :
	QLightBoxDialog(_parent),
    ui(new Ui::LoginDialog),
    m_tabs(new TabBarExpanded(this))
{
	ui->setupUi(this);

	initView();
	initConnections();


    isVerify = false;
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

QString LoginDialog::loginEmail() const
{
    return ui->loginEmail->text();
}

QString LoginDialog::regEmail() const
{
    return ui->regEmail->text();
}

QString LoginDialog::loginPassword() const
{
    return ui->loginPassword->text();
}

QString LoginDialog::regPassword() const
{
    return ui->regPassword->text();
}

void LoginDialog::setAuthError(const QString& _error)
{
    ui->errorAuth->setStyleSheet("QLabel { color : red; }");
    ui->errorAuth->setText(_error);
    ui->errorAuth->show();
    unblock();
}

void LoginDialog::setRegisterError(const QString &_error)
{
    ui->errorReg->setText(_error);
    ui->errorReg->show();
    unblock();
}

void LoginDialog::setValidateError(const QString &_error)
{
    ui->errorVerification->setStyleSheet("QLabel { color : red; }");
    ui->errorVerification->setText(_error);
    ui->errorVerification->show();
    unblock();
}

QString LoginDialog::regType() const
{
    if(ui->startButton->isChecked()) {
        return tr("start");
    } else if(ui->basicButton->isChecked()) {
        return tr("basic");
    } else if(ui->maximalButton->isChecked()) {
        return tr("maximal");
    } else return tr("unexpected");
}

QString LoginDialog::code() const
{
    return ui->code->text();
}

void LoginDialog::checkCode()
{
    QRegExpValidator validator(QRegExp("[0-9]{5}"));
    QString s = ui->code->text();
    int pos = 0;
    if(validator.validate(s, pos) == QValidator::Acceptable) {
        block();
        emit verify();
    }
}

void LoginDialog::showVerify()
{
    isVerify = true;
    ui->errorVerification->setStyleSheet("QLabel { color : green; }");
    ui->errorVerification->setText(tr("your e-mail \"%1\" was sent a letter "
                                      "with a confirmation code").arg(ui->regEmail->text()));
    ui->errorVerification->show();
    ui->stackedWidget->setCurrentWidget(ui->verificationPage);

    unblock();
}

void LoginDialog::showRestore()
{
    ui->errorAuth->setStyleSheet("QLabel { color : green; }");
    ui->errorAuth->setText(tr("your e-mail \"%1\" was sent a letter "
                                      "with a password").arg(ui->loginEmail->text()));
    ui->errorAuth->show();

    unblock();
}

void LoginDialog::setAuthPage()
{
    isVerify = false;
    m_tabs->setCurrentIndex(0);
}

void LoginDialog::clear()
{
    ui->loginEmail->clear();
    ui->loginPassword->clear();
    ui->regEmail->clear();
    ui->regPassword->clear();
    ui->code->clear();

    m_tabs->setCurrentIndex(0);
    ui->basicButton->setChecked(true);
    switchWidget();

    isVerify = false;

    ui->errorAuth->clear();
    ui->errorReg->clear();
    ui->errorVerification->clear();
}

QWidget* LoginDialog::focusedOnExec() const
{
    return ui->loginEmail;
}

void LoginDialog::initView()
{
    ui->errorAuth->hide();
    ui->buttonsAuth->addButton(tr("Login"), QDialogButtonBox::AcceptRole);

    ui->errorReg->hide();
    ui->buttonsReg->addButton(tr("Register"), QDialogButtonBox::AcceptRole);

    ui->errorVerification->hide();

    //
    // Красивые чекбоксы
    //
    m_tabs->addTab(tr("Login"));
    m_tabs->addTab(tr("Sign in"));
    m_tabs->setProperty("inTopPanel", true);
    ui->stackLayout->insertWidget(0, m_tabs);

	QLightBoxDialog::initView();
}

void LoginDialog::initConnections()
{
    connect(ui->buttonsAuth, SIGNAL(accepted()), this, SLOT(block()));
    connect(ui->buttonsAuth, SIGNAL(accepted()), this, SIGNAL(login()));

    connect(ui->buttonsReg, SIGNAL(accepted()), this, SLOT(block()));
    connect(ui->buttonsReg, SIGNAL(accepted()), this, SIGNAL(registrate()));

    connect(ui->buttonsAuth, SIGNAL(rejected()), this, SLOT(hide()));
    connect(ui->buttonsReg, SIGNAL(rejected()), this, SLOT(hide()));
    connect(ui->ButtonsVerification, SIGNAL(rejected()), this, SLOT(cancelVerify()));
    connect(ui->ButtonsVerification, SIGNAL(rejected()), this, SLOT(hide()));

    connect(m_tabs, SIGNAL(currentChanged(int)), this, SLOT(switchWidget()));
    connect(ui->restorePass, SIGNAL(clicked(bool)), this, SIGNAL(restore()));

    connect(ui->code, SIGNAL(textChanged(QString)), this, SLOT(checkCode()));

	QLightBoxDialog::initConnections();
}

void LoginDialog::block()
{
    m_tabs->setEnabled(false);

    ui->stackedWidget->setEnabled(false);

    showProgress();
}

void LoginDialog::unblock()
{
    m_tabs->setEnabled(true);

    ui->stackedWidget->setEnabled(true);

   hideProgress();
}

void LoginDialog::cancelVerify()
{
    isVerify = false;
    switchWidget();
}

void LoginDialog::switchWidget()
{
    if(m_tabs->currentIndex() == 0) {
        ui->stackedWidget->setCurrentWidget(ui->authPage);
    }
    else if(isVerify) {
        ui->stackedWidget->setCurrentWidget(ui->verificationPage);
    }
    else {
        ui->stackedWidget->setCurrentWidget(ui->regPage);
    }
}
