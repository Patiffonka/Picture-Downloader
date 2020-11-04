#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QDebug>
#include <QStandardItemModel>
#include "pictureitem.h"
#include <QStandardPaths>
#include <QDir>
#include <QSpinBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Picture Downloader v1.0");

    errorCounter = 0;

    downloader = new Downloader(this);
    connect (downloader, &Downloader::downloadComlete,
            this, &MainWindow::AcceptData);
    connect (downloader, &Downloader::picDownoload,this, &MainWindow::picDownoloadProgress);
    connect (downloader, &Downloader::downloadProgress, this, &MainWindow::pageDownloadProgress);
    connect (downloader, &Downloader::error, this, &MainWindow::error);

    //interface
    resize(860, 900);
    statusBar()->hide();

    QVBoxLayout* box_layout = new QVBoxLayout(this);



    QLabel* lbl_search = new QLabel("Search:", this);
    box_layout->addWidget(lbl_search);

    line_search = new QLineEdit(this);
    connect(line_search, SIGNAL(returnPressed()), this, SLOT(download()));
    box_layout->addWidget(line_search);

    QHBoxLayout* go_layout = new QHBoxLayout(this);

    QPushButton* btn_go = new QPushButton("Go!", this);
    connect(btn_go, SIGNAL(clicked()), this, SLOT(download()));
    go_layout->addWidget(btn_go);

    QLabel *lbl_picCount = new QLabel("Number of pictures: ", this);
    go_layout->addWidget(lbl_picCount);

    spn_picCount = new QSpinBox(this);
    spn_picCount->setValue(5);
    go_layout->addWidget(spn_picCount);


    box_layout->addLayout(go_layout);

    prgbar = new QProgressBar(this);
    box_layout->addWidget(prgbar);
    prgbar->setValue(0);
    prgbar->setTextVisible(true);
    prgbar->setMinimum(0);
    prgbar->setMaximum(1);
    prgbar->setValue(0);
    prgbar->setFormat("Loading... " + QString::number(prgbar->value()) + " / 0");

    prgbar->setStyleSheet("QProgressBar {"
                         "border: 2px solid grey;"
                         "border-radius: 5px;"
                         "text-align: center; }"

                          "QProgressBar::chunk {"
                               "background-color: #06b025;"
                               "width: 10px;"
                               "margin: 0.5px;"
                           "}");


    listArea = new QListWidget;
    listArea->setViewMode(QListWidget::IconMode);
    listArea->setIconSize(QSize(200, 200));
    listArea->setResizeMode(QListWidget::Adjust);
    listArea->setStyleSheet("QListWidget {"
                            "background-color: #282828;"
                            "color: white;"
                            "}"
                            );
    listArea->setMovement(QListWidget::Static);
    box_layout->addWidget(listArea);

    QWidget* wgt = new QWidget(this);
    wgt->setLayout(box_layout);
    setCentralWidget(wgt);

    box_layout->setAlignment(Qt::AlignHCenter);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED (event)
    qDebug() << "Close program";
    QString dirName = "PictureDownloaderIcons";
    QString temppath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/" + dirName;
    QDir dir(temppath);
    qDebug() << "Clear temp files " << dir.path();
    dir.removeRecursively();
}

void MainWindow::download()
{
    errorCounter = 0;
    if (!line_search->text().isEmpty())
    {
        downloader->download(line_search->text());
        downloader->setSearchNumber(spn_picCount->value());
        line_search->clear();
        listArea->clear();
        prgbar->setFormat("Downloading page... ");
    }
}

void MainWindow::AcceptData(QList<PictureItem*> picItemList)
{
    qDebug() << "DATA RECIEVED" << picItemList.count() << " ITEMS";

    for (int i = 0; i < picItemList.count(); ++i)
    {
        PictureItem *picItem = picItemList[i];
        QString itemName = "\"" + picItem->getPi_tooltip();
        int strokeLength = 35;
        if (itemName.count() > strokeLength)
        {
            itemName.remove(strokeLength,itemName.count());
            itemName += "...\"";
        } else itemName += "\"";
        picItem->setText(itemName);

        picItem->setToolTip(picItem->getPi_tooltip() + "\n"
                            + "Size: " + convertSize(picItem->getPi_fileSizeInBytes()) + "\n"
                            + "Resolution: " + QString::number(picItem->getPi_width()) + " x " + QString::number(picItem->getPi_height())
                            );
        listArea->addItem(picItem);
    }
        prgbar->setValue(prgbar->minimum());
        QString prgBarText = "Loading complete";
        if (errorCounter) prgBarText += " (" + QString::number(errorCounter) + " download errors)";
        prgbar->setFormat(prgBarText);

}

void MainWindow::picDownoloadProgress(int current, int total)
{
    //qDebug() << "Progress: " << current << "/" << total;

    prgbar->setMaximum(total);
    prgbar->setValue(current);
    QString prgBarText = "Downloading pictures... " + QString::number(prgbar->value()) + " / " + QString::number(prgbar->maximum());
    if (errorCounter) prgBarText += " (" + QString::number(errorCounter) + " errors)";

    prgbar->setFormat(prgBarText);
}

void MainWindow::pageDownloadProgress(qint64 value, qint64 total)
{
    prgbar->setMaximum(int(total));
    prgbar->setValue(int(value));
    prgbar->setFormat("Downloading page... ");
}

void MainWindow::error()
{
    ++errorCounter;
}

QString MainWindow::convertSize(int size)
{
    if (size < 1023) return QString::number(size)+"B";
    else if (size < 1048567) return QString::number(size/1024)+"Kb";
    else return QString::number(size/1024/1024)+"Mb";
    return "unknown";
}

