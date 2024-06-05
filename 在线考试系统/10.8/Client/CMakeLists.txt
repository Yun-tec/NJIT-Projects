cmake_minimum_required(VERSION 3.27)
project(Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Debug)

add_executable(Client
        Debug/Client.pch
        Client.cpp
        Client.h
        ClientDlg.cpp
        ClientDlg.h
        ClientSocket.cpp
        ClientSocket.h
        Item.cpp
        Item.h
        LoginDlg.cpp
        LoginDlg.h
        resource.h
        StdAfx.cpp
        StdAfx.h)
