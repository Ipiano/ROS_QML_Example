#include "ros/ros.h"
#include "std_msgs/String.h"

#include <QCoreApplication>
#include <QTimer>

#include <signal.h>

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

void dataCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);    

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
    ros::Subscriber sub = node.subscribe("chatter", 1000, dataCallback);
    ros::AsyncSpinner rosspin(1);

    //Put pointer to main app into sigint namespace
    //so handler can exit it
    sigint::mainApp = &app;
    
    //Register our sigint handler to override the ros one
    signal(SIGINT, &sigint::sigint_handler);

    //Start ros spinner
    rosspin.start();

    //Start qt app
    return app.exec();
}
