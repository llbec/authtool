#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    init();

    //load auth.dll
    hdll = LoadLibraryA("auth.dll");
    if(hdll == NULL || INVALID_HANDLE_VALUE == hdll) {
        throw "load authorize dll failed!";
    }
    //load dll func
    pfGetPCID = (funcPtrGetPCID)GetProcAddress(hdll, "GetPCID");
    pfStringToStamp = (funcPtrStringToStamp)GetProcAddress(hdll, "StringToStamp");
    pfStampToString = (funcPtrStampToString)GetProcAddress(hdll, "StampToString");
    pfSign = (funcPtrSign)GetProcAddress(hdll, "Sign");
    pfVerify = (funcPtrVerify)GetProcAddress(hdll, "Verify");
    pfReadLimit = (funcPtrReadLimit)GetProcAddress(hdll, "ReadLimit");
    pfReadPCID = (funcPtrReadPCID)GetProcAddress(hdll, "ReadPCID");
    pfReadSig = (funcPtrReadSig)GetProcAddress(hdll, "ReadSig");
}

Dialog::~Dialog()
{
    delete ui;
    FreeLibrary(hdll);
}

void Dialog::init()
{
    // init ui
    QDate curDate=QDate::currentDate();
    ui->dateEdit_limit->setDate(curDate);
}

void Dialog::on_pbtn_localID_clicked()
{
    ui->textRW_ID->setText(QString(pfGetPCID()));
}

void Dialog::on_pbtn_sign_clicked()
{
    QByteArray baID = ui->textRW_ID->toPlainText().toLatin1();
    char *cID = baID.data();
    QString slimit = ui->dateEdit_limit->text() + " 23:59:59";
    QByteArray balimit = slimit.toLatin1();
    char * climit = balimit.data();
    GoInt64 ilimit = pfStringToStamp(climit);
    ui->textRW_sig->setText(QString(pfSign(cID, ilimit)));
}

void Dialog::on_pbtn_verify_clicked()
{
    ui->textR_ID->clear();
    ui->textR_sig->clear();
    ui->textR_limit->clear();
    QByteArray baSig = ui->textRW_sig->toPlainText().toLatin1();
    char * cSig = baSig.data();
    GoInt64 ilimit = pfReadLimit(cSig);
    ui->textR_limit->setText(pfStampToString(ilimit));
    ui->textR_ID->setText(pfReadPCID(cSig));
    QString sSig = QString(pfReadSig(cSig));
    if(!pfVerify(cSig)) {
        ui->textR_sig->setText("verify failed\n" + sSig);
    }
    else {
        ui->textR_sig->setText(sSig);
    }
}

void Dialog::on_pbtn_clear_clicked()
{
    ui->textR_ID->clear();
    ui->textR_sig->clear();
    ui->textR_limit->clear();
}
