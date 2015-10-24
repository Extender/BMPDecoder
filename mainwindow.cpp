#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene());
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(buttonClicked(bool)));
    connect(ui->browseButton,SIGNAL(clicked(bool)),this,SLOT(browseButtonClicked(bool)));
    dialog=new QFileDialog(this);
    dialog->setNameFilter("Bitmaps (*.bmp *.dib)");
    connect(dialog,SIGNAL(currentChanged(QString)),this,SLOT(fileChanged(QString)));
    currentFile=QApplication::applicationDirPath().replace("/","\\")+"\\example.bmp";
    ui->lineEdit->setText(currentFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonClicked(bool checked)
{
    if(currentFile=="")
    {
        QMessageBox::critical(this,"Error","No file selected!");
        return;
    }
    QFile f(currentFile);
    if(!f.exists())
    {
        QMessageBox::critical(this,"Error","The selected file does not exist.");
        return;
    }
    f.open(QFile::ReadOnly);
    char *data=(char*)malloc(f.size());
    f.read(data,f.size());
    int32_t width;
    int32_t height;
    uint32_t *bmpData=bmp::decode(data,width,height);
    QImage *img=new QImage(width,height,QImage::Format_ARGB32);
    img->fill(0xFFFFFFFF);
    for(uint32_t y=0;y<height;y++)
    {
        for(uint32_t x=0;x<width;x++)
        {
            img->setPixel(x,y,bmpData[y*width+x]);
        }
    }
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(QPixmap::fromImage(*img));
}

void MainWindow::browseButtonClicked(bool checked)
{
    dialog->show();
}

void MainWindow::fileChanged(QString file)
{
    currentFile=file;
    ui->lineEdit->setText(file.replace("/","\\"));
}
