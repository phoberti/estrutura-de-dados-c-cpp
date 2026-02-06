/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

/*
chaves testadas: 20,40,10,30,15,35,7,26,18,22,
5,42,13,46,27,8,32,38,24,45,25
*/

/*
Este programa implementa uma árvore B e uma lista encadeada para armazenar registros.
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Define a ordem da árvore B
#define ORDEM 2

// Define o nome do arquivo onde os registros serão armazenados
#define namefile "_arquivo.dat"



// Variáveis globais para contagem e manipulação dos registros
int cont = -1, count = 0;

// Definição da estrutura de um registro
typedef struct {
  int chave;
  char nome[20];
  int idade;
  int ApPage[2];
  int rank;
} Registro;


// Definição da estrutura de uma página da árvore B
typedef struct Pagina_str *Apontador;
typedef struct Pagina_str {
  int n; // Número de registros na página
  int pageNum; // Número da página
  int num; // Número de elementos na página
  Registro r[2*ORDEM]; // Array de registros
  Apontador p[2*ORDEM+1]; // Ponteiros para as páginas filhas
} Pagina;


// Definição da estrutura de um nó da lista encadeada
struct Node {
    Registro info;
    struct Node *prox;
};
typedef struct Node node;


//##########################################################
// Funções auxiliares
void Antecessor(Apontador Ap, int Ind, Apontador ApPai, int *Diminuiu);
void Busca(Registro Reg, Apontador Ap);
void em_ordem(Apontador raiz);
void buscainFile(Registro Reg, Apontador pagina);
int isleaf(Apontador a);
int file_exists(const char *filename);
void Insere(Registro Reg, Apontador *Ap);
void InsereNaPagina(Apontador Ap, Registro Reg, Apontador ApDir);
void Ins(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno);
int Imprime2(Apontador p, int Nivel, int aux, int n);
int Imprime(Apontador p, int N, int aux, int n);
void InsertPosFile(Registro Reg, Apontador *Ap);
node *busca(Registro info, node *LISTA);
void InsPosFile(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno);
void pos_ordem(Apontador raiz);
void Retira(int Ch, Apontador *Ap);
void Ret(int Ch, Apontador *Ap, int *Diminuiu);
void Reconstitui(Apontador ApPag, Apontador ApPai, int PosPai, int *Diminuiu);
void saveAux(Apontador p, int Nivel);
void salvar(Apontador pagina, Registro Reg[]);
void recuperarReg(Apontador *arv, node *LISTA);
void insereInicio(Registro info, node *LISTA);
void execut(Registro info, node *LISTA);
void exibe(node *LISTA);


typedef Apontador TipoDicionario;
// Inicializa a árvore B
void Inicializa(TipoDicionario *Dicionario)
{
  *Dicionario = NULL;
}
 /* Inicializa a estrutura */

// Inicializa a lista encadeada
void inicia(node *LISTA)
{
    LISTA->prox = NULL;
}


//############################### Estrutura 1 Btree
//Comentarios sobre as funções estão todos dentro da cada função

void Antecessor(Apontador Ap, int Ind, Apontador ApPai, int *Diminuiu)
{
  // Verifica se a última página filha de ApPai não é nula
  if (ApPai->p[ApPai->n] != NULL)
  {
    // Chama recursivamente a função Antecessor para encontrar o antecessor
    // mais distante de Ap
    Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);
    // Se houve uma diminuição na página, reconstitui a árvore
    if (*Diminuiu)
      Reconstitui(ApPai->p[ApPai->n], ApPai, ApPai->n, Diminuiu);
    return;
  }
  // Atribui ao registro na posição Ind - 1 de Ap o valor do último registro
  // na página ApPai
  Ap->r[Ind - 1] = ApPai->r[ApPai->n - 1];
  // Decrementa o número de registros na página ApPai
  ApPai->n--;
  // Verifica se ApPai tem menos registros do que o necessário
  *Diminuiu = ApPai->n < ORDEM;
}  /* Antecessor */


