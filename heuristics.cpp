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
vector<vector<int>> gerarSolucaoGulosa(int n, int m, vector<int> vetorProdutos, vector<vector<int>> matrizPreparacao)
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
vector<int> temposProducao(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos)
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

// * Movimentos de Vizinhanças - Otimizado

vector<vector<int>> trocarProdutosMesmaLinha(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> temposSolucao)
{
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());
    vector<vector<int>> melhorSolucao = solucao;
    vector<vector<int>> solucaoCopia = solucao;

    for (int i = 0; i < solucao.size(); i++)
    {
        // Itera sobre os produtos na linha atual
        for (int j = 0; j < solucaoCopia[i].size(); j++)
        {
            int produtoAtual = solucaoCopia[i][j];

            // Tenta substituir o produto atual por outros produtos subsequentes na linha
            for (int k = j + 1; k < solucaoCopia[i].size(); k++)
            {
                vector<int> temposSolucaoAtual = temposSolucao;

                int produtoSubs = solucaoCopia[i][k];

                solucaoCopia[i][j] = produtoSubs;
                solucaoCopia[i][k] = produtoAtual;

                int prox_prodAtual = solucao[i][j + 1];
                int ant_prodSubs = solucao[i][k - 1];

                // Se o produtoAtual é o 1° da linha
                if (j == 0)
                {

                    // Se o produto que será trocado com o produtoAtual não é o último
                    if (k < solucaoCopia[i].size() - 1)
                    {
                        int prox_prodSubs = solucao[i][k + 1];

                        temposSolucaoAtual[i] -= (matrizPreparacao[produtoAtual][prox_prodAtual] + matrizPreparacao[ant_prodSubs][produtoSubs] + matrizPreparacao[produtoSubs][prox_prodSubs]);
                        temposSolucaoAtual[i] += (matrizPreparacao[produtoSubs][prox_prodAtual] + matrizPreparacao[ant_prodSubs][produtoAtual] + matrizPreparacao[produtoAtual][prox_prodSubs]);
                    }

                    // Se o produto que será trocado com o produtoAtual é o último
                    else
                    {
                        temposSolucaoAtual[i] -= (matrizPreparacao[produtoAtual][prox_prodAtual] + matrizPreparacao[ant_prodSubs][produtoSubs]);
                        temposSolucaoAtual[i] += (matrizPreparacao[produtoSubs][prox_prodAtual] + matrizPreparacao[ant_prodSubs][produtoAtual]);
                    }
                }

                // Se o produtoAtual não é o 1° da linha
                else
                {
                    int ant_prodAtual = solucao[i][j - 1];

                    if (k < solucaoCopia[i].size() - 1)
                    {
                        int prox_prodSubs = solucao[i][k + 1];

                        temposSolucaoAtual[i] -= (matrizPreparacao[produtoAtual][prox_prodAtual] + matrizPreparacao[ant_prodAtual][produtoAtual] + matrizPreparacao[ant_prodSubs][produtoSubs] + matrizPreparacao[produtoSubs][prox_prodSubs]);
                        temposSolucaoAtual[i] += (matrizPreparacao[produtoSubs][prox_prodAtual] + matrizPreparacao[ant_prodAtual][produtoSubs] + matrizPreparacao[ant_prodSubs][produtoAtual] + matrizPreparacao[produtoAtual][prox_prodSubs]);
                    }
                    else
                    {
                        temposSolucaoAtual[i] -= (matrizPreparacao[produtoAtual][prox_prodAtual] + matrizPreparacao[ant_prodAtual][produtoAtual] + matrizPreparacao[ant_prodSubs][produtoSubs]);
                        temposSolucaoAtual[i] += (matrizPreparacao[produtoSubs][prox_prodAtual] + matrizPreparacao[ant_prodAtual][produtoSubs] + matrizPreparacao[ant_prodSubs][produtoAtual]);
                    }
                }

                // Constrói uma solução possível com as trocas feitas
                // vector<int> temposPossiveis = temposProducao(solucaoCopia, matrizPreparacao, vetorProdutos);
                int possivelMaiorTempo = *max_element(temposSolucaoAtual.begin(), temposSolucaoAtual.end());

                // Verifica se a solução possível tem um tempo total menor que o tempo atual
                if (possivelMaiorTempo < tempoAtual)
                {
                    tempoAtual = possivelMaiorTempo;
                    melhorSolucao = solucaoCopia;
                }

                // Desfaz a troca para a próxima iteração do loop
                solucaoCopia[i][j] = produtoAtual;
                solucaoCopia[i][k] = produtoSubs;
            }
        }
    }

    return melhorSolucao;
}

