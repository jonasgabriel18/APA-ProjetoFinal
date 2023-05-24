#include "funcoes.h"
#include "f_heuristics.h"

using namespace std;

int main()
{

    double tempo;
    chrono::duration<double> duracao;

    // # Manipulação do Arquivo

    // Definindo o endereço do arquivo
    // string localArquivo = "S:/Programming/C++/APA/arquivo.txt";
    string localArquivo = "./instancias/n10m2_B.txt";
    // string localArquivo = "S:/Programming/C++/APA/n52m5_A.txt";
    // string localArquivo = "S:/Programming/C++/APA/n500m10_A.txt";

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

    cout << "\n=============== \n" << endl;

    // # Algoritmo Guloso

    // Obter o tempo atual antes do início da execução
    auto inicioGuloso = chrono::high_resolution_clock::now();

    vector<vector<int>> solucaoGulosa = gerarSolucaoGulosa(numeroProdutos, numeroLinhas, vetorProdutos, matrizPreparacao);

    // Obter o tempo atual após a execução
    auto fimGuloso = chrono::high_resolution_clock::now();

    // Calcular a duração em segundos
    duracao = fimGuloso - inicioGuloso;
    tempo = duracao.count();

    vector<int> tempos = temposProducao(solucaoGulosa, matrizPreparacao, vetorProdutos);
    cout << "Guloso: ...... " << *max_element(tempos.begin(), tempos.end());
    cout << " (" << tempo << "s)" << endl;

    // * Guloso + VND

    // Obter o tempo atual antes do início da execução
    auto inicioVND = chrono::high_resolution_clock::now();

    vector<vector<int>> solucaoMelhorada = melhorarLinhas(solucaoGulosa, matrizPreparacao, vetorProdutos, tempos);

    // Obter o tempo atual após a execução
    auto fimVND = chrono::high_resolution_clock::now();

    // Calcular a duração em segundos
    duracao = fimVND - inicioVND;
    tempo = duracao.count();

    vector<int> temposMelhorados = temposProducao(solucaoMelhorada, matrizPreparacao, vetorProdutos);
    cout << "Guloso + VND:  " << *max_element(temposMelhorados.begin(), temposMelhorados.end());
    cout << " (" << tempo << "s)" << endl;

    // * Guloso + RVND

    // Obter o tempo atual antes do início da execução
    
    auto inicioRVND = chrono::high_resolution_clock::now();

    vector<vector<int>> solucaoMelhoradaRVND = melhorarLinhasRVND(solucaoGulosa, matrizPreparacao, vetorProdutos, tempos);

    // Obter o tempo atual após a execução
    auto fimRVND = chrono::high_resolution_clock::now();

    // Calcular a duração em segundos
    duracao = fimRVND - inicioRVND;
    tempo = duracao.count();

    vector<int> temposMelhoradosRVND = temposProducao(solucaoMelhoradaRVND, matrizPreparacao, vetorProdutos);
    cout << "Guloso + RVND: " << *max_element(temposMelhoradosRVND.begin(), temposMelhoradosRVND.end());
    cout << " (" << tempo << "s)" << endl;

    // | Meta-heurística: GRASP

    // Obter o tempo atual antes do início da execução
    auto inicioGRASP = chrono::high_resolution_clock::now();

    vector<vector<int>> solucaoGRASP = heuristicaGRASP(vetorProdutos, matrizPreparacao, numeroLinhas, numeroProdutos, 3);

    // Obter o tempo atual após a execução
    auto fimGRASP = chrono::high_resolution_clock::now();

    // Calcular a duração em segundos
    duracao = fimGRASP - inicioGRASP;
    tempo = duracao.count();

    vector<int> temposGRASP = temposProducao(solucaoGRASP, matrizPreparacao, vetorProdutos);
    cout << "GRASP: ....... " << *max_element(temposGRASP.begin(), temposGRASP.end());
    cout << " (" << tempo << "s)" << endl;

    // | Meta-heurística: ILS

    // Obter o tempo atual antes do início da execução
    auto inicioILS = chrono::high_resolution_clock::now();

    vector<vector<int>> solucaoILS = heuristicaILS(vetorProdutos, matrizPreparacao, numeroLinhas, numeroProdutos, 3);

    // Obter o tempo atual após a execução
    auto fimILS = chrono::high_resolution_clock::now();

    // Calcular a duração em segundos
    duracao = fimILS - inicioILS;
    tempo = duracao.count();

    vector<int> temposILS = temposProducao(solucaoILS, matrizPreparacao, vetorProdutos);
    cout << "ILS: ......... " << *max_element(temposILS.begin(), temposILS.end());
    cout << " (" << tempo << "s)" << endl;

    cout << "\n=============== \n"
         << endl;

    return 0;
}
