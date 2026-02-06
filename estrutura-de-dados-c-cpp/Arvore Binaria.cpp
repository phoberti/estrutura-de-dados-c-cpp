/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <iostream>

class No
{
public:
    No *esq, *dir;
    int chave;

public:
    No(int chave)
    {
        this->chave = chave;
        esq = NULL;
        dir = NULL;
    }
    int getChave() { return chave; }
    No *getEsq() { return esq; }
    No *getDir() { return dir; }
    void setEsq(No *no) { esq = no; }
    void setDir(No *no) { dir = no; }
};

class Arvore
{
public:
    No *raiz;

public:
    Arvore() { raiz = NULL; }

    void inserir(int chave)
    {
        if (raiz == NULL)
            raiz = new No(chave);
        else
            inserirAux(raiz, chave);
    }

    void inserirAux(No *no, int chave)
    {
        if (chave < no->getChave())
        {
            if (no->getEsq() == NULL)
            {
                No *novo_no = new No(chave);
                no->setEsq(novo_no);
            }
            else
            {
                inserirAux(no->getEsq(), chave);
            }
        }
        else if (chave > no->getChave())
        {
            if (no->getDir() == NULL)
            {
                No *novo_no = new No(chave);
                no->setDir(novo_no);
            }
            else
            {
                inserirAux(no->getDir(), chave);
            }
        }
        else if (chave == no->getChave())
        {
            std::cout << "Numero ja existe..." << std::endl;
        }
    }

    No *getRaiz() { return raiz; }

    void preOrdem(No *no)
    {
        if (no != NULL)
        {
            std::cout << no->getChave() << " ";
            preOrdem(no->getEsq());
            preOrdem(no->getDir());
        }
    }

    void emOrdem(No *no)
    {
        if (no != NULL)
        {
            emOrdem(no->getEsq());
            std::cout << no->getChave() << " ";
            emOrdem(no->getDir());
        }
    }

    void posOrdem(No *no)
    {
        if (no != NULL)
        {
            posOrdem(no->getEsq());
            posOrdem(no->getDir());
            std::cout << no->getChave() << " ";
        }
    }

    // Função para encontrar um nó na árvore
    No* encontrarNo(No* no, int chave)
    {
        if (no == NULL || no->getChave() == chave)
            return no;

        if (chave < no->getChave())
            return encontrarNo(no->getEsq(), chave);
        
        return encontrarNo(no->getDir(), chave);
    }

    // Função para remover um nó da árvore
No* removerNo(No* no, int chave)
{
    if (no == NULL)
        return no;

    // Encontrando o nó a ser removido
    if (chave < no->getChave())
        no->setEsq(removerNo(no->getEsq(), chave));
    else if (chave > no->getChave())
        no->setDir(removerNo(no->getDir(), chave));
    else
    {
        // Caso 1: Nó sem filhos
        if (no->getEsq() == NULL && no->getDir() == NULL)
        {
            delete no;
            return NULL;
        }
        // Caso 2: Nó com apenas um filho
        else if (no->getEsq() == NULL)
        {
            No* temp = no->getDir();
            delete no;
            return temp;
        }
        else if (no->getDir() == NULL)
        {
            No* temp = no->getEsq();
            delete no;
            return temp;
        }
        // Caso 3: Nó com dois filhos
        else
        {
            No* temp = encontrarMinimo(no->getDir());
            no->chave = temp->chave;
            no->setDir(removerNo(no->getDir(), temp->getChave()));
        }
    }
    return no;
}


    // Função auxiliar para encontrar o nó mínimo em uma subárvore (nó mais à esquerda)
    No* encontrarMinimo(No* no)
    {
        while (no->getEsq() != NULL)
            no = no->getEsq();
        return no;
    }

    // Função para remover um número da árvore
    void remover(int chave)
    {
        raiz = removerNo(raiz, chave);
    }

    // Função para exibir as opções de remoção
    void exibirOpcoesRemocao(No* no)
    {
        if (no == NULL)
            return;
        
        std::cout << "Situação do nó com chave " << no->getChave() << ":" << std::endl;
        if (no->getEsq() == NULL && no->getDir() == NULL)
        {
            std::cout << "Nó folha" << std::endl;
        }
        else if (no->getEsq() == NULL)
        {
            std::cout << "Nó sem filho à esquerda" << std::endl;
        }
        else if (no->getDir() == NULL)
        {
            std::cout << "Nó sem filho à direita" << std::endl;
        }
        else
        {
            std::cout << "Nó com dois filhos" << std::endl;
        }
    }
};

int main()
{
    int escolha;
    int numero;
    Arvore arvore;

    while (true)
    {
        std::cout << "\nEscolha uma opção:" << std::endl;
        std::cout << "1. Adicionar número à árvore" << std::endl;
        std::cout << "2. Remover número da árvore" << std::endl;
        std::cout << "3. Visualizar árvore em Pré-ordem" << std::endl;
        std::cout << "4. Visualizar árvore em Ordem" << std::endl;
        std::cout << "5. Visualizar árvore em Pós-ordem" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        switch (escolha)
        {
        case 1:
            std::cout << "Digite o número a ser adicionado à árvore: ";
            std::cin >> numero;
            arvore.inserir(numero);
            break;
        case 2:
            std::cout << "Digite o número a ser removido da árvore: ";
            std::cin >> numero;
            if (arvore.encontrarNo(arvore.getRaiz(), numero) != NULL)
            {
                arvore.exibirOpcoesRemocao(arvore.encontrarNo(arvore.getRaiz(), numero));
                arvore.remover(numero);
                std::cout << "Número removido com sucesso." << std::endl;
            }
            else
            {
                std::cout << "Número não encontrado na árvore." << std::endl;
            }
            break;
        case 3:
            std::cout << "Árvore em Pré-ordem: ";
            arvore.preOrdem(arvore.getRaiz());
            std::cout << std::endl;
            break;
        case 4:
            std::cout << "Árvore em Ordem: ";
            arvore.emOrdem(arvore.getRaiz());
            std::cout << std::endl;
            break;
        case 5:
            std::cout << "Árvore em Pós-ordem: ";
            arvore.posOrdem(arvore.getRaiz());
            std::cout << std::endl;
            break;
        case 0:
            return 0;
        default:
            std::cout << "Escolha inválida. Tente novamente." << std::endl;
        }
    }

    return 0;
}
