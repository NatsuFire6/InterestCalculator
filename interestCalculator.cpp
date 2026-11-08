#include "Simulations/Pret.h"
#include "Simulations/Invest.h"
#include "Simulations/InvestAndPret.h"
#include "WindowUtils/CreateWindow.h"
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

#include <iomanip>
#include <iostream>
#include "imgui.h"
#include "WindowUtils/imguiCustom.h"
#include "imgui_internal.h"
#include "WindowUtils/FormatNumber.h"

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
int main()
{
    CreateWindow::Start();
    // [] est pour la capture des variables externes
    // () est pour l'entrée des paramètres
    // "-> void" est pour le type de retour
    // ex : "auto incremente = [&](int compteur) -> int"

    // "auto func = []() -> void" est équivalent à "auto func = []"
    std::cout << FormatDuration(TimeUnit::Year, 2.5) << '\n';
    std::cout << FormatDuration(TimeUnit::Month, 15) << '\n';
    std::cout << FormatDuration(TimeUnit::Hour, 1250) << '\n';
    std::cout << FormatDuration(TimeUnit::Seconde, 125.75) << '\n';

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
        static int isFlatTaxe = 0, nbrOfMore = 1, nbrOfEarn = 1;
        bool IsFlatTaxe = isFlatTaxe == 1;
        static std::vector<std::pair<double, double>> MonthsAndMores{{1,10}};
        static std::vector<double> MonthsOfEarn{1};

        if (ImGui::CollapsingHeader("Paramètres de l'investissement", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("indiquer le montant de départ");
            ImGui::SameLine(); HelpMarker(
                "Click and drag to edit value.\n"
                "Hold Shift/Alt for faster/slower edit.\n"
                "Double-Click or Ctrl+Click to input value.\n"
                "Click or Ctrl+Click on button '+' to incremente/faster_incremente value.\n"
                "Click or Ctrl+Click on button '-' to decremente/faster_decremente value.");
            ImGui::DragDouble("##montantDD", &montant, 100., 0., DBL_MAX, "%.2f€", 0., 100., 1000.);

            ImGui::Text("indiquer le taux annuel");
            ImGui::DragDouble("##tauxDD", &taux, 0.1, 0., 100., "%.2f%%", 0., 0.1, 1.);

            if (ImGui::CollapsingHeader("Gain d'argent", ImGuiChildFlags_Borders))
            {
                ImGui::DragDouble("Nombre d'éléments dans la liste qui suit##gain", &nbrOfEarn, 0, 1, 100, nullptr,0, 1, 5);
                if (MonthsOfEarn.size() < nbrOfEarn) MonthsOfEarn.resize(nbrOfEarn, 1.0); // 1.0 est la data des new elements qui seront créé lors du resize
                if (MonthsOfEarn.size() > nbrOfEarn) MonthsOfEarn.resize(nbrOfEarn);
                for (int i = 0; i < nbrOfEarn; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::BeginChild("##childgain",{0,0}, ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);

                    ImGui::Text("indiquer le nombre de mois à attendre pour percevoir les revenus");
                    ImGui::SameLine(); HelpMarker(
                        "1 = percu tous les mois.\n"
                        "2 = percu tous les deux mois.\n"
                        "Vous devriez rester en valeur entière.\n");
                    ImGui::DragDouble("##eachMonthDD", &MonthsOfEarn[i], 1., 1., DBL_MAX, "%.2f", 0, 1., 12.);

                    ImGui::EndChild();
                    ImGui::PopID();
                }
            }
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("Ajout d'argent"), ImGuiChildFlags_Borders)
            {
                ImGui::DragDouble("Nombre d'éléments dans la liste qui suit##ajout", &nbrOfMore, 0, 1, 100, nullptr,0, 1, 5);
                if (MonthsAndMores.size() < nbrOfMore) MonthsAndMores.resize(nbrOfMore, {1.0, 0.0});
                if (MonthsAndMores.size() > nbrOfMore) MonthsAndMores.resize(nbrOfMore);
                for (int i = 0; i < nbrOfMore; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::BeginChild("##childajout",{0,0}, ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);

                    ImGui::Text("Ajouter %.2f€ chaque %.2f mois", MonthsAndMores[i].second, MonthsAndMores[i].first);
                    ImGui::DragDouble("##monthMoreDD", &MonthsAndMores[i].first, 1., 1., DBL_MAX, "%.2f", 0, 1., 12.);
                    ImGui::SameLine(); HelpMarker("1 = ajout d'argent chaque mois.\n"
                        "12 = ajout d'argent tous les ans\n"
                        "Vous devriez rester en valeur entière.\n");
                    ImGui::DragDouble("##moreDD", &MonthsAndMores[i].second, 1., 0., DBL_MAX, "%.2f€", 0., 100., 1000.);

                    ImGui::EndChild();
                    ImGui::PopID();
                }
            }

            ImGui::Text("Switch FlatTaxe");
            ImGui::SameLine(); HelpMarker("Si 'True' : 30% de revenu en moins");
            //ImGui::SameLine();
            const char* FlatTaxeValue = isFlatTaxe == 1 ? "True" : "False";
            ImGui::SliderInt("##flatTaxe", &isFlatTaxe, 0, 1, FlatTaxeValue, ImGuiSliderFlags_NoInput);

            ImGui::Text("indiquer le nombre de mois de simulation");
            ImGui::SameLine(); HelpMarker("12 = résultat de la simulation au bout de 1 an.");
            ImGui::DragDouble("##monthOfSimuDD", &monthOfSimu, 1., 0., DBL_MAX, "%.2f", 0., 1., 12.);
            //ImGui::DragFloat("##monthOfSimu", &monthOfSimu, 1., 1.0, FLT_MAX, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        }

        static std::string resultString;
        if (ImGui::Button("Faire la simulation !"))
        {
            Invest inv {montant, taux, IsFlatTaxe, MonthsAndMores, MonthsOfEarn, monthOfSimu};
            resultString = inv.Info();
        }
        ImVec4 color {0.5,0.5,1,1};
        ImGui::TextColored(color, resultString.c_str());

        ImGui::End();
    };
    CreateWindow::Start(func);
    return 0;
}
