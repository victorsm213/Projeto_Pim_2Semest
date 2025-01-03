#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <algorithm>
#include <locale.h>
#include <cctype>
#include <sstream>

using namespace std;

struct Produto {
    int id;
    string nome;
    float preco;
    int estoque;
    bool porPeso;
    int limiteEstoque;
    bool qualidade;  // True se de boa qualidade, False para descarte
};

vector<Produto> produtos;

void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool somenteDigitos(const string& entrada) {
    return all_of(entrada.begin(), entrada.end(), ::isdigit);
}

bool precoValido(const string& entrada) {
    // Verifica se a entrada cont�m apenas n�meros, v�rgula ou ponto
    return all_of(entrada.begin(), entrada.end(), [](char c) {
        return isdigit(c) || c == '.' || c == ',';
    });
}

void salvarProdutos() {
    ofstream arquivo("Hortifruti/produtos.txt");
    if (arquivo.is_open()) {
        for (const auto& produto : produtos) {
            arquivo << produto.id << "," << produto.nome << "," << produto.preco << ","
                    << produto.estoque << "," << produto.porPeso << "," << produto.limiteEstoque << ","
                    << produto.qualidade << endl;
        }
        arquivo.close();
    }
}

void carregarProdutos() {
    ifstream arquivo("Hortifruti/produtos.txt");
    if (arquivo.is_open()) {
        produtos.clear();
        string linha;
        while (getline(arquivo, linha)) {
            Produto p;
            size_t pos = 0;

            pos = linha.find(",");
            p.id = stoi(linha.substr(0, pos));
            linha.erase(0, pos + 1);

            pos = linha.find(",");
            p.nome = linha.substr(0, pos);
            linha.erase(0, pos + 1);

            pos = linha.find(",");
            p.preco = stof(linha.substr(0, pos));
            linha.erase(0, pos + 1);

            pos = linha.find(",");
            p.estoque = stoi(linha.substr(0, pos));
            linha.erase(0, pos + 1);

            pos = linha.find(",");
            p.porPeso = stoi(linha.substr(0, pos));
            linha.erase(0, pos + 1);

            pos = linha.find(",");
            p.limiteEstoque = stoi(linha.substr(0, pos));
            linha.erase(0, pos + 1);

            p.qualidade = stoi(linha);  // Carrega a qualidade do produto
            produtos.push_back(p);
        }
        arquivo.close();
    }
}

bool idExiste(int id) {
    for (const auto& produto : produtos) {
        if (produto.id == id) {
            return true;
        }
    }
    return false;
}

void cadastrarProduto() {
    Produto novoProduto;
    string entrada;

    // Entrada do ID
    cout << "ID do produto: ";
    cin >> entrada;
    if (!somenteDigitos(entrada)) {
        cout << "Erro: O ID deve conter apenas n�meros." << endl;
        return;
    }
    novoProduto.id = stoi(entrada);

    if (idExiste(novoProduto.id)) {
        cout << "Erro: J� existe um produto com esse ID." << endl;
        return;
    }
    limparBuffer();

    // Entrada do nome (n�o restrito a n�meros)
    cout << "Nome do produto: ";
    getline(cin, novoProduto.nome);

    // Entrada do pre�o
    cout << "Pre�o: ";
    cin >> entrada;

    if (!precoValido(entrada)) {
        cout << "Erro: O pre�o deve conter apenas n�meros, ponto ou v�rgula." << endl;
        return;
    }

    // Substitui a v�rgula por ponto para garantir que o n�mero seja interpretado corretamente
    replace(entrada.begin(), entrada.end(), ',', '.');

    novoProduto.preco = stof(entrada);

    // Entrada do estoque
    cout << "Estoque inicial: ";
    cin >> entrada;
    if (!somenteDigitos(entrada)) {
        cout << "Erro: O estoque deve conter apenas n�meros." << endl;
        return;
    }
    novoProduto.estoque = stoi(entrada);

    // Entrada do tipo de venda (por peso)
    cout << "O produto � vendido por peso? (1 para sim, 0 para n�o): ";
    cin >> entrada;
    if (entrada != "1" && entrada != "0") {
        cout << "Erro: Entrada inv�lida. Use 1 para sim e 0 para n�o." << endl;
        return;
    }
    novoProduto.porPeso = (entrada == "1");

    // Entrada do limite de estoque
    cout << "Limite de estoque: ";
    cin >> entrada;
    if (!somenteDigitos(entrada)) {
        cout << "Erro: O limite de estoque deve conter apenas n�meros." << endl;
        return;
    }
    novoProduto.limiteEstoque = stoi(entrada);

    // Entrada da qualidade
    cout << "Produto de boa qualidade? (1 para sim, 0 para descarte): ";
    cin >> entrada;
    if (entrada != "1" && entrada != "0") {
        cout << "Erro: Entrada inv�lida. Use 1 para sim e 0 para descarte." << endl;
        return;
    }
    novoProduto.qualidade = (entrada == "1");

    produtos.push_back(novoProduto);
    salvarProdutos();
    cout << "Produto cadastrado com sucesso!" << endl;
}

