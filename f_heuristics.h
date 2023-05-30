#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <chrono>

int encontrarMenor(const std::vector<int> &vetorProdutos, const std::vector<int> &indicesUtilizados);
std::vector<std::vector<int>> gerarSolucaoGulosa(int n, int m, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos);
std::vector<int> temposProducao(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos);

std::vector<std::vector<int>> movimentoInsercao(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao);

int calculoCustoNovoLinha(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &temposSolucao, int linha, int indexProdAtual, int indexProdSubs);
std::vector<int> buscaMelhorCusto(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &temposSolucao);
std::vector<std::vector<int>> novaSolucaoMesmaLinha(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &temposSolucao);

std::vector<int> calculoCustoNovoEntreLinhas(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao, int linhaAtual, int linhaSubs, int indexProdAtual, int indexProdSubs);
std::vector<int> buscaMelhorCustoEntreLinhas(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao);
std::vector<std::vector<int>> novaSolucaoEntreLinhas(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao);

std::vector<int> calculoCustoNovoReInsertion(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao, int linhaAtual, int linhaSubs, int indexProdAtual, int indexProdSubs);
std::vector<int> buscaMelhorCustoReInsertion(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao);
std::vector<std::vector<int>> novaSolucaoReInsertion(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao);

std::vector<std::vector<int>> melhorarLinhas(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao);
std::vector<std::vector<int>> melhorarLinhasRVND(std::vector<std::vector<int>> &solucao, std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, std::vector<int> &temposSolucao);

std::vector<int> listaCandidatosRestritos(std::vector<int> &vetorProdutos, std::vector<int> &sindicesProdutosUsados, float alfa);
std::vector<std::vector<int>> grasp(std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, int numeroLinhas, int numeroIteracoes, float alfa);

std::vector<std::vector<int>> perturbacao(std::vector<std::vector<int>> &solucao, int numeroLinhas);
std::vector<std::vector<int>> perturbacaoInverter(std::vector<std::vector<int>> &solucao, int numeroLinhas);
std::vector<std::vector<int>> perturbacaoVertical(std::vector<std::vector<int>> &solucao, int numeroLinhas);

std::vector<std::vector<int>> ils(std::vector<std::vector<int>> &matrizPreparacao, std::vector<int> &vetorProdutos, int numeroLinhas, int numeroIteracoes);

#endif