void Busca(Registro Reg, Apontador Ap)
{
  int i;

  // Verifica se a página atual é nula
  if (Ap == NULL)
  {
    // Se for nula, exibe mensagem de que a chave não foi encontrada e retorna
    printf("chave nao encontrada: %d\n", Reg.chave);
    return;
  }
  // Inicializa o contador i
  i = 1;
  // Percorre os registros da página até encontrar um maior que o buscado ou chegar ao fim
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;
  // Se a chave buscada for encontrada na página atual
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    // Exibe a chave encontrada e realiza uma busca no arquivo associado a essa chave
    printf("chave: %d \n", Reg.chave);
    buscainFile(Ap->r[i-1],Ap);
    return;
  }
  // Se a chave buscada for menor que o registro na posição i-1
  if (Reg.chave < Ap->r[i - 1].chave)
    // Faz uma busca recursiva na subárvore à esquerda
    Busca(Reg, Ap->p[i-1]);
  else
    // Caso contrário, faz uma busca recursiva na subárvore à direita
    Busca(Reg, Ap->p[i]);
}


void buscainFile(Registro Reg, Apontador pagina)
{
    // Define um array para armazenar os registros da página
    Registro reg[2*ORDEM];
    int i;
    // Abre o arquivo binário para leitura
    FILE *arq = fopen(namefile,"rb");
    // Verifica se o arquivo foi aberto corretamente
    if (arq == NULL)
        // Se não foi possível abrir o arquivo, encerra o programa
        exit(1);
    // Move o ponteiro do arquivo para a posição correspondente à página
    fseek(arq, pagina->pageNum*(2*ORDEM*sizeof(Registro)), SEEK_SET);
    // Lê os registros da página do arquivo
    fread(reg, (2*ORDEM*sizeof(Registro)),1,arq);
    // Fecha o arquivo
    fclose(arq);
    // Percorre os registros lidos da página
    for(i = 0; i < 2*ORDEM; i++){
        // Se encontrar a chave correspondente ao registro
        if (Reg.chave == reg[i].chave)
            // Exibe o nome e a idade associados a essa chave
            printf("%s%d\n",reg[i].nome,reg[i].idade);
    }
}


void em_ordem(Apontador raiz)
{
    int i;
    // Verifica se a raiz não é nula
    if (raiz != NULL)
    {
        // Percorre os filhos da raiz
        for (i = 0; i < raiz->n; i++)
        {
            // Visita recursivamente os filhos da raiz em ordem
            em_ordem(raiz->p[i]);
            // Imprime a chave do elemento atual da raiz
            printf("%d ", raiz->r[i].chave);
            printf("\n");
        }
        // Visita recursivamente o último filho da raiz
        em_ordem(raiz->p[i]);
    }
}


int file_exists(const char *filename)
{
    FILE *arquivo;

    // Tenta abrir o arquivo no modo leitura binária
    if ((arquivo = fopen(filename, "rb")))
    {
        // Se o arquivo foi aberto com sucesso, fecha-o
        fclose(arquivo);
        // Retorna 1 para indicar que o arquivo existe
        return 1;
    }
    // Se não foi possível abrir o arquivo, retorna 0 indicando que não existe
    return 0;
}


int isleaf(Apontador a)
{
    // Verifica se o primeiro ponteiro da página é NULL
    // Se for, isso indica que a página é uma folha
    if (a->p[0] == NULL)
        return 1; // Retorna 1 para indicar que a página é uma folha
    else
        return 0; // Retorna 0 para indicar que a página não é uma folha
}


void InsereNaPagina(Apontador Ap, Registro Reg, Apontador ApDir)
{
  int k;
  int NaoAchouPosicao;

  // Obtém o número de elementos na página
  k = Ap->n;
  // Define a flag para indicar que ainda não achou a posição de inserção
  NaoAchouPosicao = k > 0;

  // Enquanto ainda não achou a posição de inserção
  while (NaoAchouPosicao)
  {
    // Se a chave do registro for maior ou igual à chave do elemento na posição atual
    if (Reg.chave >= Ap->r[k - 1].chave)
    {
      // Indica que a posição foi encontrada
      NaoAchouPosicao = 0;
      // Sai do loop
      break;
    }

    // Move o elemento para a direita
    Ap->r[k] = Ap->r[k - 1];
    // Move o ponteiro para a direita
    Ap->p[k + 1] = Ap->p[k];
    // Decrementa k para verificar a próxima posição
    k--;

    // Se k for menor que 1, indica que não há mais posições a serem verificadas
    if (k < 1)
      NaoAchouPosicao = 0;
  }

  // Insere o registro na posição encontrada
  Ap->r[k] = Reg;
  // Insere o ponteiro à direita do registro na posição encontrada
  Ap->p[k + 1] = ApDir;
  // Incrementa o número de elementos na página
  Ap->n++;
}


