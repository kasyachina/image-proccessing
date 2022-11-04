#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>

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

    void on_save_as_triggered();

private:
    bool srcProccessed = false;
    Ui::MainWindow *ui;
    QString filepath;
    QImage src;
    QImage dst;
};
#endif // MAINWINDOW_H
