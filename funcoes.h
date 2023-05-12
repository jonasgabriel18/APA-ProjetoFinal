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

#endif // FUNCOES_H