void Ins(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno)
{
  Apontador ApTemp;
  Registro Aux;
  int i, j;

  // Se a página é nula, indica que a inserção deve ser feita na raiz
  if (Ap == NULL)
  {
    *Cresceu = 1;
    *RegRetorno = Reg;
    *ApRetorno = NULL;
    return;
  }

  // Encontra a posição de inserção na página
  i = 1;
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;

  // Verifica se a chave já existe na página
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    printf("chave ja existente: %d \n", Reg.chave);
    *Cresceu = 0;
    return;
  }

  // Realiza a inserção recursivamente na página adequada
  if (Reg.chave < Ap->r[i - 1].chave)
    Ins(Reg, Ap->p[i-1], Cresceu, RegRetorno, ApRetorno);
  else
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

  // Se a página não cresceu após a inserção, retorna
  if (!*Cresceu)
    return;

  // Verifica se a página tem espaço para a inserção
  if (Ap->n < 2*ORDEM)
  {
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    *Cresceu = 0;
    return;
  }

  // Se a página precisa ser dividida
  ApTemp = (Apontador) malloc(sizeof(Pagina));
  ApTemp->n = 0;
  ApTemp->p[0] = NULL;
  cont++;
  ApTemp->pageNum = cont;

  // Verifica onde o novo registro deve ser inserido na página dividida
  if (i <= ORDEM + 1)
  {
    InsereNaPagina(ApTemp, Ap->r[2*ORDEM - 1], Ap->p[2*ORDEM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  }
  else
  {
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
  }

  // Move os registros restantes para a nova página
  for (j = ORDEM + 2; j <= 2*ORDEM; j++)
    InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);

  // Atualiza o número de registros na página original
  Ap->n = ORDEM;
  // Atualiza o ponteiro à esquerda da nova página
  ApTemp->p[0] = Ap->p[ORDEM + 1];
  // Atualiza o registro de retorno
  *RegRetorno = Ap->r[ORDEM];
  // Atualiza o ponteiro de retorno
  *ApRetorno = ApTemp;

  // Limpa os registros na página original após a separação
  for (j = ORDEM; j < Ap->n+2; j++)
  {
    Aux.chave = 0;
    Aux.rank = 0;
    Ap->r[j] = Aux;
  }
}


void Insere(Registro Reg, Apontador *Ap)
{
  int Cresceu;
  Registro RegRetorno;
  Apontador ApRetorno;
  Apontador ApTemp;

  // Insere o registro na árvore B
  Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

  // Se a árvore cresce na altura pela raiz
  if (Cresceu)
  {
    // Cria uma nova página
    ApTemp = (Apontador) malloc(sizeof(Pagina));
    ApTemp->n = 1;
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    cont++;
    ApTemp->pageNum = cont;
    ApTemp->p[0] = *Ap;
    *Ap = ApTemp;
    // Salva a nova raiz na memória secundária
    saveAux(*Ap, 2*ORDEM);
  }
  // Salva as alterações na memória secundária
  saveAux(*Ap, 2*ORDEM);
}  /*Insercao*/


int Imprime2(Apontador p, int Nivel, int aux, int n)
{
  int i;

  // Verifica se o nível atual é o desejado
  if (p == NULL)
    return 0;
  if (Nivel == aux)
  {
    n++;
    // Imprime os elementos do nó atual
    printf("No: %d : ", p->pageNum);
    for (i = 0; i < p->n; i++)
    {
      // Verifica se o apontador não é nulo e imprime seu valor
      if (p->p[i] != NULL)
        printf("Apontador: %d ", p->p[i]->pageNum);
      else
        printf("Apontador: null ");
      printf("chave: %d ", p->r[i].chave);
    }
    printf("\n");
    return n;
  }
  else
  {
    // Incrementa o nível atual e chama recursivamente a função para os nós filhos
    aux++;
    for (i = 0; i <= p->n; i++)
      Imprime2(p->p[i], Nivel, aux, n);
  }
  return 0;
}


