#include "f_heuristics.h"

using namespace std;

// * Algoritmo Guloso - Comentado

// Esta função auxiliar encontra o índice do menor elemento no vetor 'vetorProdutos' que não foi utilizado ainda (não está presente em 'indicesUtilizados')
int encontrarMenor(const vector<int> &vetorProduto, const vector<int> &indicesUtilizados)
{
    // Verifica se o tamanho do vetorProduto de índices é diferente do tamanho do vetor original
    if (vetorProduto.size() != indicesUtilizados.size())
    {
        // Inicializa o índice do menor tempo como -1
        int indiceMenorTempo = -1;
        int contador = 0;

        // Percorre o vetor de inteiros usando um loop for-range
        for (const auto &elemento : vetorProduto)
        {
            // Verifica se o índice atual não foi utilizado
            if (find(indicesUtilizados.begin(), indicesUtilizados.end(), contador) == indicesUtilizados.end())
            {
                // Verifica se é o primeiro elemento não utilizado ou se o elemento atual é menor que o menor tempo encontrado até agora
                if (indiceMenorTempo == -1 || elemento < vetorProduto[indiceMenorTempo])
                {
                    // Atualiza o índice do menor tempo
                    indiceMenorTempo = contador;
                }
            }
            contador++;
        }

        // Retorna o índice do menor tempo encontrado
        return indiceMenorTempo;
    }

    // Se o tamanho do vetor de índices for igual ao tamanho do vetor original, retorna -1 indicando erro
    return -1;
}

// Esta função gera uma solução gulosa para o problema de programação de produção
vector<vector<int>> gerarSolucaoGulosa(int n, int m, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos)
{
    // * Criação de vetores e matrizes para armazenar as informações do problema

    vector<int> indicesUsados;

    vector<int> tempoTotal(m, 0);

    vector<vector<int>> solucao(m, vector<int>());

    // * Variáveis para controle do tempo de produção e da linha que será adicionada o produto atual
    int menorTempo = 0;
    int linhaAdicionada = 0;
    int produtoAtual;
    int tempoResultante;

    // Encontra o produto de menor tempo de produção inicialmente e o adiciona à primeira linha da solução
    produtoAtual = encontrarMenor(vetorProdutos, indicesUsados);
    indicesUsados.push_back(produtoAtual);
    solucao[0].push_back(produtoAtual);
    tempoTotal[0] = vetorProdutos[produtoAtual];

    // Para cada produto restante...
    for (int i = 1; i < n; i++)
    {
        // Encontra o produto de menor tempo de produção que ainda não foi utilizado
        produtoAtual = encontrarMenor(vetorProdutos, indicesUsados);

        // Para cada linha de produção...
        for (int l = 0; l < m; l++)
        {
            int tempoLinhaAtual = tempoTotal[l];

            // Se a linha atual já possui produtos...
            if (solucao[l].size() > 0)
            {
                int ultimoProduto = solucao[l].back();
                tempoResultante = tempoLinhaAtual + matrizPreparacao[ultimoProduto][produtoAtual] + vetorProdutos[produtoAtual];
            }
            else
                tempoResultante = vetorProdutos[produtoAtual];

            // Se a linha atual é a primeira, atualiza o menor tempo e a linha onde o produto será adicionado
            if (l == 0)
            {
                menorTempo = tempoResultante;
                linhaAdicionada = 0;
            }

            // Se o tempo resultante for menor que o menor tempo atual, atualiza o menor tempo e a linha onde o produto será adicionado
            if (tempoResultante < menorTempo)
            {
                menorTempo = tempoResultante;
                linhaAdicionada = l;
            }
        }

        // Atualiza o tempo total da linha escolhida, adiciona o produto à linha e marca o produto como utilizado
        tempoTotal[linhaAdicionada] = menorTempo;
        solucao[linhaAdicionada].push_back(produtoAtual);
        indicesUsados.push_back(produtoAtual);
    }

    return solucao;
}

// * Construção da Solução - Comentado

// Esta função calcula os tempos de produção para cada linha de produção na solução
vector<int> temposProducao(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos)
{
    vector<int> temposLinhaAtual;

    // Para cada linha de produção na solução...
    for (int i = 0; i < solucao.size(); i++)
    {
        int tempoAtual = 0;
        // Para cada produto na linha de produção...
        for (int j = 0; j < solucao[i].size(); j++)
        {
            int indiceProdutoAtual = solucao[i][j];
            int produtoAtual = vetorProdutos[indiceProdutoAtual];
            tempoAtual += produtoAtual;
            // Se não for o último produto na linha, adiciona o tempo de preparação entre o produto atual e o próximo produto
            if (j != solucao[i].size() - 1)
            {
                int indiceProximoProduto = solucao[i][j + 1];
                int proximoProduto = vetorProdutos[indiceProximoProduto];
                tempoAtual += matrizPreparacao[indiceProdutoAtual][indiceProximoProduto];
            }
        }
        temposLinhaAtual.push_back(tempoAtual);
    }

    return temposLinhaAtual;
}

// * Busca Local

vector<vector<int>> movimentoInverter(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao)
{
    int melhorCusto = *max_element(temposSolucao.begin(), temposSolucao.end());
    vector<vector<int>> melhorSolucao = solucao;
    vector<vector<int>> solucaoCopia = solucao;

    auto iteradorMaiorCusto = find(temposSolucao.begin(), temposSolucao.end(), melhorCusto);
    int linhaMaiorCusto = distance(temposSolucao.begin(), iteradorMaiorCusto);

    // Invertendo a ordem dos produtos na linha atual
    for (int j = 0; j < solucaoCopia[linhaMaiorCusto].size() / 2; j++)
    {
        int temp = solucaoCopia[linhaMaiorCusto][j];
        solucaoCopia[linhaMaiorCusto][j] = solucaoCopia[linhaMaiorCusto][solucaoCopia[linhaMaiorCusto].size() - 1 - j];
        solucaoCopia[linhaMaiorCusto][solucaoCopia[linhaMaiorCusto].size() - 1 - j] = temp;
    }

    // Avalia a nova configuração das linhas de produção
    vector<int> temposPossiveis = temposProducao(solucaoCopia, matrizPreparacao, vetorProdutos);
    int possivelMaiorTempo = *max_element(temposPossiveis.begin(), temposPossiveis.end());

    // Atualiza a melhor configuração das linhas de produção
    if (possivelMaiorTempo < melhorCusto)
    {
        melhorCusto = possivelMaiorTempo;
        melhorSolucao = solucaoCopia;
    }

    return melhorSolucao;
}

