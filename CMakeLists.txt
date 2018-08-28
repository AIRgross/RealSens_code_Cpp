cmake_minimum_required( VERSION 3.6 )

# Require C++11 (or later)
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Create Project
project( Realsens )
add_executable( Realsens realsense.h realsense.cpp main.cpp )

# Set StartUp Project
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "Realsens" )

# Find Package
# librealsense2
set( realsense2_DIR "C:/Program Files/librealsense2/lib/cmake/realsense2" CACHE PATH "Path to librealsense2 config directory." )
find_package( realsense2 REQUIRED )

# OpenCV
set( OpenCV_DIR "C:/Program Files/opencv/build" CACHE PATH "Path to OpenCV config directory." )
find_package( OpenCV REQUIRED )

if( realsense2_FOUND AND OpenCV_FOUND )
  # Additional Include Directories
  include_directories( ${realsense_INCLUDE_DIR} )
  include_directories( ${OpenCV_INCLUDE_DIRS} )

  # Additional Dependencies
  target_link_libraries( Realsens ${realsense2_LIBRARY} )
  target_link_libraries( Realsens ${OpenCV_LIBS} )
endif()
