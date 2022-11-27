# Install script for directory: /home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/amine102/projects/vcpkg/packages/jsoncpp_x64-linux/debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/json" TYPE FILE FILES
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/allocator.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/assertions.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/config.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/forwards.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/json.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/json_features.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/reader.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/value.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/version.h"
    "/home/amine102/projects/vcpkg/buildtrees/jsoncpp/src/1.9.5-13b47286ba.clean/include/json/writer.h"
    )
endif()

