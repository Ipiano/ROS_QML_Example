#include "ros/ros.h"
#include "std_msgs/String.h"

#include <thread>
#include <chrono>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "talker");

    ros::NodeHandle node;

    ros::Publisher pub = node.advertise<std_msgs::String>("chatter", 1000);

    int i=0;
    while(ros::ok())
    {
        std_msgs::String msg;

        msg.data = string("Message #" + to_string(i++)).c_str();
        ROS_INFO("Sending [%s]", msg.data.c_str());

        pub.publish(msg);

        ros::spinOnce();

        this_thread::sleep_for(chrono::seconds(5));
    }

    return 0;
}