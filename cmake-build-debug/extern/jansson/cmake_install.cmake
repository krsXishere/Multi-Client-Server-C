# Install script for directory: /Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/extern/jansson

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/cmake-build-debug/extern/jansson/lib/libjansson.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjansson.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjansson.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjansson.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/cmake-build-debug/extern/jansson/include/jansson_config.h"
    "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/extern/jansson/src/jansson.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/cmake-build-debug/extern/jansson/jansson.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/jansson" TYPE FILE FILES
    "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/cmake-build-debug/extern/jansson/cmake/janssonConfig.cmake"
    "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/cmake-build-debug/extern/jansson/cmake/janssonConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/jansson/janssonTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/jansson/janssonTargets.cmake"
         "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/cmake-build-debug/extern/jansson/CMakeFiles/Export/9b80b8ed102b5ab69af1c2960c090ea5/janssonTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/jansson/janssonTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/jansson/janssonTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/jansson" TYPE FILE FILES "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/cmake-build-debug/extern/jansson/CMakeFiles/Export/9b80b8ed102b5ab69af1c2960c090ea5/janssonTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/jansson" TYPE FILE FILES "/Users/user/Documents/ikis/kuliah/Semester-3/Sistem-Operasi/Praktik/Tubes/server-multi-client/cmake-build-debug/extern/jansson/CMakeFiles/Export/9b80b8ed102b5ab69af1c2960c090ea5/janssonTargets-debug.cmake")
  endif()
endif()

