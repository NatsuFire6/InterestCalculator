//
// Created by valen on 07/11/2026.
//

#include "CreateWindow.h"

#include <functional>

#include "Invest.h"
#include <iostream>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "ImGUI/imgui/imgui.h"

GLFWwindow* CreateWindow::InitGLFW()
{
    // ================================================
    // Initialisation GLFW
    // ================================================
    if (!glfwInit())
    {
        std::cerr << "Impossible d'initialiser GLFW" << std::endl;
        //return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "Mon application ImGui",
        nullptr,
        nullptr
    );
    if (window == nullptr)
    {
        std::cerr << "Impossible de creer la fenetre" << std::endl;
        glfwTerminate();
        //return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}
void CreateWindow::InitImGui(GLFWwindow* window)
{
    // ================================================
    // Initialisation ImGui
    // ================================================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}
void CreateWindow::DoRender(GLFWwindow* window)
{
    // ============================================
    // Rendu
    // ============================================
    ImGui::Render();
    int displayWidth;
    int displayHeight;

    glfwGetFramebufferSize(
        window,
        &displayWidth,
        &displayHeight
    );

    glViewport(0, 0, displayWidth, displayHeight);
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}
void CreateWindow::DoNewFrame()
{
    glfwPollEvents();
    // Nouvelle frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void CreateWindow::WindowsLife(GLFWwindow* window)
{
    // ================================================
    // Boucle principale
    // ================================================
    while (!glfwWindowShouldClose(window))
    {
        DoNewFrame();
        // ============================================
        // Interface ImGui
        // ============================================
            ImGui::Begin("Ma premiere fenetre");
            ImGui::Text("Bonjour depuis Dear ImGui !");
            static int compteur = 0;
            if (ImGui::Button("Cliquer ici"))
            {
                compteur++;
            }
            ImGui::Text("Compteur : %d", compteur);
            ImGui::End();
            ImGui::Begin("Ma deuxième fenetre");
            Invest inv {10000,5,false,100};
            ImVec4 color {0.5,0.5,1,1};
            ImGui::TextColored(color,inv.Info().c_str());
            ImGui::End();
        DoRender(window);
    }
}
void CreateWindow::WindowsLife(GLFWwindow* window, const std::function<void()>& f)
{
    while (!glfwWindowShouldClose(window))
    {
        DoNewFrame();
        f();
        DoRender(window);
    }
}
void CreateWindow::CleanWindows(GLFWwindow* window)
{
    // ================================================
    // Nettoyage
    // ================================================
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
void CreateWindow::Start()
{
    GLFWwindow* window = InitGLFW();
    InitImGui(window);
    WindowsLife(window);
    CleanWindows(window);
}
void CreateWindow::Start(const std::function<void()>& f)
{
    GLFWwindow* window = InitGLFW();
    InitImGui(window);
    WindowsLife(window, f);
    CleanWindows(window);
}