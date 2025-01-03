#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <locale.h>

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

            p.qualidade = stoi(linha);
            produtos.push_back(p);
        }
        arquivo.close();
    }
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

void verificarEstoqueBaixo() {
    carregarProdutos();
    bool estoqueBaixo = false;
    cout << "\n** Produtos com Estoque Abaixo do Limite **" << endl;
    for (const auto& produto : produtos) {
        if (produto.estoque < produto.limiteEstoque) {
            cout << "- Produto: " << produto.nome
                 << " (ID: " << produto.id << ") | Estoque atual: " << produto.estoque
                 << " | Limite mínimo: " << produto.limiteEstoque << endl;
            estoqueBaixo = true;
        }
    }
    if (!estoqueBaixo) {
        cout << "Todos os produtos estão com estoque acima do limite mínimo." << endl;
    }
}

void atualizarEstoqueFornecedor(int id, int quantidade) {
    carregarProdutos();
    for (auto& produto : produtos) {
        if (produto.id == id) {
            produto.estoque += quantidade;
            cout << "\n** Estoque Atualizado **" << endl;
            cout << "Produto: " << produto.nome << " | Novo Estoque: " << produto.estoque << endl;
            salvarProdutos();
            return;
        }
    }
    cout << "Erro: Produto com ID " << id << " não encontrado." << endl;
}

void exibirTodosProdutos() {
    cout << "\n** Lista de Produtos **" << endl;
    if (produtos.empty()) {
        cout << "Nenhum produto cadastrado." << endl;
    } else {
        for (const auto& produto : produtos) {
            cout << "- ID: " << produto.id << " | Nome: " << produto.nome
                 << " | Preço: R$ " << produto.preco << " | Estoque: " << produto.estoque
                 << (produto.porPeso ? " (Por peso)" : " (Por unidade)")
                 << " | Limite Estoque: " << produto.limiteEstoque
                 << " | Qualidade: " << (produto.qualidade ? "Boa" : "Descartado") << endl;
        }
    }
}

void menuFornecedor() {
    int opcao;
    do {
        carregarProdutos();
        cout << "\n*** Área do Fornecedor ***" << endl;
        cout << "Escolha uma opção:" << endl;
        cout << "1. Verificar Estoque Baixo" << endl;
        cout << "2. Atualizar Estoque de um Produto" << endl;
        cout << "3. Exibir Todos os Produtos" << endl;
        cout << "4. Sair" << endl;
        cout << "Opção: ";
        cin >> opcao;

        int id, quantidade;
        switch (opcao) {
            case 1:
                verificarEstoqueBaixo();
                break;
            case 2:
                cout << "Digite o ID do produto que deseja atualizar: ";
                cin >> id;
                cout << "Quantidade para adicionar ao estoque: ";
                cin >> quantidade;
                atualizarEstoqueFornecedor(id, quantidade);
                break;
            case 3:
                exibirTodosProdutos();
                break;
            case 4:
                cout << "Saindo da Área do Fornecedor..." << endl;
                break;
            default:
                cout << "Opção inválida! Por favor, escolha uma opção entre 1 e 4." << endl;
        }
    } while (opcao != 4);
}

int main() {
    setlocale(LC_ALL, "");
    carregarProdutos();
    menuFornecedor();
    return 0;
}
