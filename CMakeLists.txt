cmake_minimum_required(VERSION 2.6)
project ( O2_Prototype CXX)

# Set compiler to C++11
set (CMAKE_CXX_STANDARD 11)
if (CMAKE_VERSION VERSION_LESS "3.1")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
  endif ()
else ()
  set (CMAKE_CXX_STANDARD 11)
endif ()

# declaring files in your project

set ( SOURCES
     main.cpp
     gui/MainWindow.cpp
     controller/Sensors.cpp     
     controller/UART.cpp
     controller/OxygenController.cpp
     controller/GPIO.cpp
     controller/PWM.cpp
      )

set ( MOC_HEADERS
       gui/MainWindow.h
       controller/Sensors.h
       controller/Pinout.h
       controller/UART.h
       controller/OxygenController.h
       controller/GPIO.h
       controller/PWM.h
      )
#
#   set ( UIS
#        mainwindow.ui
#        )
#
#    set ( RESOURCES
#         main.qrc
#         )

# Next lines needed for building all Qt projects
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
include_directories( ${CMAKE_BINARY_DIR} )
# Next, using precompiler, compiler and linker
# using Qt meta-system (precompiler)
QT4_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )
QT4_WRAP_UI( UI_HEADERS ${UIS} )
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )
# compile
add_executable( O2_Prototype ${SOURCES} ${MOC_SRCS} ${RES_SOURCES}
         ${UI_HEADERS} )

# or use line below instead, if you using Windows ™ Operating System.
#add_executable( O2_Prototype WIN32 ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )

# Threads
find_package(Threads)

# WiringPI
find_library(wiringPi_LIB wiringPi)

# build it (link libraries)
target_link_libraries( O2_Prototype ${QT_LIBRARIES} ${wiringPi_LIB} ${CMAKE_THREAD_LIBS_INIT})
