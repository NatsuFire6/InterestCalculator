//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#include "Invest.h"
#include "FormatNumber.h"

#include <iostream>
#include <fstream>

Invest::Invest(double montant, double taux, bool flatTaxe, double more, int EachMonthOfEarn, int monthOfSimulation) : Montant(montant), Taux(taux), FlatTaxe(flatTaxe), More(more), EachEarnMonth(EachMonthOfEarn), Month(monthOfSimulation)
{
    Invesstissement(Month);
}

void Invest::Invesstissement(int monthOfSimulation){
    double monthResult = 0.0;
    double allCapital = Montant;
    double gains = 0;
    double personalInvest = allCapital;

    for (int i = 1; i <= monthOfSimulation; i++) {

        if (monthOfSimulation % EachEarnMonth == 0)
        {
            monthResult = allCapital * (Taux / 100.0 / 12);
        }
        else
        {
            monthResult = 0;
        }
        if (FlatTaxe) monthResult *= 0.7;
        allCapital += monthResult;
        gains += monthResult;

        //ss << "Vous aurez "<< allCapital << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, soit " << personalInvest << " donnee de votre poche et " << gains << " d'argent generer, dont " << monthresult << " ce mois !"  << endl;

        personalInvest += More;
        allCapital += More;
    }
    InfoFlux += "\nInvesstissement cree avec les parametres : " + FormatNumberToText(Montant) + " euros, " + FormatNumberToText(Taux) + "% d'interets (percu tous les " + FormatNumberToText(EachEarnMonth) + " mois), " + FormatNumberToText(More) +
        " euros invesstis en plus chaque mois, " + (FlatTaxe ? "avec flat taxe" : "sans flat taxe")  + "\nAu bout de " + FormatNumberToText(monthOfSimulation/12) + " annees et " +
        FormatNumberToText(monthOfSimulation%12)+ " mois, Vous aurez " + FormatNumberToText(allCapital) + " euros, Soit " + FormatNumberToText(personalInvest) +
        " donnee de votre poche et " + FormatNumberToText(gains) + " d'argent generer, dont " + FormatNumberToText(monthResult) + " ce dernier mois !\n";

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
                for (int e = 1; e < 60; ++e) // 1 → 60 (5ans)
                {
                    Invest investF{montant, taux, true, moreEachMonth, e};
                    Invest invest{montant, taux, false, moreEachMonth, e};
                    buffer += investF.Info();
                    buffer += invest.Info();
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
