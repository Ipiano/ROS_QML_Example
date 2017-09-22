#include "ros/ros.h"
#include "std_msgs/String.h"

#include <functional>

#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QApplication>
#include <QUrl>
#include <QString>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

#include <signal.h>

#include "mainwindow.h"

using namespace std;

int main(int argc, char** argv)
{
    //Init ros stuff
    ros::init(argc, argv, "listener");
    ros::NodeHandle node;

    //Init Qt
    QApplication app(argc, argv);
    MainWindow w;
    ros::Subscriber sub = node.subscribe("chatter", 1000, &MainWindow::addString, &w);

    //Start ros in separate thread, and trigger Qt shutdown when it exits
    //If Qt exits before ros, be sure to shutdown ros
    QFutureWatcher<void> rosThread;
    rosThread.setFuture(QtConcurrent::run(&ros::spin));
    QObject::connect(&rosThread, &QFutureWatcher<void>::finished, &app, &QCoreApplication::quit);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [](){ros::shutdown();});

    //Show the window
    w.show();

    //Start qt app
    return app.exec();
}
