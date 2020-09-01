#ifndef UTILS_H
#define UTILS_H
#include <QString>

typedef unsigned long long UInt64;

class Utils
{
public:
    Utils();

    static QString Encodebs64(const QByteArray & binaryData) throw();
    static QByteArray Decodebs64(const QString & base64String) throw(int);
    static qint64 AtoInt64(char * a);
};

#endif // UTILS_H