// | Horizontal

int calculoCustoNovoLinha(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &temposSolucao, int linha, int indexProdAtual, int indexProdSubs)
{

    int custo = temposSolucao[linha];

    int produtoAtual = solucao[linha][indexProdAtual];
    int produtoSubs = solucao[linha][indexProdSubs];

    int ant_prodAtual = solucao[linha][indexProdAtual - 1];
    int prox_prodAtual = solucao[linha][indexProdAtual + 1];

    int ant_prodSubs = solucao[linha][indexProdSubs - 1];
    int prox_prodSubs = solucao[linha][indexProdSubs + 1];

    // Se o produtoAtual for o 1° da linha
    if (indexProdAtual == 0)
    {

        // Conexão do produtoAtual com o próxmio retirada
        custo -= matrizPreparacao[produtoAtual][prox_prodAtual];

        // Se o produto seguinte ao produtoAtual for o produtoSubs
        if (indexProdSubs == indexProdAtual + 1 && indexProdSubs != solucao[linha].size() - 1)
        {
            // Conexão do produtoSubs com o próximo retirada
            custo -= matrizPreparacao[produtoSubs][prox_prodSubs];

            // Conexão do produtoAtual com o próximo do produtoSubs adicionada
            custo += matrizPreparacao[produtoAtual][prox_prodSubs];

            // Conexão do produtoSubs com o produtoAtual adicionada
            custo += matrizPreparacao[produtoSubs][produtoAtual];
        }

        // Se o produtoSubs estiver entre 2° e o último
        else if (indexProdSubs > indexProdAtual + 1 && indexProdSubs < solucao[linha].size() - 1)
        {
            // Conexão do produto anterior ao produtoSubs com o produtoSubs retirada
            custo -= matrizPreparacao[ant_prodSubs][produtoSubs];

            // Conexão do produtoSubs com o próximo retirada
            custo -= matrizPreparacao[produtoSubs][prox_prodSubs];

            // Conexão do produto anterior ao produtoSubs com o produtoAtual adicionada
            custo += matrizPreparacao[ant_prodSubs][produtoAtual];

            // Conexão do produtoAtual com o próximo do produtoSubs adicionada
            custo += matrizPreparacao[produtoAtual][prox_prodSubs];

            // Conexão do produtoSubs com o antigo anterior do produtoSubs adicionada
            custo += matrizPreparacao[produtoSubs][prox_prodAtual];
        }

        // Se o produtoSubs for o último da linha e não é o próximo do produtoAtual
        else if (indexProdSubs != indexProdAtual + 1 && indexProdSubs == solucao[linha].size() - 1)
        {
            // Conexão do produtoSubs com o anterior retirada
            custo -= matrizPreparacao[ant_prodSubs][produtoSubs];

            // Conexão do produtoAtual com o anterior ao produtoSubs adicionada
            custo += matrizPreparacao[ant_prodSubs][produtoAtual];

            // Conexão do produtoSubs com o próximo do produtoAtual adicionada
            custo += matrizPreparacao[produtoSubs][prox_prodAtual];
        }

        // Se o produtoSubs for o último da linha e é o próximo do produtoAtual
        else if (indexProdSubs == indexProdAtual + 1 && indexProdSubs == solucao[linha].size() - 1)
        {
            // Conexão do produtoSubs com o produtoAtual adicionada
            custo += matrizPreparacao[produtoSubs][produtoAtual];
        }
    }

    // Se o produtoAtual for o penúltimo da linha
    else if (indexProdAtual == solucao[linha].size() - 2)
    {
        // Conexão do produto anterior ao produtoAtual com o produtoAtual retirada
        custo -= matrizPreparacao[ant_prodAtual][produtoAtual];

        // Conexão do produtoAtual com o produtoSubs retirada
        custo -= matrizPreparacao[produtoAtual][produtoSubs];

        // Conexão do produto anterior ao produtoAtual com o produtoSubs adicionada
        custo += matrizPreparacao[ant_prodAtual][produtoSubs];

        // Conexão do produtoSubs com o produtoAtual adicionada
        custo += matrizPreparacao[produtoSubs][produtoAtual];
    }

    // Se o produtoAtual estiver entre o 1° e o penúltimo da linha
    else if (indexProdAtual > 0 && indexProdAtual < solucao[linha].size() - 2)
    {

        // Conexão do produto anterior ao produtoAtual com o produtoAtual retirada
        custo -= matrizPreparacao[ant_prodAtual][produtoAtual];

        // Conexão do produtoAtual com o próximo retirada
        custo -= matrizPreparacao[produtoAtual][prox_prodAtual];

        // Se o produtoSubs for o próximo do produtoAtual
        if (indexProdSubs == indexProdAtual + 1 && indexProdSubs != solucao[linha].size() - 1)
        {
            // Conexão do produtoSubs com o próximo retirada
            custo -= matrizPreparacao[produtoSubs][prox_prodSubs];

            // Conexão do produto anterior ao produtoAtual com o produtoSubs adicionada
            custo += matrizPreparacao[ant_prodAtual][produtoSubs];

            // Conexão do produtoSubs com o produtoAtual adicionada
            custo += matrizPreparacao[produtoSubs][produtoAtual];

            // Conexão do produtoAtual com o próximo do produtoSubs
            custo += matrizPreparacao[produtoAtual][prox_prodSubs];
        }

        // Se o produtoSubs estiver entre o 1° após o produtoAtual e o último da linha
        else if (indexProdSubs > indexProdAtual + 1 && indexProdSubs != solucao[linha].size() - 1)
        {
            // Conexão do produto anterior ao produtoSubs com o produtoSubs retirada
            custo -= matrizPreparacao[ant_prodSubs][produtoSubs];

            // Conexão do produtoSubs com o próximo retirada
            custo -= matrizPreparacao[produtoSubs][prox_prodSubs];

            // Conexão do produto anterior ao produtoAtual com o produtoSubs adicionada
            custo += matrizPreparacao[ant_prodAtual][produtoSubs];

            // Conexão do produtoSubs com o produto seguinte ao produtoAtual adicionada
            custo += matrizPreparacao[produtoSubs][prox_prodAtual];

            // Conexão do produto anterior ao produtoSubs com o produtoAtual adicionada
            custo += matrizPreparacao[ant_prodSubs][produtoAtual];

            // Conexão do produtoAtual com o produto seguinte do produtoSubs adicionada
            custo += matrizPreparacao[produtoAtual][prox_prodSubs];
        }

        // Se o produtoSubs for, pelo menos, o 2° depois do produtoAtual e é o último da linha
        else if (indexProdSubs > indexProdAtual + 1 && indexProdSubs == solucao[linha].size() - 1)
        {
            // Conexão do produto anterior ao produtoSubs com o produtoSubs retirada
            custo -= matrizPreparacao[ant_prodSubs][produtoSubs];

            // Conexão do produto anterior ao produtoAtual com o produtoSubs adicionada
            custo += matrizPreparacao[ant_prodAtual][produtoSubs];

            // Conexão do produtoSubs com o produto seguinte ao produtoAtual adicionada
            custo += matrizPreparacao[produtoSubs][prox_prodAtual];

            // Conexão do produto anterior ao produtoSubs com o produtoAtual adicionada
            custo += matrizPreparacao[ant_prodSubs][produtoAtual];
        }
    }

    return custo;
}

