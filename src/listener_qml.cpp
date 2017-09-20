#include "ros/ros.h"
#include "std_msgs/String.h"

#include <functional>

#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QString>
#include <QQuickWindow>
#include <QtQml>

#include <signal.h>

#include "qml_mediator.h"

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
    QMLMediator* mediate;

    void dataCallback(const std_msgs::String::ConstPtr& msg)
    {
        mediate->addString("I heard: [" + QString(msg->data.c_str()) + "]");
    }
}

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QMLMediator mediate(&app);

    //Set up ros stuff
    ros::init(argc, argv, "listener");
    ros::NodeHandle node;

    cb::mediate = &mediate;
    ros::Subscriber sub = node.subscribe("chatter", 1000, cb::dataCallback);
    ros::AsyncSpinner rosspin(1);

    QQmlApplicationEngine engine(&app);
    engine.rootContext()->setContextProperty("mediator", &mediate);
    engine.load(QUrl("qrc:///qml/line_display.qml"));    

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