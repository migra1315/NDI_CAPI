#ifndef QINFOFILE_H
#define QINFOFILE_H
#include <list>
#include <fstream>
#include <iostream>
#include <string>
#include <QString>
#include <QFile>
#include <QTextStream>

class QInfoFile
{
public:
    QInfoFile();
    void ReadConfiguration(QString path,QString &path1,QString &path2,QString &path3);
};

#endif // QINFOFILE_H
