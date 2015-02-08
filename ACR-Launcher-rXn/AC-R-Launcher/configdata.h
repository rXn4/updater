#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QTextStream>
#include <QTemporaryFile>

//TODO
#include <QtDebug>

#include <JlCompress.h>

//Class for doing low-level stuff
class ConfigData
{
public:
    ConfigData();
    int readConfig();
    int readModConfig();
    int init();

    int writeClientScript(QString content, int os);
    int writeServerScript(QString content, int os);

    int extractZipfile(QString zipfile);
    int extractZipfile(QString zipfile, QString target);
    int createZipfile(QStringList files, QString name);
    void rec_copy(QDir folder, QDir current);
    void rec_rem(QDir folder);

    int installMod(QByteArray modData);



private:
        
};

#endif // CONFIGDATA_H
