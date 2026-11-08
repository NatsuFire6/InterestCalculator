//
// Created by valen on 07/11/2026.
//

#include "CreateWindow.h"

#include <functional>

#include <iostream>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "../ImGUI/imgui/imgui.h"

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

        // Démonstration officielle de Dear ImGui
        static bool showDemo = true;
        if (showDemo) ImGui::ShowDemoWindow(&showDemo);
        // Fenêtre des statistiques et métriques
        static bool showMetrics = false;
        if (showMetrics) ImGui::ShowMetricsWindow(&showMetrics);
        // Fenêtre d'informations sur ImGui
        static bool showAbout = false;
        if (showAbout) ImGui::ShowAboutWindow(&showAbout);
        // Fenêtre de débogage du style
        static bool showStyleEditor = false;
        if (showStyleEditor)
        {
            ImGui::Begin("Style Editor");
            ImGui::ShowStyleEditor();
            ImGui::End();
        }
        // ============================================
        // Menu de contrôle
        // ============================================
        ImGui::Begin("ImGui Test Launcher");
        ImGui::Text("Outils de demonstration");
        ImGui::Separator();
        ImGui::Checkbox("Demo Window", &showDemo);
        ImGui::Checkbox("Metrics Window", &showMetrics);
        ImGui::Checkbox("About Window", &showAbout);
        ImGui::Checkbox("Style Editor", &showStyleEditor);
        ImGui::End();

        // ============================================
        // Interface ImGui
        // ============================================
        /*static int compteur = 0;

        ImGui::Begin("Ceci est une fenêtre de test appelé par la fonction de test 'Start'");
        ImGui::TextColored({1,0.5,0.5,1},"Je sers à montrer à quoi ressemble les fonctions de ImGui.");

        if (ImGui::Button("Je suis un bouton"))
        {
            compteur++;
        }
        ImGui::BulletText("Compteur : %d", compteur);
        ImGui::Text("Je suis du text");
        ImVec4 color {0.5,0.5,1,1};
        ImGui::TextColored(color,"Je suis du text colorer.");
        constexpr char* const items[] = {"élément 0", "élément 1"};
        ImGui::ListBox("Je suis une listBox",0,items,3);


        ImGui::End();*/
        DoRender(window);
    }
}
void CreateWindow::WindowsLife(GLFWwindow* window, const std::function<void()>& fLife, const std::function<void()>& fDead)
{
    while (!glfwWindowShouldClose(window))
    {
        DoNewFrame();
        fLife();
        DoRender(window);
    }
    fDead();
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
void CreateWindow::Start(const std::function<void()>& fLife, const std::function<void()>& fDead)
{
    GLFWwindow* window = InitGLFW();
    InitImGui(window);
    WindowsLife(window, fLife, fDead);
    CleanWindows(window);
}