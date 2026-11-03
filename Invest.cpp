//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#include "Invest.h"
#include "FormatNumber.h"

Invest::Invest(double montant, double taux, bool flatTaxe, double more, int monthOfSimulation) : Montant(montant), Taux(taux), FlatTaxe(flatTaxe), More(more), Month(monthOfSimulation) {
    Invesstissement(Month);
}

void Invest::Invesstissement(int monthOfSimulation){
    double monthResult = 0.0;
    double allCapital = Montant;
    double gains = 0;
    double personalInvest = Montant;

    personalInvest = allCapital;

    for (int i = 1; i <= monthOfSimulation; i++) {

        monthResult = allCapital * (Taux / 100.0 / 12);
        if (FlatTaxe) monthResult *= 0.7;
        allCapital += monthResult;
        gains += monthResult;

        //ss << "Vous aurez "<< allCapital << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, soit " << personalInvest << " donnee de votre poche et " << gains << " d'argent generer, dont " << monthresult << " ce mois !"  << endl;

        personalInvest += More;
        allCapital += More;
    }
    InfoFlux += "\nInvesstissement cree avec les parametres : " + formatNumber(Montant) + " euros, " + formatNumber(Taux) + "% d'interets, " + formatNumber(More) +
        " euros invesstis en plus chaque mois, " + (FlatTaxe ? "avec flat taxe" : "sans flat taxe")  + "\nAu bout de " + formatNumber(monthOfSimulation/12) + " annees et " +
        formatNumber(monthOfSimulation%12)+ " mois, Vous aurez " + formatNumber(allCapital) + " euros, Soit " + formatNumber(personalInvest) +
        " donnee de votre poche et " + formatNumber(gains) + " d'argent generer, dont " + formatNumber(monthResult) + " ce dernier mois !\n";

};

std::string Invest::Info() {
    return InfoFlux;
}