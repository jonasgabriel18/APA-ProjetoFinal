#include "funcoes.h"

using namespace std;

// Função para tentar fazer o cast de uma string para int
bool tentarCast(string linha)
{
	int valor;
	try
	{
		valor = stoi(linha);
	}
	// Se a linha tiver uma string sem números
	catch (invalid_argument &e)
	{
		return false;
	}
	// Se o inteiro da linha exceder o limite do tipo int
	catch (out_of_range &e)
	{
		return false;
	}
	return true;
}

// Função para converter uma string para um número inteiro
int castStringDado(string linha)
{
	bool castPossivel = tentarCast(linha);
	if (castPossivel)
		return stoi(linha);
	else
		return -1;
}

// Função para converter uma string contendo vários números em um vetor de inteiros
vector<int> castStringVetor(string linha)
{
	vector<int> vetor;
	string temporaria = "";
	bool castPossivel;

	// Percorre cada caracter da string
	for (int i = 0; i < linha.size(); i++)
	{
		if (linha[i] == ' ')
		{
			// Quando encontrar um espaço em branco, tenta fazer o cast da string temporária para int
			castPossivel = tentarCast(temporaria);
			if (castPossivel)
			{
				// Se for possível fazer o cast, adiciona o número ao vetor
				vetor.push_back(stoi(temporaria));
				temporaria = "";
			}
		}
		else if (linha[i] != '\n')
		{
			// Se não for espaço em branco nem quebra de linha, adiciona o caracter à string temporária
			temporaria = temporaria + linha[i];
		}
	}
	// Adiciona o último número da string temporária ao vetor
	vetor.push_back(stoi(temporaria));

	return vetor;
}

// Função que lê o arquivo e retorna as informações em uma tupla
tuple<int, int, vector<int>, vector<vector<int>>> definirInformacoesArquivo(string localArquivo)
{
	ifstream arquivo(localArquivo);

	int numeroLinhas, numeroProdutos;
	vector<int> vetorTempoProducao;
	vector<int> vetorGeral;
	vector<int> vetorTemporario;
	vector<vector<int>> matriz;

	if (arquivo.is_open())
	{
		cout << "Arquivo aberto!" << endl;
		cout << "=============== \n"
			 << endl;

		int contadorLinhas = 0;

		string linha;
		while (getline(arquivo, linha))
		{
			switch (contadorLinhas)
			{
			case 0:
				// Linha 1 do arquivo contém o número de linhas que serão lidas (não utilizada)
				numeroLinhas = castStringDado(linha);
				break;
			case 1:
				// Linha 2 do arquivo contém o número de produtos
				numeroProdutos = castStringDado(linha);
				break;
			case 2:
				// Linha 3 do arquivo não é utilizada
				break;
			case 3:
				// Linha 4 contém o vetor de tempo de produção de cada produto
				vetorTempoProducao = castStringVetor(linha);
				break;
			case 4:
				// Linha 5 do arquivo não é utilizada
				break;
			default:
				// Linhas restantes contêm a matriz de tempo de limpeza entre a produção de um produtos e os outros
				vetorTemporario = castStringVetor(linha);
				vetorGeral.insert(vetorGeral.end(), vetorTemporario.begin(), vetorTemporario.end());
				break;
			}
			contadorLinhas += 1;
		}

		// Preenchendo a matriz com os valores lidos
		matriz.resize(numeroProdutos, vector<int>(numeroProdutos));
		for (int i = 0; i < numeroProdutos; i++)
			for (int j = 0; j < numeroProdutos; j++)
				matriz[i][j] = vetorGeral[j + i * numeroProdutos];
	}
	else
	{
		cout << "Falha ao abrir arquivo!";
		arquivo.close();
		exit(0);
	}

	arquivo.close();

	return make_tuple(numeroLinhas, numeroProdutos, vetorTempoProducao, matriz);
}

