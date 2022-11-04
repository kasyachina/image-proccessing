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
        clearLayout();
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


void MainWindow::on_adaptive_thresholding_triggered()
{
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::adaptiveThreshold(cvsrc, cvdst, 255, cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C, cv::ThresholdTypes::THRESH_BINARY, 7, 0);
    setData(cvsrc, cvdst);
}


void MainWindow::on_otsu_thresholding_triggered()
{
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::threshold(cvsrc, cvdst, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    setData(cvsrc, cvdst);
}


void MainWindow::on_histogramm_thresholding_triggered()
{
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::threshold(cvsrc, cvdst, getHistogramThreshold(cvsrc), 255, cv::THRESH_BINARY);
    setData(cvsrc, cvdst);
}

