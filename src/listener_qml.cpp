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
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

#include "qml_mediator.h"

using namespace std;

int main(int argc, char** argv)
{
    //Init ros stuff
    ros::init(argc, argv, "listener");
    ros::NodeHandle node;

    //Init Qt
    QGuiApplication app(argc, argv);
    QMLMediator mediate(&app);
    ros::Subscriber sub = node.subscribe("chatter", 1000, &QMLMediator::addString, &mediate);

    //Start ros in separate thread, and trigger Qt shutdown when it exits
    //If Qt exits before ros, be sure to shutdown ros
    QFutureWatcher<void> rosThread;
    rosThread.setFuture(QtConcurrent::run(&ros::spin));
    QObject::connect(&rosThread, &QFutureWatcher<void>::finished, &app, &QCoreApplication::quit);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [](){ros::shutdown();});

    QQmlApplicationEngine engine(&app);
    engine.rootContext()->setContextProperty("mediator", &mediate);
    engine.load(QUrl("qrc:///qml/line_display.qml"));    

    //Start qt app
    return app.exec();
}
