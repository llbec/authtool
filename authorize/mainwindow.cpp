#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDateTime>
#include "utils/utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();

    //import auth.dll
#ifdef Q_OS_WINDOWS
    hdll_auth = LoadLibraryA("auth.dll");
    if(hdll_auth == NULL || INVALID_HANDLE_VALUE == hdll_auth) {
        throw "load authorize dll failed!";
    }
    UpdateUserCode = (funcPtrUpdateUserCode)GetProcAddress(hdll_auth, "UpdateUserCode");
    StringToStamp = (funcPtrStringToStamp)GetProcAddress(hdll_auth, "StringToStamp");
    StampToString = (funcPtrStampToString)GetProcAddress(hdll_auth, "StampToString");
    SignUserCode = (funcPtrSignUserCode)GetProcAddress(hdll_auth, "SignUserCode");
    VerifyUserCode = (funcPtrVerifyUserCode)GetProcAddress(hdll_auth, "VerifyUserCode");
    GetLimit = (funcPtrGetLimit)GetProcAddress(hdll_auth, "GetLimit");
    GetCPU = (funcPtrGetCPU)GetProcAddress(hdll_auth, "GetCPU");
    GetPhoneNum = (funcPtrGetPhoneNum)GetProcAddress(hdll_auth, "GetPhoneNum");
    GetIndex = (funcPtrGetIndex)GetProcAddress(hdll_auth, "GetIndex");
    InviteCode = (funcPtrInviteCode)GetProcAddress(hdll_auth, "InviteCode");

    hdll_inviter = LoadLibraryA("inviter.dll");
    if(hdll_inviter == NULL || INVALID_HANDLE_VALUE == hdll_inviter) {
        throw "load inviter dll failed!";
    }
    MakeInviter = (funcPtrMakeInviter)GetProcAddress(hdll_inviter, "MakeInviter");
    ReadAll = (funcPtrReadAll)GetProcAddress(hdll_inviter, "ReadAll");

    hdll_authlog = LoadLibraryA("authlog.dll");
    if(hdll_authlog == NULL || INVALID_HANDLE_VALUE == hdll_authlog) {
        throw "load inviter dll failed!";
    }
    WriteSig = (funcPtrWriteSig)GetProcAddress(hdll_authlog, "WriteSig");
    LoadSigLog = (funcPtrLoadSigLog)GetProcAddress(hdll_authlog, "LoadSigLog");
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUI()
{
    QDate curDate=QDate::currentDate();
    ui->date_limit->setDate(curDate);
    ui->date_authlog_start->setDate(curDate);
    ui->date_authlog_end->setDate(curDate);
}

