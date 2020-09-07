#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif
#include "../libs/auth/auth.h"
#include "../libs/inviter/inviter.h"
#include "../libs/authlog/authlog.h"

namespace Ui {
class MainWindow;
}

//import auth.dll
typedef char* (*funcPtrUpdateUserCode)(char* p0, GoInt64 p1);
typedef GoInt64 (*funcPtrStringToStamp)(char* p0);
typedef char* (*funcPtrStampToString)(GoInt64 p0);
typedef char* (*funcPtrSignUserCode)(char* p0);
typedef GoUint8 (*funcPtrVerifyUserCode)(char* c, GoUint8 b);
typedef GoInt64 (*funcPtrGetLimit)(char* p0);
typedef char* (*funcPtrGetCPU)(char* p0);
typedef GoInt64 (*funcPtrGetPhoneNum)(char* p0);
typedef GoInt64 (*funcPtrGetIndex)(char* p0);
typedef char* (*funcPtrInviteCode)(GoInt64 p0, GoInt64 p1);

//import inviter.dll
typedef char* (*funcPtrMakeInviter)(GoInt64 p0);
typedef char* (*funcPtrReadAll)();

//import authlog.dll
typedef void (*funcPtrWriteSig)(char* p0);
typedef char* (*funcPtrLoadSigLog)(GoInt64 p0, GoInt64 p1);

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pbtn_invite_clicked();

    void on_pbtn_allInviter_clicked();

    void on_pbtn_authlog_refresh_clicked();

private:
    bool IsValidPhoneNumber(const QString & phoneNum);

private:
    Ui::MainWindow *ui;
    void InitUI();

#ifdef Q_OS_WINDOWS
    HMODULE hdll_auth;
    HMODULE hdll_inviter;
    HMODULE hdll_authlog;
#endif
    funcPtrUpdateUserCode UpdateUserCode;
    funcPtrStringToStamp StringToStamp;
    funcPtrStampToString StampToString;
    funcPtrSignUserCode SignUserCode;
    funcPtrVerifyUserCode VerifyUserCode;
    funcPtrGetLimit GetLimit;
    funcPtrGetCPU GetCPU;
    funcPtrGetPhoneNum GetPhoneNum;
    funcPtrGetIndex GetIndex;
    funcPtrInviteCode InviteCode;

    funcPtrMakeInviter MakeInviter;
    funcPtrReadAll ReadAll;

    funcPtrWriteSig WriteSig;
    funcPtrLoadSigLog LoadSigLog;
};

#endif // MAINWINDOW_H