vector<vector<int>> trocarProdutosEntreLinhas(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    // Tempo atual da melhor solução
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());
    // Melhor configuração das linhas de produção
    vector<vector<int>> melhorSolucao = solucao;
    // Cópia das linhas de produção para realizar as trocas
    vector<vector<int>> solucaoCopia = solucao;

    for (int i = 0; i < solucao.size(); i++)
    {
        for (int j = 0; j < solucaoCopia[i].size(); j++)
        {
            for (int k = i + 1; k < solucao.size(); k++)
            {
                // Produto atual da linha i
                int produtoAtual = solucaoCopia[i][j];

                for (int l = 0; l < solucaoCopia[k].size(); l++)
                {
                    // Produto a ser substituído da linha k
                    int produtoSubs = solucaoCopia[k][l];

                    // Substitui o produto atual pelo produto de substituição na linha i
                    solucaoCopia[i][j] = produtoSubs;
                    // Substitui o produto de substituição pelo produto atual na linha k
                    solucaoCopia[k][l] = produtoAtual;

                    // Avalia a nova configuração das linhas de produção
                    // vector<int> temposPossiveis = temposProducao(solucaoCopia, matrizPreparacao, vetorProdutos);
                    vector<int> temposSolucaoAtual = temposSolucao;
                    temposSolucaoAtual[i] -= vetorProdutos[produtoAtual];
                    temposSolucaoAtual[i] += vetorProdutos[produtoSubs];

                    temposSolucaoAtual[k] -= vetorProdutos[produtoSubs];
                    temposSolucaoAtual[k] += vetorProdutos[produtoAtual];

                    int prox_prodAtual = solucao[i][j + 1];
                    int ant_prodSubs = solucao[k][l - 1];

                    if (j == 0)
                    {
                        if (l < solucaoCopia[k].size() - 1)
                        {
                            int prox_prodSubs = solucao[k][l + 1];

                            temposSolucaoAtual[i] -= matrizPreparacao[produtoAtual][prox_prodAtual];
                            temposSolucaoAtual[i] += matrizPreparacao[produtoSubs][prox_prodAtual];

                            temposSolucaoAtual[k] -= (matrizPreparacao[ant_prodSubs][produtoSubs] + matrizPreparacao[produtoSubs][prox_prodSubs]);
                            temposSolucaoAtual[k] += (matrizPreparacao[ant_prodSubs][produtoAtual] + matrizPreparacao[produtoAtual][prox_prodSubs]);
                        }
                        else
                        {
                            temposSolucaoAtual[i] -= matrizPreparacao[produtoAtual][prox_prodAtual];
                            temposSolucaoAtual[i] += matrizPreparacao[produtoSubs][prox_prodAtual];

                            temposSolucaoAtual[k] -= (matrizPreparacao[ant_prodSubs][produtoSubs]);
                            temposSolucaoAtual[k] += (matrizPreparacao[ant_prodSubs][produtoAtual]);
                        }
                    }
                    else
                    {
                        int ant_prodAtual = solucao[i][j - 1];

                        if (l < solucaoCopia[k].size() - 1)
                        {
                            int prox_prodSubs = solucao[k][l + 1];

                            temposSolucaoAtual[i] -= (matrizPreparacao[produtoAtual][prox_prodAtual] + matrizPreparacao[ant_prodAtual][produtoAtual]);
                            temposSolucaoAtual[i] += (matrizPreparacao[produtoSubs][prox_prodAtual] + matrizPreparacao[ant_prodAtual][produtoSubs]);

                            temposSolucaoAtual[k] -= (matrizPreparacao[ant_prodSubs][produtoSubs] + matrizPreparacao[produtoSubs][prox_prodSubs]);
                            temposSolucaoAtual[k] += (matrizPreparacao[ant_prodSubs][produtoAtual] + matrizPreparacao[produtoAtual][prox_prodSubs]);
                        }
                        else
                        {
                            temposSolucaoAtual[i] -= (matrizPreparacao[produtoAtual][prox_prodAtual] + matrizPreparacao[ant_prodAtual][produtoAtual]);
                            temposSolucaoAtual[i] += (matrizPreparacao[produtoSubs][prox_prodAtual] + matrizPreparacao[ant_prodAtual][produtoSubs]);

                            temposSolucaoAtual[k] -= (matrizPreparacao[ant_prodSubs][produtoSubs]);
                            temposSolucaoAtual[k] += (matrizPreparacao[ant_prodSubs][produtoAtual]);
                        }
                    }

                    int possivelMaiorTempo = *max_element(temposSolucaoAtual.begin(), temposSolucaoAtual.end());

                    if (possivelMaiorTempo < tempoAtual)
                    {
                        // Atualiza o tempo atual da melhor solução
                        tempoAtual = possivelMaiorTempo;
                        // Atualiza a melhor configuração das linhas de produção
                        melhorSolucao = solucaoCopia;
                    }

                    // Desfaz a troca para a próxima iteração do loop
                    solucaoCopia[i][j] = produtoAtual;
                    solucaoCopia[k][l] = produtoSubs;
                }
            }
        }
    }

    // Retorna a melhor configuração das linhas de produção encontrada
    return melhorSolucao;
}

