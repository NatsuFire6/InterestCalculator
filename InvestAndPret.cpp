//
// Created by valen on 06/11/2026.
//

#include "InvestAndPret.h"

#include <iostream>
using namespace std;

void InvestAndPret::AskParam()
{
    cout << "Entree le capital de depart (en euro) : ";
    cin >> personalInvest;
    cout << "Entree le taux d'interet de l'invesstissement par an (en %) : ";
    cin >> tauxInv;
    string r;
    do
    {
        cout << "Entree le nombre de mois a attendre pour percevoir les interets (ex: 1 = interet percu tous les mois),";
        cout << "Valeur minimal : 1 ";
        cout << "Si > 1 et que vous rembourser votre pret avec differer inferieur, vous serez oblige de payer de votre poche : ";
        cin >> interestMonth;
    }
    while (interestMonth < 1);
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
        cout << "Entree le montant que vous voulez rembourser chaque mois :\n 1. uniquement les interet generer (a partir du premier mois ou vous commencer a rembourser et dans la limite de ce que vous genere)\n 2. les interets generer + un montant\n 3. un montant ";
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
            cout << "Le montant ne peut pas être plus petit que les interets generer par le pret ! (" << pret + pret*(tauxPret/100.0/12) << " euros)" << endl;
            cout << "Entree le montant que vous voulez rembourser" << endl;
            cin >> refund;
        }while (refund <= pret + (pret*(tauxPret/100.0/12)));
    }
    do {
        cout << "Voulez vous rembourser le pret en diferer ? (o/n)" << endl;
        cin >> r;
    }while(r != "n" && r != "o");
    if (r == "o") {
        cout << "Entrer le nombre de mois de diferer : " << endl;
        cin >> monthOfDeferred;
        do {
            cout << "Diferer total (vous ne rembourser meme pas les interets genere par le pret (meme pas avec les interet genere par l'investissement) avant la fin du diferer,"
                    "\nmais les interets du pret genere des interets suplementaire sur les interets de ce meme pret) ? (o/n)" << endl;
            cin >> r;
        }while(r != "n" && r != "o");
        if (r == "n") {
            totalDefered = false;
        }else {
            totalDefered = true;
        }
    }
}

