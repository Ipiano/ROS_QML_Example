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

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);  
    
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
        ROS_INFO("Sending [%s]", msg.data.c_str());

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

    //Start main app
    return app.exec();
}