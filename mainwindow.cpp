#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene());
    connect(ui->loadButton,SIGNAL(clicked(bool)),this,SLOT(loadButtonClicked()));
    connect(ui->browseButton,SIGNAL(clicked(bool)),this,SLOT(browseButtonClicked()));
    dialog=new QFileDialog(this);
    dialog->setNameFilter("Bitmaps (*.bmp *.dib)");
    connect(dialog,SIGNAL(currentChanged(QString)),this,SLOT(fileChanged(QString)));
    QString testFilePath=QApplication::applicationDirPath()+"/example.bmp";
    QFile testFile(testFilePath);
    if(testFile.exists())
    {
        currentFile=testFilePath;
        ui->lineEdit->setText(currentFile);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadButtonClicked()
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
    for(int32_t y=0;y<height;y++)
    {
        for(int32_t x=0;x<width;x++)
        {
            img->setPixel(x,y,bmpData[y*width+x]);
        }
    }
    free(bmpData);
    free(data);
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(QPixmap::fromImage(*img));
}

void MainWindow::browseButtonClicked()
{
    dialog->exec();
}

void MainWindow::fileChanged(QString file)
{
    currentFile=file;
    ui->lineEdit->setText(file.replace("/","\\"));
    ui->loadButton->click();
}
