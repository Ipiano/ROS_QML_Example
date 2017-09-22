#include "ros/ros.h"
#include "std_msgs/String.h"

#include <thread>
#include <chrono>
#include <string>
#include <functional>
#include <iostream>

#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QGuiApplication>
#include <QUrl>
#include <QString>
#include <QApplication>

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

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow w;

    //Timer to periodically check that ros is still alive
    QTimer rosCheck;
    rosCheck.setInterval(1000);
    QObject::connect(&rosCheck, &QTimer::timeout, [&]()
    {
        if(!ros::ok()) app.exit();
    });
    rosCheck.start();

    //5 second timer to publish
    QTimer sec5;
    sec5.setInterval(5000);

    //Init ros stuff
    ros::init(argc, argv, "talker");
    ros::NodeHandle node;
    ros::Publisher pub = node.advertise<std_msgs::String>("chatter", 1000);
    ros::AsyncSpinner rosspin(1);

    //Set up slot for 5 second timer
    int i=0;
    QObject::connect(&sec5, &QTimer::timeout, [&]()
    {
        std_msgs::String msg;

        msg.data = string("Message #" + to_string(i++)).c_str();
        w.addTextLine("Sending [" + QString(msg.data.c_str()) + "]");

        pub.publish(msg);
    });

    //Put pointer to main app into sigint namespace
    //so handler can exit it
    sigint::mainApp = &app;

    //Register our sigint handler to override the ros one
    signal(SIGINT, &sigint::sigint_handler);

    //Start ros spinner
    rosspin.start();

    //Start timer
    sec5.start();

    //Show main window
    w.show();

    //Start main app
    return app.exec();
}
