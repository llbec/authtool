#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <Windows.h>
#include "../libs/auth/auth.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

//import auth.dll
typedef char* (*funcPtrGetPCID)();
typedef GoInt64 (*funcPtrStringToStamp)(char* p0);
typedef char* (*funcPtrStampToString)(GoInt64 p0);
typedef char* (*funcPtrSign)(char* p0, GoInt64 p1);
typedef GoUint8 (*funcPtrVerify)(char* p0);
typedef GoInt64 (*funcPtrReadLimit)(char* p0);
typedef char* (*funcPtrReadPCID)(char* p0);
typedef char* (*funcPtrReadSig)(char* p0);

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pbtn_localID_clicked();

    void on_pbtn_sign_clicked();

    void on_pbtn_verify_clicked();

    void on_pbtn_clear_clicked();

private:
    void init();

private:
    Ui::Dialog *ui;
    HMODULE hdll;
    funcPtrGetPCID pfGetPCID;
    funcPtrStringToStamp pfStringToStamp;
    funcPtrStampToString pfStampToString;
    funcPtrSign pfSign;
    funcPtrVerify pfVerify;
    funcPtrReadLimit pfReadLimit;
    funcPtrReadPCID pfReadPCID;
    funcPtrReadSig pfReadSig;
};
#endif // DIALOG_H