vector<int> buscaMelhorCusto(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &temposSolucao)
{

    int melhorCusto = *max_element(temposSolucao.begin(), temposSolucao.end());

    auto iteradorMaiorCusto = find(temposSolucao.begin(), temposSolucao.end(), melhorCusto);
    int linhaMaiorCusto = distance(temposSolucao.begin(), iteradorMaiorCusto);

    vector<int> trocaParaFazer(3, 0); // linha que vai ser modificada, index do primeiro prod, index do prod subs

    for (int j = 0; j < solucao[linhaMaiorCusto].size() - 1; j++)
    {
        for (int k = j + 1; k < solucao[linhaMaiorCusto].size(); k++)
        {
            vector<int> temposSolucaoAtual = temposSolucao;

            temposSolucaoAtual[linhaMaiorCusto] = calculoCustoNovoLinha(solucao, matrizPreparacao, temposSolucao, linhaMaiorCusto, j, k);

            int custoAtual = *max_element(temposSolucaoAtual.begin(), temposSolucaoAtual.end());

            if (custoAtual < melhorCusto)
            {
                melhorCusto = custoAtual;

                trocaParaFazer[0] = linhaMaiorCusto;
                trocaParaFazer[1] = j;
                trocaParaFazer[2] = k;
            }
        }
    }

    return trocaParaFazer;
}

vector<vector<int>> novaSolucaoMesmaLinha(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &temposSolucao)
{
    vector<int> indicesTroca = buscaMelhorCusto(solucao, matrizPreparacao, temposSolucao);
    int linha = indicesTroca[0];
    int indicePrimeiroProd = indicesTroca[1];
    int indiceSubsProd = indicesTroca[2];

    vector<vector<int>> novaSolucao = solucao;

    iter_swap(novaSolucao[linha].begin() + indicePrimeiroProd, novaSolucao[linha].begin() + indiceSubsProd);

    return novaSolucao;
}

// | Vertical

vector<int> calculoCustoNovoEntreLinhas(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao, int linhaAtual, int linhaSubs, int indexProdAtual, int indexProdSubs)
{
    vector<int> custos = temposSolucao;

    int produtoAtual = solucao[linhaAtual][indexProdAtual];
    int produtoSubs = solucao[linhaSubs][indexProdSubs];

    int ant_prodAtual = solucao[linhaAtual][indexProdAtual - 1];
    int prox_prodAtual = solucao[linhaAtual][indexProdAtual + 1];

    int ant_prodSubs = solucao[linhaSubs][indexProdSubs - 1];
    int prox_prodSubs = solucao[linhaSubs][indexProdSubs + 1];

    custos[linhaAtual] -= vetorProdutos[produtoAtual];
    custos[linhaAtual] += vetorProdutos[produtoSubs];

    custos[linhaSubs] -= vetorProdutos[produtoSubs];
    custos[linhaSubs] += vetorProdutos[produtoAtual];

    // Se o produtoAtual é o 1° da linha
    if (indexProdAtual == 0)
    {
        // Conexão do produtoAtual com o produto seguinte retirada
        custos[linhaAtual] -= matrizPreparacao[produtoAtual][prox_prodAtual];

        // Conexão do produtoSubs com o produto seguinte ao produtoAtual adicionada
        custos[linhaAtual] += matrizPreparacao[produtoSubs][prox_prodAtual];
    }

    // Se o produtoAtual é o último da linha
    else if (indexProdAtual == solucao[linhaAtual].size() - 1)
    {
        // Conexão do produto anterior ao produtoAtual com o produtoAtual retirada
        custos[linhaAtual] -= matrizPreparacao[ant_prodAtual][produtoAtual];

        // Conexão do produto anterior ao produtoAtual com o produtoSubs adicionada
        custos[linhaAtual] += matrizPreparacao[ant_prodAtual][produtoSubs];
    }

    // Se o produtoAtual está entre o 1° e o último da linha
    else
    {
        // Conexão do produto anterior ao produtoAtual com o produtoAtual retirada
        custos[linhaAtual] -= matrizPreparacao[ant_prodAtual][produtoAtual];

        // Conexão do produtoAtual com o produto seguinte retirada
        custos[linhaAtual] -= matrizPreparacao[produtoAtual][prox_prodAtual];

        // Conexão do produto anterior ao produtoAtual com o produtoSubs adicionada
        custos[linhaAtual] += matrizPreparacao[ant_prodAtual][produtoSubs];

        // Conexão do produtoSubs com o produto seguinte ao produtoAtual adicionada
        custos[linhaAtual] += matrizPreparacao[produtoSubs][prox_prodAtual];
    }

    // Se o produtoSubs for o 1° da sua linha
    if (indexProdSubs == 0)
    {
        // Conexão do produtoSubs com o produto seguinte retirada
        custos[linhaSubs] -= matrizPreparacao[produtoSubs][prox_prodSubs];

        // Conexão do produtoAtual com o produto seguinte ao produtoSubs adicionada
        custos[linhaSubs] += matrizPreparacao[produtoAtual][prox_prodSubs];
    }

    // Se o produtoSubs for o último da sua linha
    else if (indexProdSubs == solucao[linhaSubs].size() - 1)
    {
        // Conexão do produto anterior ao produtoSubs com o produtoSubs retirada
        custos[linhaSubs] -= matrizPreparacao[ant_prodSubs][produtoSubs];

        // Conexão do produto anterior ao produtoSubs com o produtoAtual adicionada
        custos[linhaSubs] += matrizPreparacao[ant_prodSubs][produtoAtual];
    }

    // Se o produtoSubs está entre o 1° e o último de sua linha
    else
    {
        // Conexão do produto anterior ao produtoSubs com o produtoSubs retirada
        custos[linhaSubs] -= matrizPreparacao[ant_prodSubs][produtoSubs];

        // Conexão do produtoSubs com o produto seguinte retirada
        custos[linhaSubs] -= matrizPreparacao[produtoSubs][prox_prodSubs];

        // Conexão do prouto anterior ao produtoSubs com o produtoAtual adicionada
        custos[linhaSubs] += matrizPreparacao[ant_prodSubs][produtoAtual];

        // Conexão do produtoAtual com o produto seguinte ao produtoSubs adicionada
        custos[linhaSubs] += matrizPreparacao[produtoAtual][prox_prodSubs];
    }

    return custos;
}

