#include "ros/ros.h"
#include "std_msgs/String.h"

#include <QCoreApplication>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

using namespace std;

void dataCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char** argv)
{
    //Init ros stuff
    ros::init(argc, argv, "listener");
    ros::NodeHandle node;
    ros::Subscriber sub = node.subscribe("chatter", 1000, dataCallback);

    //Init Qt
    QCoreApplication app(argc, argv);

    //Start ros in separate thread, and trigger Qt shutdown when it exits
    //If Qt exits before ros, be sure to shutdown ros
    QFutureWatcher<void> rosThread;
    rosThread.setFuture(QtConcurrent::run(&ros::spin));
    QObject::connect(&rosThread, &QFutureWatcher<void>::finished, &app, &QCoreApplication::quit);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [](){ros::shutdown();});

    //Start qt app
    return app.exec();
}
