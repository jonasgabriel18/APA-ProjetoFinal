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
	//string localArquivo = "./instancias/n10m2_A.txt";
	//string localArquivo = "./instancias/n52m5_A.txt";
	//string localArquivo = "./instancias/n500m10_A.txt";
    string localArquivo = "./arquivo.txt";

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

	vector<vector<int>> solucaoGulosa = gerarSolucaoGulosa(numeroProdutos, numeroLinhas, vetorProdutos, matrizPreparacao);

	// Obter o tempo atual após a execução
	auto fimGuloso = chrono::high_resolution_clock::now();

	// Calcular a duração em segundos
	duracao = fimGuloso - inicioGuloso;
	tempo = duracao.count();

	vector<int> temposGuloso = temposProducao(solucaoGulosa, matrizPreparacao, vetorProdutos);
	cout << "Guloso: ...... " << *max_element(temposGuloso.begin(), temposGuloso.end());
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
	cout << "Guloso + VND:  " << *max_element(temposMelhorados.begin(), temposMelhorados.end());
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

	// * Testes de qualidade

	vector<vector<int>> solucaoH1 = movimentoHorizontal(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposH1 = temposProducao(solucaoH1, matrizPreparacao, vetorProdutos);

	cout << "H1: ......... " << *max_element(temposH1.begin(), temposH1.end()) << endl;

	vector<vector<int>> solucaoH2 = trocarProdutosMesmaLinha(solucaoGulosa, matrizPreparacao, temposGuloso);
	vector<int> temposH2 = temposProducao(solucaoH2, matrizPreparacao, vetorProdutos);

	cout << "H2: ......... " << *max_element(temposH2.begin(), temposH2.end()) << endl;

	vector<vector<int>> solucaoV1 = movimentoVertical(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposV1 = temposProducao(solucaoV1, matrizPreparacao, vetorProdutos);

	cout << "V1: ......... " << *max_element(temposV1.begin(), temposV1.end()) << endl;

	vector<vector<int>> solucaoV2 = trocarProdutosEntreLinhas(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposV2 = temposProducao(solucaoV2, matrizPreparacao, vetorProdutos);

	cout << "V2: ......... " << *max_element(temposV2.begin(), temposV2.end()) << endl;

	vector<vector<int>> solucaoI1 = movimentoInsercao(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposI1 = temposProducao(solucaoI1, matrizPreparacao, vetorProdutos);

	cout << "I1: ......... " << *max_element(temposI1.begin(), temposI1.end()) << endl;

	vector<vector<int>> solucaoI2 = inserirProdutoEmOutrasPosicoes(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposI2 = temposProducao(solucaoI2, matrizPreparacao, vetorProdutos);

	cout << "I2: ......... " << *max_element(temposI2.begin(), temposI2.end()) << endl;

	cout << "\n=============== \n"
		 << endl;
	
	/*
	vector<vector<int>> teste = novaSolucaoMesmaLinha(solucaoGulosa, matrizPreparacao, temposGuloso);

	for(int i = 0; i < teste.size(); i++) {
		for(int j = 0; j < teste[i].size(); j++) {
			cout << " " << teste[i][j]; 
		}
		cout << endl;
	} */

	return 0;
}