vector<int> buscaMelhorCustoEntreLinhas(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao)
{
    int melhorCusto = *max_element(temposSolucao.begin(), temposSolucao.end());

    auto iteradorMaiorCusto = find(temposSolucao.begin(), temposSolucao.end(), melhorCusto);
    int linhaMaiorCusto = distance(temposSolucao.begin(), iteradorMaiorCusto);

    vector<int> trocaParaFazer{0, 0, 0, 0}; // primeiro linha, linha subs, primeiro prod, prod subs

    for (int j = 0; j < solucao[linhaMaiorCusto].size(); j++)
    {
        for (int k = 0; k < solucao.size(); k++)
        {
            if (k == linhaMaiorCusto)
            {
                continue;
            }

            for (int l = 0; l < solucao[k].size(); l++)
            {
                vector<int> temposSolucaoAtual = temposSolucao;
                temposSolucaoAtual = calculoCustoNovoEntreLinhas(solucao, matrizPreparacao, vetorProdutos, temposSolucao, linhaMaiorCusto, k, j, l);

                int custoAtual = *max_element(temposSolucaoAtual.begin(), temposSolucaoAtual.end());

                if (custoAtual < melhorCusto)
                {
                    melhorCusto = custoAtual;
                    trocaParaFazer[0] = linhaMaiorCusto;
                    trocaParaFazer[1] = k;
                    trocaParaFazer[2] = j;
                    trocaParaFazer[3] = l;
                }
            }
        }
    }

    return trocaParaFazer;
}

vector<vector<int>> novaSolucaoEntreLinhas(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao)
{
    vector<int> indicesTroca = buscaMelhorCustoEntreLinhas(solucao, matrizPreparacao, vetorProdutos, temposSolucao);
    int linha1 = indicesTroca[0];
    int linha2 = indicesTroca[1];
    int indicePrimeiroProd = indicesTroca[2];
    int indiceSubsProd = indicesTroca[3];

    vector<vector<int>> novaSolucao = solucao;
    swap(novaSolucao[linha1][indicePrimeiroProd], novaSolucao[linha2][indiceSubsProd]);

    return novaSolucao;
}

// | Inserir

vector<int> calculoCustoNovoReInsertion(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao, int linhaAtual, int linhaSubs, int indexProdAtual, int indexProdSubs)
{
    vector<int> custos = temposSolucao;

    int produtoAtual = solucao[linhaAtual][indexProdAtual];

    int ant_prodAtual = solucao[linhaAtual][indexProdAtual - 1];
    int prox_prodAtual = solucao[linhaAtual][indexProdAtual + 1];

    int produtoSeguinte = solucao[linhaSubs][indexProdSubs];
    int produtoAnterior = solucao[linhaSubs][indexProdSubs - 1];
    if (indexProdSubs == solucao[linhaSubs].size())
    {
        produtoSeguinte = solucao[linhaSubs][indexProdSubs - 1];
        produtoAnterior = solucao[linhaSubs][indexProdSubs - 2];
    }

    custos[linhaAtual] -= vetorProdutos[produtoAtual];
    custos[linhaSubs] += vetorProdutos[produtoAtual];

    // Se o produtoAtual é o 1° da sua linha
    if (indexProdAtual == 0)
    {
        // Conexão do produtoAtual com o produto seguinte retirada
        custos[linhaAtual] -= matrizPreparacao[produtoAtual][prox_prodAtual];
    }

    // Se o produtoAtual é o último da sua linha
    else if (indexProdAtual == solucao[linhaAtual].size() - 1)
    {
        // Conexão do produto anterior ao produtoAtual com o produtoAtual retirada
        custos[linhaAtual] -= matrizPreparacao[ant_prodAtual][produtoAtual];
    }

    // Se o produtoAtual está entre o 1° e o último da sua linha
    else if (indexProdAtual > 0 && indexProdAtual < solucao[linhaAtual].size() - 1)
    {
        // Conexão do produto anterior ao produtoAtual com o produtoAtual retirada
        custos[linhaAtual] -= matrizPreparacao[ant_prodAtual][produtoAtual];

        // Conexão do produtoAtual com o produto seguinte retirada
        custos[linhaAtual] -= matrizPreparacao[produtoAtual][prox_prodAtual];

        // Conexão do produto anterior ao produtoAtual com o produto seguinte adicionada
        custos[linhaAtual] += matrizPreparacao[ant_prodAtual][prox_prodAtual];
    }

    // Se o produtoAtual será inserido no início da linha
    if (indexProdSubs == 0)
    {
        // Conexão do produtoAtual com 1° produto da linha adicionada
        custos[linhaSubs] += matrizPreparacao[produtoAtual][produtoSeguinte];
    }

    // Se o produtoAtual será inserido entre o penúltimo e o último da linha
    else if (indexProdSubs == solucao[linhaSubs].size() - 1)
    {
        int ultimoProduto = solucao[linhaSubs][solucao[linhaSubs].size() - 1];
        int penultimoProduto = solucao[linhaSubs][solucao[linhaSubs].size() - 2];

        // Conexão do penúltimo com o último produto da linha retirada
        custos[linhaSubs] -= matrizPreparacao[penultimoProduto][ultimoProduto];

        // Conexão do penúltimo produto com o produtoAtual adicionada
        custos[linhaSubs] += matrizPreparacao[penultimoProduto][produtoAtual];

        // Conexão do produtoAtual com o último adicionada
        custos[linhaSubs] += matrizPreparacao[produtoAtual][ultimoProduto];
    }

    // Se o produtoAtual será inserido entre o 1° e o último
    else if (indexProdSubs > 0 && indexProdSubs < solucao[linhaSubs].size() - 1)
    {
        // Conexão do produtoAnterior com o produtoSeguinte retirada
        custos[linhaSubs] -= matrizPreparacao[produtoAnterior][produtoSeguinte];

        // Conexão do produtoAnterior com o produtoAtual adicionada
        custos[linhaSubs] += matrizPreparacao[produtoAnterior][produtoAtual];

        // Conexão do produtoAtual com o produtoSeguinte adicionada
        custos[linhaSubs] += matrizPreparacao[produtoAtual][produtoSeguinte];
    }

    // Se o produtoAtual será inserido no final da linha
    else if (indexProdSubs == solucao[linhaSubs].size())
    {
        // Conexão do último produto da linha com produtoAtual adicionada
        custos[linhaSubs] += matrizPreparacao[produtoSeguinte][produtoAtual];
    }

    return custos;
}

