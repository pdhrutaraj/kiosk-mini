cmake_minimum_required(VERSION 3.16)

# ------------------------------------------------------------
# Project
# ------------------------------------------------------------
project(kiosk-mini
    VERSION 1.0.0
    LANGUAGES CXX
)

# ------------------------------------------------------------
# Cross-compilation sanity
# ------------------------------------------------------------

message(STATUS "Building kiosk-mini")
message(STATUS "CMAKE_SYSTEM_NAME     = ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR= ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_CXX_COMPILER    = ${CMAKE_CXX_COMPILER}")

# ------------------------------------------------------------
# C++ standard (safe for Qt6 + Yocto)
# ------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------
# Build type default (Yocto SDK often omits it)
# ------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# ------------------------------------------------------------
# Compiler flags (i.MX93 Cortex-A55)
# ------------------------------------------------------------
add_compile_options(
    -Wall
    -Wextra
    -Wno-psabi
    -O2
#    -mcpu=cortex-a55
)

# ------------------------------------------------------------
# Qt 6 (Widgets only – no QtMultimedia)
# ------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Widgets
)

qt_standard_project_setup()

# ------------------------------------------------------------
# ZXing (vendored)
# ------------------------------------------------------------
# third_party/zxing must contain ZXing CMakeLists.txt
# ============================================================
# ZXing (embedded-safe, NO OpenCV, NO examples, NO tests)
# ============================================================
#set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Old + new option names (ZXing changed them over time)
#set(ZXING_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(ZXING_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(ZXING_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
#set(ZXING_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)

# OpenCV hard-disable
#set(ZXING_USE_OPENCV OFF CACHE BOOL "" FORCE)
#set(ZXING_ENABLE_OPENCV OFF CACHE BOOL "" FORCE)

# C API not needed
#set(ZXING_C_API OFF CACHE BOOL "" FORCE)

#set(OpenCV_FOUND FALSE)
#set(OpenCV_DIR "" CACHE PATH "" FORCE)

# ============================================================
# ZXing — build ONLY core library (NO examples, NO OpenCV)
# ============================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
# Force static ZXing
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Disable ZXing extras
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(ZXING_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/zxing-cpp)

#add_subdirectory(
#    external/zxing-cpp/core/src
#    ${CMAKE_BINARY_DIR}/zxing-core
#)

#add_library(ZXing::ZXing ALIAS ZXing)
#add_subdirectory(external/zxing-cpp EXCLUDE_FROM_ALL)
#add_subdirectory(external/zxing-cpp)

# ------------------------------------------------------------
# Source layout
# ------------------------------------------------------------

# ------------------------------------------------------------
# Executable
# ------------------------------------------------------------
#add_executable(kiosk-mini
#    ${APP_SOURCES}
#)
qt_standard_project_setup()

qt_add_executable(kiosk-mini
    main.cpp

    model/barcodemodel.cpp

    controller/barcodegenerator.cpp
    controller/barcodescanner

    view/mainwindow.cpp


#    platform/v4l2camera.cpp
#    platform/rotaryencoder.cpp
)


# ------------------------------------------------------------
# Include paths
# ------------------------------------------------------------
target_include_directories(kiosk-mini
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ------------------------------------------------------------
# Link libraries
# ------------------------------------------------------------
target_link_libraries(kiosk-mini
    PRIVATE
        Qt6::Widgets
        ZXing::ZXing
)

# ------------------------------------------------------------
# Definitions for embedded Linux
# ------------------------------------------------------------
#target_compile_definitions(kiosk-mini
#    PRIVATE
#        QT_NO_KEYWORDS
#        QT_WIDGETS_LIB
#)

# ------------------------------------------------------------
# RPATH handling (important for Yocto runtime)
# ------------------------------------------------------------
set_target_properties(kiosk-mini PROPERTIES
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN"
)

# ------------------------------------------------------------
# Install rules (Yocto-friendly)
# ------------------------------------------------------------
install(TARGETS kiosk-mini
    RUNTIME DESTINATION bin
)


