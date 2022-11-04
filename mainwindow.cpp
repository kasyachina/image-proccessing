#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete h;
}

void MainWindow::on_open_triggered()
{
    filepath = QFileDialog::getOpenFileName(this, "Открыть файл", "/home", "Images (*.jpg *.gif *.tif *.bmp *.png *.pcx)");
    if (!filepath.isEmpty())
    {
        ui -> default_images -> setChecked(false);
        ui -> default_images -> setEnabled(true);
    }
}

void MainWindow::on_save_as_triggered()
{
    if (srcProccessed)
    {
        QString savepath = QFileDialog::getSaveFileName(this, "Сохранить файл как", "/home");
        if (!savepath.isEmpty())
        {
            QFile file(savepath);
            if (file.open(QIODevice::WriteOnly))
            {
                QDataStream out(&file);
                out << dst;
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Невозможно открыть файл");
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Нечего сохранять");
    }
}


void MainWindow::on_default_images_triggered()
{
    filepath = "";
    clearLayout();
    srcProccessed = false;
    ui -> default_images -> setEnabled(false);
}


void MainWindow::on_blur_triggered()
{
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::blur(cvsrc, cvdst, cv::Size(5, 5));
    setData(cvsrc, cvdst);
}


void MainWindow::on_gaussian_triggered()
{
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::GaussianBlur(cvsrc, cvdst, cv::Size(5, 5), 0, 0);
    setData(cvsrc, cvdst);
}

