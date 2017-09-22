#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, &MainWindow::newString, this, &MainWindow::newStringSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newStringSlot(QString txt)
{
    ui->list_lines->addItem(txt);
}
