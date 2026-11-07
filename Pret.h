//
// Created by LA TOUR MSI DU TURFU on 14/01/2026.
//
#ifndef C_PRET_H
#define C_PRET_H
#include <string>

class Pret {
    public:
        Pret() = delete;
        Pret(double montant, double taux, int monthOfSimulation = 0, double monthPaid = 0, int monthOfDefered = 0, bool totalDefered = true);

        std::string Info();

        static void DoAllPret();
        static void DoAllPretWithDefered();
    private:
        double Montant;
        double Taux;
        int MonthOfSimulation;
        int MonthOfDefered;
        double MonthPaid;
        bool TotalDefered;
        std::string InfoFlux;

        void PretByPaid(double monthPaid);
        void PretByMonth(int monthOfSimulation);
};
#endif