#ifndef DOWLOADER_H
#define DOWLOADER_H

#include <QObject>
#include <QNetworkReply>
#include <QIcon>
#include "pictureitem.h"

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);

    void download(const QString&);
    void setSearchNumber(int);


signals:
    void downloadProgress(qint64, qint64);
    void error();
    void downloadComlete(QList<PictureItem*>);
    void picDownoload (int, int);


private slots:
    void slotFinished(QNetworkReply*);
    void downloadPicture(QNetworkReply*);



private:
    QNetworkAccessManager *pnam;
    QNetworkAccessManager *manager;
    QUrl url;
    QString dirName;
    QString temppath;

    QList<PictureItem*> picItemList;


    int searchRequestsCount;
    int currentSearchRequest;

    int currentDownloadPic;

    int GetItemIndex(QString);




};

#endif // DOWLOADER_H
