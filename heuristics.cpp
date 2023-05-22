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

// * Movimentos de Vizinhanças - Jonas

vector<vector<int>> trocarProdutosMesmaLinha(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> temposSolucao)
{
    // Inicializa a melhor solução como a solução atual
    vector<vector<int>> melhorSolucao(solucao);

    // Calcula o tempo máximo da solução atual
    int melhorTempoSolucao = *max_element(temposSolucao.begin(), temposSolucao.end());

    // Percorre cada linha de produção na solução
    for (int i = 0; i < solucao.size(); i++)
    {
        // Percorre cada produto na linha de produção
        for (int j = 0; j < solucao[i].size(); j++)
        {
            int produtoAtual = solucao[i][j];

            // Percorre os produtos restantes na mesma linha
            for (int k = j + 1; k < solucao[i].size(); k++)
            {
                // Cria uma cópia da solução atual
                vector<vector<int>> solucaoAtual(solucao);

                // Cria uma cópia dos tempos de produção da solução atual
                vector<int> temposSolucaoAtual(temposSolucao);
                int substituto = solucao[i][k];

                // Realiza a troca entre o produto atual e o substituto
                solucaoAtual[i][j] = substituto;
                solucaoAtual[i][k] = produtoAtual;

                // Atualiza os tempos de produção para refletir a troca de produtos
                if (j != 0)
                {
                    // Caso o produto não esteja no começo, remove e adiciona os novos tempos de preparação de acordo com o produto anterior
                    int produtoAnterior = solucao[i][j - 1];
                    temposSolucaoAtual[i] -= matrizPreparacao[produtoAnterior][produtoAtual] + matrizPreparacao[produtoAnterior][produtoAtual];
                }
                else if (j != solucao[i].size() - 1)
                {
                    // Caso o produto não esteja no final da linha, remove e adiciona os novos tempos de preparação de acordo com o próximo produto
                    int proximoProduto = solucao[i][j + 1];
                    temposSolucaoAtual[i] += matrizPreparacao[substituto][proximoProduto] - matrizPreparacao[produtoAtual][proximoProduto];
                }
                else
                {
                    // Caso o produto esteja no meio da linha, remove e adiciona os tempos de preparação do produto anterior e posterior
                    int proximoProduto = solucao[i][j + 1];
                    int produtoAnterior = solucao[i][j - 1];
                    temposSolucaoAtual[i] -= matrizPreparacao[produtoAnterior][produtoAtual] + matrizPreparacao[produtoAnterior][produtoAtual];
                    temposSolucaoAtual[i] += matrizPreparacao[substituto][proximoProduto] - matrizPreparacao[produtoAtual][proximoProduto];
                }

                // Calcula o tempo máximo da solução atualizada
                int valorSolucaoAtual = *max_element(temposSolucaoAtual.begin(), temposSolucaoAtual.end());

                // Verifica se a solução atualizada é melhor que a melhor solução atual e atualiza-as se necessário
                if (valorSolucaoAtual < melhorTempoSolucao)
                {
                    melhorTempoSolucao = valorSolucaoAtual;
                    melhorSolucao = solucaoAtual;
                }
            }
        }
    }

    return melhorSolucao;
}

