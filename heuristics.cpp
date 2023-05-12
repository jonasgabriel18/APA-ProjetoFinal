#include "f_heuristics.h"

using namespace std;

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

vector<int> solution_time(vector<vector<int>> solution, vector<vector<int>> maintance, vector<int> products) {
	vector<int> curr_line_times;

	for(int i = 0; i < solution.size(); i++) {
		int curr_time = 0;
		for(int j = 0; j < solution[i].size(); j++) {
			int curr_product_index = solution[i][j];
			int curr_product = products[curr_product_index];
			curr_time += curr_product;
			if(j != solution[i].size()-1) {
				int next_product_index = solution[i][j+1];
				int next_product = products[next_product_index];
				curr_time += maintance[curr_product][next_product];
			}
		}
		curr_line_times.push_back(curr_time);
	}

	return curr_line_times;
}

vector<vector<int>> swap_products_between_lines(vector<vector<int>> solution, vector<vector<int>> maintance, vector<int> products, vector<int> solution_times) {
    vector<vector<int>> best_solution(solution);
    int best_solution_time = *max_element(solution_times.begin(), solution_times.end());

    for(int i = 0; i < solution.size(); i++) {
        vector<int> curr_solution_times(solution_times);
        
        for(int j = 0; j < solution[i].size(); j++) {

            for (int k = i+1; k < solution.size(); k++) {
                int curr_product = solution[i][j];
                
                for(int l = 0; l < solution[k].size(); l++) {
                    vector<vector<int>> curr_solution(solution);
                    int substitute = solution[k][l];
                    curr_solution[i][j] = substitute;
                    curr_solution[k][l] = curr_product;

                    //Calcular impacto
                    curr_solution_times[i] -= products[curr_product] + products[substitute];
                    if(i != solution.size()-1) {
                        int next_product = solution[i+1][j+1];
                        curr_solution_times[i] += maintance[substitute][next_product];
                    }
                    
                    curr_solution_times[k] -= products[substitute] + products[curr_product] ;
                    if(k != solution.size()-1) {
                        int next_product = solution[k+1][l+1];
                        curr_solution_times[k] += maintance[curr_product][next_product];
                    }
                    
                    int curr_solution_value = *max_element(curr_solution_times.begin(), curr_solution_times.end());
                    if (curr_solution_value < best_solution_time) {
                        best_solution_time = curr_solution_value;
                        best_solution = curr_solution;
                    }
                }
            }
        }
    }

    return best_solution;
}