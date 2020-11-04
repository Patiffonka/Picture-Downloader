#include "pictureitem.h"

PictureItem::PictureItem() : QListWidgetItem()
{

}

PictureItem::PictureItem(const QIcon &icon, const QString &text) : QListWidgetItem (icon, text) {}

QString PictureItem::getPi_Url() const
{
    return pi_Url;
}

void PictureItem::setPi_Url(const QString &value)
{
    pi_Url = value;
}

QString PictureItem::getPi_tooltip() const
{
    return pi_tooltip;
}

void PictureItem::setPi_tooltip(const QString &value)
{
    pi_tooltip = value;
}

int PictureItem::getPi_fileSizeInBytes() const
{
    return pi_fileSizeInBytes;
}

void PictureItem::setPi_fileSizeInBytes(int value)
{
    pi_fileSizeInBytes = value;
}

int PictureItem::getPi_width() const
{
    return pi_width;
}

void PictureItem::setPi_width(int value)
{
    pi_width = value;
}

int PictureItem::getPi_height() const
{
    return pi_height;
}

void PictureItem::setPi_height(int value)
{
    pi_height = value;
}
