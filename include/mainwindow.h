#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addString(const QString& s)
    {
        newString(s);
    }

    //Emit this as a signal to be caught locally in order to prevent race conditions
    void addString(const std_msgs::String::ConstPtr& msg)
    {
        newString("I heard: [" + QString(msg->data.c_str()) + "]");
    }

private:
    Ui::MainWindow *ui;


signals:
    void stringsChanged();
    void newString(QString s);

private slots:
    void newStringSlot(QString txt);
};

#endif // MAINWINDOW_H