void editarProduto() {
    int id;
    string entrada;

    cout << "ID do produto que deseja editar: ";
    cin >> id;

    auto it = find_if(produtos.begin(), produtos.end(), [id](const Produto& p) { return p.id == id; });
    if (it != produtos.end()) {
        Produto& p = *it;

        // Editar pre�o
        cout << "Novo pre�o (atualmente: " << p.preco << "): ";
        cin >> entrada;
        if (precoValido(entrada)) {
            replace(entrada.begin(), entrada.end(), ',', '.');
            p.preco = stof(entrada);
        } else {
            cout << "Pre�o inv�lido. N�o foi alterado." << endl;
        }

        // Editar estoque
        cout << "Novo estoque (atualmente: " << p.estoque << "): ";
        cin >> entrada;
        if (somenteDigitos(entrada)) {
            p.estoque = stoi(entrada);
        } else {
            cout << "Estoque inv�lido. N�o foi alterado." << endl;
        }

        // Editar qualidade
        cout << "Nova qualidade (1 para boa, 0 para descartado) (atualmente: "
             << (p.qualidade ? "Boa" : "Descartado") << "): ";
        cin >> entrada;
        if (entrada == "1" || entrada == "0") {
            p.qualidade = (entrada == "1");
        } else {
            cout << "Qualidade inv�lida. N�o foi alterada." << endl;
        }

        salvarProdutos();
        cout << "Produto atualizado com sucesso!" << endl;
    } else {
        cout << "Produto n�o encontrado." << endl;
    }
}

void removerProduto() {
    int id;
    cout << "ID do produto a ser removido: ";
    cin >> id;

    auto it = find_if(produtos.begin(), produtos.end(), [id](const Produto& p) { return p.id == id; });
    if (it != produtos.end()) {
        produtos.erase(it);
        salvarProdutos();
        cout << "Produto removido com sucesso!" << endl;
    } else {
        cout << "Produto n�o encontrado." << endl;
    }
}

void exibirProdutos() {
    if (produtos.empty()) {
        cout << "Nenhum produto cadastrado." << endl;
    } else {
        for (const auto& produto : produtos) {
            // Verifica se o pre�o � inteiro ou decimal
            string precoStr = to_string(produto.preco);

            // Se for decimal, converte o ponto para v�rgula
            if (precoStr.find('.') != string::npos) {
                replace(precoStr.begin(), precoStr.end(), '.', ',');
            } else {
                // Se for inteiro, remove as casas decimais
                precoStr = to_string((int)produto.preco);
            }

            cout << "ID: " << produto.id << ", Nome: " << produto.nome
                 << ", Pre�o: " << precoStr << ", Estoque: " << produto.estoque
                 << ", Vendido por Peso: " << (produto.porPeso ? "Sim" : "N�o")
                 << ", Limite de Estoque: " << produto.limiteEstoque
                 << ", Qualidade: " << (produto.qualidade ? "Boa" : "Descartado") << endl;
        }
    }
}

void menuAdministrador() {
    int opcao;
    do {
        carregarProdutos();
        cout << "\n** �rea do Administrador **" << endl;
        cout << "1. Cadastrar Produto" << endl;
        cout << "2. Ver Estoque" << endl;
        cout << "3. Editar Produto" << endl;
        cout << "4. Remover Produto" << endl;
        cout << "5. Sair" << endl;
        cout << "Escolha uma op��o: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                exibirProdutos();
                break;
            case 3:
                editarProduto();
                break;
            case 4:
                removerProduto();
                break;
            case 5:
                cout << "Saindo da �rea do Administrador..." << endl;
                break;
            default:
                cout << "Op��o inv�lida!" << endl;
        }
    } while (opcao != 5);
}

int main() {
    setlocale(LC_ALL, "");
    carregarProdutos();
    menuAdministrador();
    return 0;
}
