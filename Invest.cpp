//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#include "Invest.h"
#include "FormatNumber.cpp"

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
    InfoFlux += "\nInvesstissement cree avec les parametres : " + FormatNumber(Montant) + " euros, " + FormatNumber(Taux) + "% d'interets, " + FormatNumber(More) +
        " euros invesstis en plus chaque mois, " + (FlatTaxe ? "avec flat taxe" : "sans flat taxe")  + "\nAu bout de " + FormatNumber(monthOfSimulation/12) + " annees et " +
        FormatNumber(monthOfSimulation%12)+ " mois, Vous aurez " + FormatNumber(allCapital) + " euros, Soit " + FormatNumber(personalInvest) +
        " donnee de votre poche et " + FormatNumber(gains) + " d'argent generer, dont " + FormatNumber(monthResult) + " ce dernier mois !\n";

};

std::string Invest::Info() {
    return InfoFlux;
}