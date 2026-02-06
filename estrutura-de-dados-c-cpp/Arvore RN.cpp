/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <iostream>
// a operacao de busca está dentro da remocao que verifica se o nó está na arvore
class No {
public:
    No *esq, *dir, *pai;
    int chave;
    int cor;

    No(int chave) {
        this->chave = chave;
        esq = NULL;
        dir = NULL;
        pai = NULL;
        cor = 0;
    }

    int getChave() { return chave; }
    No *getEsq() { return esq; }
    No *getDir() { return dir; }
    No *getPai() { return pai; }
    void setEsq(No *no) { esq = no; }
    void setDir(No *no) { dir = no; }
    void setPai(No *no) { pai = no; }
    int getCor() { return cor; }
    void setCor(int cor) { this->cor = cor; }
};

class ArvoreRubroNegra {
public:
    No *raiz;

public:
    ArvoreRubroNegra() { raiz = NULL; }

    void rotacaoEsquerda(No *&raiz, No *&x) {
        No *y = x->dir;
        x->dir = y->esq;
        if (y->esq != NULL)
            y->esq->pai = x;
        y->pai = x->pai;
        if (x->pai == NULL)
            raiz = y;
        else if (x == x->pai->esq)
            x->pai->esq = y;
        else
            x->pai->dir = y;
        y->esq = x;
        x->pai = y;
    }

    void rotacaoDireita(No *&raiz, No *&x) {
        No *y = x->esq;
        x->esq = y->dir;
        if (y->dir != NULL)
            y->dir->pai = x;
        y->pai = x->pai;
        if (x->pai == NULL)
            raiz = y;
        else if (x == x->pai->esq)
            x->pai->esq = y;
        else
            x->pai->dir = y;
        y->dir = x;
        x->pai = y;
    }

    void balancearInsercao(No *&raiz, No *&x) {
        No *pai = NULL;
        No *avo = NULL;
        while (x != raiz && x->cor == 0 && x->pai->cor == 0) {
            pai = x->pai;
            avo = x->pai->pai;
            if (pai == avo->esq) {
                No *tio = avo->dir;
                if (tio != NULL && tio->cor == 0) {
                    avo->cor = 0;
                    pai->cor = 1;
                    tio->cor = 1;
                    x = avo;
                }
                else {
                    if (x == pai->dir) {
                        rotacaoEsquerda(raiz, pai);
                        x = pai;
                        pai = x->pai;
                    }
                    rotacaoDireita(raiz, avo);
                    std::swap(pai->cor, avo->cor);
                    x = pai;
                }
            }
            else {
                No *tio = avo->esq;
                if (tio != NULL && tio->cor == 0) {
                    avo->cor = 0;
                    pai->cor = 1;
                    tio->cor = 1;
                    x = avo;
                }
                else {
                    if (x == pai->esq) {
                        rotacaoDireita(raiz, pai);
                        x = pai;
                        pai = x->pai;
                    }
                    rotacaoEsquerda(raiz, avo);
                    std::swap(pai->cor, avo->cor);
                    x = pai;
                }
            }
        }
        raiz->cor = 1;
    }

    void inserir(int chave) {
        No *novoNo = new No(chave);
        if (raiz == NULL) {
            raiz = novoNo;
            raiz->cor = 1;
        }
        else {
            inserirRecursivo(raiz, novoNo);
            balancearInsercao(raiz, novoNo); // Chamar balanceamento após a inserção
        }
    }

    void inserirRecursivo(No *&raiz, No *&novoNo) {
        if (raiz == NULL) {
            raiz = novoNo;
        }
        else if (novoNo->chave < raiz->chave) {
            novoNo->pai = raiz;
            inserirRecursivo(raiz->esq, novoNo);
        }
        else if (novoNo->chave > raiz->chave) {
            novoNo->pai = raiz;
            inserirRecursivo(raiz->dir, novoNo);
        }
    }

    No *encontrarMinimo(No *no) {
        while (no->esq != NULL)
            no = no->esq;
        return no;
    }

    No *encontrarSucessor(No *no) {
        no = no->dir;
        while (no != NULL && no->esq != NULL)
            no = no->esq;
        return no;
    }

