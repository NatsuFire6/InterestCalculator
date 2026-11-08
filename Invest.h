//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//
#ifndef UNTITLED_INVEST_H
#define UNTITLED_INVEST_H
#include <string>

class Invest {
    public:
        Invest() = delete;
        Invest(double montant, double taux, bool flatTaxe = true, double moreEachMonth = 0, double eachMonthOfMore = 0, double eachMonthOfEarn = 1, double month = 120);

        std::string Info();
        static void DoAllInvest();

    private:
        double Montant;
        double Taux;
        bool FlatTaxe;
        double More;
        double EachMonthOfMore;
        double EachEarnMonth;
        double Month;
        std::string InfoFlux;

        void Invesstissement(double monthOfSimulation);

};
#endif
