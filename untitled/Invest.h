//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//

#ifndef UNTITLED_INVEST_H
#define UNTITLED_INVEST_H
#include <string>


class Invest {
    public:
        Invest() = delete;
        Invest(double montant, double taux, bool flatTaxe = true, double moreEachMonth = 0, int month = 120);

        void Invesstissement(int monthOfSimulation);
        std::string Info();

    private:
        double Montant;
        double Taux;
        bool FlatTaxe;
        double More;
        int Month;
        std::string InfoFlux;

};


#endif //UNTITLED_INVEST_H