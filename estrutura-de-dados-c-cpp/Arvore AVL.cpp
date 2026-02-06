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
    int altura;

public:
    No(int chave)
    {
        this->chave = chave;
        esq = NULL;
        dir = NULL;
        altura = 1;
    }
    int getChave() { return chave; }
    No *getEsq() { return esq; }
    No *getDir() { return dir; }
    void setEsq(No *no) { esq = no; }
    void setDir(No *no) { dir = no; }
    int getAltura() { return altura; }
};

class ArvoreAVL
{
public:
    No *raiz;

public:
    ArvoreAVL() { raiz = NULL; }

    // Função para calcular a altura de um nó
    int altura(No *no)
    {
        if (no == NULL)
            return 0;
        return no->getAltura();
    }

    // Função para calcular o fator de balanceamento de um nó
    int fatorBalanceamento(No *no)
    {
        if (no == NULL)
            return 0;
        return altura(no->getEsq()) - altura(no->getDir());
    }

    // Função para atualizar a altura de um nó
    void atualizarAltura(No *no)
    {
        if (no == NULL)
            return;
        no->altura = 1 + std::max(altura(no->getEsq()), altura(no->getDir()));
    }

    // Função para realizar uma rotação simples para a direita (RR rotation)
    No* rotacionarRR(No *p)
    {
        No *temp = p->getEsq();
        p->setEsq(temp->getDir());
        temp->setDir(p);
        atualizarAltura(p);
        atualizarAltura(temp);
        return temp;
    }

    // Função para realizar uma rotação simples para a esquerda (LL rotation)
    No* rotacionarLL(No *p)
    {
        No *temp = p->getDir();
        p->setDir(temp->getEsq());
        temp->setEsq(p);
        atualizarAltura(p);
        atualizarAltura(temp);
        return temp;
    }

