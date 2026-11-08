//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//
#ifndef UNTITLED_INVEST_H
#define UNTITLED_INVEST_H
#include <string>
#include <unordered_map>
#include <vector>

class Invest {
    public:
        Invest() = delete;
        Invest(double montant, double taux, bool flatTaxe = true, std::vector<std::pair<double, double>> eachMonthOfMore = {{1,0}}, std::vector<double> eachMonthOfEarn = {1}, double month = 120);

        std::string Info();
        static void DoAllInvest();

    private:
        double Montant;
        double Taux;
        bool FlatTaxe;
        std::vector<std::pair<double, double>> EachMonthOfMore;
        std::vector<double> EachEarnMonth;
        double Month;
        std::string InfoFlux;

        void Invesstissement(double monthOfSimulation);

};
#endif
