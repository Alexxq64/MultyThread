#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <numeric>
#include <chrono>
#include <iomanip>
#include <locale>

const int BASIS_LENGTH = 20;
const int LOW_BOUND = 1;
const int UPPER_BOUND = 100;

void calculateSum(const std::vector<int>& array, int startIndex, int endIndex, long long& partialSum);

double threadCalculation(int M, int N, std::vector<int>& array, long long& totalSum);

void tests(int N, int numCores);
