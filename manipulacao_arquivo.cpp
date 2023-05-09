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

// Função que encontra o índice do menor elemento no vetor não utilizado
// Recebe um vetor de inteiros e um vetor com os índices já utilizados
int encontrarMenor(const vector<int> &vetor, const vector<int> &indicesUtilizados)
{
	// Verifica se o tamanho do vetor de índices é diferente do tamanho do vetor original
	if (vetor.size() != indicesUtilizados.size())
	{
		// Inicializa o índice do menor tempo como 0
		int indiceMenorTempo = 0;

		// Percorre o vetor de inteiros a partir do índice 1
		for (int i = 1; i < vetor.size(); i++)
		
			// Verifica se o índice atual já foi utilizado
			if (find(indicesUtilizados.begin(), indicesUtilizados.end(), i) == indicesUtilizados.end())
			
				// Verifica se o elemento atual é menor que o elemento do índice do menor tempo
				if (vetor[i] < vetor[indiceMenorTempo])
					indiceMenorTempo = i;
			
		// Retorna o índice do menor tempo encontrado
		return indiceMenorTempo;
	}

	// Se o tamanho do vetor de índices for igual ao tamanho do vetor original, retorna -1 indicando erro
	return -1;
}

// Função que irá criar uma solução gulosa para o problema
vector<vector<int>> gerarSolucaoGulosa(int n, int m, vector<int> vetor, vector<vector<int>> matriz)
{

	// * Criação de vetores e matrizes para armazenar as informações do problema

	// Vetor para armazenar os produtos já utilizados
	vector<int> indicesUsados;

	// Vetor para armazenar o tempo total de cada linha de produção
	vector<int> tempoTotal(m, 0);

	// Matriz para armazenar a sequência de produtos de cada linha de produção
	vector<vector<int>> linhasProducao(m, vector<int>());

	// * Variáveis para controle do tempo de produção e da linha que será adicionada o produto atual
	int menorTempo = 0;
	int linhaAdicionada = 0;
	int produtoAtual;
	int tempoResultante;

	// Encontra o primeiro produto a ser utilizado e insere as informações referentes à ele nas variáveis e estruturas criadas anteriormente
	produtoAtual = encontrarMenor(vetor, indicesUsados);
	indicesUsados.push_back(produtoAtual);
	linhasProducao[0].push_back(produtoAtual);
	tempoTotal[0] = vetor[produtoAtual];

	// Loop para inserir os próximos produtos nas linhas de produção
	for (int i = 1; i < n; i++)
	{
		produtoAtual = encontrarMenor(vetor, indicesUsados);

		// Loop para percorrer todas as linhas de produção e verificar em qual delas o produto atual deve ser adicionado
		for (int l = 0; l < m; l++)
		{
			int tempoLinhaAtual = tempoTotal[l];

			// Se a linha de produção já possuir pelo menos um produto, calcula o tempo de produção resultante da inserção do produto atual nesta linha
			if (linhasProducao[l].size() > 0)
			{
				int ultimoProduto = linhasProducao[l].back();
				tempoResultante = tempoLinhaAtual + matriz[ultimoProduto][produtoAtual] + vetor[produtoAtual];
			}
			else
				tempoResultante = vetor[produtoAtual];

			// Se a linha de produção for a primeira a ser avaliada, armazena o tempo resultante e a linha escolhida
			if (l == 0)
			{
				menorTempo = tempoResultante;
				linhaAdicionada = 0;
			}

			// Se o tempo resultante for menor que o menor tempo encontrado até agora, atualiza as variáveis de controle
			if (tempoResultante < menorTempo)
			{
				menorTempo = tempoResultante;
				linhaAdicionada = l;
			}
		}

		// Insere o produto atual na linha de produção escolhida e atualiza as informações de controle
		tempoTotal[linhaAdicionada] = menorTempo;
		linhasProducao[linhaAdicionada].push_back(produtoAtual);
		indicesUsados.push_back(produtoAtual);
	}

	// Retorna a matriz contendo as sequências de produtos de cada linha de produção
	return linhasProducao;
}
