#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <locale.h>
#include <iomanip>

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
vector<Produto> carrinho;

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

            p.qualidade = stoi(linha);  // Carrega a qualidade do produto
            if (p.qualidade) {  // Adiciona apenas produtos de boa qualidade
                produtos.push_back(p);
            }
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

void exibirProdutos() {
    carregarProdutos();
    cout << "\n--- Produtos Disponíveis ---" << endl;
    for (const auto& produto : produtos) {
        cout << "ID: " << produto.id << " | Nome: " << produto.nome
             << " | Preço: R$" << fixed << setprecision(2) << produto.preco
             << " | Estoque: " << produto.estoque << endl;
    }
    cout << "-----------------------------" << endl;
}

void adicionarAoCarrinho(int id, int quantidade) {
    for (auto& produto : produtos) {
        if (produto.id == id) {
            if (produto.estoque >= quantidade) {
                produto.estoque -= quantidade;
                Produto item = produto;
                item.estoque = quantidade;
                carrinho.push_back(item);
                cout << "Produto '" << produto.nome << "' adicionado ao carrinho! Quantidade: " << quantidade << endl;
            } else {
                cout << "Quantidade indisponível em estoque. Estoque atual de '" << produto.nome << "': " << produto.estoque << endl;
            }
            return;
        }
    }
    cout << "Produto não encontrado." << endl;
}

void removerDoCarrinho(int id) {
    for (auto it = carrinho.begin(); it != carrinho.end(); ++it) {
        if (it->id == id) {
            int opcao, quantidade;
            cout << "Produto '" << it->nome << "' encontrado no carrinho. Quantidade atual no carrinho: " << it->estoque << endl;
            cout << "Deseja:\n";
            cout << "1. Remover quantidade específica\n";
            cout << "2. Remover o item inteiro\n";
            cout << "Escolha uma opção: ";
            cin >> opcao;

            if (opcao == 1) {
                cout << "Digite a quantidade a ser removida (máximo " << it->estoque << "): ";
                cin >> quantidade;

                if (quantidade <= it->estoque) {
                    it->estoque -= quantidade;
                    cout << "Quantidade atualizada no carrinho. Quantidade restante de '" << it->nome << "': " << it->estoque << endl;
                    // Retornar quantidade ao estoque
                    for (auto& produto : produtos) {
                        if (produto.id == id) {
                            produto.estoque += quantidade;
                            break;
                        }
                    }
                } else {
                    cout << "Quantidade inválida! Você tentou remover mais do que a quantidade no carrinho." << endl;
                }
            } else if (opcao == 2) {
                for (auto& produto : produtos) {
                    if (produto.id == id) {
                        produto.estoque += it->estoque;
                        break;
                    }
                }
                cout << "Item removido do carrinho." << endl;
                carrinho.erase(it);
            } else {
                cout << "Opção inválida!" << endl;
            }
            return;
        }
    }
    cout << "Produto não encontrado no carrinho." << endl;
}

void exibirCarrinho() {
    if (carrinho.empty()) {
        cout << "O carrinho está vazio." << endl;
        return;
    }
    cout << "\n--- Itens no Carrinho ---" << endl;
    float totalCarrinho = 0;
    for (const auto& item : carrinho) {
        float totalItem = item.estoque * item.preco;
        cout << "Nome: " << item.nome << " | Quantidade: " << item.estoque
             << " | Total: R$" << fixed << setprecision(2) << totalItem << endl;
        totalCarrinho += totalItem;
    }
    cout << "Total do Carrinho: R$" << fixed << setprecision(2) << totalCarrinho << endl;
    cout << "--------------------------" << endl;
}

void finalizarCompra() {
    float total = 0;
    for (const auto& item : carrinho) {
        total += item.preco * item.estoque;
    }

    int metodoPagamento;
    cout << "\nEscolha o método de pagamento:\n";
    cout << "1. Cartão de Crédito (Juros de 5%)\n";
    cout << "2. Cartão de Débito\n";
    cout << "3. Dinheiro (Desconto de 10%)\n";
    cout << "Escolha uma opção: ";
    cin >> metodoPagamento;

    switch (metodoPagamento) {
        case 1:
            total *= 1.05;  // Juros de 5%
            cout << "Pagamento no cartão de crédito. Juros de 5% aplicados.\n";
            break;
        case 2:
            cout << "Pagamento no cartão de débito. Sem juros ou desconto.\n";
            break;
        case 3:
            total *= 0.90;  // Desconto de 10%
            cout << "Pagamento em dinheiro. Desconto de 10% aplicado.\n";
            break;
        default:
            cout << "Método de pagamento inválido.\n";
            return;
    }

    carrinho.clear();
    salvarProdutos();
    cout << "\nCompra finalizada! Total a pagar: R$" << fixed << setprecision(2) << total << endl;
    cout << "Obrigado por comprar conosco!" << endl;
}

void menuCaixa() {
    int opcao;
    do {
        cout << "\n** Área do Cliente **" << endl;
        cout << "1. Ver Produtos Disponíveis" << endl;
        cout << "2. Adicionar Produto ao Carrinho" << endl;
        cout << "3. Remover Produto do Carrinho" << endl;
        cout << "4. Ver Carrinho" << endl;
        cout << "5. Finalizar Compra" << endl;
        cout << "6. Sair" << endl;
        cout << "Escolha uma opção: ";
        cin >> opcao;

        int id, quantidade;
        switch (opcao) {
            case 1:
                exibirProdutos();
                break;
            case 2:
                cout << "Digite o ID do produto que deseja adicionar ao carrinho: ";
                cin >> id;
                cout << "Digite a quantidade desejada para o produto '" << id << "': ";
                cin >> quantidade;
                adicionarAoCarrinho(id, quantidade);
                break;
            case 3:
                cout << "Digite o ID do produto para remover do carrinho: ";
                cin >> id;
                removerDoCarrinho(id);
                break;
            case 4:
                exibirCarrinho();
                break;
            case 5:
                finalizarCompra();
                break;
            case 6:
                cout << "Obrigado por visitar! Volte sempre!" << endl;
                break;
            default:
                cout << "Opção inválida! Tente novamente." << endl;
        }
    } while (opcao != 6);
}

int main() {
    setlocale(LC_ALL, "");
    carregarProdutos();
    menuCaixa();
    return 0;
}
