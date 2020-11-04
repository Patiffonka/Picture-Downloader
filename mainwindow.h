#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLayout>
#include <QListWidget>
#include "downloader.h"
#include <QUrl>
#include <QProgressBar>
#include <QSpinBox>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;

    //interface
    QLineEdit* line_search;
    QGridLayout* find_window;
    QStringList imagePaths;
    QList<QPixmap> imageList;
    QProgressBar* prgbar;
    QSpinBox *spn_picCount;

    //QList<QIcon> iconList;
    QListWidget* listArea;
    Downloader* downloader;
    QUrl* url;
    int errorCounter;

    void closeEvent (QCloseEvent *event) override;

private slots:

    void download();

    void AcceptData(QList<PictureItem*>);
    void picDownoloadProgress (int, int);
    void pageDownloadProgress(qint64 value, qint64 total);
    void error();
    QString convertSize(int);


};
#endif // MAINWINDOW_H