vector<vector<int>> inserirProdutoEmOutrasPosicoes(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());
    vector<vector<int>> melhorSolucao = solucao;
    vector<vector<int>> solucaoCopia = solucao;

    for (int i = 0; i < solucao.size(); i++)
    {
        for (int j = 0; j < solucao[i].size(); j++)
        {
            int produtoAtual = solucaoCopia[i][j];

            for (int k = i + 1; k < solucao.size(); k++)
            {
                for (int l = 0; l < solucaoCopia[k].size(); l++)
                {

                    vector<int> temposSolucaoAtual = temposSolucao;

                    // Remover o tempo do produtoAtual e suas conexões da linha que ele está
                    temposSolucaoAtual[i] -= vetorProdutos[produtoAtual];

                    // Se o produtoAtual for o 1° da linha
                    if (j == 0)
                    {
                        int produtoSeguinte = solucaoCopia[i][j + 1];
                        temposSolucaoAtual[i] -= matrizPreparacao[produtoAtual][produtoSeguinte];
                    }

                    // Se o produtoAtual for o último da linha
                    else if (j == solucao[i].size() - 1)
                    {
                        int produtoAnterior = solucaoCopia[i][j - 1];
                        temposSolucaoAtual[i] -= matrizPreparacao[produtoAnterior][produtoAtual];
                    }

                    // Se o produtoAtual está entre o 1° e o último
                    else
                    {
                        int produtoAnterior = solucaoCopia[i][j - 1];
                        temposSolucaoAtual[i] -= matrizPreparacao[produtoAnterior][produtoAtual];

                        int produtoSeguinte = solucaoCopia[i][j + 1];
                        temposSolucaoAtual[i] -= matrizPreparacao[produtoAtual][produtoSeguinte];

                        temposSolucaoAtual[i] += matrizPreparacao[produtoAnterior][produtoSeguinte];
                    }

                    // Adicionando o tempo de produtoAtual e suas conexões na linha em questão
                    temposSolucaoAtual[k] += vetorProdutos[produtoAtual];

                    // Inserindo o produtoAtual no começo da linha
                    if (l == 0)
                    {
                        int produtoSeguinte = solucaoCopia[k][l];
                        temposSolucaoAtual[k] += matrizPreparacao[produtoAtual][produtoSeguinte];
                    }

                    // Inserindo no final da linha
                    else if (l == solucaoCopia[k].size())
                    {
                        int produtoAnterior = solucaoCopia[k][l - 1];
                        temposSolucaoAtual[k] += matrizPreparacao[produtoAnterior][produtoAtual];
                    }

                    // Inserindo entre o 1° e o último
                    else
                    {
                        int produtoSeguinte = solucaoCopia[k][l];
                        int produtoAnterior = solucaoCopia[k][l - 1];

                        temposSolucaoAtual[k] -= matrizPreparacao[produtoAnterior][produtoSeguinte];

                        temposSolucaoAtual[k] += matrizPreparacao[produtoAtual][produtoSeguinte];
                        temposSolucaoAtual[k] += matrizPreparacao[produtoAnterior][produtoAtual];
                    }

                    int possivelMaiorTempo = *max_element(temposSolucaoAtual.begin(), temposSolucaoAtual.end());

                    if (possivelMaiorTempo < tempoAtual)
                    {
                        // Adicionando na linha o produtoAtual
                        solucaoCopia[k].insert(solucaoCopia[k].begin() + l, produtoAtual);
                        // Atualiza o tempo atual da melhor solução
                        tempoAtual = possivelMaiorTempo;
                        // Atualiza a melhor configuração das linhas de produção
                        melhorSolucao = solucaoCopia;
                        // Removendo da linha o produtoAtual
                        solucaoCopia[k].erase(remove(solucaoCopia[k].begin(), solucaoCopia[k].end(), produtoAtual), solucaoCopia[k].end());
                    }
                }
            }
        }
    }

    return melhorSolucao;
}