bool MainWindow::IsValidPhoneNumber(const QString & phoneNum)
{
    QRegExp regx("^1([358][0-9]|4[579]|66|7[0135678]|9[89])[0-9]{8}$");
    QRegExpValidator regs(regx, 0);
    QString pNum = phoneNum;
    int pos = 0;
    QValidator::State res = regs.validate(pNum, pos);
    if (QValidator::Acceptable == res) {
        return true;
    }
    else {
        return false;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString slimit = ui->date_limit->text() + " 23:59:59";
    QByteArray baUser = ui->text_user->toPlainText().toLatin1();
    QByteArray baLmt = slimit.toLatin1();
    QString sig = QString(SignUserCode(UpdateUserCode(baUser.data(), StringToStamp(baLmt.data()))));
    QByteArray basig = sig.toLatin1();
    char * cSig = basig.data();
    if (VerifyUserCode(cSig)) {
        ui->text_auth->setText(sig);
        qint64 lmt = GetLimit(cSig);
        qint64 idx = GetIndex(cSig);
        QString cpu = QString(GetCPU(cSig));
        qint64 phone = GetPhoneNum(cSig);
        ui->text_result->setText(QString("Success: sign %1 to date %2. invite num is %3(%4)").arg(cpu).arg(QString(StampToString(lmt))).arg(phone).arg(idx));
        WriteSig(cSig);
    } else {
        ui->text_result->setText("Sign failed!");
    }

}

void MainWindow::on_pbtn_invite_clicked()
{
    if (!IsValidPhoneNumber(ui->text_phone->toPlainText())) {
        QMessageBox::information(NULL, "error", "Invalid phone number");
        return;
    }
    ui->text_invite->setText(QString(MakeInviter(ui->text_phone->toPlainText().toLongLong())));
}

void MainWindow::on_pbtn_allInviter_clicked()
{
    QStandardItemModel * standItemModel = new QStandardItemModel();
    //add table title
    standItemModel->setColumnCount(2);
    standItemModel->setHeaderData(0,Qt::Horizontal,"Phone number");
    standItemModel->setHeaderData(1,Qt::Horizontal,"Index");

    QString rs = QString(ReadAll());
    QByteArray barray = Utils::Decodebs64(rs);
    if (barray.isEmpty())
        return;
    qint64 count = Utils::AtoInt64(barray.mid(0, 8).data());
    ui->label_invitercount->setText(QString("%1").arg(count));
    int idoff = 0;
    bool ok;
    for (int i = 0; i < count; i++) {
        idoff += 8;
        QStandardItem *standItem1 = new QStandardItem(tr("%1").arg(QString(barray.mid(idoff, 8).toHex()).toULongLong(&ok, 16)));
        idoff += 8;
        QStandardItem *standItem2 = new QStandardItem(tr("%1").arg(QString(barray.mid(idoff, 8).toHex()).toULongLong(&ok, 16)));
        standItemModel->setItem(i,0,standItem1);
        //standItemModel->item(i,0)->setForeground(QBrush(QColor(255,0,0)));      //设置字符颜色
        standItemModel->item(i,0)->setTextAlignment(Qt::AlignCenter);           //设置表格内容居中
        standItemModel->setItem(i,1,standItem2);
        standItemModel->item(i,1)->setTextAlignment(Qt::AlignCenter);
    }

    ui->table_inviter->setModel(standItemModel);
    //setting table
    ui->table_inviter->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->table_inviter->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->table_inviter->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->table_inviter->setColumnWidth(0,200);
    ui->table_inviter->setColumnWidth(1,100);
    ui->table_inviter->verticalHeader()->hide();
    //ui->table_inviter->setSelectionBehavior(QAbstractItemView::SelectRows); //select row
    //ui->table_inviter->setEditTriggers(QAbstractItemView::NoEditTriggers);  //readonly
}

void MainWindow::on_pbtn_authlog_refresh_clicked()
{
    QStandardItemModel * standItemModel = new QStandardItemModel();
    //add table title
    standItemModel->setColumnCount(4);
    standItemModel->setHeaderData(0,Qt::Horizontal,"sign time");
    standItemModel->setHeaderData(1,Qt::Horizontal,"valid period");
    standItemModel->setHeaderData(2,Qt::Horizontal,"inviter'phone");
    standItemModel->setHeaderData(3,Qt::Horizontal,"signature");

    QString stfrom = ui->date_authlog_start->text() + " 00:00:00";
    QByteArray bafrom = stfrom.toLatin1();
    qint64 tFrom = StringToStamp(bafrom.data());
    QString stend = ui->date_authlog_end->text() + " 00:00:00";
    QByteArray baend = stend.toLatin1();
    qint64 tEnd = StringToStamp(baend.data());
    QString logs = QString(LoadSigLog(tFrom, tEnd));
    QByteArray barray = Utils::Decodebs64(logs);
    if (barray.isEmpty())
        return;
    qint64 count = Utils::AtoInt64(barray.mid(0, 8).data());
    int idoff = 8;
    bool ok;
    for (int i = 0; i < count; i++) {
        qint64 signdate = QString(barray.mid(idoff, 8).toHex()).toULongLong(&ok, 16);
        idoff += 8;
        int siglen = int(Utils::AtoInt64(barray.mid(idoff, 8).data()));
        idoff += 8;
        QByteArray baSig = barray.mid(idoff, siglen);
        QString sigstr = QString(baSig);
        idoff += siglen;
        QStandardItem *standItem1 = new QStandardItem(tr("%1").arg(StampToString(signdate)));
        QStandardItem *standItem2 = new QStandardItem(tr("%1").arg(StampToString(GetLimit(baSig.data()))));
        QStandardItem *standItem3 = new QStandardItem(tr("%1").arg(GetPhoneNum(baSig.data())));
        QStandardItem *standItem4 = new QStandardItem(tr("%1").arg(sigstr));
        standItemModel->setItem(i,0,standItem1);
        standItemModel->item(i,0)->setTextAlignment(Qt::AlignCenter);           //设置表格内容居中
        standItemModel->setItem(i,1,standItem2);
        standItemModel->item(i,1)->setTextAlignment(Qt::AlignCenter);
        standItemModel->setItem(i,2,standItem3);
        standItemModel->item(i,2)->setTextAlignment(Qt::AlignCenter);           //设置表格内容居中
        standItemModel->setItem(i,3,standItem4);
    }
    ui->table_authlog->setModel(standItemModel);
    //setting table
    ui->table_authlog->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->table_authlog->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->table_authlog->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->table_authlog->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->table_authlog->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->table_authlog->setColumnWidth(0,150);
    ui->table_authlog->setColumnWidth(1,150);
    ui->table_authlog->setColumnWidth(2,100);
    ui->table_authlog->setColumnWidth(3,350);
    ui->table_authlog->verticalHeader()->hide();
}
