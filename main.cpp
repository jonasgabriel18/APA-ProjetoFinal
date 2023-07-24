#include "funcoes.h"
#include "f_heuristics.h"

using namespace std;

int main(int argc, char *argv[])
{

	double tempo;
	chrono::duration<double> duracao;

	int numeroIteracoes = 1000;

	// # Manipulação do Arquivo
	string instancia = argv[1];
	string localArquivo = "./instancias/" + instancia;
	cout << localArquivo << endl;

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

	// Obter o tempo atual antes do início da execução
	auto inicioGuloso = chrono::high_resolution_clock::now();

	vector<vector<int>> solucaoGulosa = gerarSolucaoGulosa(numeroProdutos, numeroLinhas, matrizPreparacao, vetorProdutos);

	// Obter o tempo atual após a execução
	auto fimGuloso = chrono::high_resolution_clock::now();

	// Calcular a duração em segundos
	duracao = fimGuloso - inicioGuloso;
	tempo = duracao.count();

	vector<int> temposGuloso = temposProducao(solucaoGulosa, matrizPreparacao, vetorProdutos);
	cout << "Guloso: ....... " << *max_element(temposGuloso.begin(), temposGuloso.end());
	cout << " (" << tempo << "s)" << endl;

	// * Guloso + VND

	// Obter o tempo atual antes do início da execução
	auto inicioVND = chrono::high_resolution_clock::now();

	vector<vector<int>> solucaoMelhorada = melhorarLinhas(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);

	// Obter o tempo atual após a execução
	auto fimVND = chrono::high_resolution_clock::now();

	// Calcular a duração em segundos
	duracao = fimVND - inicioVND;
	tempo = duracao.count();

	vector<int> temposMelhorados = temposProducao(solucaoMelhorada, matrizPreparacao, vetorProdutos);
	cout << "Guloso + VND: . " << *max_element(temposMelhorados.begin(), temposMelhorados.end());
	cout << " (" << tempo << "s)" << endl;

	// * Guloso + RVND

	// Obter o tempo atual antes do início da execução
	auto inicioRVND = chrono::high_resolution_clock::now();

	vector<vector<int>> solucaoMelhoradaRVND = melhorarLinhasRVND(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);

	// Obter o tempo atual após a execução
	auto fimRVND = chrono::high_resolution_clock::now();

	// Calcular a duração em segundos
	duracao = fimRVND - inicioRVND;
	tempo = duracao.count();

	vector<int> temposMelhoradosRVND = temposProducao(solucaoMelhoradaRVND, matrizPreparacao, vetorProdutos);
	cout << "Guloso + RVND:  " << *max_element(temposMelhoradosRVND.begin(), temposMelhoradosRVND.end());
	cout << " (" << tempo << "s)" << endl;

	// | Meta-heurística: GRASP
	// Obter o tempo atual antes do início da execução
	auto inicioGRASP2 = chrono::high_resolution_clock::now();

	vector<vector<int>> solucaoGRASP2 = grasp(matrizPreparacao, vetorProdutos, numeroLinhas, numeroIteracoes, 0.87);

	// Obter o tempo atual após a execução
	auto fimGRASP2 = chrono::high_resolution_clock::now();

	// Calcular a duração em segundos
	duracao = fimGRASP2 - inicioGRASP2;
	tempo = duracao.count();

	vector<int> temposGRASP2 = temposProducao(solucaoGRASP2, matrizPreparacao, vetorProdutos);
	cout << "GRASP2: ....... " << *max_element(temposGRASP2.begin(), temposGRASP2.end());
	cout << " (" << tempo << "s)" << endl;

	// | Meta-heurística: ILS
	// Obter o tempo atual antes do início da execução
	auto inicioILS2 = chrono::high_resolution_clock::now();

	vector<vector<int>> solucaoILS2 = ils(matrizPreparacao, vetorProdutos, numeroLinhas, numeroIteracoes);

	// Obter o tempo atual após a execução
	auto fimILS2 = chrono::high_resolution_clock::now();

	// Calcular a duração em segundos
	duracao = fimILS2 - inicioILS2;
	tempo = duracao.count();

	vector<int> temposILS2 = temposProducao(solucaoILS2, matrizPreparacao, vetorProdutos);
	cout << "ILS2: ......... " << *max_element(temposILS2.begin(), temposILS2.end());
	cout << " (" << tempo << "s)" << endl;

	cout << "\n=============== \n"
		 << endl;
	return 0;
}