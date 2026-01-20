//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#include "Pret.hpp"

#include <sstream>
#include <cmath>
#include<algorithm>
#include <iostream>
using namespace std;

Pret::Pret(double montant, double taux, int monthOfSimulation, double monthPaid, int monthOfDefered, bool totalDefered) :
    Montant(montant), Taux(taux), MonthOfSimulation(monthOfSimulation), MonthPaid(monthPaid), MonthOfDefered(monthOfDefered), TotalDefered(totalDefered)
{
    MonthOfSimulation > 0 ? PretByMonth(MonthOfSimulation) : PretByPaid(MonthPaid);
};

void Pret::PretByMonth(int monthOfSimulation) {
    std::stringstream ss;
    //ss << "--------------------" << endl;
    ss << "\n";
    ss << "Pret cree avec les parametres : " << Montant << " euros, " << Taux << "% d'interets, sur " << monthOfSimulation << " mois";

    if (MonthOfDefered != 0) {
        ss << ", avec un difere ";
        if (TotalDefered) {
            ss << "total ";
        }
        ss << "de " << MonthOfDefered << " mois" ;
    }
    ss << " !" << endl;

    //cout << "Pret cree avec les parametres : " << Montant << " euros, " << Taux << "% d'interets, sur " << monthOfSimulation << " mois !" << endl;

    double monthPaid = 0;
    double totalPaid = 0;
    double restToPaid = Montant;
    const double tauxMensuel = (Taux / 100.0) / 12.0;

    // Différé
    for (int i = 0; i < MonthOfDefered; ++i) {
        if (TotalDefered) {
            restToPaid += restToPaid * tauxMensuel; // intérêts capitalisés
        } else {
            totalPaid += restToPaid * tauxMensuel;
        }
    }

    monthPaid = restToPaid * (tauxMensuel / (1 - pow(1 + tauxMensuel, -monthOfSimulation)));
    for (int i = 1; i <= monthOfSimulation; i++) {

        double interets = restToPaid * tauxMensuel;
        restToPaid += restToPaid * tauxMensuel;
        monthPaid = std::min(monthPaid, restToPaid);
        totalPaid += monthPaid;
        restToPaid -= monthPaid;

        //ss << "Vous aurez payez "<< totalPaid << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, dont " << monthPaid << " ce mois ! Il ne vous reste plus que " << restToPaid << " a payer !" << endl;
    }
    //cout << "Au bout de " << monthOfSimulation/12 << " annees et " << monthOfSimulation%12 << " mois, \nVous aurez payez " << totalPaid << " euros au total, dont " << monthPaid << " ce dernier mois !" << endl;
    ss << "Au bout de " << (monthOfSimulation+MonthOfDefered)/12 << " annees et " << (monthOfSimulation+MonthOfDefered)%12 << " mois, Vous aurez payez " << totalPaid << " euros au total, dont " << monthPaid << " ce dernier mois !" << endl;
    InfoFlux += ss.str();
}
void Pret::PretByPaid(double monthPaid) {
    std::stringstream ss;
    //ss << "--------------------" << endl;
    ss << "\n";
    ss << "Pret cree avec les parametres : " << Montant << " euros, " << Taux << "% d'interets, avec un remboursement de " << monthPaid << " par mois";

    if (MonthOfDefered != 0) {
        ss << ", avec un difere ";
        if (TotalDefered) {
            ss << "total ";
        }
        ss << "de " << MonthOfDefered << " mois" ;
    }
    ss << " !" << endl;

    //cout << "Pret cree avec les parametres : " << Montant << " euros, " << Taux << "% d'interets, avec un remboursement de " << monthPaid << " par mois !" << endl;
    int monthOfSimulation;
    double totalPaid = 0;
    double restToPaid = Montant;
    const double tauxMensuel = (Taux / 100.0) / 12.0;

    for (int i = 0; i < MonthOfDefered; ++i) {
        if (TotalDefered) {
            restToPaid += restToPaid * tauxMensuel;
        } else {
            totalPaid += restToPaid * tauxMensuel;
        }
    }

    for (int i = 1; restToPaid > 0; i++) {
        monthOfSimulation = i;

        restToPaid += restToPaid * tauxMensuel;
        monthPaid = std::min(monthPaid, restToPaid);
        totalPaid += monthPaid;
        restToPaid -= monthPaid;

        //ss << "Vous aurez payez "<< totalPaid << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, dont " << monthPaid << " ce mois ! Il ne vous reste plus que " << restToPaid << " a payer !" << endl;
    }
    //cout << "Au bout de " << monthOfSimulation/12 << " annees et " << monthOfSimulation%12 << " mois, \nVous aurez payez " << totalPaid << " euros au total, dont " << monthPaid << " ce dernier mois !" << endl;
    ss << "Au bout de " << (monthOfSimulation+MonthOfDefered)/12 << " annees et " << (monthOfSimulation+MonthOfDefered)%12 << " mois, Vous aurez payez " << totalPaid << " euros au total, dont " << monthPaid << " ce dernier mois !" << endl;
    InfoFlux += ss.str();
}
void Pret::PretDefered(int monthOfDefered) {

}
std::string Pret::Info() {
    return InfoFlux;
}
