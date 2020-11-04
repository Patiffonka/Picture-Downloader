#include "downloader.h"
#include <QDebug>
#include <QTemporaryDir>
#include <QStandardPaths>
#include <QRegExp>
#include <QIcon>
#include <QTimer>
#include <QEventLoop>
#include "pictureitem.h"


Downloader::Downloader(QObject *parent) : QObject(parent)
{
    pnam = new QNetworkAccessManager(this);
    connect(pnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotFinished(QNetworkReply*)));

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadPicture(QNetworkReply*)));

    temppath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir dir(temppath);
    dir.mkpath(dirName);
    temppath += "/" + dirName;
    //qDebug() << "Temp location: " << temppath;

    searchRequestsCount = 5;
    currentSearchRequest = 0;
    currentDownloadPic=0;
}

void Downloader::download(const QString &replyText)
{
    currentSearchRequest = 0;
    currentDownloadPic=0;

    QDir picPath(temppath);
    picPath.removeRecursively();

    QString dirName = "PictureDownloaderIcons";
    temppath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir dir(temppath);
    dir.mkpath(dirName);
    temppath += "/" + dirName;
    qDebug() << "Temp location: " << temppath;

    QString path = "https://yandex.ru/images/search?from=tabbar&text=" + replyText;

    url = path;
    //qDebug() << url;

    QNetworkRequest request(url);
    QNetworkReply *reply = pnam->get(request);

    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
             this, SIGNAL(downloadProgress(qint64, qint64)));

}

void Downloader::setSearchNumber(int count)
{
    searchRequestsCount = count;
}

void Downloader::slotFinished(QNetworkReply *reply)
{
    qDebug() << "Page downoading is finished!";

    if (reply->error() != QNetworkReply::NoError){
        qDebug() << "QNetworkReply Error";
        emit error();
    }
    else {

        picItemList.clear();

        QByteArray ba = reply->readAll();

        //qDebug() << ba;

        QRegExp rx_hires("(\\\"img_href\\\":\\\")([^\"]*[^\"])");
        QRegExp rx_preview("(src=\"//)([^\"]*[^\"])");
        QRegExp rx_tooltip("(alt=\\\")([^\"]*[^\"])");
        QRegExp rx_filesize("(fileSizeInBytes\\\":)([^/,]*[^/,])");
        QRegExp rx_width("(w\\\":)([^/,]*[^/,])");
        QRegExp rx_height("(h\\\":)([^/,]*[^/,])");
        int pos = 0;

        qDebug() << "Starting parsing paths...";

        while ((pos = rx_hires.indexIn(ba, pos)) != -1 && currentSearchRequest < searchRequestsCount) {

            //qDebug() << "currentSearchRequest: " << currentSearchRequest;

            PictureItem *picItem = new PictureItem();

            qDebug() << "Hires: " << rx_hires.cap(2);
            picItem->setPi_Url(rx_hires.cap(2));

            pos = rx_preview.indexIn(ba,pos);
            QString previewUrl = rx_preview.cap(2);
            previewUrl.chop(9);
            //qDebug() << "Preview: " << rx_preview.cap(2);

            pos = rx_tooltip.indexIn(ba,pos);
            picItem->setPi_tooltip(rx_tooltip.cap(2));
            //qDebug() << "Tooltip: " << rx_tooltip.cap(2);

            pos = rx_filesize.indexIn(ba,pos);
            int fileSize = rx_filesize.cap(2).toInt();
            picItem->setPi_fileSizeInBytes(rx_filesize.cap(2).toInt());
            //qDebug() << "File size: " << fileSize;

            pos = rx_width.indexIn(ba,pos);
            int fileWidth = rx_width.cap(2).toInt();
            picItem->setPi_width(rx_width.cap(2).toInt());
            //qDebug() << "File Width: " << fileWidth;

            pos = rx_height.indexIn(ba,pos);
            int fileHeight = rx_height.cap(2).toInt();
            picItem->setPi_height(rx_height.cap(2).toInt());
            //qDebug() << "File Height: " << fileHeight;

            ++currentSearchRequest;

            picItemList.append(picItem);

        }

        qDebug() << "Parsing path is done." << picItemList.count() << " objects finded";

    }

    reply->deleteLater();


    qDebug() << "Downloading preview...";

    for (int i = 0; i < picItemList.count(); ++i){

        QUrl url = picItemList[i]->getPi_Url();

        QNetworkRequest request(url);
        QNetworkReply *reply = manager->get(request);

    }

}

void Downloader::downloadPicture(QNetworkReply *reply)
{

    ++currentDownloadPic;
    emit picDownoload(currentDownloadPic, picItemList.count());

    if (reply->error() != QNetworkReply::NoError){
        qDebug() << "QNetworkReply Error: " << reply-> error();
        QString url = reply->url().toString();
        int index = GetItemIndex(url);
        picItemList.removeAt(index);
        --currentDownloadPic;
        emit error();
    } else
    {
        QString url = reply->url().toString();
        int index = GetItemIndex(url);
        qDebug() << "Download Picture(" << currentDownloadPic << "/" << picItemList.count() << ") Index(" << index << ")..." <<  url;

        QByteArray ba = reply->readAll();

        const QString filename = temppath + "/" + reply->url().path().section('/', -1);

        QFile file(filename);
        if (file.open(QIODevice::WriteOnly)){
            file.write(ba);
            file.close();
        }

        QIcon *newIcon = new QIcon(filename);

        picItemList[index]->setIcon(*newIcon);

        reply->deleteLater();

        emit picDownoload(currentDownloadPic, picItemList.count());

        if (currentDownloadPic == picItemList.count())
        {
            emit downloadComlete(picItemList);
        }
    }
}

int Downloader::GetItemIndex(QString url)
{
    for (int i = 0; i < picItemList.count(); ++i)
    {
        if (url == picItemList[i]->getPi_Url()) return i;
    }
    return -1;
}




