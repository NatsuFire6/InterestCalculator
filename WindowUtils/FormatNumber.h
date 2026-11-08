//
// Created by LA TOUR MSI DU TURFU on 20/01/2026.
//

#pragma once
#include <string>
#include <type_traits>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

template<typename T>
std::string FormatNumberToTextString(T value, int precision = 2) {
    static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");
    if (precision < 0) throw std::invalid_argument("Precision cannot be negative");

    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    std::string s = ss.str();

    size_t dotPos = s.find('.');
    if (dotPos == std::string::npos) dotPos = s.size();
    int insertPos = static_cast<int>(dotPos) - 3;
    while (insertPos > 0)
    {
        if (s[insertPos - 1] == '-') break;
        s.insert(insertPos, " ");
        insertPos -= 3;
    }
    return s;
}
template<typename T>
const char* FormatNumberToTextChar(T value, int precision = 2)
{
    static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");
    if (precision < 0) throw std::invalid_argument("Precision cannot be negative");

    thread_local std::string result;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    result = ss.str();

    // Recherche de la position de la virgule
    size_t dotPos = result.find('.');
    if (dotPos == std::string::npos) dotPos = result.size();
    // Ajout d'un espace tous les 3 chiffres
    int insertPos = static_cast<int>(dotPos) - 3;
    while (insertPos > 0)
    {
        // Ne pas séparer le signe négatif
        if (result[insertPos - 1] == '-') break;
        result.insert(insertPos, " ");
        insertPos -= 3;
    }
    return result.c_str();
}
enum class TimeUnit {
    Year,
    Month,
    Week,
    Day,
    Hour,
    Minute,
    Seconde
};

template<typename T>
std::string FormatDuration(TimeUnit type, T value)
{
    static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

    constexpr long double SECOND = 1.0L;
    constexpr long double MINUTE = 60.0L;
    constexpr long double HOUR   = 3600.0L;
    constexpr long double DAY    = 86400.0L;
    constexpr long double WEEK   = 604800.0L;
    constexpr long double MONTH  = 2592000.0L;
    constexpr long double YEAR   = 31536000.0L;
    long double totalSeconds = static_cast<long double>(value);
    switch (type)
    {
        case TimeUnit::Year:    totalSeconds *= YEAR; break;
        case TimeUnit::Month:   totalSeconds *= MONTH; break;
        case TimeUnit::Week:    totalSeconds *= WEEK; break;
        case TimeUnit::Day:     totalSeconds *= DAY; break;
        case TimeUnit::Hour:    totalSeconds *= HOUR; break;
        case TimeUnit::Minute:  totalSeconds *= MINUTE; break;
        case TimeUnit::Seconde: break;
        default:
            throw std::invalid_argument("Invalid time unit");
    }

    const bool negative = totalSeconds < 0;
    totalSeconds = std::abs(totalSeconds);
    long long years = static_cast<long long>(totalSeconds / YEAR);
    totalSeconds = std::fmod(totalSeconds, YEAR);
    long long months = static_cast<long long>(totalSeconds / MONTH);
    totalSeconds = std::fmod(totalSeconds, MONTH);
    long long weeks = static_cast<long long>(totalSeconds / WEEK);
    totalSeconds = std::fmod(totalSeconds, WEEK);
    long long days = static_cast<long long>(totalSeconds / DAY);
    totalSeconds = std::fmod(totalSeconds, DAY);
    long long hours = static_cast<long long>(totalSeconds / HOUR);
    totalSeconds = std::fmod(totalSeconds, HOUR);
    long long minutes = static_cast<long long>(totalSeconds / MINUTE);
    long double seconds = std::fmod(totalSeconds, MINUTE);

    std::stringstream ss;
    if (negative) ss << "-";
    ss << years   << " année(s), "
       << months  << " mois, "
       << weeks   << " semaine(s), "
       << days    << " jour(s), "
       << hours   << " heure(s), "
       << minutes << " minute(s), "
       << std::setprecision(10) << std::defaultfloat
       << seconds << " seconde(s)";
    return ss.str();
}