vector<int> buscaMelhorCustoReInsertion(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao)
{
    int melhorCusto = *max_element(temposSolucao.begin(), temposSolucao.end());

    auto iteradorMaiorCusto = find(temposSolucao.begin(), temposSolucao.end(), melhorCusto);
    int linhaMaiorCusto = distance(temposSolucao.begin(), iteradorMaiorCusto);

    vector<int> trocaParaFazer{0, 0, 0, 0}; // primeiro linha, linha subs, primeiro prod, prod subs

    for (int j = 0; j < solucao[linhaMaiorCusto].size(); j++)
    {
        for (int k = linhaMaiorCusto + 1; k < solucao.size(); k++)
        {
            for (int l = 0; l < solucao[k].size() + 1; l++)
            {
                vector<int> temposSolucaoAtual = temposSolucao;
                temposSolucaoAtual = calculoCustoNovoReInsertion(solucao, matrizPreparacao, vetorProdutos, temposSolucao, linhaMaiorCusto, k, j, l);

                int custoAtual = *max_element(temposSolucaoAtual.begin(), temposSolucaoAtual.end());

                if (custoAtual < melhorCusto)
                {
                    melhorCusto = custoAtual;
                    trocaParaFazer[0] = linhaMaiorCusto;
                    trocaParaFazer[1] = k;
                    trocaParaFazer[2] = j;
                    trocaParaFazer[3] = l;
                }
            }
        }
    }

    return trocaParaFazer;
}

vector<vector<int>> novaSolucaoReInsertion(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao)
{
    vector<vector<int>> novaSolucao = solucao;

    vector<int> indicesTroca = buscaMelhorCustoReInsertion(novaSolucao, matrizPreparacao, vetorProdutos, temposSolucao);
    int linha1 = indicesTroca[0];
    int linha2 = indicesTroca[1];
    int indicePrimeiroProd = indicesTroca[2];
    int indiceSubsProd = indicesTroca[3];

    if (linha2 != 0)
    {
        int prod = novaSolucao[linha1][indicePrimeiroProd];
        novaSolucao[linha1].erase(novaSolucao[linha1].begin() + indicePrimeiroProd);
        novaSolucao[linha2].insert(novaSolucao[linha2].begin() + indiceSubsProd, prod);
    }

    return novaSolucao;
}

// * VND - Comentado

vector<vector<int>> melhorarLinhas(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao)
{

    // Inicialização do tempo atual com o maior tempo da solução atual
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());

    // Inicialização da melhor solução com a solução atual
    vector<vector<int>> melhorSolucao = solucao;
    // Declaração de variáveis de solução temporárias
    vector<vector<int>> possivelMelhorSolucao;
    vector<vector<int>> solucaoAtual = solucao;

    vector<int> temposAtuais = temposSolucao;
    vector<int> temposAnteriores = temposAtuais;

    // Controle de casos e
    int caso = 0;

    while (1)
    {
        switch (caso)
        {
        case 0:
            // Chama a função de movimento de inserção
            possivelMelhorSolucao = novaSolucaoReInsertion(solucaoAtual, matrizPreparacao, vetorProdutos, temposAtuais);
            break;
        case 1:
            // Chama a função de movimento horizontal
            possivelMelhorSolucao = novaSolucaoMesmaLinha(solucaoAtual, matrizPreparacao, temposAtuais);
            break;
        case 2:
            // Chama a função de movimento vertical
            possivelMelhorSolucao = novaSolucaoEntreLinhas(solucaoAtual, matrizPreparacao, vetorProdutos, temposAtuais);
            break;
        case 3:
            // Chama a função de movimento de inverter
            possivelMelhorSolucao = movimentoInverter(solucaoAtual, matrizPreparacao, vetorProdutos, temposAtuais);
            break;
        }

        // Avalia os tempos de produção da nova solução
        temposAtuais = temposProducao(possivelMelhorSolucao, matrizPreparacao, vetorProdutos);

        // Encontra o maior tempo de produção possível na nova solução
        int possivelMaiorTempo = *max_element(temposAtuais.begin(), temposAtuais.end());

        if (possivelMaiorTempo < tempoAtual)
        {
            // Atualiza o tempo atual com o novo maior tempo de produção
            tempoAtual = possivelMaiorTempo;
            // Atualiza a melhor solução com a nova solução encontrada
            melhorSolucao = possivelMelhorSolucao;
            // Atualiza a solução atual com a nova solução encontrada
            solucaoAtual = possivelMelhorSolucao;
            // Reinicia o caso para o movimento inverter
            caso = 0;
            temposAnteriores = temposAtuais;
        }
        else
        {
            temposAtuais = temposAnteriores;

            if (caso != 3)
            {
                // Passa para a próxima vizinhança
                caso += 1;
            }
            else
            {
                break;
            }
        }
    }

    // Retorna a melhor solução encontrada
    return melhorSolucao;
}

