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

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imguiCustom.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
bool InputDouble(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
    return ImGui::InputScalar(label, ImGuiDataType_Double, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}
bool DragDouble(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
{
    return ImGui::DragScalar(label, ImGuiDataType_Double, v, v_speed, &v_min, &v_max, format, flags);
}
/*
double* castFtoD(const float* v)
{
    double dVal = *v; // Value conversion, no precision loss
    return &dVal;        // Pointer to the converted value
}
float* castDtoF(const double* v)
{
    float fVal = *v; // Value conversion with precision loss
    return &fVal;       // Pointer to the converted value
}*/
int main()
{
    CreateWindow::Start();
    // [] est pour la capture des variables externes
    // () est pour l'entrée des paramètres
    // "-> void" est pour le type de retour
    // ex : "auto incremente = [&](int compteur) -> int"

    // "auto func = []() -> void" est équivalent à "auto func = []"
    auto func = []
    {
        if (ImGui::Begin("Choose param"))
        {
            ImGui::Text("Bonjour depuis Dear ImGui !");
            static int compteur = 0;

            // transforme mon text avec des valeur en "const char" dans la variable "ButtonText"
            char ButtonText[32];
            sprintf(ButtonText, "Cliquer ici : %d", compteur);
            // ---
            if (ImGui::Button(ButtonText))
            {
                compteur++;
            }
            ImGui::Text("Compteur : %d", compteur);

            static char str1[128] = "";
            ImGui::InputTextWithHint("input text (w/ hint)", "enter text here", str1, IM_COUNTOF(str1));
            ImGui::SameLine();
            HelpMarker("Je suis un tooltip, et ceci <-- est un InputTextWithHint");
            ImGui::Text("Je suis un text avant AlignTextToFramePadding mais après un SameLine");
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Je suis un text après AlignTextToFramePadding");

            static float progress_accum = 0.0f, progress_dir = 1.0f;
            progress_accum += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
            if (progress_accum >= +1.1f) { progress_accum = +1.1f; progress_dir *= -1.0f; }
            if (progress_accum <= -0.1f) { progress_accum = -0.1f; progress_dir *= -1.0f; }
            const float progress = ImClamp(progress_accum, 0.0f, 1.0f);

            ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Progress Bar 0");

            char val[32];
            sprintf(val, "%f/%d", progress*200, 200);
            ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), val);
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Progress Bar 0.2");

            ImGui::ProgressBar(static_cast<float>(compteur)/200.f, ImVec2(0.0f, 0.0f));
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Progress Bar 1");

            char buf[32];
            sprintf(buf, "%d/%d", compteur, 200);
            ImGui::ProgressBar(static_cast<float>(compteur)/200.f, ImVec2(0.0f, 0.0f), buf);
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Progress Bar 2");

            // Pass an animated negative value, e.g. -1.0f * (float)ImGui::GetTime() is the recommended value.
            // Adjust the factor if you want to adjust the animation speed.
            ImGui::ProgressBar(-1.0f * (float)ImGui::GetTime(), ImVec2(0.0f, 0.0f), "Searching..");
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Progress Bar 3");


            static double value1 = 50.0;
            static double value2 = 50.0;
            static double value3 = 50.0;
            static double value4 = 50.0;

            // DragDouble simple
            ImGui::DragDouble( "Valeur simple", &value1 );
            // DragDouble avec clamp [0, 100]
            ImGui::DragDouble( "Valeur clamp", &value2, 1.0f, 0.0, 100.0 );
            // DragDouble avec boutons +/- de 5
            ImGui::DragDouble("Valeur step", &value3, 1.0f, 0.0, 0.0, "%.2f", 0, 5.0 );
            // DragDouble avec clamp et step rapide
            ImGui::DragDouble("Valeur complete", &value4, 0.5f, 0.0, 100.0, "%.3f", 0, 1.0, 10.0 );

            ImGui::End();
        }
        ImGui::Begin("Fenêtre de simulation d'investissement");

        static double montant = 0, more = 0, eachMonthOfMore = 1, taux = 0, eachMonthOfEarn = 1, monthOfSimu = 1;
        static int isflattaxe = 0, nbrOfMore = 1, nbrOfEarn = 1;
         bool IsFlatTaxe = isflattaxe == 1;
        if (ImGui::CollapsingHeader("Paramètres de l'investissement", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("indiquer le montant de départ");
            ImGui::DragDouble("##montantDD", &montant, 100, 0, DBL_MAX, "%.2f€", 0, 100, 1000);
            //ImGui::InputDouble("##montant", &montant, 100, 1000, "%.2f€");

            ImGui::Text("indiquer le taux annuel");
            ImGui::SameLine(); HelpMarker(
                "Click and drag to edit value.\n"
                "Hold Shift/Alt for faster/slower edit.\n"
                "Double-Click or Ctrl+Click to input value.");
            ImGui::DragDouble("##tauxDD", &taux, 0.1, 0, 100, "%.2f%%", 0, 0.1, 1);
            //ImGui::DragFloat("##taux", &taux, 0.5, 0., 100., "%.2f%%", ImGuiSliderFlags_AlwaysClamp);

            ImGui::Text("indiquer le nombre de mois à attendre pour percevoir les revenus");
            ImGui::SameLine(); HelpMarker(
                "2 = percu tous les deux mois.\n"
                "0.5 = percu deux fois par mois.\n"
                "0.25 = percu toutes les semaines (environ).\n"
                //"La valeur minimum vous donne de l'argent 31 fois par mois (peu importe le mois)"
                );
            ImGui::DragDouble("##eachMonthDD", &eachMonthOfEarn, 1, 1./*/31.*/, DBL_MAX, "%.2f", 0, 1, 12);
            //ImGui::DragFloat("##eachMonth", &eachMonthOfEarn, 1., 1., FLT_MAX, "%.2f", ImGuiSliderFlags_AlwaysClamp);

            /*enum isFlatTaxe { True, False, ElementCount };
        static int elem = True;
        const char* elems_names[ElementCount] = { "True", "False" };
        const char* elem_name = (elem >= 0 && elem < ElementCount) ? elems_names[elem] : "Unknown";
        ImGui::SliderInt("slider enum", &elem, 0, ElementCount - 1, elem_name); // Use ImGuiSliderFlags_NoInput flag to disable Ctrl+Click here.
        */

            if (ImGui::CollapsingHeader("Ajout d'argent"))
            {
                ImGui::InputInt("Nombre d'éléments dans la liste qui suit", &nbrOfMore, 1, 5, ImGuiDataType_U8);
                ImGui::BeginChild("##child",{0,0}, ImGuiCol_ChildBg | ImGuiChildFlags_Borders);
                ImGui::Text("Ajouter de l'argent chaque %.2f mois", eachMonthOfMore);
                ImGui::SameLine(); HelpMarker("1 = ajout d'argent chaque mois.\n"
                                              "12 = ajout d'argent tous les ans\n"
                                              //"Si valeur minimum, vous donnez de l'argent 31 fois par mois (peu importe le mois)"
                                              );
                ImGui::DragDouble("##monthMoreDD", &eachMonthOfMore, 1, 1./*/31.*/, DBL_MAX, "%.2f", 0, 1, 12);
                //ImGui::InputDouble("##monthMore", &eachMonthOfMore, 1, 12, "%.2f");
                //ImGui::DragFloat("##monthMoref", reinterpret_cast<float*>(&eachMonthOfMore), 1., 0., 0., "%.2f");

                ImGui::Text("Ajouter %.2f€ chaque %.2f mois", more, eachMonthOfMore);
                ImGui::DragDouble("##moreDD", &more, 1, 0, DBL_MAX, "%.2f€", 0, 100, 1000);
                //ImGui::InputDouble("##more", &more, 100, 1000, "%.2f€");
                //ImGui::DragFloat("##moref", reinterpret_cast<float*>(&more), 1., 0., 0., "%.2f€");
                ImGui::EndChild();
            }

            ImGui::Text("Switch FlatTaxe");
            ImGui::SameLine(); HelpMarker("Si 'True' : 30% de revenu en moins");
            //ImGui::SameLine();
            const char* FlatTaxeValue = isflattaxe == 1 ? "True" : "False";
            ImGui::SliderInt("##flatTaxe", &isflattaxe, 0, 1, FlatTaxeValue, ImGuiSliderFlags_NoInput);

            ImGui::Text("indiquer le nombre de mois de simulation");
            ImGui::SameLine(); HelpMarker("12 = résultat de la simulation au bout de 1 an.");
            ImGui::DragDouble("##monthOfSimuDD", &monthOfSimu, 1, 0, DBL_MAX, "%.2f", 0, 1, 12);
            //ImGui::DragFloat("##monthOfSimu", &monthOfSimu, 1., 1.0, FLT_MAX, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        }

        static std::string resultString;
        if (ImGui::Button("Faire la simulation !"))
        {
            Invest inv {montant, taux, IsFlatTaxe, more, eachMonthOfMore, eachMonthOfEarn, monthOfSimu};
            resultString = inv.Info();
        }
        ImVec4 color {0.5,0.5,1,1};
        ImGui::TextColored(color, resultString.c_str());

        ImGui::End();
    };
    CreateWindow::Start(func);
    return 0;
}