    // Função para inserir um nó na árvore AVL. Dentro dessa Função são tratados casos de rotação duplas sem precisar implementar outras funções para casos de rotações duplas
    No* inserir(No* no, int chave)
    {
        if (no == NULL)
            return new No(chave);

        if (chave < no->getChave())
            no->setEsq(inserir(no->getEsq(), chave));
        else if (chave > no->getChave())
            no->setDir(inserir(no->getDir(), chave));
        else // Duplicatas não são permitidas em uma ABB
            return no;

        // Atualizar altura do nó atual
        atualizarAltura(no);

        // Verificar e realizar rotações se necessário
        int balanceamento = fatorBalanceamento(no);

        // Caso LL
        if (balanceamento > 1 && chave < no->getEsq()->getChave())
            return rotacionarRR(no);

        // Caso RR
        if (balanceamento < -1 && chave > no->getDir()->getChave())
            return rotacionarLL(no);

        // Caso LR
        if (balanceamento > 1 && chave > no->getEsq()->getChave())
        {
            no->setEsq(rotacionarLL(no->getEsq()));
            return rotacionarRR(no);
        }

        // Caso RL
        if (balanceamento < -1 && chave < no->getDir()->getChave())
        {
            no->setDir(rotacionarRR(no->getDir()));
            return rotacionarLL(no);
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

    // Função para remover um nó da árvore AVL
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
            // Caso 1: Nó sem filhos ou com apenas um filho
            if (no->getEsq() == NULL || no->getDir() == NULL)
            {
                No* temp = no->getEsq() ? no->getEsq() : no->getDir();
                
                // Caso nó sem filhos
                if (temp == NULL)
                {
                    temp = no;
                    no = NULL;
                }
                else // Caso nó com um filho
                    *no = *temp; // Copia o conteúdo do filho para o nó

                delete temp;
            }
            else // Caso 3: Nó com dois filhos
            {
                No* temp = encontrarMinimo(no->getDir());
                no->chave = temp->getChave();
                no->setDir(removerNo(no->getDir(), temp->getChave()));
            }
        }

        // Se a árvore tinha apenas um nó e foi removido
        if (no == NULL)
            return no;

        // Atualizar altura do nó atual
        atualizarAltura(no);

        // Verificar e realizar rotações se necessário
        int balanceamento = fatorBalanceamento(no);

        // Caso LL
        if (balanceamento > 1 && fatorBalanceamento(no->getEsq()) >= 0)
            return rotacionarRR(no);

        // Caso RR
        if (balanceamento < -1 && fatorBalanceamento(no->getDir()) <= 0)
            return rotacionarLL(no);

        // Caso LR
        if (balanceamento > 1 && fatorBalanceamento(no->getEsq()) < 0)
        {
            no->setEsq(rotacionarLL(no->getEsq()));
            return rotacionarRR(no);
        }

        // Caso RL
        if (balanceamento < -1 && fatorBalanceamento(no->getDir()) > 0)
        {
            no->setDir(rotacionarRR(no->getDir()));
            return rotacionarLL(no);
        }

        return no;
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

    // Função para remover um número da árvore AVL
    void remover(int chave)
    {
        raiz = removerNo(raiz, chave);
    }

    // Função para exibir as opções de remoção
    void exibirOpcoesRemocao(No* no)
    {
        if (no == NULL)
            return;
        
        std::cout << "Situacao do no com chave " << no->getChave() << ":" << std::endl;
        if (no->getEsq() == NULL && no->getDir() == NULL)
        {
            std::cout << "No folha" << std::endl;
        }
        else if (no->getEsq() == NULL)
        {
            std::cout << "No sem filho a esquerda" << std::endl;
        }
        else if (no->getDir() == NULL)
        {
            std::cout << "No sem filho a direita" << std::endl;
        }
        else
        {
            std::cout << "No com dois filhos" << std::endl;
        }
    }

    // Função para realizar a inserção na árvore AVL
    void inserir(int chave)
    {
        raiz = inserir(raiz, chave);
    }

    // Função para percorrer a árvore em Pré-ordem
    void preOrdem(No *no)
    {
        if (no != NULL)
        {
            std::cout << no->getChave() << " ";
            preOrdem(no->getEsq());
            preOrdem(no->getDir());
        }
    }

    // Função para percorrer a árvore em Ordem
    void emOrdem(No *no)
    {
        if (no != NULL)
        {
            emOrdem(no->getEsq());
            std::cout << no->getChave() << " ";
            emOrdem(no->getDir());
        }
    }

    // Função para percorrer a árvore em Pós-ordem
    void posOrdem(No *no)
    {
        if (no != NULL)
        {
            posOrdem(no->getEsq());
            posOrdem(no->getDir());
            std::cout << no->getChave() << " ";
        }
    }



};

int main()
{
        int escolha;
    int numero;
    ArvoreAVL arvore;

    while (true)
    {
        std::cout << "\nEscolha uma opcao:" << std::endl;
        std::cout << "1. Adicionar numero a arvore" << std::endl;
        std::cout << "2. Remover numero da arvore" << std::endl;
        std::cout << "3. Visualizar arvore em Pre-ordem" << std::endl;
        std::cout << "4. Visualizar arvore em Ordem" << std::endl;
        std::cout << "5. Visualizar arvore em Pos-ordem" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        switch (escolha)
        {
        case 1:
            std::cout << "Digite o numero a ser adicionado a arvore: ";
            std::cin >> numero;
            arvore.inserir(numero);
            break;
        case 2:
            std::cout << "Digite o numero a ser removido da arvore: ";
            std::cin >> numero;
            if (arvore.encontrarNo(arvore.raiz, numero) != NULL)
            {
                arvore.exibirOpcoesRemocao(arvore.encontrarNo(arvore.raiz, numero));
                arvore.remover(numero);
                std::cout << "Numero removido com sucesso." << std::endl;
            }
            else
            {
                std::cout << "Numero não encontrado na arvore." << std::endl;
            }
            break;
        case 3:
            std::cout << "Arvore em Pre-ordem: ";
            arvore.preOrdem(arvore.raiz);
            std::cout << std::endl;
            break;
        case 4:
            std::cout << "Arvore em Ordem: ";
            arvore.emOrdem(arvore.raiz);
            std::cout << std::endl;
            break;
        case 5:
            std::cout << "Arvore em Pos-ordem: ";
            arvore.posOrdem(arvore.raiz);
            std::cout << std::endl;
            break;
        case 0:
            return 0;
        default:
            std::cout << "Escolha invalida. Tente novamente." << std::endl;
        }
    }

    return 0;
}