    void balancearRemocao(No *&raiz, No *&no) {
        No *irmao = NULL;
        while (no != raiz && no->cor == 1) {
            if (no == no->pai->esq) {
                irmao = no->pai->dir;
                if (irmao == NULL)
                    break;
                if (irmao->cor == 0) {
                    irmao->cor = 1;
                    no->pai->cor = 0;
                    rotacaoEsquerda(raiz, no->pai);
                    irmao = no->pai->dir;
                }
                if (irmao->esq == NULL && irmao->dir == NULL) {
                    irmao->cor = 0;
                    no = no->pai;
                }
                else {
                    if (irmao->dir == NULL || (irmao->esq != NULL && irmao->esq->cor == 1 && irmao->dir->cor == 1)) {
                        if (irmao->esq != NULL)
                            irmao->esq->cor = 1;
                        irmao->cor = 0;
                        rotacaoDireita(raiz, irmao);
                        irmao = no->pai->dir;
                    }
                    irmao->cor = no->pai->cor;
                    no->pai->cor = 1;
                    irmao->dir->cor = 1;
                    rotacaoEsquerda(raiz, no->pai);
                    no = raiz;
                }
            }
            else {
                irmao = no->pai->esq;
                // Lógica simétrica para o lado direito
            }
        }
        if (no != NULL)
            no->cor = 1;
    }

    void remover(int chave) {
        if (raiz == NULL)
            return;

        No *no = raiz;
        No *noRemovido = NULL;
        while (no != NULL && chave != no->chave) {
            if (chave < no->chave)
                no = no->esq;
            else
                no = no->dir;
        }

        if (no == NULL) {
            std::cout << "Chave não encontrada na árvore." << std::endl;
            return;
        }

        noRemovido = no;

        No *substituto = NULL;
        if (noRemovido->esq == NULL || noRemovido->dir == NULL) {
            substituto = noRemovido;
        }
        else {
            substituto = encontrarSucessor(noRemovido);
        }

        No *filhoSubstituto = NULL;
        if (substituto->esq != NULL) {
            filhoSubstituto = substituto->esq;
        }
        else {
            filhoSubstituto = substituto->dir;
        }

        if (filhoSubstituto != NULL) {
            filhoSubstituto->pai = substituto->pai;
        }

        if (substituto->pai == NULL) {
            raiz = filhoSubstituto;
        }
        else if (substituto == substituto->pai->esq) {
            substituto->pai->esq = filhoSubstituto;
        }
        else {
            substituto->pai->dir = filhoSubstituto;
        }

        if (substituto != noRemovido) {
            noRemovido->chave = substituto->chave;
        }

        if (substituto->cor == 1) {
            balancearRemocao(raiz, filhoSubstituto);
        }

        delete substituto;
    }

    void preOrdem(No *no) {
        if (no != NULL) {
            std::cout << no->chave << " ";
            preOrdem(no->esq);
            preOrdem(no->dir);
        }
    }

    void emOrdem(No *no) {
        if (no != NULL) {
            emOrdem(no->esq);
            std::cout << no->chave << " ";
            emOrdem(no->dir);
        }
    }

    void posOrdem(No *no) {
        if (no != NULL) {
            posOrdem(no->esq);
            posOrdem(no->dir);
            std::cout << no->chave << " ";
        }
    }
};

int main() {
    int escolha;
    int numero;
  
    ArvoreRubroNegra arvore;

     while (true) {
        std::cout << "\nEscolha uma opcao:" << std::endl;
        std::cout << "1. Adicionar numero a arvore" << std::endl;
        std::cout << "2. Remover numero da arvore" << std::endl;
        std::cout << "3. Visualizar arvore em Pre-ordem" << std::endl;
        std::cout << "4. Visualizar arvore em Ordem" << std::endl;
        std::cout << "5. Visualizar arvore em Pos-ordem" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        switch (escolha) {
            case 1:
                std::cout << "Digite o numero a ser adicionado a arvore: ";
                std::cin >> numero;
                arvore.inserir(numero);
                break;
            case 2:
                std::cout << "Digite o numero a ser removido da arvore: ";
                std::cin >> numero;
                arvore.remover(numero);
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
