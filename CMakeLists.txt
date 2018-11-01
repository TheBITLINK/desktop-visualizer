cmake_minimum_required(VERSION 2.6)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project("Desktop Visualizer")
set(EXECUTABLE_NAME "tblvis")

add_executable(${EXECUTABLE_NAME} main.cpp)

# X11
find_package(X11)
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${X11_LIBRARIES})

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} sfml-graphics sfml-window sfml-system)

# fftw
find_package(FFTW3)
include_directories(${FFTW3_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${FFTW3_LIBRARIES})

# PulseAudio
find_package(PulseAudio)
include_directories(${PULSEAUDIO_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${PULSEAUDIO_LIBRARY})
target_link_libraries(${EXECUTABLE_NAME} pulse-simple)

# pthread
find_package(Threads)
target_link_libraries (${EXECUTABLE_NAME} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
