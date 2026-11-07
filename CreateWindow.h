//
// Created by valen on 07/11/2026.
//

#ifndef INTERESTCALCULATOR_CREATEWINDOW_H
#define INTERESTCALCULATOR_CREATEWINDOW_H
#include <functional>

#include "GLFW/glfw3.h"


class CreateWindow
{
public :
    static void Start();
    static void Start(const std::function<void()>&);
private:
    static GLFWwindow* InitGLFW();
    static void InitImGui(GLFWwindow*);
    static void DoRender(GLFWwindow*);
    static void DoNewFrame();
    static void WindowsLife(GLFWwindow*);
    static void WindowsLife(GLFWwindow*, const std::function<void()>&);
    static void CleanWindows(GLFWwindow*);
};


#endif //INTERESTCALCULATOR_CREATEWINDOW_H