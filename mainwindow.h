#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include "bmp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QFileDialog *dialog;
    QString currentFile;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void buttonClicked(bool checked);
    void browseButtonClicked(bool checked);
    void fileChanged(QString file);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
