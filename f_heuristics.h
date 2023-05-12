#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

int encontrarMenor(const std::vector<int> &vetor, const std::vector<int> &indicesUtilizados);
std::vector<std::vector<int>> gerarSolucaoGulosa(int n, int m, std::vector<int> vetor, std::vector<std::vector<int>> matriz);
int solution_time(std::vector<std::vector<int>> solution, std::vector<std::vector<int>> maintance, std::vector<int> products);

#endif