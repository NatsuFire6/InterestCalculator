cmake_minimum_required(VERSION 4.0)
project(InterestCalculator)

set(CMAKE_CXX_STANDARD 20)

add_executable(InterestCalculator
        interestCalculator.cpp
        Invest.cpp
        Pret.cpp
        FormatNumber.cpp
)
