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
    // Verifica se a entrada contém apenas números, vírgula ou ponto
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
        cout << "Erro: O ID deve conter apenas números." << endl;
        return;
    }
    novoProduto.id = stoi(entrada);

    if (idExiste(novoProduto.id)) {
        cout << "Erro: Já existe um produto com esse ID." << endl;
        return;
    }
    limparBuffer();

    // Entrada do nome (não restrito a números)
    cout << "Nome do produto: ";
    getline(cin, novoProduto.nome);

    // Entrada do preço
    cout << "Preço: ";
    cin >> entrada;

    if (!precoValido(entrada)) {
        cout << "Erro: O preço deve conter apenas números, ponto ou vírgula." << endl;
        return;
    }

    // Substitui a vírgula por ponto para garantir que o número seja interpretado corretamente
    replace(entrada.begin(), entrada.end(), ',', '.');

    novoProduto.preco = stof(entrada);

    // Entrada do estoque
    cout << "Estoque inicial: ";
    cin >> entrada;
    if (!somenteDigitos(entrada)) {
        cout << "Erro: O estoque deve conter apenas números." << endl;
        return;
    }
    novoProduto.estoque = stoi(entrada);

    // Entrada do tipo de venda (por peso)
    cout << "O produto é vendido por peso? (1 para sim, 0 para não): ";
    cin >> entrada;
    if (entrada != "1" && entrada != "0") {
        cout << "Erro: Entrada inválida. Use 1 para sim e 0 para não." << endl;
        return;
    }
    novoProduto.porPeso = (entrada == "1");

    // Entrada do limite de estoque
    cout << "Limite de estoque: ";
    cin >> entrada;
    if (!somenteDigitos(entrada)) {
        cout << "Erro: O limite de estoque deve conter apenas números." << endl;
        return;
    }
    novoProduto.limiteEstoque = stoi(entrada);

    // Entrada da qualidade
    cout << "Produto de boa qualidade? (1 para sim, 0 para descarte): ";
    cin >> entrada;
    if (entrada != "1" && entrada != "0") {
        cout << "Erro: Entrada inválida. Use 1 para sim e 0 para descarte." << endl;
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

        // Editar preço
        cout << "Novo preço (atualmente: " << p.preco << "): ";
        cin >> entrada;
        if (precoValido(entrada)) {
            replace(entrada.begin(), entrada.end(), ',', '.');
            p.preco = stof(entrada);
        } else {
            cout << "Preço inválido. Não foi alterado." << endl;
        }

        // Editar estoque
        cout << "Novo estoque (atualmente: " << p.estoque << "): ";
        cin >> entrada;
        if (somenteDigitos(entrada)) {
            p.estoque = stoi(entrada);
        } else {
            cout << "Estoque inválido. Não foi alterado." << endl;
        }

        // Editar qualidade
        cout << "Nova qualidade (1 para boa, 0 para descartado) (atualmente: "
             << (p.qualidade ? "Boa" : "Descartado") << "): ";
        cin >> entrada;
        if (entrada == "1" || entrada == "0") {
            p.qualidade = (entrada == "1");
        } else {
            cout << "Qualidade inválida. Não foi alterada." << endl;
        }

        salvarProdutos();
        cout << "Produto atualizado com sucesso!" << endl;
    } else {
        cout << "Produto não encontrado." << endl;
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
        cout << "Produto não encontrado." << endl;
    }
}

void exibirProdutos() {
    if (produtos.empty()) {
        cout << "Nenhum produto cadastrado." << endl;
    } else {
        for (const auto& produto : produtos) {
            // Verifica se o preço é inteiro ou decimal
            string precoStr = to_string(produto.preco);

            // Se for decimal, converte o ponto para vírgula
            if (precoStr.find('.') != string::npos) {
                replace(precoStr.begin(), precoStr.end(), '.', ',');
            } else {
                // Se for inteiro, remove as casas decimais
                precoStr = to_string((int)produto.preco);
            }

            cout << "ID: " << produto.id << ", Nome: " << produto.nome
                 << ", Preço: " << precoStr << ", Estoque: " << produto.estoque
                 << ", Vendido por Peso: " << (produto.porPeso ? "Sim" : "Não")
                 << ", Limite de Estoque: " << produto.limiteEstoque
                 << ", Qualidade: " << (produto.qualidade ? "Boa" : "Descartado") << endl;
        }
    }
}

void menuAdministrador() {
    int opcao;
    do {
        carregarProdutos();
        cout << "\n** Área do Administrador **" << endl;
        cout << "1. Cadastrar Produto" << endl;
        cout << "2. Ver Estoque" << endl;
        cout << "3. Editar Produto" << endl;
        cout << "4. Remover Produto" << endl;
        cout << "5. Sair" << endl;
        cout << "Escolha uma opção: ";
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
                cout << "Saindo da Área do Administrador..." << endl;
                break;
            default:
                cout << "Opção inválida!" << endl;
        }
    } while (opcao != 5);
}

int main() {
    setlocale(LC_ALL, "");
    carregarProdutos();
    menuAdministrador();
    return 0;
}