// * Movimentos de Vizinhaças - Força Bruta

vector<vector<int>> movimentoHorizontal(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());
    vector<vector<int>> melhorSolucao = solucao;
    vector<vector<int>> solucaoCopia = solucao;

    for (int i = 0; i < solucao.size(); i++)
    {
        // Itera sobre os produtos na linha atual
        for (int j = 0; j < solucaoCopia[i].size(); j++)
        {
            int produtoAtual = solucaoCopia[i][j];

            // Tenta substituir o produto atual por outros produtos subsequentes na linha
            for (int k = j + 1; k < solucaoCopia[i].size(); k++)
            {
                vector<int> temposSolucaoAtual = temposSolucao;
                int produtoSubs = solucaoCopia[i][k];
                solucaoCopia[i][j] = produtoSubs;
                solucaoCopia[i][k] = produtoAtual;

                // Constrói uma solução possível com as trocas feitas
                vector<int> temposPossiveis = temposProducao(solucaoCopia, matrizPreparacao, vetorProdutos);
                int possivelMaiorTempo = *max_element(temposPossiveis.begin(), temposPossiveis.end());

                // Verifica se a solução possível tem um tempo total menor que o tempo atual
                if (possivelMaiorTempo < tempoAtual)
                {
                    tempoAtual = possivelMaiorTempo;
                    melhorSolucao = solucaoCopia;
                }

                // Desfaz a troca para a próxima iteração do loop
                solucaoCopia[i][j] = produtoAtual;
                solucaoCopia[i][k] = produtoSubs;
            }
        }
    }

    return melhorSolucao;
}

