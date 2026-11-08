//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#include "Invest.h"
#include "../WindowUtils/FormatNumber.h"

#include <iostream>
#include <fstream>
#include <cmath>

Invest::Invest(double montant, double taux, bool flatTaxe, std::vector<std::pair<double, double>> eachMonthOfMore, std::vector<double> eachMonthOfEarn, double monthOfSimulation)
: Montant(montant), Taux(taux), FlatTaxe(flatTaxe), EachMonthOfMore(eachMonthOfMore), EachEarnMonth(eachMonthOfEarn), Month(monthOfSimulation)
{
    Invesstissement(Month);
}

void Invest::Invesstissement(double monthOfSimulation){
    double monthResult = 0.0;
    double bestMonthResult = 0.0;
    double allCapital = Montant;
    double gains = 0;
    double personalInvest = allCapital;

    for (int i = 1; i <= monthOfSimulation; i++) {

        //InfoFlux += "std::fmod("+FormatNumberToTextString(i)+", "+FormatNumberToTextString(EachEarnMonth)+") = " + std::to_string(std::fmod(i, EachEarnMonth)) +"\n";

        for (double monthOfEarn : EachEarnMonth)
        {
            if (std::fmod(i, monthOfEarn) == 0)
            {
                monthResult = allCapital * (Taux / 100.0 / (12 / monthOfEarn));
            }
            else
            {
                monthResult = 0;
            }
            if (FlatTaxe) monthResult *= 0.7;
            if(monthResult > bestMonthResult) bestMonthResult = monthResult;
            allCapital += monthResult;
            gains += monthResult;
        }
        //ss << "Vous aurez "<< allCapital << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, soit " << personalInvest << " donnee de votre poche et " << gains << " d'argent generer, dont " << monthresult << " ce mois !"  << endl;
        for (auto MonthAndMore : EachMonthOfMore)
        {
            if (std::fmod(i, MonthAndMore.first) == 0)
            {
                personalInvest += MonthAndMore.second;
                allCapital += MonthAndMore.second;
            }
        }
    }
    InfoFlux += "\nInvestissement créé avec les paramètres : \n "
        + FormatNumberToTextString(Montant) + " euros, \n "
        + FormatNumberToTextString(Taux) + "%% d'intérêts perçu tous les : \n ";
    for (double monthOfEarn : EachEarnMonth)
    {
        InfoFlux += " " + FormatDuration(TimeUnit::Month, monthOfEarn) + ", \n ";
    }
    for (auto MonthAndMore : EachMonthOfMore)
    {
        InfoFlux += FormatNumberToTextString(MonthAndMore.second) + " euros investis en plus chaque " + FormatDuration(TimeUnit::Month, MonthAndMore.first)+ ", \n ";

    }
    InfoFlux += std::string(FlatTaxe ? "avec flat taxe" : "sans flat taxe") + " !"
        + "\nAu bout de " + FormatDuration(TimeUnit::Month, monthOfSimulation) + ", \n "
        + "Vous aurez " + FormatNumberToTextString(allCapital) + " euros, \n "
        + "Soit " + FormatNumberToTextString(personalInvest) + " donnée de votre poche et " + FormatNumberToTextString(gains) + " d'argent généré, \n "
        + "dont " + FormatNumberToTextString(monthResult) + " ce dernier mois, votre meilleur mois à généré " + FormatNumberToTextString(bestMonthResult) + " !\n";

};

std::string Invest::Info()
{
    return InfoFlux;
}

void Invest::DoAllInvest() {
    std::ofstream f{"Invesstissements.txt"};
    std::string buffer;
    buffer.reserve(500'000);

    if (!f.is_open()) return;
    int i = 0;
    for (int m = 1; m <= 100; ++m) {            // 10 000 → 1 000 000 par 10 000
        const double montant = m * 10000.0;
        for (int t = 1; t <= 32; ++t) {         // 0.5 → 16.0 par 0.5
            const double taux = t * 0.5;
            ++i;
            std::cout << i << "/3200 ";
            for (int more = 1; more < 1000; ++more) { // 100 → 100 000 par 100
                const double moreEachMonth = more * 100;
                for (double e = 1; e < 60; e+=1) // 1 → 60 (5ans)
                {
                    //Invest investF{montant, taux, true, moreEachMonth, e};
                    //Invest invest{montant, taux, false, moreEachMonth, e};
                    //buffer += investF.Info();
                    //buffer += invest.Info();
                }
            }
        }
        if (buffer.size() > 200'000) {
            f << buffer;
            buffer.clear();
        }
    }
    f << buffer;
}
