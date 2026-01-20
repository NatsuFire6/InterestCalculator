//
// Created by LA TOUR MSI DU TURFU on 20/01/2026.
//

#include "FormatNumber.h"

#include <iomanip>

std::string formatNumber(double value) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    std::string s = ss.str();

    size_t dotPos = s.find('.');
    int insertPos = static_cast<int>(dotPos) - 3;
    while (insertPos > 0) {
        s.insert(insertPos, " ");
        insertPos -= 3;
    }
    return s;
}