cmake_minimum_required(VERSION 2.8.3)
project(pubsub)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

# Find Qt5 libraries
find_package(Qt5 REQUIRED COMPONENTS
  Core
  Qml
  Quick
  Widgets
)

# Configuration to make Qt compiles go smoother
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
include_directories( ${CMAKE_BINARY_DIR} )

# Configure Catkin dependencies
catkin_package(
  CATKIN_DEPENDS
    roscpp
    std_msgs
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

###############################
# Basic, non-qt talker/listener
add_executable(listener src/listener.cpp)
add_executable(talker src/talker.cpp)


target_link_libraries(listener
  ${catkin_LIBRARIES}
)

target_link_libraries(talker
  ${catkin_LIBRARIES}
)

################################################
# Cmd line talker/listener running qt event loop
add_executable(listener_qt src/listener_qt.cpp)
add_executable(talker_qt src/talker_qt.cpp)

# Configure Qt5 Modules
# Equivalent to doing
# Qt += Core
# In Qt .pro file
qt5_use_modules(listener_qt Core)
qt5_use_modules(talker_qt Core)

target_link_libraries(listener_qt
  ${catkin_LIBRARIES}
)

target_link_libraries(talker_qt
  ${catkin_LIBRARIES}
)

######################################
# Talker and Listener using QML gui

# Run MOC on headers of Q_OBJECT classes
set(MOC_HDRS include/qml_mediator.h)
qt5_wrap_cpp(MOC_SRCS ${MOC_HDRS})

# Run RCC on QRC file
set(RCC_FILES src/display.qrc)
qt5_add_resources(RCC_SOURCES ${RCC_FILES})

# Build executables with source, MOC result files, RCC result files
add_executable(listener_qml src/listener_qml.cpp ${MOC_SRCS} ${RCC_SOURCES})
add_executable(talker_qml src/talker_qml.cpp ${MOC_SRCS} ${RCC_SOURCES})

# Configure Qt5 Modules
# Equivalent to doing
# Qt += Core Qml Quick
# In Qt .pro file
qt5_use_modules(listener_qml Core Qml Quick)
qt5_use_modules(talker_qml Core Qml Quick)

target_link_libraries(listener_qml
  ${catkin_LIBRARIES}
)

target_link_libraries(talker_qml
  ${catkin_LIBRARIES}
)

########################################
# Talker and Listener using QWidgets gui

# Run MOC on headers of Q_OBJECT classes
set(MOC_HDRS include/mainwindow.h)
qt5_wrap_cpp(MOC_SRCS ${MOC_HDRS})

# Run UIC on ui definitions
set(UI_FILES ui/mainwindow.ui)
qt5_wrap_ui(UI_SRCS ${UI_FILES})

# Build executables with source, MOC result files, UIC result files
add_executable(talker_wig src/talker_wig.cpp src/mainwindow.cpp ${MOC_SRCS} ${UI_SRCS})
add_executable(listener_wig src/listener_wig.cpp src/mainwindow.cpp ${MOC_SRCS} ${UI_SRCS})

# Configure Qt5 Modules
# Equivalent to doing
# Qt += Core Widgets
# In Qt .pro file
qt5_use_modules(talker_wig Core Widgets)
qt5_use_modules(listener_wig Core Widgets)

target_link_libraries(talker_wig
  ${catkin_LIBRARIES}
)

target_link_libraries(listener_wig
  ${catkin_LIBRARIES}
)
