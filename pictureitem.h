#ifndef PICTUREITEM_H
#define PICTUREITEM_H

#include <QListWidgetItem>
#include <QObject>
#include <QWidget>

class PictureItem : public QListWidgetItem
{

public:
    PictureItem();

    PictureItem(const QIcon &icon, const QString &text);

    QString getPi_Url() const;
    void setPi_Url(const QString &value);

    QString getPi_tooltip() const;
    void setPi_tooltip(const QString &value);

    int getPi_fileSizeInBytes() const;
    void setPi_fileSizeInBytes(int value);

    int getPi_width() const;
    void setPi_width(int value);

    int getPi_height() const;
    void setPi_height(int value);

private:

    QString pi_Url;
    QString pi_tooltip;
    int pi_fileSizeInBytes;
    int pi_width;
    int pi_height;


};

#endif // PICTUREITEM_H
