//
// Created by valen on 06/11/2026.
//

#ifndef INTERESTCALCULATOR_INVESTANDPRET_H
#define INTERESTCALCULATOR_INVESTANDPRET_H


class InvestAndPret
{
    double monthResultWithPret = 0;
    double monthResultWithoutPret = 0;
    double tauxInv;
    double invest;
    int interestMonth;
    double refund;
    bool refundGenerer;
    double allCapitalWithPret = 0;
    double allCapitalWithoutPret = 0;
    double gainsWithPret = 0;
    double gainsWithPretHavePayed = 0;
    double gainsWithoutPret = 0;
    double personalInvest;
    bool flatTax;
    bool totalDefered;
    int monthOfDeferred = 0;

    double monthPaid;
    double tauxPret;
    double pret;
    double totalPaid = 0;
    double totalOwnPaid = 0;
    double restToPaid = 0;

    void AskParam();
    void DoSimulation();
public :
    static void InvestWithPret();
};


#endif //INTERESTCALCULATOR_INVESTANDPRET_H