int Imprime(Apontador p, int N, int aux, int n)
{
  int i;

  // Verifica se o nó atual é nulo
  if (p == NULL)
    return 0;
  // Verifica se o número do nó atual é diferente do número desejado
  if (p->pageNum != aux)
  {
    // Atribui o número desejado ao nó atual e atualiza o valor de aux
    p->num = N;
    aux = p->pageNum;
    return aux;
  }
  else
  {
    // Incrementa aux e chama recursivamente a função para os nós filhos
    aux++;
    for (i = 0; i <= p->n; i++)
      Imprime(p->p[i], N, aux, n);
  }
  return 0;
}


void InsertPosFile(Registro Reg, Apontador *Ap)
{
  int Cresceu;
  Registro RegRetorno;
  Apontador ApRetorno;
  Apontador ApTemp;

  // Insere o registro na posição correta na árvore B
  InsPosFile(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

  // Se a árvore cresceu na altura pela raiz, adiciona um novo nó na raiz
  if (Cresceu)
  {
    ApTemp = (Apontador) malloc(sizeof(Pagina));
    ApTemp->n = 1;
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    cont++;
    ApTemp->pageNum = cont;
    ApTemp->p[0] = *Ap;
    *Ap = ApTemp;
  }
}


void InsPosFile(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno)
{
  Apontador ApTemp;
  Registro Aux;
  int i, j;

  // Se a página atual for nula, o registro é inserido e sinaliza que a árvore cresceu
  if (Ap == NULL)
  {
    *Cresceu = 1;
    *RegRetorno = Reg;
    *ApRetorno = NULL;
    return;
  }

  // Busca pela posição correta para inserir o registro na página atual
  i = 1;
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;

  // Se o registro já existe na página, não é inserido novamente
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    *Cresceu = 0;
    return;
  }

  // Insere o registro na página correta da árvore
  if (Reg.chave < Ap->r[i - 1].chave)
    InsPosFile(Reg, Ap->p[i-1], Cresceu, RegRetorno, ApRetorno);
  else
    InsPosFile(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

  // Se a árvore não cresceu na altura, não há mais ações a serem realizadas
  if (!*Cresceu)
    return;

  // Verifica se a página atual tem espaço para inserir o registro
  if (Ap->n < 2*ORDEM)
  {
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno); // Insere o registro na página atual
    *Cresceu = 0; // Marca que a árvore não cresceu
    return;
  }

  // Split: A página precisa ser dividida em duas
  ApTemp = (Apontador) malloc(sizeof(Pagina));
  ApTemp->n = 0;
  ApTemp->p[0] = NULL;
  cont++;
  ApTemp->pageNum = cont;

  // Insere o registro na posição correta na nova página temporária
  if (i <= ORDEM + 1)
  {
    InsereNaPagina(ApTemp, Ap->r[2*ORDEM - 1], Ap->p[2*ORDEM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  }
  else
  {
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
  }
  
  // Insere os registros restantes na nova página
  for (j = ORDEM + 2; j <= 2*ORDEM; j++)
    InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);

  // Atualiza a quantidade de registros na página atual
  Ap->n = ORDEM;
  // Atualiza o ponteiro da nova página
  ApTemp->p[0] = Ap->p[ORDEM + 1];
  // Retorna o registro que deve subir para o nível acima
  *RegRetorno = Ap->r[ORDEM];
  // Retorna a nova página criada
  *ApRetorno = ApTemp;

  // Limpa os registros que foram movidos para a nova página
  for (j = ORDEM; j < Ap->n + 2; j++)
  {
    Aux.chave = 0;
    Aux.rank = 0;
    Ap->r[j] = Aux;
  }
}


void pos_ordem(Apontador raiz)
{
  int i;

  // Verifica se a raiz é uma folha
  if (isleaf(raiz))
  {
    // Se for uma folha, imprime os registros em ordem decrescente
    for (i = raiz->n - 1; i >= 0; i--)
    {
      printf("%d\n", raiz->r[i].chave);
    }
  }
  else
  {
    // Se não for uma folha, percorre os filhos da direita antes de imprimir os registros da página atual
    pos_ordem(raiz->p[raiz->n]);
    // Imprime os registros da página atual em ordem decrescente
    for (i = raiz->n - 1; i >= 0; i--)
    {
      printf("%d\n", raiz->r[i].chave);
      // Percorre os filhos da página atual recursivamente
      pos_ordem(raiz->p[i]);
    }
  }
}


void Pesquisa(Registro *x, Apontador Ap)
{
  int i;

  // Verifica se a página é nula
  if (Ap == NULL)
  {
    // Registro não encontrado
    return;
  }

  i = 1;
  // Encontra a posição do registro na página
  while (i < Ap->n && x->chave > Ap->r[i - 1].chave)
    i++;

  // Se o registro for encontrado na página atual
  if (x->chave == Ap->r[i - 1].chave)
  {
    // Copia o registro encontrado para o parâmetro de saída
    *x = Ap->r[i - 1];
    return;
  }

  // Se o registro não for encontrado na página atual, continua a busca nos filhos
  if (x->chave < Ap->r[i - 1].chave)
    Pesquisa(x, Ap->p[i - 1]); // Busca no filho esquerdo
  else
    Pesquisa(x, Ap->p[i]); // Busca no filho direito
}


void Reconstitui(Apontador ApPag, Apontador ApPai, int PosPai, int *Diminuiu)
{
  Apontador Aux;
  int DispAux, j;

  // Se a posição do pai for menor que o número de registros no pai
  if (PosPai < ApPai->n)
  {  
    // Aux recebe a página à direita de ApPag
    Aux = ApPai->p[PosPai + 1];
    // Calcula a quantidade de registros disponíveis em Aux
    DispAux = (Aux->n - ORDEM + 1) / 2;

    // Adiciona o registro do pai à página ApPag
    ApPag->r[ApPag->n] = ApPai->r[PosPai];
    ApPag->p[ApPag->n + 1] = Aux->p[0];
    ApPag->n++;

    // Se houver folga em Aux
    if (DispAux > 0)
    {  
      // Transfere registros de Aux para ApPag
      for (j = 1; j < DispAux; j++)
        InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
      ApPai->r[PosPai] = Aux->r[DispAux - 1];
      Aux->n -= DispAux;
      for (j = 0; j < Aux->n; j++)
        Aux->r[j] = Aux->r[j + DispAux];
      for (j = 0; j <= Aux->n; j++)
        Aux->p[j] = Aux->p[j + DispAux];
      *Diminuiu = 0;
    }
    else
    { 
      // Fusão: intercala Aux em ApPag e libera Aux
      for (j = 1; j <= ORDEM; j++)
        InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
      free(Aux);
      for (j = PosPai + 1; j < ApPai->n; j++)
      {   
        // Preenche o vazio em ApPai
        ApPai->r[j - 1] = ApPai->r[j];
        ApPai->p[j] = ApPai->p[j + 1];
      }
      ApPai->n--;
      if (ApPai->n >= ORDEM)
        *Diminuiu = 0;
    }
  }
  else
  { 
    // Aux recebe a página à esquerda de ApPag
    Aux = ApPai->p[PosPai - 1];
    // Calcula a quantidade de registros disponíveis em Aux
    DispAux = (Aux->n - ORDEM + 1) / 2;

    // Move os registros e ponteiros em ApPag para criar espaço para o novo registro do pai
    for (j = ApPag->n; j >= 1; j--)
      ApPag->r[j] = ApPag->r[j - 1];
    ApPag->r[0] = ApPai->r[PosPai - 1];
    for (j = ApPag->n; j >= 0; j--)
      ApPag->p[j + 1] = ApPag->p[j];
    ApPag->n++;

    // Se houver folga em Aux
    if (DispAux > 0)
    {  
      // Transfere registros de Aux para ApPag
      for (j = 1; j < DispAux; j++)
        InsereNaPagina(ApPag, Aux->r[Aux->n - j], Aux->p[Aux->n - j + 1]);
      ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
      ApPai->r[PosPai - 1] = Aux->r[Aux->n - DispAux];
      Aux->n -= DispAux;
      *Diminuiu = 0;
    }
    else
    { 
      // Fusão: intercala ApPag em Aux e libera ApPag
      for (j = 1; j <= ORDEM; j++)
        InsereNaPagina(Aux, ApPag->r[j - 1], ApPag->p[j]);
      free(ApPag);
      ApPai->n--;
      if (ApPai->n >= ORDEM)
        *Diminuiu = 0;
    }
  }
}  /* Reconstitui */


void Ret(int Ch, Apontador *Ap, int *Diminuiu)
{
  int Ind, j;
  Apontador WITH;
  Registro Reg;

  // Verifica se a página é nula
  if (*Ap == NULL)
  {
    // Chave não encontrada
    printf("chave nao encontrada: %i\n", Ch);
    *Diminuiu = 0;
    return;
  }

  // Copia o valor da página atual
  WITH = *Ap;
  Ind = 1;

  // Encontra a posição da chave na página
  while (Ind < WITH->n && Ch > WITH->r[Ind - 1].chave)
    Ind++;

  // Se a chave for encontrada na página atual
  if (Ch == WITH->r[Ind - 1].chave)
  {
    // Remove o registro
    Reg.chave = 0;
    Reg.rank = 0;
    WITH->r[Ind - 1] = Reg;

    // Se a página for uma folha
    if (WITH->p[Ind - 1] == NULL)
    {
      // Reduz o número de registros na página
      WITH->n--;
      *Diminuiu = WITH->n < ORDEM;

      // Realiza a reorganização dos registros e ponteiros
      for (j = Ind; j <= WITH->n; j++)
      {
        WITH->r[j - 1] = WITH->r[j];
        WITH->p[j] = WITH->p[j + 1];
      }
      return;
    }

    // Se a página não for uma folha, chama a função Antecessor
    Antecessor(*Ap, Ind, WITH->p[Ind - 1], Diminuiu);

    // Se a página foi reduzida após a remoção do registro
    if (*Diminuiu)
      // Reconstitui a árvore
      Reconstitui(WITH->p[Ind - 1], *Ap, Ind - 1, Diminuiu);

    return;
  }

  // Se a chave não for encontrada na página atual, continua a busca nos filhos
  if (Ch > WITH->r[Ind - 1].chave)
    Ind++;

  // Chama recursivamente a função Ret para buscar a chave nos filhos
  Ret(Ch, &WITH->p[Ind - 1], Diminuiu);

  // Se a página foi reduzida após a remoção do registro
  if (*Diminuiu)
    // Reconstitui a árvore
    Reconstitui(WITH->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
}


void Retira(int Ch, Apontador *Ap)
{
  int Diminuiu;
  Apontador Aux;

  // Chama a função Ret para remover a chave Ch da árvore
  Ret(Ch, Ap, &Diminuiu);

  // Se a árvore diminuiu na altura e a página raiz ficou vazia
  if (Diminuiu && (*Ap)->n == 0) { 
    // Auxiliar recebe a página raiz
    Aux = *Ap;
    // A raiz agora é o filho da página raiz original
    *Ap = Aux->p[0];
    // Libera a memória ocupada pela página raiz original
    free(Aux);
  }
}  /* Retira */


void recuperarReg(Apontador *arv, node *LISTA)
{
    FILE* arq;
    Registro Reg[2*ORDEM];
    node *tmp;
    int i=0,j=0, tam;

    // Abre o arquivo para leitura em modo binário
    arq = fopen(namefile,"rb");
    if (arq == NULL)
        exit(1);

    // Obtém o tamanho do arquivo
    fseek(arq, 0, SEEK_END);
    tam = ftell(arq);
    rewind(arq);
    fclose(arq);

    // Reabre o arquivo para leitura em modo binário
    arq = fopen(namefile,"rb");
    if (arq == NULL)
        exit(1);

    // Lê os registros do arquivo e os insere na lista
    while (j*(2*ORDEM*sizeof(Registro)) < tam){
        fseek(arq, j*(2*ORDEM*sizeof(Registro)), SEEK_SET);
        fread(Reg, 2*ORDEM*sizeof(Registro),1,arq);

        for(i = 0; i < 2*ORDEM; i++){
           if (Reg[i].chave > 0)
               execut(Reg[i], LISTA);
        }
        j++;
    }
    fclose(arq);

    // Insere os registros da lista na árvore B
    tmp = LISTA->prox;
    while( tmp != NULL){
        InsertPosFile(tmp->info, arv);
        tmp = tmp->prox;
    }
    free(tmp);
    free(LISTA);
    //Imprime(*arv, 2*ORDEM, 0);
}


void saveAux(Apontador p, int Nivel)
{
  int i,j;

  // Verifica se o nó atual é nulo e retorna se for
  if (p == NULL)
    return;

  // Percorre os registros no nó atual
  for (i = 0; i < p->n; i++)
    // Chama a função salvar para cada registro no nó atual
    salvar(p, p->r);

  // Percorre os ponteiros para os filhos do nó atual
  for (j = 0; j <= p->n; j++)
    // Chama recursivamente a função saveAux para cada filho do nó atual
    saveAux(p->p[j], Nivel + 1);
}


void salvar(Apontador pagina, Registro Reg[])
{

    FILE* arq;

    // Verifica se o arquivo já existe
    if (!file_exists(namefile)){
        // Se não existe, cria o arquivo e escreve os registros
        arq = fopen(namefile,"wb");
        if (arq == NULL)
            exit(1);
        fseek(arq, pagina->pageNum*(2*ORDEM*sizeof(Registro)), SEEK_SET);
        fwrite(Reg,(2*ORDEM*sizeof(Registro)),1,arq);
        fclose(arq);
    }

    else{
        // Se o arquivo já existe, abre-o para leitura e escrita e escreve os registros
        arq = fopen(namefile,"r+b");
        if (arq == NULL)
            exit(1);
        fseek(arq, pagina->pageNum*(2*ORDEM*sizeof(Registro)), SEEK_SET);
        fwrite(Reg, (2*ORDEM*sizeof(Registro)),1,arq);
        fclose(arq);
    }
}


//############################### //Estrutura 1 Btree

//############################### //Estrutura 2 lista encadeada
void insereInicio(Registro info, node *LISTA)
{
    // Aloca memória para o novo nó
    node *novo=(node *) malloc(sizeof(node));

    // Busca o nó onde o novo nó será inserido
    node *tmp = busca(info, LISTA);

    // Se a lista estiver vazia, insere o novo nó no início
    if(LISTA->prox == NULL){
        novo->info = info;
        novo->prox = LISTA->prox;
        LISTA->prox = novo;
    }
    else{
        // Insere o novo nó antes do nó encontrado
        novo->prox = tmp->prox;
        novo->info = info;
        tmp->prox = novo;
    }
}


node *busca(Registro info, node *LISTA)
{
    // Ponteiro para o nó atual e o nó anterior
    node *atual,*antNode;
    
    // Inicializa os ponteiros
    atual = LISTA->prox;
    antNode = LISTA;

    // Percorre a lista até encontrar a posição onde o novo nó será inserido
    while(atual != NULL && info.rank > atual->info.rank){
        antNode = atual;
        atual = atual->prox;
    }

    // Retorna o nó anterior à posição onde o novo nó será inserido
    return antNode;
}


void exibe(node *LISTA)
{
    // Verifica se a lista está vazia
    if (!LISTA){
        return;
    }

    // Ponteiro temporário para percorrer a lista
    node *tmp;
    tmp = LISTA->prox;

    // Percorre a lista e exibe os elementos
    while (tmp != NULL){
        printf(" %d ", tmp->info.chave);
        tmp = tmp->prox;
    }
    printf("\n");
}


void execut(Registro info, node *LISTA)
{
    // Insere o registro no início da lista
    insereInicio(info, LISTA);
}

//############################# //Estrutura 2 lista encadeada

// Função principal
int main() {
    // Declaração da árvore B e da lista encadeada
    Apontador *arv;
    Registro reg;
    char tecla;
    int chave, i, num = 0;

    // Alocação de memória para a árvore B
    arv = (Apontador*) malloc(sizeof(Apontador));

    // Criação da lista encadeada
    node *LISTA = (node *) malloc(sizeof(node));
    
    // Inicializa a árvore B e a lista encadeada
    Inicializa(arv);
    inicia(LISTA);

    // Verifica se o arquivo existe e recupera os registros se necessário
    if (file_exists(namefile)) {
        recuperarReg(arv, LISTA);
    }

    // Loop principal do programa
    while (1) {
        // Aguarda entrada do usuário
        scanf("%c", &tecla);

        // Verifica a opção escolhida pelo usuário
        switch(tecla) {
            case 'i':
                // Opção de inserção de registro
                printf("Insercao\n");
                scanf("%d", &reg.chave);
                scanf("%s", reg.nome);
                scanf("%d", &reg.idade);
                execut(reg, LISTA);
                break;
            case 'e':
                // Opção de exibição de registros
                printf("Exibicao\n");
                exibe(LISTA);
                break;
            case 'c':
                // Opção de consulta de registros
                printf("Consulta\n");
                scanf("%d", &reg.chave);
                Busca(reg, *arv);
                break;
            case 's':
                // Opção de saída do programa
                // Salva os registros no arquivo antes de sair
                saveAux(*arv, 0);
                return 0;
            default:
                break;
        }
    }
    return 0;
}

