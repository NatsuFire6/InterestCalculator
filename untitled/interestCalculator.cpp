#include "Pret.hpp"
#include "Invest.h"

#include <fstream>
#include <iostream>
using namespace std;

void investWithPret() {
    double monthResultWithPret = 0;
    double monthResultWithoutPret = 0;
    double tauxInv;
    double invest;
    double refund;
    bool refundGenerer;
    double allCapitalWithPret = 0;
    double allCapitalWithoutPret = 0;
    double gainsWithPret = 0;
    double gainsWithoutPret = 0;
    double personalInvest;
    bool flatTax;
    bool totalDefered;
    int monthOfDeferred = 0;

    double monthPaid;
    double tauxPret;
    double pret;
    double totalPaid = 0;
    double restToPaid = 0;

    cout << "Entree le capital de depart (en euro) : ";
    cin >> personalInvest;
    cout << "Entree le taux d'interet de l'invesstissement par an (en %) : ";
    cin >> tauxInv;
    string r;
    do {
        cout << "Entree o/n si il y a la flat tax (30% de revenu en moins) : ";
        cin >> r;
    }while(r != "o" && r != "n");
    if (r == "n") {
        flatTax = false;
    }else {
        flatTax = true;
    }
    cout << "Entree le montant du pret (en euro) : ";
    cin >> pret;
    do {
        cout << "Le taux du pret ne peux pas etre superieur au taux de l'invesstissement !" << endl;
        cout << "Entree le taux d'interet du pret par an (en %) : ";
        cin >> tauxPret;
    }while(tauxPret > tauxInv);
    cout << "Entree le montant supplemenaire investi chaque mois (en euro) : ";
    cin >> invest;
    r = "";
    do {
        cout << "Entree le montant que vous voulez rembourser par mois :\n 1. uniquement les interet generer (a partir du premier mois ou vous commencer a rembourser)\n 2. les interets generer + un montant\n 3. un montant ";
        cin >> r;
    }while(r != "1" && r != "2" && r != "3");
    if (r == "1") {
        refundGenerer = true;
        refund = 0;
    }else if (r == "2") {
        refundGenerer = true;
        cout << "Entree le montant que vous voulez rembourser" << endl;
        cin >> refund;
    }else{
        refundGenerer = false;
        do {
            cout << "Le montant ne peut pas être plus petit que les interets generer par le pret !" << endl;
            cout << "Entree le montant que vous voulez rembourser" << endl;
            cin >> refund;
        }while (refund <= pret + (pret*(tauxPret/100.0)));
    }
    do {
        cout << "Voulez vous rembourser le pret en diferer ? (o/n)" << endl;
        cin >> r;
    }while(r != "n" && r != "o");
    if (r == "o") {
        cout << "Entrer le nombre de mois de diferer : " << endl;
        cin >> monthOfDeferred;
        do {
            cout << "Diferer total (vous ne rembourser rien du tout avant la fin du deferer mais les interets sont capitalisant)? (o/n)" << endl;
            cin >> r;
        }while(r != "n" && r != "o");
        if (r == "n") {
            totalDefered = false;
        }else {
            totalDefered = true;
        }
    }

    allCapitalWithPret = personalInvest + pret;
    allCapitalWithoutPret = personalInvest;
    restToPaid = pret + (pret*(tauxPret/100.0/12));
    refundGenerer ? monthPaid = refund + (allCapitalWithPret*(tauxInv/100.0))/12 : monthPaid = refund;;
    if (flatTax) monthPaid *= 0.7;
    int totalMonth = 0;
    bool firstPaid = true;

    for (int i = 1; restToPaid > 0; i++) {
        totalMonth = i;

        monthResultWithPret = allCapitalWithPret * (tauxInv / 100.0 / 12);
        if (flatTax) monthResultWithPret *= 0.7;
        allCapitalWithPret += monthResultWithPret;
        gainsWithPret += monthResultWithPret;

        if (i <= monthOfDeferred) {
            monthPaid = 0;
            if (totalDefered) {
                restToPaid += restToPaid * (tauxPret/100.0/12);
            } else {
                totalPaid += restToPaid * (tauxPret/100.0/12);
            }
        }else {
            if (firstPaid) {
                refundGenerer ? monthPaid = refund + (allCapitalWithPret*(tauxInv/100.0))/12 : monthPaid = refund;;
                if (flatTax) monthPaid *= 0.7;
                firstPaid = false;
            }

            restToPaid += restToPaid * (tauxPret/100.0/12);
            monthPaid = std::min(monthPaid, restToPaid);
            restToPaid -= monthPaid;
            totalPaid += monthPaid;

            allCapitalWithPret -= monthPaid;
        }

        monthResultWithoutPret = allCapitalWithoutPret * (tauxInv / 100.0 / 12);
        if (flatTax) monthResultWithoutPret *= 0.7;
        allCapitalWithoutPret += monthResultWithoutPret;
        gainsWithoutPret += monthResultWithoutPret;

        cout << "SANS PRET : Vous aurez "<< allCapitalWithoutPret << " au bout de "<< i/12 <<" annees et " << i%12 << " mois. Soit " << personalInvest << " donnee de votre poche et " << gainsWithoutPret << " d'argent generer, dont " << monthResultWithoutPret << " ce mois !"  << endl;
        cout << "AVEC PRET : Vous aurez "<< allCapitalWithPret << " au bout de "<< i/12 <<" annees et " << i%12 << " mois. Soit " << personalInvest << " donnee de votre poche et " << gainsWithPret << " d'argent generer, dont " << monthResultWithPret << " ce mois !"  << endl;
        cout << "AVEC PRET : Vous aurez payez "<< totalPaid << " au bout de "<< i/12 <<" annees et " << i%12 << " mois, dont " << monthPaid << " ce mois ! Il vous reste " << restToPaid << " a payer !\n" << endl;

        personalInvest += invest;
        allCapitalWithPret += invest;
        allCapitalWithoutPret += invest;
    }

    cout << "A la fin du remboursement du pret potentiel :" << endl;
    cout << "SANS PRET : Vous aurez "<< allCapitalWithoutPret << " au bout de "<< totalMonth/12 <<" annees et " << totalMonth%12 << " mois. Soit " << personalInvest << " donnee de votre poche et " << gainsWithoutPret << " d'argent generer, dont " << monthResultWithoutPret << " ce dernier mois !"  << endl;
    cout << "AVEC PRET : Vous aurez "<< allCapitalWithPret << " au bout de "<< totalMonth/12 <<" annees et " << totalMonth%12 << " mois. Soit " << personalInvest << " donnee de votre poche et " << gainsWithPret << " d'argent generer, dont " << monthResultWithPret << " ce dernier mois !"  << endl;
    cout << "AVEC PRET : Vous aurez payez "<< totalPaid << " au bout de "<< totalMonth/12 <<" annees et " << totalMonth%12 << " mois !" << endl;
    cout << "Avec un pret, vous aurez donc gagnez au total " << allCapitalWithPret-allCapitalWithoutPret << " de plus et aurez un revenu mensuel suplementaire de " << monthResultWithPret-monthResultWithoutPret << endl;
};
void DoAllInvest() {
    std::ofstream f{"Invesstissements.txt"};
    std::string buffer;
    buffer.reserve(500'000);

    if (!f.is_open()) return;
        int i = 0;
        for (int m = 1; m <= 100; ++m) {            // 10 000 → 1 000 000
            const double montant = m * 10000.0;
            for (int t = 1; t <= 32; ++t) {         // 0.5 → 16.0 par 0.5
                const double taux = t * 0.5;
                ++i;
                std::cout << i << "/3200 ";

                Invest investF{montant, taux, true};
                Invest invest{montant, taux, false};
                buffer += investF.Info();
                buffer += invest.Info();
            }
            if (buffer.size() > 200'000) {
                f << buffer;
                buffer.clear();
            }
        }
    f << buffer;

}
void DoAllPret() {
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
void DoAllPretWithDefered() {
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
                    for (int md = 1; md <= 120; ++md) {
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
int main()
{

    investWithPret();
    //DoAllInvest();
    //DoAllPret();
    //DoAllPretWithDefered();

    return 0;
}