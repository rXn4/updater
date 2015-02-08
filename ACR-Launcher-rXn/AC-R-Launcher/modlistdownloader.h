#ifndef MODLISTDOWNLOADER_H
#define MODLISTDOWNLOADER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "modentry.h"

class ModListDownloader : public QObject
{
    Q_OBJECT

public:
    ModListDownloader(QString url);
    QNetworkReply *getData();
    void start();
public slots:
    void parseModList(QNetworkReply *list);
signals:
    void isReady(QList<ModEntry> l);
private:
    QNetworkAccessManager *qna;
    QNetworkRequest *qnreq;
    QString file;
    QList<ModEntry> parsedMods;
    QNetworkReply *result;
};


#endif // MODLISTDOWNLOADER_H