vector<vector<int>> movimentoVertical(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    // Tempo atual da melhor solução
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());
    // Melhor configuração das linhas de produção
    vector<vector<int>> melhorSolucao = solucao;
    // Cópia das linhas de produção para realizar as trocas
    vector<vector<int>> solucaoCopia = solucao;

    for (int i = 0; i < solucao.size(); i++)
    {
        for (int j = 0; j < solucaoCopia[i].size(); j++)
        {
            for (int k = i + 1; k < solucao.size(); k++)
            {
                // Produto atual da linha i
                int produtoAtual = solucaoCopia[i][j];

                for (int l = 0; l < solucaoCopia[k].size(); l++)
                {
                    // Produto a ser substituído da linha k
                    int produtoSubs = solucaoCopia[k][l];

                    // Substitui o produto atual pelo produto de substituição na linha i
                    solucaoCopia[i][j] = produtoSubs;
                    // Substitui o produto de substituição pelo produto atual na linha k
                    solucaoCopia[k][l] = produtoAtual;

                    // Avalia a nova configuração das linhas de produção
                    vector<int> temposPossiveis = temposProducao(solucaoCopia, matrizPreparacao, vetorProdutos);
                    int possivelMaiorTempo = *max_element(temposPossiveis.begin(), temposPossiveis.end());

                    if (possivelMaiorTempo < tempoAtual)
                    {
                        // Atualiza o tempo atual da melhor solução
                        tempoAtual = possivelMaiorTempo;
                        // Atualiza a melhor configuração das linhas de produção
                        melhorSolucao = solucaoCopia;
                    }

                    // Desfaz a troca para a próxima iteração do loop
                    solucaoCopia[i][j] = produtoAtual;
                    solucaoCopia[k][l] = produtoSubs;
                }
            }
        }
    }

    // Retorna a melhor configuração das linhas de produção encontrada
    return melhorSolucao;
}

vector<vector<int>> movimentoInverter(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());
    vector<vector<int>> melhorSolucao = solucao;
    vector<vector<int>> solucaoCopia = solucao;

    for (int i = 0; i < solucao.size(); i++)
    {
        // Invertendo a ordem dos produtos na linha atual
        for (int j = 0; j < solucaoCopia[i].size() / 2; j++)
        {
            int temp = solucaoCopia[i][j];
            solucaoCopia[i][j] = solucaoCopia[i][solucaoCopia[i].size() - 1 - j];
            solucaoCopia[i][solucaoCopia[i].size() - 1 - j] = temp;
        }

        // Avalia a nova configuração das linhas de produção
        vector<int> temposPossiveis = temposProducao(solucaoCopia, matrizPreparacao, vetorProdutos);
        int possivelMaiorTempo = *max_element(temposPossiveis.begin(), temposPossiveis.end());

        // Atualiza a melhor configuração das linhas de produção
        if (possivelMaiorTempo < tempoAtual)
        {
            tempoAtual = possivelMaiorTempo;
            melhorSolucao = solucaoCopia;
        }

        // Desfazendo o movimento
        for (int j = 0; j < solucaoCopia[i].size() / 2; j++)
        {
            int temp = solucaoCopia[i][j];
            solucaoCopia[i][j] = solucaoCopia[i][solucaoCopia[i].size() - 1 - j];
            solucaoCopia[i][solucaoCopia[i].size() - 1 - j] = temp;
        }
    }

    return melhorSolucao;
}

vector<vector<int>> movimentoInsercao(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());
    vector<vector<int>> melhorSolucao = solucao;

    for (int i = 0; i < solucao.size(); i++)
    {
        for (int j = 0; j < solucao[i].size(); j++)
        {
            vector<vector<int>> solucaoCopia = solucao;

            int produtoAtual = solucaoCopia[i][j];

            // Remove o produto atual da linha atual
            solucaoCopia[i].erase(remove(solucaoCopia[i].begin(), solucaoCopia[i].end(), produtoAtual), solucaoCopia[i].end());

            vector<vector<int>> linhasSubs = solucaoCopia;

            for (int k = i + 1; k < solucao.size(); k++)
            {
                for (int l = 0; l < solucaoCopia[k].size(); l++)
                {
                    // Insere o produto atual em diferentes posições nas linhas subsequentes
                    solucaoCopia[k].insert(solucaoCopia[k].begin() + l, produtoAtual);

                    // Avalia a nova configuração das linhas de produção
                    vector<int> temposPossiveis = temposProducao(solucaoCopia, matrizPreparacao, vetorProdutos);
                    int possivelMaiorTempo = *max_element(temposPossiveis.begin(), temposPossiveis.end());

                    if (possivelMaiorTempo < tempoAtual)
                    {
                        // Atualiza o tempo atual da melhor solução
                        tempoAtual = possivelMaiorTempo;
                        // Atualiza a melhor configuração das linhas de produção
                        melhorSolucao = solucaoCopia;
                    }

                    // Restaura as linhas de produção para a configuração original
                    solucaoCopia = linhasSubs;
                }
            }
        }
    }

    return melhorSolucao;
}

