//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#include "Pret.hpp"
#include "FormatNumber.h"

#include <sstream>
#include <cmath>
#include<algorithm>

Pret::Pret(double montant, double taux, int monthOfSimulation, double monthPaid, int monthOfDefered, bool totalDefered) :
    Montant(montant), Taux(taux), MonthOfSimulation(monthOfSimulation), MonthPaid(monthPaid), MonthOfDefered(monthOfDefered), TotalDefered(totalDefered)
{
    MonthOfSimulation > 0 ? PretByMonth(MonthOfSimulation) : PretByPaid(MonthPaid);
};

void Pret::PretByMonth(int monthOfSimulation) {
    double totalPaid = 0;
    double restToPaid = Montant;
    const double tauxMensuel = (Taux / 100.0) / 12.0;
    double interest;

    for (int i = 1; i <= MonthOfDefered; ++i) {
        interest = restToPaid * tauxMensuel;
        if (TotalDefered) {
            restToPaid += interest; // intérêts capitalisés
            //ss << "Au bout de " << i/12 << " annees et " << i%12 << " mois, Il vous resteras " << restToPaid << " euros a payer, " << interest << " euros vous on ete rajouter ce mois !" << endl;
        } else {
            totalPaid += interest;
            //ss << "Au bout de " << i/12 << " annees et " << i%12 << " mois, Vous aurez payez " << totalPaid << " euros au total, dont " << interest << " ce mois !" << endl;
        }
    }
    double monthPaid = restToPaid * (tauxMensuel / (1 - std::pow(1 + tauxMensuel, -monthOfSimulation)));
    if (monthPaid <= restToPaid * tauxMensuel * 1.0001 || monthPaid < 10.0) return;

    for (int i = 1; i <= monthOfSimulation; i++) {

        restToPaid += restToPaid * tauxMensuel;
        monthPaid = std::min(monthPaid, restToPaid);
        totalPaid += monthPaid;
        restToPaid -= monthPaid;

        //ss << "Vous aurez payez "<< totalPaid << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, dont " << monthPaid << " ce mois ! Il ne vous reste plus que " << restToPaid << " a payer !" << endl;
    }
    InfoFlux +=
            "\nPret cree avec les parametres : " + formatNumber(Montant) + " euros, " +
            formatNumber(Taux) + "% d'interets, sur " + formatNumber(monthOfSimulation) + " mois" +
            (MonthOfDefered != 0 ? (", avec un difere " + std::string(TotalDefered ? "total " : "") +
            "de " + formatNumber(MonthOfDefered) + " mois") : "") +
            " !\nAu bout de " + formatNumber((monthOfSimulation + MonthOfDefered) / 12) + " annees et " +
            formatNumber((monthOfSimulation + MonthOfDefered) % 12) + " mois, Vous aurez payez " +
            formatNumber(totalPaid) + " euros au total, dont " + formatNumber(monthPaid) + " ce dernier mois !\n";
}
void Pret::PretByPaid(double monthPaid) {
    int monthOfSimulation = 0;
    double totalPaid = 0;
    double restToPaid = Montant;
    const double tauxMensuel = (Taux / 100.0) / 12.0;
    double interest;

    for (int i = 1; i <= MonthOfDefered; ++i) {
         interest = restToPaid * tauxMensuel;
        if (TotalDefered) {
            restToPaid += interest; // intérêts capitalisés
            //ss << "Au bout de " << i/12 << " annees et " << i%12 << " mois, Il vous resteras " << restToPaid << " euros a payer, " << interest << " euros vous on ete rajouter ce mois !" << endl;
        } else {
            totalPaid += interest;
            //ss << "Au bout de " << i/12 << " annees et " << i%12 << " mois, Vous aurez payez " << totalPaid << " euros au total, dont " << interest << " ce mois !" << endl;
        }
    }
    if (monthPaid <= restToPaid * tauxMensuel * 1.0001 || monthPaid < 10.0) return;

    for (int i = 1; restToPaid > 0; i++) {
        monthOfSimulation = i;

        restToPaid += restToPaid * tauxMensuel;
        double paidThisMonth = std::min(monthPaid, restToPaid);
        totalPaid += paidThisMonth;
        restToPaid -= paidThisMonth;

        //ss << "Vous aurez payez "<< totalPaid << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, dont " << monthPaid << " ce mois ! Il ne vous reste plus que " << restToPaid << " a payer !" << endl;
    }
    InfoFlux +=
            "\nPret cree avec les parametres : " + formatNumber(Montant) + " euros, " +
            formatNumber(Taux) + "% d'interets, avec un remboursement de " + formatNumber(monthPaid) + " par mois" +
            (MonthOfDefered != 0 ? (", avec un difere " + std::string(TotalDefered ? "total " : "") +
            "de " + formatNumber(MonthOfDefered) + " mois") : "") +
            " !\nAu bout de " + formatNumber((monthOfSimulation + MonthOfDefered) / 12) + " annees et " +
            formatNumber((monthOfSimulation + MonthOfDefered) % 12) + " mois, Vous aurez payez " +
            formatNumber(totalPaid) + " euros au total, dont " + formatNumber(std::min(monthPaid, totalPaid)) + " ce dernier mois !\n";
}
std::string Pret::Info() {
    return InfoFlux;
}
