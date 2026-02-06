/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <iostream>

class IntDLLNode
{
public:
    IntDLLNode()
    {
        ant = prox = nullptr;
    }
    IntDLLNode(int el, IntDLLNode *prev = nullptr, IntDLLNode *next = nullptr)
    {
        info = el;
        ant = prev;
        prox = next;
    }
    int info;
    IntDLLNode *ant;
    IntDLLNode *prox;
};

class IntDLList
{
public:
    IntDLList()
    {
        topo = fim = nullptr;
    }
    ~IntDLList()
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
    void adicionarAoTopo(int el);
    void adicionarAoFim(int el);
    void deletarDoTopo();
    void deletarDoFim();
    void deletarElemento(int el);
    bool estaNaLista(int el) const;
    void imprimirTudo() const;
    void inserirOrdenadamente(int el);
    void ordenarLista();

private:
    IntDLLNode *topo, *fim;
};

void IntDLList::adicionarAoTopo(int el)
{
    topo = new IntDLLNode(el, nullptr, topo);
    if (fim == nullptr)
    {
        fim = topo;
    }
    if (topo->prox != nullptr)
    {
        topo->prox->ant = topo;
    }
}

void IntDLList::adicionarAoFim(int el)
{
    if (fim != nullptr)
    {
        fim->prox = new IntDLLNode(el, fim, nullptr);
        fim = fim->prox;
    }
    else
    {
        topo = fim = new IntDLLNode(el);
    }
}

void IntDLList::deletarDoTopo()
{
    if (estaVazia())
    {
        std::cout << "Lista Vazia\n";
        return;
    }
    IntDLLNode *tmp = topo;
    topo = topo->prox;
    if (topo != nullptr)
    {
        topo->ant = nullptr;
    }
    delete tmp;
}

void IntDLList::deletarDoFim()
{
    if (estaVazia())
    {
        std::cout << "Lista Vazia\n";
        return;
    }
    if (topo == fim)
    {
        delete topo;
        topo = fim = nullptr;
    }
    else
    {
        IntDLLNode *tmp = fim;
        fim = fim->ant;
        fim->prox = nullptr;
        delete tmp;
    }
}

void IntDLList::deletarElemento(int el)
{
    if (estaVazia())
    {
        std::cout << "Lista Vazia\n";
        return;
    }

    IntDLLNode *atual = topo;

    while (atual != nullptr)
    {
        if (atual->info == el)
        {
            if (atual == topo)
            {
                topo = topo->prox;
                if (topo != nullptr)
                {
                    topo->ant = nullptr;
                }
                delete atual;
                atual = topo;
            }
            else if (atual == fim)
            {
                fim = fim->ant;
                fim->prox = nullptr;
                delete atual;
                atual = nullptr;
            }
            else
            {
                atual->ant->prox = atual->prox;
                atual->prox->ant = atual->ant;
                IntDLLNode *temp = atual;
                atual = atual->prox;
                delete temp;
            }
        }
        else
        {
            atual = atual->prox;
        }
    }
}

bool IntDLList::estaNaLista(int el) const
{
    for (IntDLLNode *tmp = topo; tmp != nullptr; tmp = tmp->prox)
    {
        if (tmp->info == el)
        {
            return true;
        }
    }
    return false;
}

void IntDLList::imprimirTudo() const
{
    for (IntDLLNode *tmp = topo; tmp != nullptr; tmp = tmp->prox)
    {
        std::cout << tmp->info << " -> ";
    }
    std::cout << "null" << std::endl;
}

void IntDLList::inserirOrdenadamente(int el)
{
    IntDLLNode *novo = new IntDLLNode(el);

    if (estaVazia() || el < topo->info)
    {
        novo->prox = topo;
        topo->ant = novo;
        topo = novo;
        if (fim == nullptr)
        {
            fim = topo;
        }
        return;
    }

    IntDLLNode *anterior = topo;
    IntDLLNode *atual = topo->prox;

    while (atual != nullptr && el > atual->info)
    {
        anterior = atual;
        atual = atual->prox;
    }

    anterior->prox = novo;
    novo->ant = anterior;
    novo->prox = atual;
    if (atual != nullptr)
    {
        atual->ant = novo;
    }
    else
    {
        fim = novo;
    }
}

void IntDLList::ordenarLista()
{
    if (estaVazia() || topo == fim)
    {
        // Nada a fazer, lista vazia ou com um único elemento
        return;
    }

    bool trocou;

    do
    {
        trocou = false;
        IntDLLNode *atual = topo;
        IntDLLNode *anterior = nullptr;
        IntDLLNode *seguinte = topo->prox;

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
    IntDLList lista;

    while (true)
    {
        std::cout << "Escolha uma opção:" << std::endl;
        std::cout << "1. Adicionar número ao topo" << std::endl;
        std::cout << "2. Adicionar número ao fim" << std::endl;
        std::cout << "3. Deletar do topo" << std::endl;
        std::cout << "4. Deletar do fim" << std::endl;
        std::cout << "5. Deletar elemento" << std::endl;
        std::cout << "6. Inserir ordenadamente ( Automaticamente Ordena a Lista)" << std::endl;
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
            std::cout << "Digite o número a ser adicionado ao fim: ";
            std::cin >> numero;
            lista.adicionarAoFim(numero);
            break;
        case 3:
            lista.deletarDoTopo();
            break;
        case 4:
            lista.deletarDoFim();
            break;
        case 5:
            std::cout << "Digite o número a ser deletado: ";
            std::cin >> numero;
            lista.deletarElemento(numero);
            break;
        case 6:
            lista.ordenarLista();
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
