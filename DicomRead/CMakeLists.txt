cmake_minimum_required(VERSION 3.10)

# Project name and version
project(DICOMReader VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find DCMTK package with necessary components
find_package(DCMTK REQUIRED
    COMPONENTS dcmdata dcmimgle dcmimage dcmjpeg dcmjpls dcmnet oflog ofstd)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${DCMTK_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

# Source files
set(SRC_FILES main.cpp DICOMReader.cpp)

# Add the executable target
add_executable(DICOMReader ${SRC_FILES})

# Link the necessary libraries
target_link_libraries(DICOMReader ${DCMTK_LIBRARIES} ${OpenCV_LIBRARIES})
