cmake_minimum_required(VERSION 2.6)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project("Desktop Visualizer")
set(EXECUTABLE_NAME "vis")

add_executable(${EXECUTABLE_NAME} main.cpp)

# X11
find_package(X11)
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${X11_LIBRARIES})

# SFML
find_package(SFML 2 REQUIRED graphics window system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

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
