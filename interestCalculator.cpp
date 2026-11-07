#include "Pret.h"
#include "Invest.h"
#include "InvestAndPret.h"
#include "CreateWindow.h"
/*
int main()
{
    //InvestAndPret::InvestWithPret();
    //Invest::DoAllInvest();
    //Pret::DoAllPret();
    //Pret::DoAllPretWithDefered();

    CreateWindow::Start();

    return 0;
}*/

#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main()
{
    //CreateWindow::Start();
    // [] est pour la capture des variables externes
    // () est pour l'entrée des paramètres
    // "-> void" est pour le type de retour
    // ex : "auto incremente = [&](int compteur) -> int"

    // "auto func = []() -> void" est équivalent à "auto func = []"
    auto func = []
    {
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
    };
    CreateWindow::Start(func);
    return 0;
}
