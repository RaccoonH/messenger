cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(messenger LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Network)

set(project_headers
	    channel.h
        channelinfo.h
        chat.h
        client.h
        clientinfo.h
        connector.h
        data_struct.h
        group.h
        groupinfo.h
        listofcommands.h
        message.h
        ui/chatdisplay.h
        ui/chatwidget.h
        ui/clientinfo_widnow.h
        ui/clientlabel.h
        ui/clientlist.h
        ui/clientsettings_window.h
        ui/login_window.h
        ui/mainwindow.h
        ui/messagewidget.h
        ui/messenger_window.h
        ui/registration_window.h
        ui/settings_window.h
        ui/settingsui.h
        ui/user_label.h
        user.h
        userinfo.h)

set(project_sources 
		channel.cpp
        channelinfo.cpp
        chat.cpp
        client.cpp
        clientinfo.cpp
        connector.cpp
        group.cpp
        groupinfo.cpp
        main.cpp
        message.cpp
        ui/chatdisplay.cpp
        ui/chatwidget.cpp
        ui/clientinfo_widnow.cpp
        ui/clientlabel.cpp
        ui/clientlist.cpp
        ui/clientsettings_window.cpp
        ui/login_window.cpp
        ui/mainwindow.cpp
        ui/messagewidget.cpp
        ui/messenger_window.cpp
        ui/registration_window.cpp
        ui/settings_window.cpp
        ui/user_label.cpp
        user.cpp
        userinfo.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${project_headers} ${project_sources})

add_executable(${PROJECT_NAME} WIN32 ${project_headers} ${project_sources} 
    ${project_sources_moc} ${project_headers_wrapped})

target_include_directories(${PROJECT_NAME} PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
	
target_link_libraries(${PROJECT_NAME} ${QT_QTMAIN_LIBRARY}
    PUBLIC 
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
	Qt5::Network)