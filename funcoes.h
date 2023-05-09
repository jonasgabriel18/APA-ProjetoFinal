#ifndef FUNCOES_H
#define FUNCOES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

bool tentarCast(std::string linha);
int castStringDado(std::string linha);
std::vector<int> castStringVetor(std::string linha);
std::tuple<int, int, std::vector<int>, std::vector<std::vector<int>>> definirInformacoesArquivo(std::string localArquivo);
int encontrarMenor(const vector<int> &vetor, const vector<int> &indicesUtilizados);
std::vector<std::vector<int>> gerarSolucaoGulosa(int n, int m, std::vector<int> vetor, std::vector<std::vector<int>> matriz);

#endif // FUNCOES_H
