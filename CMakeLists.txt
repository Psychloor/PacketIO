
cmake_minimum_required(VERSION 3.31)
project(PacketIO
        VERSION 1.0.0
        DESCRIPTION "A packet I/O library"
        LANGUAGES CXX)

option(PACKETIO_ENABLE_UNSAFE_READS "Enable unsafe reads in packet_reader" OFF)

# Specify C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add visibility settings
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# Create a regular static library instead of INTERFACE
add_library(${PROJECT_NAME} STATIC
        src/packet_buffers.cpp
        include/framed_packet_reader.hpp
        include/packet_buffers.hpp
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Set include directories
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
        $<$<BOOL:${PACKETIO_ENABLE_UNSAFE_READS}>:PACKETIO_ENABLE_UNSAFE_READS>
)

# Installation rules
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)