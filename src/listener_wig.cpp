#include "ros/ros.h"
#include "std_msgs/String.h"

#include <functional>

#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QApplication>
#include <QUrl>
#include <QString>

#include <signal.h>

#include "mainwindow.h"

using namespace std;

namespace sigint
{
    QCoreApplication* mainApp = nullptr;

    void sigint_handler(int code)
    {
        ros::shutdown();

        if(mainApp)
        {
            mainApp->exit();
        }
    }
}

namespace cb
{
    MainWindow* win;

    void dataCallback(const std_msgs::String::ConstPtr& msg)
    {
        win->addTextLine("I heard: [" + QString(msg->data.c_str()) + "]");
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow w;
    cb::win = &w;

    //Timer to periodically check that ros is still alive
    QTimer rosCheck;
    rosCheck.setInterval(1000);
    QObject::connect(&rosCheck, &QTimer::timeout, [&]()
    {
        if(!ros::ok()) app.exit();
    });
    rosCheck.start();

    //Set up ros stuff
    ros::init(argc, argv, "listener");
    ros::NodeHandle node;

    ros::Subscriber sub = node.subscribe("chatter", 1000, cb::dataCallback);
    ros::AsyncSpinner rosspin(1);

    //Put pointer to main app into sigint namespace
    //so handler can exit it
    sigint::mainApp = &app;

    //Register our sigint handler to override the ros one
    signal(SIGINT, &sigint::sigint_handler);

    //Start ros spinner
    rosspin.start();

    //Show the window
    w.show();

    //Start qt app
    return app.exec();
}
