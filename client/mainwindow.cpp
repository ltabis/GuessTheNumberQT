#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    QObject::connect(_ui->pushButton, SIGNAL(clicked()),this, SLOT(on_sendButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

// Open up a socket to send the data
void MainWindow::on_sendButton_clicked()
{
    _ui->label->setText(QCoreApplication::translate("MainWindow", "button clicked", nullptr));
}