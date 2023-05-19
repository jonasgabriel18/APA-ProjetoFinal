#include "funcoes.h"
#include "f_heuristics.h"

using namespace std;

int main()
{

    // # Manipulação do Arquivo

    // Definindo o endereço do arquivo
    // string localArquivo = "S:/Programming/C++/APA/arquivo.txt";
    string localArquivo = "S:/Programming/C++/APA/n15m3_A.txt";

    // Chamando a função para definir as informações do arquivo
    tuple<int, int, vector<int>, vector<vector<int>>>
        informacoes = definirInformacoesArquivo(localArquivo);

    // Extraindo as informações retornadas pela função
    int numeroLinhas = get<0>(informacoes);
    int numeroProdutos = get<1>(informacoes);
    vector<int> vetorProdutos = get<2>(informacoes);
    vector<vector<int>> matrizPreparacao = get<3>(informacoes);

    cout << "\n=============== \n"
         << endl;

    // Imprimindo as informações extraídas
    cout << "Numero de linhas: " << numeroLinhas << endl;
    cout << "Numero de produtos: " << numeroProdutos << endl;

    cout << "\n=============== \n"
         << endl;

    // # Algoritmo Guloso

    vector<vector<int>> solucaoGulosa = gerarSolucaoGulosa(numeroProdutos, numeroLinhas, vetorProdutos, matrizPreparacao);
    vector<int> tempos = temposProducao(solucaoGulosa, matrizPreparacao, vetorProdutos);
    cout << "Guloso: " << *max_element(tempos.begin(), tempos.end()) << endl;

    // * Guloso + VND

    vector<vector<int>> solucaoMelhorada = melhorarLinhas(solucaoGulosa, matrizPreparacao, vetorProdutos, tempos);
    vector<int> temposMelhorados = temposProducao(solucaoMelhorada, matrizPreparacao, vetorProdutos);
    cout << "Guloso + VND: " << *max_element(temposMelhorados.begin(), temposMelhorados.end()) << endl;

    // | Meta-heurística: GRASP

    vector<vector<int>> solucaoGRASP = heuristicaGRASP(vetorProdutos, matrizPreparacao, numeroLinhas, numeroProdutos);
    vector<int> temposGRASP = temposProducao(solucaoGRASP, matrizPreparacao, vetorProdutos);
    cout << "GRASP: " << *max_element(temposGRASP.begin(), temposGRASP.end()) << endl;

    // | Meta-heurística: ILS

    vector<vector<int>> solucaoILS = heuristicaILS(vetorProdutos, matrizPreparacao, numeroLinhas, numeroProdutos);
    vector<int> temposILS = temposProducao(solucaoILS, matrizPreparacao, vetorProdutos);
    cout << "ILS:" << *max_element(temposILS.begin(), temposILS.end()) << endl;

    return 0;
}