// * RVND - Comentado

vector<vector<int>> melhorarLinhasRVND(vector<vector<int>> &solucao, vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, vector<int> &temposSolucao)
{

    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());

    vector<vector<int>> melhorSolucao = solucao;
    vector<vector<int>> possivelMelhorSolucao;
    vector<vector<int>> solucaoAtual = solucao;

    vector<int> temposAtuais = temposSolucao;
    vector<int> temposAnteriores = temposAtuais;

    // Crie um vetor de frequência para controlar as buscas locais utilizadas
    vector<int> buscaLocalUtilizada(4, 0);

    // Inicializa o gerador de números aleatórios
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    while (1)
    {

        // Crie uma lista das buscas locais não utilizadas
        vector<int> buscasLocaisNaoUtilizadas;
        for (int i = 0; i < buscaLocalUtilizada.size(); i++)
        {
            if (!buscaLocalUtilizada[i])
                buscasLocaisNaoUtilizadas.push_back(i);
        }

        // Gera um número aleatório para determinar qual a busca local que será utilizada
        std::uniform_int_distribution<int> dist(0, buscasLocaisNaoUtilizadas.size() - 1);
        int buscaLocal = buscasLocaisNaoUtilizadas[dist(gen)];

        switch (buscaLocal)
        {
        case 0:
            // Chama a função de movimento de inverter
            possivelMelhorSolucao = movimentoInverter(solucaoAtual, matrizPreparacao, vetorProdutos, temposAtuais);
            break;
        case 1:
            // Chama a função de movimento horizontal
            possivelMelhorSolucao = novaSolucaoMesmaLinha(solucaoAtual, matrizPreparacao, temposAtuais);
            break;
        case 2:
            // Chama a função de movimento vertical
            possivelMelhorSolucao = novaSolucaoEntreLinhas(solucaoAtual, matrizPreparacao, vetorProdutos, temposAtuais);
            break;
        case 3:
            // Chama a função de movimento de inserção
            possivelMelhorSolucao = novaSolucaoReInsertion(solucaoAtual, matrizPreparacao, vetorProdutos, temposAtuais);
            break;
        }

        // Avalia os tempos de produção da nova solução
        temposAtuais = temposProducao(possivelMelhorSolucao, matrizPreparacao, vetorProdutos);

        // Encontra o maior tempo de produção possível na nova solução
        int possivelMaiorTempo = *max_element(temposAtuais.begin(), temposAtuais.end());

        // Se a busca local utilizada foi efetiva
        if (possivelMaiorTempo < tempoAtual)
        {
            // Atualiza o tempo atual com o novo maior tempo de produção
            tempoAtual = possivelMaiorTempo;
            // Atualiza a melhor solução com a nova solução encontrada
            melhorSolucao = possivelMelhorSolucao;
            // Atualiza a solução atual com a nova solução encontrada
            solucaoAtual = possivelMelhorSolucao;

            // Setando os valores das buscas locais para false
            for (int i = 0; i < buscaLocalUtilizada.size(); i++)
            {
                buscaLocalUtilizada[i] = 0;
            }

            temposAnteriores = temposAtuais;
        }

        // Se a busca local utilizada não foi efetiva
        else
        {
            // Atualize a frequência da busca local utilizada
            buscaLocalUtilizada[buscaLocal] = 1;

            temposAtuais = temposAnteriores;

            int quantidadeMovimentos = 0;
            for (int i = 0; i < buscaLocalUtilizada.size(); i++)
            {
                if (buscaLocalUtilizada[i] == 1)
                    quantidadeMovimentos += 1;
            }

            if (quantidadeMovimentos == 4)
            {
                break;
            }
        }
    }

    return melhorSolucao;
}

// * Meta-herusística: GRASP com VND

vector<int> listaCandidatosRestritos(vector<int> &vetorProdutos, vector<int> &indicesProdutosUsados, float alfa)
{
    // alfa = 1 -> guloso
    // alfa = 0 -> aleatório
    // 0 < alfa < 1 -> guloso e aleatório

    // Criar lista de candidatos restritos
    //      1° - Escolher o produto com o menor tempo de produção
    //      2° - Dividir o tempo do produto escolhido pelo alfa -> critério de seleção -> valorAceitavel
    //      3° - Passar por todo o vetorProduto, adicionando os produtos com tempo maior que o valorAceitavel
    //      4° - Retornar candidatos
    // Encontrar o menor

    // Fazer uma lista com os índices dos produtos não usados
    vector<int> indicesProdutosNaoUsados;

    for (int i = 0; i < vetorProdutos.size(); i++)
    {
        if (find(indicesProdutosUsados.begin(), indicesProdutosUsados.end(), i) == indicesProdutosUsados.end())
        {
            indicesProdutosNaoUsados.push_back(i);
        }
    }

    int valorMinimo;
    // Encontrando o menor tempo entre os produtos não usados
    for (int i = 0; i < indicesProdutosNaoUsados.size(); i++)
    {
        if (i == 0 || vetorProdutos[i] < vetorProdutos[valorMinimo])
        {
            valorMinimo = i;
        }
    }

    // Calculando o valor aceitável
    float valorAceitavel;
    if (alfa == 0)
    {
        valorAceitavel = *max_element(vetorProdutos.begin(), vetorProdutos.end());
    }
    else
    {
        valorAceitavel = vetorProdutos[valorMinimo] / alfa;
    }

    // Encontrando os produtos com tempos <= que o valorAceitavel
    vector<int> produtosAceitaveis;
    for (int i = 0; i < indicesProdutosNaoUsados.size(); i++)
    {
        if (vetorProdutos[i] <= valorAceitavel)
        {
            produtosAceitaveis.push_back(indicesProdutosNaoUsados[i]);
        }
    }

    return produtosAceitaveis;
}

