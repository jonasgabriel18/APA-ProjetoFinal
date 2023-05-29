#include "funcoes.h"
#include "f_heuristics.h"

using namespace std;

int main()
{

	double tempo;
	chrono::duration<double> duracao;

	int numeroIteracoes = 1000;

	// # Manipulação do Arquivo

	// Definindo o endereço do arquivo
	// string localArquivo = "S:/Programming/C++/APA/arquivo.txt";
	// string localArquivo = "./instancias/n10m2_B.txt";
	// string localArquivo = "./instancias/n15m3_A.txt";
	// string localArquivo = "./instancias/n29m4_B.txt";
	// string localArquivo = "./instancias/n40m5_B.txt";
	// string localArquivo = "./instancias/n52m5_B.txt";
	string localArquivo = "./instancias/n450m16_A.txt";
	// string localArquivo = "./instancias/n500m10_A.txt";

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

	// // Obter o tempo atual antes do início da execução
	// auto inicioGRASP1 = chrono::high_resolution_clock::now();

	// vector<vector<int>> solucaoGRASP1 = heuristicaGRASP(vetorProdutos, matrizPreparacao, numeroLinhas, numeroProdutos, numeroIteracoes);

	// // Obter o tempo atual após a execução
	// auto fimGRASP1 = chrono::high_resolution_clock::now();

	// // Calcular a duração em segundos
	// duracao = fimGRASP1 - inicioGRASP1;
	// tempo = duracao.count();

	// vector<int> temposGRASP1 = temposProducao(solucaoGRASP1, matrizPreparacao, vetorProdutos);
	// cout << "GRASP1: ....... " << *max_element(temposGRASP1.begin(), temposGRASP1.end());
	// cout << " (" << tempo << "s)" << endl;

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

	// // Obter o tempo atual antes do início da execução
	// auto inicioILS1 = chrono::high_resolution_clock::now();

	// vector<vector<int>> solucaoILS1 = heuristicaILS(vetorProdutos, matrizPreparacao, numeroLinhas, numeroProdutos, numeroIteracoes);

	// // Obter o tempo atual após a execução
	// auto fimILS1 = chrono::high_resolution_clock::now();

	// // Calcular a duração em segundos
	// duracao = fimILS1 - inicioILS1;
	// tempo = duracao.count();

	// vector<int> temposILS1 = temposProducao(solucaoILS1, matrizPreparacao, vetorProdutos);
	// cout << "ILS1: ......... " << *max_element(temposILS1.begin(), temposILS1.end());
	// cout << " (" << tempo << "s)" << endl;

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

	// * Testes de qualidade

	vector<vector<int>> solucaoH1 = movimentoHorizontal(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposH1 = temposProducao(solucaoH1, matrizPreparacao, vetorProdutos);

	cout << "H1: ......... " << *max_element(temposH1.begin(), temposH1.end()) << endl;

	vector<vector<int>> solucaoH2 = trocarProdutosMesmaLinha(solucaoGulosa, matrizPreparacao, temposGuloso);
	vector<int> temposH2 = temposProducao(solucaoH2, matrizPreparacao, vetorProdutos);

	cout << "H2: ......... " << *max_element(temposH2.begin(), temposH2.end()) << endl;

	vector<vector<int>> solucaoH3 = novaSolucaoMesmaLinha(solucaoGulosa, matrizPreparacao, temposGuloso);
	vector<int> temposH3 = temposProducao(solucaoH3, matrizPreparacao, vetorProdutos);

	cout << "H3: ......... " << *max_element(temposH3.begin(), temposH3.end()) << endl;

	vector<vector<int>> solucaoV1 = movimentoVertical(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposV1 = temposProducao(solucaoV1, matrizPreparacao, vetorProdutos);

	cout << "V1: ......... " << *max_element(temposV1.begin(), temposV1.end()) << endl;

	vector<vector<int>> solucaoV2 = trocarProdutosEntreLinhas(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposV2 = temposProducao(solucaoV2, matrizPreparacao, vetorProdutos);

	cout << "V2: ......... " << *max_element(temposV2.begin(), temposV2.end()) << endl;

	vector<vector<int>> solucaoV3 = novaSolucaoEntreLinhas(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposV3 = temposProducao(solucaoV3, matrizPreparacao, vetorProdutos);

	cout << "V3: ......... " << *max_element(temposV3.begin(), temposV3.end()) << endl;

	vector<vector<int>> solucaoI1 = movimentoInsercao(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposI1 = temposProducao(solucaoI1, matrizPreparacao, vetorProdutos);

	cout << "I1: ......... " << *max_element(temposI1.begin(), temposI1.end()) << endl;

	vector<vector<int>> solucaoI2 = inserirProdutoEmOutrasPosicoes(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposI2 = temposProducao(solucaoI2, matrizPreparacao, vetorProdutos);

	cout << "I2: ......... " << *max_element(temposI2.begin(), temposI2.end()) << endl;

	vector<vector<int>> solucaoI3 = novaSolucaoReInsertion(solucaoGulosa, matrizPreparacao, vetorProdutos, temposGuloso);
	vector<int> temposI3 = temposProducao(solucaoI3, matrizPreparacao, vetorProdutos);

	cout << "I3: ......... " << *max_element(temposI3.begin(), temposI3.end()) << endl;

	cout
		<< "\n=============== \n"
		<< endl;

	return 0;
}