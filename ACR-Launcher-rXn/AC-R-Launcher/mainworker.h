#ifndef MAINWORKER_H
#define MAINWORKER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QTextStream>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QTemporaryFile>

//TODO
#include <QDebug>

//Compression
#include <JlCompress.h>

#include "modentry.h"
#include "modlistdownloader.h"
#include "downloaddialog.h"
#include "serverconsole.h"

class MainWorker : public QObject
{
    Q_OBJECT

public:
    MainWorker();

    QStringList getInstalledMods();
    QStringList getAvialableMods();

    void pruneMod(QString badMod);

    //Load stuff at startup (mods etc)
    QString loadData();

    //To be removed
    //void help(QString cmd);

    //Startup
    void init();

    //Exit
    void writeToDisk();

    //Starting ACR
    QString writeClientExecutable();
    QString writeServerExecutable();
    QString runClient();
    QString runServer(bool new_window);

    //Update
    void updateGame();
    void updateAll();
    void updateMe();

    //Backup
    QString createBackup();
    QString installBackup(QString file);

    //Installing and showing mods
    QStringList getModInfos(QString mod);
    QString installMod(QString mod);

    //File stuff
    QString extractZipfile(QString zipfile, QString target = NULL);
    QString createZipfile(QStringList files, QString name);
    void rec_copy(QDir folder, QDir current);
    void rec_rem(QDir folder);

public slots:
    void on_modListDownloader_isReady(QList<ModEntry> l);

    void on_DownloadDialog_downloadReady(QByteArray data);

signals:
    void modListDownloader_isReady();
    void installerReady(QString msg);

private:
    //Config data
    QString version = "-";
    QList<ModEntry> installedMods;
    QList<ModEntry> availableMods;
    QString modserverurl = "https://ruler501git.tk/acr/mods";
    //Network
    ModListDownloader *mdloader;
    DownloadDialog *dloadDialog;
    ModEntry *tmpModEntry;


};

#endif // MAINWORKER_H