void InvestAndPret::DoSimulation()
{
    / initialisation des capitaux avec ou sans prêt
    allCapitalWithPret = personalInvest + pret;
    allCapitalWithoutPret = personalInvest;
    // initialisation du reste à payer avec déjà : un mois d'intérêts généré
    restToPaid = pret + (pret*(tauxPret/100.0/12));
    // si on veut rembourser ce qu'on génère alors, on initialise ce qu'on va payer ce mois-ci à :
    //  l'argent généré par l'investissement + l'argent que l'on veut rembourser en plus chaque mois,
    // sinon on l'initialise uniquement au montant que l'on veut générer chaque mois
    // sans oublier la flat taxe si elle est présente
    // -----not sure about that if interest arn't generated each month----
    refundGenerer ? monthPaid = refund + (flatTax ? ((allCapitalWithPret*(tauxInv/100.0))/12)*0.7 : (allCapitalWithPret*(tauxInv/100.0))/12) : monthPaid = refund;
    //if (flatTax) monthPaid *= 0.7;
    int totalMonth = 0;

    // calcul de l'investissement jusqu'à ce que le prêt soit entièrement rembourser
    for (int i = 1; restToPaid > 0; i++) {
        totalMonth = i;

        // ------------------ calcul avec un prêt ---------------------

        // résultat du mois en fonction : de si on perçoit de l'argent ce mois-ci, du taux d'investissement, du capital de départ, et du capital ajouter grâce au prêt
        totalMonth % interestMonth == 0 ? monthResultWithPret = allCapitalWithPret * (tauxInv / 100.0 / 12) : monthResultWithPret = 0;
        // retire 30% des bénéfices à cause de la flat taxe (qui est un impôt)
        if (flatTax) monthResultWithPret *= 0.7;
        // ajout de l'argent généré par l'investissement ce mois-ci
        allCapitalWithPret += monthResultWithPret;
        gainsWithPret += monthResultWithPret;

        // ne payer qu'en différé si demandé
        if (i <= monthOfDeferred) {
            // nous ne sommes pas arrivé au temps de remboursement donc on ne rembourse pas le prêt (sauf les intérêts générés par le prêt)
            monthPaid = 0;
            if (totalDefered) {
                // on ne rembourse même pas les intérêts générés par le prêt
                restToPaid += restToPaid * (tauxPret/100.0/12);
            }
            else
            {
                // on rembourse les intérêts générés par le prêt avec les interets généré par l'investissement (si demandé et que suffisamment d'argent a été généré)
                if (refundGenerer && gainsWithPret > restToPaid * (tauxPret/100.0/12))
                {
                    // on paye et retire donc ce qu'on vient de payer au capital total
                    gainsWithPretHavePayed += restToPaid * (tauxPret/100.0/12);
                    totalPaid += restToPaid * (tauxPret/100.0/12);
                    allCapitalWithPret -= restToPaid * (tauxPret/100.0/12);;
                }
                else
                {
                    // on rembourse les intérêts générés par le prêt avec son propre argent, car l'argent généré par l'investissement ne suffit pas
                    totalOwnPaid += restToPaid * (tauxPret/100.0/12);
                    totalPaid += restToPaid * (tauxPret/100.0/12);
                }
            }
        }
        else
        {
            // le différé est fini, on commence donc à rembourser

            // si on veut rembourser ce qu'on génère alors, on initialise ce qu'on va payer ce mois-ci à :
            //  l'argent généré par l'investissement + l'argent que l'on veut rembourser en plus chaque mois,
            // sinon on l'initialise uniquement au montant que l'on veut générer chaque mois
            // sans oublier la flat taxe si elle est présente
            refundGenerer ? monthPaid = refund + (flatTax ? ((allCapitalWithPret*(tauxInv/100.0))/12)*0.7 : (allCapitalWithPret*(tauxInv/100.0))/12) : monthPaid = refund;

            // on génère les intérêts du prêt
            restToPaid += restToPaid * (tauxPret/100.0/12);
            // on s'assure de ne pas rembourser trop par rapport à ce qu'il reste à payer
            monthPaid = std::min(monthPaid, restToPaid);
            // on retire ce qu'on doit payer au reste à payer
            restToPaid -= monthPaid;
            totalPaid += monthPaid;
            // on retire ce qu'on vient de payer au capital qu'il nous reste
            allCapitalWithPret -= monthPaid;
        }

        // ------------------ calcul sans le prêt ---------------------
        // calcul des intérêts généré ce mois-ci, sachant que nous ne sommes pas forcément à un moins qui génère de l'argent et qu'il peut y avoir la flat taxe
        monthResultWithoutPret = totalMonth % interestMonth == 0 ? allCapitalWithoutPret * (tauxInv / 100.0 / 12) * (flatTax ? 0.7 : 1) : 0;
        //ajout des intérêts généré, au capital et au gain total
        allCapitalWithoutPret += monthResultWithoutPret;
        gainsWithoutPret += monthResultWithoutPret;

        // log de chaque mois pour les conditions AVEC et SANS prêt
        cout << "SANS PRET : Vous aurez "<< allCapitalWithoutPret << " au bout de "<< i/12 <<" annees et " << i%12 << " mois. Soit " << personalInvest << " donnee de votre poche et " << gainsWithoutPret << " d'argent generer, dont " << monthResultWithoutPret << " ce mois !"  << endl;
        cout << "AVEC PRET : Vous aurez "<< allCapitalWithPret << " au bout de "<< i/12 <<" annees et " << i%12 << " mois. Soit " << personalInvest << " donnee de votre poche et " << gainsWithPret << " d'argent generer, dont " << monthResultWithPret << " ce mois !"  << endl;
        cout << "AVEC PRET : Vous aurez payez "<< totalPaid << " dont " << totalOwnPaid << " votre poche, au bout de " << i/12 << " annees et " << i%12 << " mois, dont " << monthPaid << " ce mois ! Il vous reste " << restToPaid << " a payer !\n" << endl;

        // ajout du montant investi en plus chaque mois aux capitaux
        personalInvest += invest;
        allCapitalWithPret += invest;
        allCapitalWithoutPret += invest;
    }
    //log de la fin du prêt (le dernier mois, donc le prêt a été entièrement remboursé)
    cout << "A la fin du remboursement du pret potentiel :" << endl;
    cout << "SANS PRET : Vous aurez "<< allCapitalWithoutPret << " au bout de "<< totalMonth/12 <<" annees et " << totalMonth%12 << " mois. Soit " << personalInvest << " donnee de votre poche et " << gainsWithoutPret << " d'argent generer, dont " << monthResultWithoutPret << " ce dernier mois !"  << endl;
    cout << "AVEC PRET : Vous aurez "<< allCapitalWithPret << " au bout de "<< totalMonth/12 <<" annees et " << totalMonth%12 << " mois. Soit " << personalInvest << " donnee de votre poche et " << gainsWithPret << " d'argent generer, dont " << monthResultWithPret << " ce dernier mois !"  << endl;
    cout << "AVEC PRET : Vous aurez payez "<< totalPaid << " dont " << totalOwnPaid << " votre poche, au bout de " << totalMonth/12 << " annees et " << totalMonth%12 << " mois !" << endl;
    cout << "Avec un pret, vous aurez donc gagnez au total " << allCapitalWithPret-allCapitalWithoutPret << " de plus et aurez un revenu mensuel suplementaire de " << monthResultWithPret-monthResultWithoutPret << endl;
}

void InvestAndPret::InvestWithPret()
{
    InvestAndPret simu {};
    simu.AskParam();
    simu.DoSimulation();
}
