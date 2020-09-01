#ifndef INVITER_H
#define INVITER_H
#include <QPointer>

class QSettings;

class Inviter
{
public:
    Inviter();

private:
    QPointer<QSettings> m_settings;
    QString m_path;
};

#endif // INVITER_H