vector<vector<int>> grasp(vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, int numeroLinhas, int numeroIteracoes, float alfa)
{

    int tempoAtual;
    vector<vector<int>> melhorSolucao;

    for (int k = 0; k < numeroIteracoes; k++)
    {

        vector<vector<int>> solucaoInicial(numeroLinhas, vector<int>());

        int linha = 0;

        vector<int> indicesProdutosUsados;

        vector<int> indicesProdutosNaoUsados;
        for (int i = 0; i < vetorProdutos.size(); i++)
        {
            indicesProdutosNaoUsados.push_back(i);
        }

        // Escolhendo um produto aleatório para colocar em indicesProdutosUsados
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);

        // 1° Produto
        std::uniform_int_distribution<int> distProduto(0, vetorProdutos.size() - 1);
        int produtoEscolhido = distProduto(gen);

        // Removendo o indiceProdutoEscolhido de indicesProdutosNaoUsados
        indicesProdutosNaoUsados.erase(remove(indicesProdutosNaoUsados.begin(), indicesProdutosNaoUsados.end(), produtoEscolhido), indicesProdutosNaoUsados.end());

        // Adicionando o indiceProdutoEscolhido em indicesProdutosUsados
        indicesProdutosUsados.push_back(produtoEscolhido);

        // Adicionando produto de indiceProdutoEscolhido na solução
        solucaoInicial[linha].push_back(produtoEscolhido);

        linha += 1;

        while (indicesProdutosNaoUsados.size() > 0)
        {

            // Encontrando os candidatos válidos para inserir na solução
            // vector<int> candidatosValidos = listaCandidatosRestritos(vetorProdutos, indicesProdutosUsados, distAlfa(gen));
            vector<int> candidatosValidos = listaCandidatosRestritos(vetorProdutos, indicesProdutosUsados, alfa);

            // Gera um número aleatório para determinar o produto que será usado para criar a lista de candidatos restritos
            std::uniform_int_distribution<int> distProdutoValido(0, candidatosValidos.size() - 1);
            int indiceProdutoValido = distProdutoValido(gen);
            int produtoCerto = candidatosValidos[indiceProdutoValido];

            // Removendo o indiceProdutoValido de indicesProdutosNaoUsados
            indicesProdutosNaoUsados.erase(remove(indicesProdutosNaoUsados.begin(), indicesProdutosNaoUsados.end(), produtoCerto), indicesProdutosNaoUsados.end());

            // Adicionando o indiceProdutoValido em indicesProdutosUsados
            indicesProdutosUsados.push_back(produtoCerto);

            // Adicionar produto de indiceProdutoValido na solução
            solucaoInicial[linha].push_back(produtoCerto);

            linha += 1;

            if (linha == numeroLinhas)
            {
                linha = 0;
            }
        }

        // Tempos para a solução inicial
        vector<int> novosTempos = temposProducao(solucaoInicial, matrizPreparacao, vetorProdutos);
        int tempoInicial = *max_element(novosTempos.begin(), novosTempos.end());

        // Tentando melhorar a solução
        vector<vector<int>> solucaoComMelhorias = melhorarLinhas(solucaoInicial, matrizPreparacao, vetorProdutos, novosTempos);
        // vector<vector<int>> solucaoComMelhorias = melhorarLinhasRVND(solucaoInicial, matrizPreparacao, vetorProdutos, novosTempos);

        // Tempos para a solução possivelmente melhorada
        vector<int> tempoComMelhorias = temposProducao(solucaoComMelhorias, matrizPreparacao, vetorProdutos);
        int tempoMelhor = *max_element(tempoComMelhorias.begin(), tempoComMelhorias.end());

        if (k == 0)
        {
            tempoAtual = tempoInicial;
            melhorSolucao = solucaoInicial;
        }

        if (tempoMelhor < tempoAtual)
        {
            melhorSolucao = solucaoComMelhorias;
            tempoAtual = tempoMelhor;
        }
    }

    return melhorSolucao;
}

// * Meta-heurística: ILS com VND

vector<vector<int>> perturbacaoHorizontal(vector<vector<int>> &solucao, int numeroLinhas)
{
    // Cria uma cópia da solução original
    vector<vector<int>> solucaoCopia = solucao;

    // Inicializa o gerador de números aleatórios
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    // Gera um número aleatório para determinar quantas linhas serão afetadas
    std::uniform_int_distribution<int> sorteioNumeroLinha(1, numeroLinhas);
    int numeroLinhasAfetadas = sorteioNumeroLinha(gen);

    // Cria um vetor para armazenar as linhas não usadas
    vector<int> linhasNaoUsadas(numeroLinhas);
    for (int i = 0; i < numeroLinhas; i++)
        linhasNaoUsadas[i] = i;

    // Cria um vetor para armazenar as linhas afetadas
    vector<int> linhasAfetadas = linhasNaoUsadas;
    linhasAfetadas.resize(numeroLinhasAfetadas);

    if (numeroLinhasAfetadas != numeroLinhas)
    {
        for (int i = 0; i < numeroLinhasAfetadas; i++)
        {
            // Seleciona aleatoriamente uma posição no vetor de linhas não usadas
            std::uniform_int_distribution<int> sorteioLinha(0, linhasNaoUsadas.size() - 1);
            int posicaoLinha = sorteioLinha(gen);
            int realLinha = linhasNaoUsadas[posicaoLinha];

            // Remove a linha selecionada do vetor de linhas não usadas e adiciona ao vetor de linhas afetadas
            linhasNaoUsadas.erase(linhasNaoUsadas.begin() + posicaoLinha);
            linhasAfetadas[i] = realLinha;
        }
    }
    else
    {
        linhasAfetadas = linhasNaoUsadas;
    }

    // Itera sobre cada linha da solução
    for (int i = 0; i < numeroLinhasAfetadas; i++)
    {
        // Vetor temporário para armazenar os elementos perturbados
        vector<int> listaTemp;

        // Itera sobre os elementos da linha atual
        for (int j = 0; j < solucao[linhasAfetadas[i]].size(); j++)
        {
            // Distribuição uniforme para selecionar uma posição aleatória
            std::uniform_int_distribution<int> posicaoProduto(0, solucaoCopia[linhasAfetadas[i]].size() - 1);

            // Obtém uma posição aleatória e o produto correspondente
            int posicao = posicaoProduto(gen);
            int produtoAtual = solucaoCopia[linhasAfetadas[i]][posicao];

            // Remove o produto atual da cópia da linha
            solucaoCopia[linhasAfetadas[i]].erase(remove(solucaoCopia[linhasAfetadas[i]].begin(), solucaoCopia[linhasAfetadas[i]].end(), produtoAtual), solucaoCopia[linhasAfetadas[i]].end());

            // Adiciona o produto atual à lista temporária
            listaTemp.push_back(produtoAtual);
        }

        // Substitui a linha perturbada pela lista temporária
        solucaoCopia[linhasAfetadas[i]] = listaTemp;
    }

    // Retorna a solução perturbada
    return solucaoCopia;
}

