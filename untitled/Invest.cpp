//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#include "Invest.h"

#include <sstream>
#include <iostream>
using namespace  std;

Invest::Invest(double montant, double taux, bool flatTaxe, double more, int monthOfSimulation) : Montant(montant), Taux(taux), FlatTaxe(flatTaxe), More(more), Month(monthOfSimulation) {
    Invesstissement(Month);
}

void Invest::Invesstissement(int monthOfSimulation){
    std::stringstream ss;
    //ss << "--------------------" << endl;
    ss << "\n";
    ss << "Invesstissement cree avec les parametres : " << Montant << " euros, " << Taux << "% d'interets, " << More << " euros invesstis en plus chaque mois, flat taxe : " << FlatTaxe  << endl;

    //cout << "Invesstissement cree avec les parametres : " << Montant << " euros, " << Taux << "% d'interets, " << More << " euros invesstis en plus chaque mois, flat taxe : " << FlatTaxe  << endl;
    double monthresult;
    double allCapital = Montant;
    double gains = 0;
    double personalInvest = Montant;

    personalInvest = allCapital;

    for (int i = 1; i <= monthOfSimulation; i++) {

        monthresult = allCapital * (Taux / 100.0 / 12);
        if (FlatTaxe) monthresult *= 0.7;
        allCapital += monthresult;
        gains += monthresult;

        //ss << "Vous aurez "<< allCapital << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, soit " << personalInvest << " donnee de votre poche et " << gains << " d'argent generer, dont " << monthresult << " ce mois !"  << endl;

        personalInvest += More;
        allCapital += More;
    }
    //cout << "Au bout de " << monthOfSimulation/12 << " annees et " << monthOfSimulation%12 << " mois, \nVous aurez "<< allCapital << " euros, Soit " << personalInvest << " donnee de votre poche et " << gains << " d'argent generer, dont " << monthresult << " ce dernier mois !" << endl;
    ss << "Au bout de " << monthOfSimulation/12 << " annees et " << monthOfSimulation%12 << " mois, Vous aurez "<< allCapital << " euros, Soit " << personalInvest << " donnee de votre poche et " << gains << " d'argent generer, dont " << monthresult << " ce dernier mois !" << endl;
    InfoFlux += ss.str();
};

std::string Invest::Info() {
    return InfoFlux;
}