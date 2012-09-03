# Install script for directory: /home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so.6.0.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/src/libchipmunk.so.6.0.3"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/src/libchipmunk.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so.6.0.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libchipmunk.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/src/libchipmunk.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/chipmunk" TYPE FILE FILES
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/chipmunk.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/chipmunk_ffi.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/chipmunk_private.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/chipmunk_types.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/chipmunk_unsafe.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/cpArbiter.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/cpBB.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/cpBody.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/cpPolyShape.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/cpShape.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/cpSpace.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/cpSpatialIndex.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/cpVect.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/chipmunk/constraints" TYPE FILE FILES
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpConstraint.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpDampedRotarySpring.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpDampedSpring.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpGearJoint.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpGrooveJoint.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpPinJoint.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpPivotJoint.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpRatchetJoint.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpRotaryLimitJoint.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpSimpleMotor.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/cpSlideJoint.h"
    "/home/rpg/Игры/cheetah/thirdparty/Chipmunk-6.0.3/include/chipmunk/constraints/util.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

