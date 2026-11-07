cmake_minimum_required(VERSION 4.0)
project(InterestCalculator)

set(CMAKE_CXX_STANDARD 20)

# =====================================================
# GLFW
# =====================================================

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

# =====================================================
# OpenGL
# =====================================================

find_package(OpenGL REQUIRED)

# =====================================================
# Dear ImGui
# =====================================================

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ImGUI/imgui)

if(NOT EXISTS "${IMGUI_DIR}/imgui.cpp")
    message(FATAL_ERROR "Dear ImGui introuvable dans ${IMGUI_DIR}")
endif()

add_library(imgui STATIC

        # ImGui Core
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp

        # ImGui Backends
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC
        glfw
        OpenGL::GL
)

# =====================================================
# Application
# =====================================================

add_executable(InterestCalculator
        interestCalculator.cpp
        Invest.cpp
        Pret.cpp
        FormatNumber.cpp
        InvestAndPret.cpp
        CreateWindow.cpp
)
target_link_libraries(InterestCalculator PRIVATE
        imgui
)