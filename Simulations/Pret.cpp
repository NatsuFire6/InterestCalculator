//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#include "Pret.h"
#include "../WindowUtils/FormatNumber.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

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
            "\nPrêt créé avec les paramètres : " + FormatNumberToTextString(Montant) + " euros, " +
            FormatNumberToTextString(Taux) + "% d'intérêts, sur " + FormatNumberToTextString(monthOfSimulation) + " mois" +
            (MonthOfDefered != 0 ? (", avec un diféré " + std::string(TotalDefered ? "total " : "") +
            "de " + FormatNumberToTextString(MonthOfDefered) + " mois") : "") +
            " !\nAu bout de " + FormatNumberToTextString((monthOfSimulation + MonthOfDefered) / 12) + " années et " +
            FormatNumberToTextString((monthOfSimulation + MonthOfDefered) % 12) + " mois, Vous aurez payez " +
            FormatNumberToTextString(totalPaid) + " euros au total, dont " + FormatNumberToTextString(monthPaid) + " ce dernier mois !\n";
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
            "\nPrêt créé avec les paramètres : " + FormatNumberToTextString(Montant) + " euros, " +
            FormatNumberToTextString(Taux) + "% d'intérêts, avec un remboursement de " + FormatNumberToTextString(monthPaid) + " par mois" +
            (MonthOfDefered != 0 ? (", avec un différé " + std::string(TotalDefered ? "total " : "") +
            "de " + FormatNumberToTextString(MonthOfDefered) + " mois") : "") +
            " !\nAu bout de " + FormatNumberToTextString((monthOfSimulation + MonthOfDefered) / 12) + " années et " +
            FormatNumberToTextString((monthOfSimulation + MonthOfDefered) % 12) + " mois, Vous aurez payez " +
            FormatNumberToTextString(totalPaid) + " euros au total, dont " + FormatNumberToTextString(std::min(monthPaid, totalPaid)) + " ce dernier mois !\n";
}
std::string Pret::Info()
{
    return InfoFlux;
}
void Pret::DoAllPret() {
    std::ofstream f{"Prets.txt"};
    std::string buffer;
    buffer.reserve(2'000'000);

    if (!f.is_open()) return;
        int i = 0;
        for (int m = 1; m <= 100; ++m) {            // 10 000 → 1 000 000
            const double montant = m * 10000.0;
            for (int t = 2; t <= 16; ++t) {         // 1.0 → 8.0 par 0.5
                const double taux = t * 0.5;
                ++i;
                std::cout << i << "/1500 ";
                for (int mp = 1; mp <= 100; ++mp) { // 100 → 10 000
                    const double monthPaid = mp * 100.0;
                    Pret pret2{montant, taux, 0, monthPaid};
                    buffer += pret2.Info();
                }
                for (int monthOfSimulation = 12; monthOfSimulation <= 300; monthOfSimulation += 12) {
                    Pret pret1{montant, taux, monthOfSimulation};
                    buffer += pret1.Info();
                }
                if (buffer.size() > 1'000'000) {
                    f << buffer;
                    buffer.clear();
                }
            }
        }
    f << buffer;
}
void Pret::DoAllPretWithDefered() {
    std::ofstream f{"PretsAvecDiferer.txt"};
    std::string buffer;
    buffer.reserve(20'000'000);

    if (!f.is_open()) return;
        int i = 0;
        for (int m = 1; m <= 100; ++m) {            // 10 000 → 1 000 000
            double montant = m * 10000.0;
            for (int t = 2; t <= 16; ++t) {         // 1.0 → 8.0 par 0.5
                double taux = t * 0.5;
                ++i;
                std::cout << i << "/1500 ";
                for (int mp = 1; mp <= 100; ++mp) { // 100 → 10 000
                    double monthPaid = mp * 100.0;
                    for (int md = 1; md <= 120; ++md) { // 1 → 120
                        Pret pret1{montant, taux, 0, monthPaid, md, true};
                        Pret pret2{montant, taux, 0, monthPaid, md, false};
                        buffer += pret1.Info();
                        buffer += pret2.Info();
                    }
                }
                for (int monthOfSimulation = 12; monthOfSimulation <= 300; monthOfSimulation += 12) {
                    for (int md = 1; md <= monthOfSimulation/2; ++md) {
                        Pret pret2{montant, taux, monthOfSimulation, 0, md, false};
                        Pret pret1{montant, taux, monthOfSimulation, 0, md, true};
                        buffer += pret2.Info();
                        buffer += pret1.Info();
                    }
                }
                if (buffer.size() > 5'000'000) {
                    f << buffer;
                    buffer.clear();
                }
            }
        }
    f << buffer;
}