#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuBar()->installEventFilter(&filter);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete h;
}

void MainWindow::clearLayout()
{
    delete lsrc;
    delete ldst;
    delete h;
    h = nullptr;
    lsrc = nullptr;
    ldst = nullptr;
}

void MainWindow::getSrc(cv::Mat& cvsrc, cv::Mat& cvdst)
{
    if (!filepath.isEmpty())
    {
        cvsrc = cv::imread(filepath.toStdString(), cv::IMREAD_GRAYSCALE);
    }
    else
    {
        switch(curProccess)
        {
            case imageProccess::Blur:
                QImage img(":/images/blur.bmp");
                cvsrc = cv::Mat(img.height(), img.width(), CV_8U, img.bits(), img.bytesPerLine()).clone();
                break;
        };

        //handle default images
    }
    cvdst = cvsrc.clone();
}

void MainWindow::setData(const cv::Mat& cvsrc, const cv::Mat& cvdst)
{
    srcProccessed = true;
    clearLayout();
    QImage im1((uchar*) cvsrc.data, cvsrc.cols, cvsrc.rows, cvsrc.step, QImage::Format_Grayscale8);
    QImage im2((uchar*) cvdst.data, cvdst.cols, cvdst.rows, cvdst.step, QImage::Format_Grayscale8);
    h = new QHBoxLayout(centralWidget());
    src.convertFromImage(im1);
    dst.convertFromImage(im2);
    lsrc = new QLabel(this), ldst = new QLabel(this);
    lsrc -> setPixmap(src);
    ldst -> setPixmap(dst);
    h -> addWidget(lsrc);
    h -> addWidget(ldst);
}

int MainWindow::getHistogramThreshold(const cv::Mat& cvsrc)
{
    int prev = 0, cur = 128;
    const int eps = 5;
    int sums[2], cnts[2], u[2];

    //calculate histogram manually
    int hist[256] = {0};
    for(int i = 0; i < cvsrc.rows; ++i)
        for(int j = 0; j < cvsrc.cols; ++j)
        {
            ++hist[cvsrc.at<uchar>(i,j)];
        }

    while(std::abs<int>(cur - prev) > eps)
    {
        sums[0] = sums[1] = 0;
        cnts[0] = cnts[1] = 0;
        for (int i = 0; i < 256; ++i)
        {
            sums[i > cur] += hist[i] * i;
            cnts[i > cur] += hist[i];
        }
        u[0] = sums[0] / (std::max(cnts[0], 1));
        u[1] = sums[1] / (std::max(cnts[1], 1));
        prev = cur;
        cur = (u[0] + u[1]) / 2;
    }
    return cur;
}

void MainWindow::on_open_triggered()
{
    filepath = QFileDialog::getOpenFileName(this, "Открыть файл", "/home", "Images (*.jpg *.gif *.tif *.bmp *.png *.pcx)");
    if (!filepath.isEmpty())
    {
        ui -> default_images -> setChecked(false);
        ui -> default_images -> setEnabled(true);
        clearLayout();
        statusBar() -> showMessage("Выбран файл: " + filepath);
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
    curProccess = imageProccess::Blur;
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::blur(cvsrc, cvdst, cv::Size(5, 5));
    setData(cvsrc, cvdst);
}


void MainWindow::on_gaussian_triggered()
{
    curProccess = imageProccess::Gaussian;
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::GaussianBlur(cvsrc, cvdst, cv::Size(5, 5), 0, 0);
    setData(cvsrc, cvdst);
}


void MainWindow::on_adaptive_thresholding_triggered()
{
    curProccess = imageProccess::Adaptive;
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::adaptiveThreshold(cvsrc, cvdst, 255, cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C, cv::ThresholdTypes::THRESH_BINARY, 7, 0);
    setData(cvsrc, cvdst);
}


void MainWindow::on_otsu_thresholding_triggered()
{
    curProccess = imageProccess::Otsu;
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::threshold(cvsrc, cvdst, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    setData(cvsrc, cvdst);
}


void MainWindow::on_histogramm_thresholding_triggered()
{
    curProccess = imageProccess::Histogram;
    cv::Mat cvdst, cvsrc;
    getSrc(cvsrc, cvdst);
    cv::threshold(cvsrc, cvdst, getHistogramThreshold(cvsrc), 255, cv::THRESH_BINARY);
    setData(cvsrc, cvdst);
}

