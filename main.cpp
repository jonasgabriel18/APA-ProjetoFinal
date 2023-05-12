#include "funcoes.h"
#include "f_heuristics.h"

using namespace std;

int main()
{
    // Definindo o endereço do arquivo
    string localArquivo = "arquivo.txt";

    // Chamando a função para definir as informações do arquivo
    tuple<int, int, vector<int>, vector<vector<int>>> informacoes = definirInformacoesArquivo(localArquivo);

    // Extraindo as informações retornadas pela função
    int numeroLinhas = get<0>(informacoes);
    int numeroProdutos = get<1>(informacoes);
    vector<int> vetorT = get<2>(informacoes);
    vector<vector<int>> matriz = get<3>(informacoes);

    // Imprimindo as informações extraídas
    cout << "Numero de linhas: " << numeroLinhas << endl;
    cout << "Numero de produtos: " << numeroProdutos << endl;

    cout << "Vetor T: ";
    for (int i = 0; i < vetorT.size(); i++)
    {
        cout << vetorT[i] << " ";
    }
    cout << endl;

    cout << "Matriz: " << endl;
    for (int i = 0; i < matriz.size(); i++)
    {
        for (int j = 0; j < matriz[i].size(); j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    vector<vector<int>> solucao = gerarSolucaoGulosa(numeroProdutos, numeroLinhas, vetorT, matriz);

    cout << ". \n";

    for (int i = 0; i < numeroLinhas; i++)
    {
        for (int j = 0; j < solucao[i].size(); j++)
        {
            cout << solucao[i][j] << " ";
        }
        cout << endl;
    }

    int valor = solution_time(solucao, matriz, vetorT);
    cout << valor << "\n";

    return 0;
}
