#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>
#include <unordered_set>

int encontrarMenor(const std::vector<int> &vetorProdutos, const std::vector<int> &indicesUtilizados);
std::vector<std::vector<int>> gerarSolucaoGulosa(int n, int m, std::vector<int> vetorProdutos, std::vector<std::vector<int>> matrizPreparacao);
std::vector<int> temposProducao(std::vector<std::vector<int>> solucao, std::vector<std::vector<int>> matrizPreparacao, std::vector<int> vetorProdutos);

std::vector<std::vector<int>> trocarProdutosMesmaLinha(std::vector<std::vector<int>> solucao, std::vector<std::vector<int>> matrizPreparacao, std::vector<int> temposSolucao);
std::vector<std::vector<int>> trocarProdutosEntreLinhas(std::vector<std::vector<int>> solucao, std::vector<std::vector<int>> matrizPreparacao, std::vector<int> vetorProdutos, std::vector<int> temposSolucao);

std::vector<std::vector<int>> movimentoInverter(std::vector<std::vector<int>> solucao, std::vector<std::vector<int>> matrizPreparacao, std::vector<int> vetorProdutos, std::vector<int> solution_times);
std::vector<std::vector<int>> movimentoInsercao(std::vector<std::vector<int>> solucao, std::vector<std::vector<int>> matrizPreparacao, std::vector<int> vetorProdutos, std::vector<int> solution_times);

std::vector<std::vector<int>> movimentoHorizontal(std::vector<std::vector<int>> solucao, std::vector<int> vetorProdutos, std::vector<std::vector<int>> matrizPreparacao, std::vector<int> temposSolucao);
std::vector<std::vector<int>> movimentoVertical(std::vector<std::vector<int>> solucao, std::vector<int> vetorProdutos, std::vector<std::vector<int>> matrizPreparacao, std::vector<int> temposSolucao);

std::vector<std::vector<int>> melhorarLinhas(std::vector<std::vector<int>> solucao, std::vector<std::vector<int>> matrizPreparacao, std::vector<int> vetorProdutos, std::vector<int> tempos);
std::vector<std::vector<int>> heuristicaGRASP(std::vector<int> &vetorProdutos, std::vector<std::vector<int>> &matrizPreparacao, int numeroLinhas, int produtos);
std::vector<std::vector<int>> pertubacao(std::vector<std::vector<int>> &solucao, int numeroLinhas);
std::vector<std::vector<int>> heuristicaILS(std::vector<int> &vetorProdutos, std::vector<std::vector<int>> &matrizPreparacao, int numeroLinhas, int produtos);

#endif