vector<vector<int>> trocarProdutosEntreLinhas(vector<vector<int>> solucao, vector<vector<int>> matrizPreparacao, vector<int> vetorProdutos, vector<int> temposSolucao)
{
    // Inicializa a melhor solução como a solução atual
    vector<vector<int>> melhorSolucao(solucao);

    // Calcula o tempo máximo da solução atual
    int melhorTempoSolucao = *max_element(temposSolucao.begin(), temposSolucao.end());

    // Percorre cada linha de produção na solução
    for (int i = 0; i < solucao.size(); i++)
    {
        // Percorre cada produto na linha de produção
        for (int j = 0; j < solucao[i].size(); j++)
        {
            int produtoAtual = solucao[i][j];

            // Percorre as linhas de produção restantes
            for (int k = i + 1; k < solucao.size(); k++)
            {
                // Percorre os produtos nas linhas de produção restantes
                for (int l = 0; l < solucao[k].size(); l++)
                {
                    // Cria uma cópia da solução atual
                    vector<vector<int>> solucaoAtual(solucao);

                    // Cria uma cópia dos tempos de produção da solução atual
                    vector<int> temposSolucaoAtual(temposSolucao);
                    int substituto = solucao[k][l];

                    // Realiza a troca entre o produto atual e o substituto
                    solucaoAtual[i][j] = substituto;
                    solucaoAtual[k][l] = produtoAtual;

                    // Atualiza os tempos de produção para refletir a troca de produtos

                    // Atualiza a linha de produção atual

                    // Remove o tempo do produto substituído
                    temposSolucaoAtual[i] -= vetorProdutos[produtoAtual];

                    // Adiciona o tempo do produto adicionado
                    temposSolucaoAtual[i] += vetorProdutos[substituto];

                    if (j != 0)
                    {
                        // Caso o produto não esteja no começo, remove e adiciona os novos tempos de preparação de acordo com o produto anterior
                        int produtoAnterior = solucao[i][j - 1];
                        temposSolucaoAtual[i] -= matrizPreparacao[produtoAnterior][produtoAtual] + matrizPreparacao[produtoAnterior][produtoAtual];
                    }
                    else if (j != solucao[i].size() - 1)
                    {
                        // Caso o produto não esteja no final da linha, remove e adiciona os novos tempos de preparação de acordo com o próximo produto
                        int proximoProduto = solucao[i][j + 1];
                        temposSolucaoAtual[i] += matrizPreparacao[substituto][proximoProduto] - matrizPreparacao[produtoAtual][proximoProduto];
                    }
                    else
                    {
                        // Caso o produto esteja no meio da linha, remove e adiciona os tempos de preparação do produto anterior e posterior
                        int proximoProduto = solucao[i][j + 1];
                        int produtoAnterior = solucao[i][j - 1];
                        temposSolucaoAtual[i] -= matrizPreparacao[produtoAnterior][produtoAtual] + matrizPreparacao[produtoAnterior][produtoAtual];
                        temposSolucaoAtual[i] += matrizPreparacao[substituto][proximoProduto] - matrizPreparacao[produtoAtual][proximoProduto];
                    }

                    // Atualiza a linha de produção substituta
                    temposSolucaoAtual[k] -= vetorProdutos[substituto];
                    temposSolucaoAtual[k] += vetorProdutos[produtoAtual];
                    if (l != 0)
                    {
                        int produtoAnterior = solucao[k][l - 1];
                        temposSolucaoAtual[k] -= matrizPreparacao[produtoAnterior][produtoAtual] + matrizPreparacao[produtoAnterior][produtoAtual];
                    }
                    else if (l != solucao[k].size() - 1)
                    {
                        int proximoProduto = solucao[k][l + 1];
                        temposSolucaoAtual[k] += matrizPreparacao[substituto][proximoProduto] - matrizPreparacao[produtoAtual][proximoProduto];
                    }
                    else
                    {
                        int proximoProduto = solucao[k][l + 1];
                        int produtoAnterior = solucao[k][l - 1];
                        temposSolucaoAtual[k] -= matrizPreparacao[produtoAnterior][produtoAtual] + matrizPreparacao[produtoAnterior][produtoAtual];
                        temposSolucaoAtual[k] += matrizPreparacao[substituto][proximoProduto] - matrizPreparacao[produtoAtual][proximoProduto];
                    }

                    // Calcula o tempo máximo da solução atualizada
                    int valorSolucaoAtual = *max_element(temposSolucaoAtual.begin(), temposSolucaoAtual.end());

                    // Verifica se a solução atualizada é melhor que a melhor solução atual e atualiza-as se necessário
                    if (valorSolucaoAtual < melhorTempoSolucao)
                    {
                        melhorTempoSolucao = valorSolucaoAtual;
                        melhorSolucao = solucaoAtual;
                    }
                }
            }
        }
    }

    return melhorSolucao;
}

// * Movimentos de Vizinhaças - Vinicius

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
        for (int j = 0; j < solucaoCopia[i].size() / 2; j++)
        {
            // Inverte a ordem dos produtos na linha atual
            int temp = solucaoCopia[i][j];
            solucaoCopia[i][j] = solucaoCopia[i][solucaoCopia[i].size() - 1 - j];
            solucaoCopia[i][solucaoCopia[i].size() - 1 - j] = temp;
        }
    }

    // Avalia a nova configuração das linhas de produção
    vector<int> temposPossiveis = temposProducao(solucaoCopia, matrizPreparacao, vetorProdutos);
    int possivelMaiorTempo = *max_element(temposPossiveis.begin(), temposPossiveis.end());

    if (possivelMaiorTempo < tempoAtual)
    {
        // Atualiza a melhor configuração das linhas de produção
        melhorSolucao = solucaoCopia;
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
            switch (caso)
            {
            case 0:
                // Passa para o caso de movimento vertical (ou trocarProdutosEntreLinhas)
                caso = 1;
                break;
            case 1:
                // Passa para o caso de movimento de inverter
                caso = 2;
                break;
            case 2:
                // Passa para o caso de movimento de inserção
                caso = 3;
                break;
            case 3:
                // Reinicia o caso para o movimento horizontal (ou trocarProdutosMesmaLinha)
                caso = 0;
                // Incrementa o contador de iterações
                contador += 1;
                break;
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

// # NÃO FINALIZADOS

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
