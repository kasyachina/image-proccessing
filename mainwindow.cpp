#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBoxLayout>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_triggered()
{
    filepath = QFileDialog::getOpenFileName(this, "Открыть файл", "/home", "Images (*.jpg *.gif *.tif *.bmp *.png *.pcx)");
    if (!filepath.isEmpty())
    {
        ui -> default_images -> setChecked(false);
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

