/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <iostream>

class IntSLLNode
{
public:
    IntSLLNode()
    {
        prox = nullptr;
    }
    IntSLLNode(int el, IntSLLNode *ptr = nullptr)
    {
        info = el;
        prox = ptr;
    }
    int info;
    IntSLLNode *prox;
};

class IntSLList
{
public:
    IntSLList()
    {
        topo = resto = nullptr;
    }
    ~IntSLList()
    {
        while (!estaVazia())
        {
            deletarDoTopo();
        }
    }
    bool estaVazia()
    {
        return topo == nullptr;
    }
    void adicionarAoTopo(int);
    void adicionarAoResto(int);
    void deletarDoTopo();
    void deletarDoResto();
    void deletarElemento(int);
    bool estaNaLista(int) const;
    void imprimirTudo() const;
    void inserirOrdenadamente(int);
    void ordenarLista();

private:
    IntSLLNode *topo, *resto;
};

void IntSLList::adicionarAoTopo(int el)
{
    topo = new IntSLLNode(el, topo);
    if (resto == nullptr)
    {
        resto = topo;
    }
}

void IntSLList::adicionarAoResto(int el)
{
    if (resto != nullptr)
    {
        resto->prox = new IntSLLNode(el);
        resto = resto->prox;
    }
    else
    {
        topo = resto = new IntSLLNode(el);
    }
}

void IntSLList::deletarDoTopo()
{
    if (estaVazia())
    {
        std::cout << "Lista Vazia\n";
        return;
    }
    IntSLLNode *tmp = topo;
    topo = topo->prox;
    delete tmp;
}

void IntSLList::deletarDoResto()
{
    if (estaVazia())
    {
        std::cout << "Lista Vazia\n";
        return;
    }
    if (topo == resto)
    {
        delete topo;
        topo = resto = nullptr;
    }
    else
    {
        IntSLLNode *tmp;
        for (tmp = topo; tmp->prox != resto; tmp = tmp->prox)
        {
            // Não exclua nada aqui
        }
        delete resto;
        resto = tmp;
        resto->prox = nullptr;
    }
}

void IntSLList::deletarElemento(int el)
{
    if (estaVazia())
    {
        std::cout << "Lista Vazia\n";
        return;
    }

    IntSLLNode *anterior = nullptr;
    IntSLLNode *atual = topo;
    IntSLLNode *proximo;

    while (atual != nullptr)
    {
        if (atual->info == el)
        {
            if (anterior != nullptr)
            {
                anterior->prox = atual->prox;
                delete atual;
                atual = anterior->prox;
            }
            else
            {
                // Se o elemento a ser removido é o topo
                topo = topo->prox;
                delete atual;
                atual = topo;
            }
        }
        else
        {
            anterior = atual;
            atual = atual->prox;
        }
    }

    // Verifique se o elemento removido era o último elemento (resto)
    if (resto != nullptr && resto->info == el)
    {
        if (topo == resto)
        {
            delete topo;
            topo = resto = nullptr;
        }
        else
        {
            anterior->prox = nullptr;
            delete resto;
            resto = anterior;
        }
    }
}

bool IntSLList::estaNaLista(int el) const
{
    for (IntSLLNode *tmp = topo; tmp != nullptr; tmp = tmp->prox)
    {
        if (tmp->info == el)
        {
            return true;
        }
    }
    return false;
}

void IntSLList::imprimirTudo() const
{
    for (IntSLLNode *tmp = topo; tmp != nullptr; tmp = tmp->prox)
    {
        std::cout << tmp->info << " -> ";
    }
    std::cout << "null" << std::endl;
}

void IntSLList::inserirOrdenadamente(int el)
{
    IntSLLNode *novo = new IntSLLNode(el);

    if (estaVazia() || el < topo->info)
    {
        novo->prox = topo;
        topo = novo;
        if (resto == nullptr)
        {
            resto = topo;
        }
        return;
    }

    IntSLLNode *anterior = topo;
    IntSLLNode *atual = topo->prox;

    while (atual != nullptr && el > atual->info)
    {
        anterior = atual;
        atual = atual->prox;
    }

    anterior->prox = novo;
    novo->prox = atual;

    if (atual == nullptr)
    {
        resto = novo;
    }
}

void IntSLList::ordenarLista()
{
    if (estaVazia() || topo == resto)
    {
        // Nada a fazer, lista vazia ou com um único elemento
        return;
    }

    bool trocou;

    do
    {
        trocou = false;
        IntSLLNode *atual = topo;
        IntSLLNode *anterior = nullptr;
        IntSLLNode *seguinte = topo->prox;

        while (seguinte != nullptr)
        {
            if (atual->info > seguinte->info)
            {
                if (anterior == nullptr)
                {
                    // Troca os elementos e ajusta o topo
                    topo = seguinte;
                    atual->prox = seguinte->prox;
                    seguinte->prox = atual;
                }
                else
                {
                    anterior->prox = seguinte;
                    atual->prox = seguinte->prox;
                    seguinte->prox = atual;
                }
                trocou = true;
            }
            anterior = atual;
            atual = seguinte;
            seguinte = seguinte->prox;
        }
    } while (trocou);
}

int main()
{
    int escolha;
    int numero;
    IntSLList lista;

    while (true)
    {
        std::cout << "Escolha uma opção:" << std::endl;
        std::cout << "1. Adicionar número ao topo" << std::endl;
        std::cout << "2. Adicionar número ao resto" << std::endl;
        std::cout << "3. Deletar do topo" << std::endl;
        std::cout << "4. Deletar do resto" << std::endl;
        std::cout << "5. Deletar elemento" << std::endl;
        std::cout << "6. Inserir ordenadamente" << std::endl;
        std::cout << "7. Ordenar lista" << std::endl;
        std::cout << "8. Imprimir lista" << std::endl;
        std::cout << "9. Sair" << std::endl;
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        switch (escolha)
        {
        case 1:
            std::cout << "Digite o número a ser adicionado ao topo: ";
            std::cin >> numero;
            lista.adicionarAoTopo(numero);
            break;
        case 2:
            std::cout << "Digite o número a ser adicionado ao resto: ";
            std::cin >> numero;
            lista.adicionarAoResto(numero);
            break;
        case 3:
            lista.deletarDoTopo();
            break;
        case 4:
            lista.deletarDoResto();
            break;
        case 5:
            std::cout << "Digite o número a ser deletado: ";
            std::cin >> numero;
            lista.deletarElemento(numero);
            break;
        case 6:
            std::cout << "Digite o número a ser inserido ordenadamente: ";
            std::cin >> numero;
            lista.inserirOrdenadamente(numero);
            break;
        case 7:
            lista.ordenarLista();
            std::cout << "Lista ordenada." << std::endl;
            break;
        case 8:
            std::cout << "Lista: ";
            lista.imprimirTudo();
            break;
        case 9:
            return 0;
        default:
            std::cout << "Escolha inválida. Tente novamente." << std::endl;
        }
    }

    return 0;
}
