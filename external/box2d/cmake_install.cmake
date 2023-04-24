# Install script for directory: C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/box2d")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/include/box2d")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/src/cmake_install.cmake")
  include("C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/unit-test/cmake_install.cmake")
  include("C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/extern/glad/cmake_install.cmake")
  include("C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/extern/glfw/cmake_install.cmake")
  include("C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/extern/imgui/cmake_install.cmake")
  include("C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/extern/sajson/cmake_install.cmake")
  include("C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/testbed/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/Zun Wang/Documents/CS5850/FinalProject/finalproject-ecs/finalproject-boston-group6/Engine/external/box2d/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
