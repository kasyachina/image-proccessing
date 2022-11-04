#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_open_triggered();

    void on_default_images_triggered();

    void on_blur_triggered();

    void on_gaussian_triggered();

    void on_adaptive_thresholding_triggered();

    void on_otsu_thresholding_triggered();

private:
    void clearLayout()
    {
        delete lsrc;
        delete ldst;
        delete h;
        h = nullptr;
    }
    void getSrc(cv::Mat& cvsrc, cv::Mat& cvdst)
    {
        if (!filepath.isEmpty())
        {
            cvsrc = cv::imread(filepath.toStdString(), cv::IMREAD_GRAYSCALE);
        }
        else
        {
            //handle default images
        }
        cvdst = cvsrc.clone();
    }
    void setData(const cv::Mat& cvsrc, const cv::Mat& cvdst)
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
    bool srcProccessed = false;
    QHBoxLayout *h = nullptr;
    Ui::MainWindow *ui;
    QString filepath;
    QPixmap src;
    QPixmap dst;
    QLabel *lsrc = nullptr;
    QLabel *ldst = nullptr;
};
#endif // MAINWINDOW_H