vector<vector<int>> perturbacaoVertical(vector<vector<int>> &solucao, int numeroLinhas)
{
    // Cria uma cópia da solução original
    vector<vector<int>> solucaoCopia = solucao;

    // Inicializa o gerador de números aleatórios
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    // Sorteio para determinar se o produto será trocado ou não
    std::uniform_int_distribution<int> sorteioTroca(0, 1);

    // Gera um número aleatório para determinar quantas linhas serão afetadas
    std::uniform_int_distribution<int> sorteioNumeroLinhas(2, numeroLinhas);
    int numeroLinhasAfetadas = sorteioNumeroLinhas(gen);

    // Cria um vetor para armazenar as linhas não usadas
    vector<int> linhasNaoUsadas(numeroLinhas);
    for (int i = 0; i < numeroLinhas; i++)
        linhasNaoUsadas[i] = i;

    // Cria um vetor para armazenar as linhas afetadas
    vector<int> linhasAfetadas;
    linhasAfetadas.resize(numeroLinhasAfetadas);

    if (numeroLinhasAfetadas != numeroLinhas)
    {
        for (int i = 0; i < numeroLinhasAfetadas; i++)
        {
            // Seleciona aleatoriamente uma posição no vetor de linhas não usadas
            std::uniform_int_distribution<int> sorteioLinha(0, linhasNaoUsadas.size() - 1);
            int posicaoLinha = sorteioLinha(gen);
            int realLinha = linhasNaoUsadas[posicaoLinha];

            // Remove a linha selecionada do vetor de linhas não usadas e adiciona ao vetor de linhas afetadas
            linhasNaoUsadas.erase(linhasNaoUsadas.begin() + posicaoLinha);
            linhasAfetadas[i] = realLinha;
        }
    }
    else
    {
        linhasAfetadas = linhasNaoUsadas;
    }

    // Escolhe qual a linha que será o pivô
    std::uniform_int_distribution<int> linhaPivo(0, linhasAfetadas.size() - 1);
    int sorteioLinhaEscolhida = linhaPivo(gen);
    int linhaEscolhida = linhasAfetadas[sorteioLinhaEscolhida];
    linhasAfetadas.erase(linhasAfetadas.begin() + sorteioLinhaEscolhida);

    // Itera sobre cada linha da solução
    for (int i = 0; i < solucaoCopia[linhaEscolhida].size(); i++)
    {
        int trocar = sorteioTroca(gen);

        if (trocar)
        {
            // Produto que vai sair da linha pivô
            int produtoSaida = solucaoCopia[linhaEscolhida][i];

            // Escolher qual a linha e o produto dela que será trocado de maneira aleatória
            std::uniform_int_distribution<int> linhaProduto(0, linhasAfetadas.size() - 1);
            int linhaTroca = linhasAfetadas[linhaProduto(gen)];

            std::uniform_int_distribution<int> posicaoProduto(0, solucaoCopia[linhaTroca].size() - 1);
            int sorteioProduto = posicaoProduto(gen);

            // Produto que vai entrar na linha pivô
            int produtoEntrada = solucaoCopia[linhaTroca][sorteioProduto];

            // Realizando a troca
            solucaoCopia[linhaEscolhida][i] = produtoEntrada;
            solucaoCopia[linhaTroca][sorteioProduto] = produtoSaida;
        }
    }

    // Retorna a solução perturbada
    return solucaoCopia;
}

vector<vector<int>> ils(vector<vector<int>> &matrizPreparacao, vector<int> &vetorProdutos, int numeroLinhas, int numeroIteracoes)
{
    vector<vector<int>> melhorSolucao;
    int tempoAtual = 0;

    // Inicializa o gerador de números aleatórios
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(0, 1);

    for (int r = 0; r < numeroIteracoes; r++)
    {
        // vector<vector<int>> solucaoInicial = grasp(matrizPreparacao, vetorProdutos, numeroLinhas, 1, 1);
        vector<vector<int>> solucaoInicial = gerarSolucaoGulosa(vetorProdutos.size(), numeroLinhas, matrizPreparacao, vetorProdutos);

        int perturbacaoUtilizada = dist(gen);

        switch (perturbacaoUtilizada)
        {
        case 0:
            solucaoInicial = perturbacaoHorizontal(solucaoInicial, numeroLinhas);
            break;
        case 1:
            solucaoInicial = perturbacaoVertical(solucaoInicial, numeroLinhas);
            break;
        }

        vector<int> novosTempos = temposProducao(solucaoInicial, matrizPreparacao, vetorProdutos);

        // Padrão: VND

        vector<vector<int>> solucaoComMelhorias = melhorarLinhas(solucaoInicial, matrizPreparacao, vetorProdutos, novosTempos);

        // Alternativo: RVND
        // vector<vector<int>> solucaoComMelhorias = melhorarLinhasRVND(solucaoInicial, matrizPreparacao, vetorProdutos, novosTempos);

        vector<int> tempoComMelhorias = temposProducao(solucaoComMelhorias, matrizPreparacao, vetorProdutos);

        int tempoMelhor = *max_element(tempoComMelhorias.begin(), tempoComMelhorias.end());

        if (r == 0 || tempoMelhor < tempoAtual)
        {
            tempoAtual = tempoMelhor;
            melhorSolucao = solucaoComMelhorias;
        }
    }

    return melhorSolucao;
}