// * VND - Comentado

vector<vector<int>> melhorarLinhas(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    // Inicialização do tempo atual com o maior tempo da solução atual
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());

    // Inicialização da melhor solução com a solução atual
    vector<vector<int>> melhorSolucao = solucao;
    // Declaração de variáveis de solução temporárias
    vector<vector<int>> possivelMelhorSolucao;
    vector<vector<int>> solucaoAtual = solucao;

    // Controle de casos e contador de iterações
    int caso = 0;
    int contador = 0;

    while (contador < 3)
    {
        switch (caso)
        {
        case 0:
            // Chama a função de movimento de inverter
            possivelMelhorSolucao = movimentoInverter(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            break;
        case 1:
            // Chama a função de movimento horizontal (ou trocarProdutosMesmaLinha)
            // possivelMelhorSolucao = movimentoHorizontal(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            possivelMelhorSolucao = trocarProdutosMesmaLinha(solucaoAtual, matrizPreparacao, temposSolucao);
            break;
        case 2:
            // Chama a função de movimento vertical (ou trocarProdutosEntreLinhas)
            possivelMelhorSolucao = movimentoVertical(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            // possivelMelhorSolucao = trocarProdutosEntreLinhas(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            break;
        case 3:
            // Chama a função de movimento de inserção
            // possivelMelhorSolucao = movimentoInsercao(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            possivelMelhorSolucao = inserirProdutoEmOutrasPosicoes(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            break;
        }

        // Avalia os tempos de produção da nova solução
        temposSolucao = temposProducao(possivelMelhorSolucao, matrizPreparacao, vetorProdutos);

        // Encontra o maior tempo de produção possível na nova solução
        int possivelMaiorTempo = *max_element(temposSolucao.begin(), temposSolucao.end());

        if (possivelMaiorTempo < tempoAtual)
        {
            // Atualiza o tempo atual com o novo maior tempo de produção
            tempoAtual = possivelMaiorTempo;
            // Atualiza a melhor solução com a nova solução encontrada
            melhorSolucao = possivelMelhorSolucao;
            // Atualiza a solução atual com a nova solução encontrada
            solucaoAtual = possivelMelhorSolucao;
            // Reinicia o caso para o movimento horizontal (ou trocarProdutosMesmaLinha)
            caso = 0;
        }
        else
        {
            if (caso != 3)
            {
                // Passa para a próxima vizinhança
                caso++;
            }
            else
            {
                caso = 0;
                contador++;
            }
        }
    }

    // Retorna a melhor solução encontrada
    return melhorSolucao;
}

// * RVND - Comentado

vector<vector<int>> melhorarLinhasRVND(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    int tempoAtual = *max_element(temposSolucao.begin(), temposSolucao.end());
    int tempoAnterior = tempoAtual;
    vector<vector<int>> melhorSolucao = solucao;
    vector<vector<int>> possivelMelhorSolucao;
    vector<vector<int>> solucaoAtual = solucao;

    // Crie um vetor de frequência para controlar as buscas locais utilizadas
    vector<bool> buscaLocalUtilizada(4, false);

    // Controle de casos e contador de iterações
    int contador = 0;

    while (contador < 3)
    {

        // Crie uma lista das buscas locais não utilizadas
        vector<int> buscasLocaisNaoUtilizadas;
        for (int i = 0; i < buscaLocalUtilizada.size(); i++)
        {
            if (!buscaLocalUtilizada[i])
                buscasLocaisNaoUtilizadas.push_back(i);
        }

        // Verifique se todas as buscas locais foram utilizadas
        if (buscasLocaisNaoUtilizadas.empty())
            break;

        // Escolha aleatoriamente uma busca local não utilizada
        int buscaLocal = buscasLocaisNaoUtilizadas[rand() % buscasLocaisNaoUtilizadas.size()];

        switch (buscaLocal)
        {
        case 0:
            // Chama a função de movimento de inverter
            possivelMelhorSolucao = movimentoInverter(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            break;
        case 1:
            // Chama a função de movimento horizontal (ou trocarProdutosMesmaLinha)
            possivelMelhorSolucao = movimentoHorizontal(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            // possivelMelhorSolucao = trocarProdutosMesmaLinha(solucaoAtual, matrizPreparacao, temposSolucao);
            break;
        case 2:
            // Chama a função de movimento vertical (ou trocarProdutosEntreLinhas)
            possivelMelhorSolucao = movimentoVertical(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            // possivelMelhorSolucao = trocarProdutosEntreLinhas(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            break;
        case 3:
            // Chama a função de movimento de inserção
            possivelMelhorSolucao = movimentoInsercao(solucaoAtual, matrizPreparacao, vetorProdutos, temposSolucao);
            break;
        }

        // Avalia os tempos de produção da nova solução
        temposSolucao = temposProducao(possivelMelhorSolucao, matrizPreparacao, vetorProdutos);

        // Encontra o maior tempo de produção possível na nova solução
        int possivelMaiorTempo = *max_element(temposSolucao.begin(), temposSolucao.end());

        // Se a busca local utilizada não foi efetiva
        if (possivelMaiorTempo >= tempoAtual)
        {
            // Atualize a frequência da busca local utilizada
            buscaLocalUtilizada[buscaLocal] = true;
        }

        // Se a busca local utilizada foi efetiva
        else
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
                buscaLocalUtilizada[i] = false;
            }
        }

        // Se não foi utilizada nenhuma busca local
        if (tempoAnterior == tempoAtual)
        {
            contador += 1;
        }

        tempoAnterior = tempoAtual;
    }

    return melhorSolucao;
}

// # TOTALMENTE ERRADOS

// * Meta-herusística: GRASP - Comentado

vector<vector<int>> heuristicaGRASP(vector<int> &vetorProdutos, vector<vector<int>> &matrizPreparacao, int numeroLinhas, int produtos, int numeroIteracoes)
{
    // Variável para armazenar a melhor solução encontrada
    vector<vector<int>> melhorSolucao;
    int tempoAtual = 0;

    // Inicialização do gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, produtos - 1);

    // Iteração principal do algoritmo GRASP
    for (int r = 0; r < numeroIteracoes; r++)
    {
        // Criação de uma solução inicial aleatória
        vector<vector<int>> solucaoInicial(numeroLinhas, vector<int>());
        unordered_set<int> produtosUsados;

        int linha = 0;
        for (int i = 0; i < produtos; i++)
        {
            // Seleciona um produto aleatório que ainda não foi utilizado
            int produtoAtual = dist(gen);

            while (produtosUsados.count(produtoAtual) > 0)
            {
                produtoAtual = dist(gen);
            }

            produtosUsados.insert(produtoAtual);
            solucaoInicial[linha].push_back(produtoAtual);

            // Distribui os produtos entre as linhas
            linha += 1;
            if (linha == numeroLinhas)
                linha = 0;
        }

        // Calcula os tempos de produção para a solução inicial
        vector<int> novosTempos = temposProducao(solucaoInicial, matrizPreparacao, vetorProdutos);

        // Aplica melhorias na solução inicial usando a estratégia VND
        vector<vector<int>> solucaoComMelhorias = melhorarLinhas(solucaoInicial, matrizPreparacao, vetorProdutos, novosTempos);

        // Aplica melhorias na solução inicial usando a estratégia RVND
        // vector<vector<int>> solucaoComMelhorias = melhorarLinhasRVND(solucaoInicial, matrizPreparacao, vetorProdutos, novosTempos);

        // Calcula os tempos de produção após as melhorias
        vector<int> tempoComMelhorias = temposProducao(solucaoComMelhorias, matrizPreparacao, vetorProdutos);

        // Encontra o maior tempo de produção na solução com melhorias
        int tempoMelhor = *max_element(tempoComMelhorias.begin(), tempoComMelhorias.end());

        // Verifica se a nova solução é a melhor encontrada até o momento
        if (r == 0 || tempoMelhor < tempoAtual)
        {
            tempoAtual = tempoMelhor;
            melhorSolucao = solucaoComMelhorias;
        }
    }

    // Retorna a melhor solução encontrada
    return melhorSolucao;
}

// * Meta-heurística: ILS

vector<vector<int>> perturbacao(vector<vector<int>> &solucao, int numeroLinhas)
{
    // Cria uma cópia da solução original
    vector<vector<int>> solucaoCopia = solucao;

    // Inicializa o gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd());

    // Gera um número aleatório para determinar quantas linhas serão afetadas
    std::uniform_int_distribution<int> dist(0, numeroLinhas - 1);
    int numeroLinhasAfetadas = dist(gen);

    // Cria um vetor para armazenar as linhas não usadas
    vector<int> linhasNaoUsadas(numeroLinhas);
    for (int i = 0; i < numeroLinhas; i++)
        linhasNaoUsadas[i] = i;

    // Cria um vetor para armazenar as linhas afetadas
    vector<int> linhasAfetadas = linhasNaoUsadas;
    if (numeroLinhasAfetadas != numeroLinhas - 1)
    {
        linhasAfetadas.resize(numeroLinhasAfetadas);
        for (int i = 0; i < numeroLinhasAfetadas; i++)
        {
            // Seleciona aleatoriamente uma posição no vetor de linhas não usadas
            std::uniform_int_distribution<int> dist(0, linhasNaoUsadas.size() - 1);
            int posicaoLinha = dist(gen);
            int realLinha = linhasNaoUsadas[posicaoLinha];

            // Remove a linha selecionada do vetor de linhas não usadas e adiciona ao vetor de linhas afetadas
            linhasNaoUsadas.erase(linhasNaoUsadas.begin() + posicaoLinha);
            linhasAfetadas[i] = realLinha;
        }
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
            std::uniform_int_distribution<int> dist(0, solucaoCopia[linhasAfetadas[i]].size() - 1);

            // Obtém uma posição aleatória e o produto correspondente
            int posicao = dist(gen);
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

vector<vector<int>> heuristicaILS(vector<int> &vetorProdutos, vector<vector<int>> &matrizPreparacao, int numeroLinhas, int produtos, int numeroIteracoes)
{
    vector<vector<int>> melhorSolucao;
    int tempoAtual = 0;

    double tempo;
    chrono::duration<double> duracao;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, produtos - 1);

    for (int r = 0; r < numeroIteracoes; r++)
    {
        vector<vector<int>> solucaoInicial(numeroLinhas, vector<int>());
        unordered_set<int> produtosUsados;

        int linha = 0;
        for (int i = 0; i < produtos; i++)
        {
            int produtoAtual = dist(gen);

            while (produtosUsados.count(produtoAtual) > 0)
            {
                produtoAtual = dist(gen);
            }

            produtosUsados.insert(produtoAtual);
            solucaoInicial[linha].push_back(produtoAtual);

            linha += 1;
            if (linha == numeroLinhas)
                linha = 0;
        }

        solucaoInicial = perturbacao(solucaoInicial, numeroLinhas);

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