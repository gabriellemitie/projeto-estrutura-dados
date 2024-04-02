#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu();
struct Data {
  int dia;
  int mes;
  int ano;
};

struct Registro {
  char nome[255];
  int idade;
  char rg[11];
  struct Data entrada;
};

typedef struct Elista { // celula
  struct Registro dados;
  struct Elista *proximo;
} Elista;

typedef struct {
  struct Elista *inicio;
  int qtde;
} Lista;

typedef struct {
  struct Registro *dados;
  struct Efila *proximo;
} Efila;

typedef struct {
  Efila *head;
  Efila *tail;
  int qtde;
} Fila;

typedef struct {
  struct Registro *dados;
  struct EABB *FilhoEsq;
  struct EABB *FilhoDir;
  struct EABB *pai;
} EABB; // vertice da arvore

typedef struct {
  EABB *raiz;
  int qtde;
} ABB; // arvore

Fila *cria_fila() {
  Fila *f = malloc(sizeof(Fila));
  f->head = NULL;
  f->tail = NULL;
  f->qtde = 0;
  return f;
}

Efila *cria_efila() {
  Efila *ef = malloc(sizeof(Efila));
  ef->dados = NULL;
  ef->proximo = NULL;
  return ef;
}

EABB *cria_vertice(ABB *arvore) {
  EABB *novo = malloc(sizeof(EABB));
  novo->FilhoDir = NULL;
  novo->FilhoEsq = NULL;
  novo->dados = NULL;

  return novo;
}

ABB *cria_arvore() {
  ABB *arvore = malloc(sizeof(ABB));
  arvore->raiz = NULL;
  arvore->qtde = 0;
  return arvore;
}

void in_ordem(EABB *raiz) {
  if (raiz != NULL) {
    in_ordem(raiz->FilhoEsq);
    in_ordem(raiz->FilhoDir);
  }
}

void Enfileirar(Fila *f, struct Registro registro) {
  Efila *novo = malloc(sizeof(Efila));
  novo->dados = malloc(sizeof(struct Registro)); // Adicionar essa linha
  *(novo->dados) = registro;
  novo->proximo = NULL;

  if (f->qtde == 0) {
    f->head = novo;
  } else {
    f->tail->proximo = novo;
  }
  f->tail = novo;
  f->qtde++;
  return;
}

void Desenfileirar(Fila *f) {
  if (f->qtde == 0) {
    printf("Fila vazia! Não é possível remover.\n");
    return;
  }
  Efila *removido = f->head;
  f->head = f->head->proximo;
  f->qtde--;
  free(removido);
}

Lista *inicializa_lista() {
  Lista *l = malloc(sizeof(Lista));
  l->inicio = NULL;
  l->qtde = 0;
  return l;
}

void Inserir(Lista *l, struct Registro *registro) {
  Elista *novo = malloc(sizeof(Elista));
  novo->dados = *registro;
  novo->proximo = l->inicio;
  l->inicio = novo;
  l->qtde++;
}

void ImprimirLista(Lista *l) {
  Elista *no = l->inicio;
  while (no != NULL) {
    printf("Nome: %s\n", no->dados.nome);
    printf("Idade: %d\n", no->dados.idade);
    printf("RG: %s\n", no->dados.rg);
    printf("Data de entrada: %d/%d/%d\n", no->dados.entrada.dia,
           no->dados.entrada.mes, no->dados.entrada.ano);
    no = no->proximo;
  }
}
void Salvar(Lista *l) {
  FILE *arq;
  arq = fopen("cadastros.txt", "a");

  Elista *no = l->inicio;
  while (no != NULL) {
    fprintf(arq, "Nome: %s\n", no->dados.nome);
    fprintf(arq, "Idade: %d\n", no->dados.idade);
    fprintf(arq, "RG: %s\n", no->dados.rg);
    fprintf(arq, "Data de entrada: %d/%d/%d\n", no->dados.entrada.dia,
            no->dados.entrada.mes, no->dados.entrada.ano);
    no = no->proximo;
  }

  fclose(arq);
}
void Consultar(Lista *l, char *nome) {
  Elista *no = l->inicio;
  while (no != NULL) {
    if (strcmp(no->dados.nome, nome) == 0) {
      printf("Paciente encontrado!\n");
      printf("Nome: %s\n", no->dados.nome);
      printf("Idade: %d\n", no->dados.idade);
      printf("RG: %s\n", no->dados.rg);
      printf("Data de entrada: %d/%d/%d\n", no->dados.entrada.dia,
             no->dados.entrada.mes, no->dados.entrada.ano);
      return;
    }
    no = no->proximo;
  }
  printf("Paciente não encontrado!\n");
}
// arvores
void inserirArvoreAno(ABB *arvore, struct Data ano) {
  EABB *novo = cria_vertice(arvore);
  novo->dados = ano.ano;
  if (arvore->raiz == NULL) {
    arvore->raiz = novo;
    arvore->qtde++;
  } else if (arvore->raiz != NULL) {
    EABB *aux = arvore->raiz;
    while (aux != NULL) {
      if (ano.ano < aux->dados) {
        if (aux->FilhoEsq == NULL) {
          novo->pai = aux;
          aux->FilhoEsq = novo;
          arvore->qtde++;
          break;
        } else {
          aux = aux->FilhoEsq;
        }
      } else if (ano.ano > aux->dados) {
        if (aux->FilhoDir == NULL) {
          novo->pai = aux;
          aux->FilhoDir = novo;
          arvore->qtde++;
          break;
        } else {
          aux = aux->FilhoDir;
        }
      } else {
        break;
      }
    }
  }
}

void inserirArvore(ABB *arvore, int chave) {
  EABB *novo = cria_vertice(arvore);
  novo->dados = chave;
  if (arvore->raiz == NULL) {
    arvore->raiz = novo;
    arvore->qtde++;
  } else {
    EABB *aux = arvore->raiz;
    while (aux != NULL) {
      if (chave < aux->dados) {
        if (aux->FilhoEsq == NULL) {
          novo->pai = aux;
          aux->FilhoEsq = novo;
          arvore->qtde++;
          break;
        } else {
          aux = aux->FilhoEsq;
        }
      } else if (chave > aux->dados) {
        if (aux->FilhoDir == NULL) {
          novo->pai = aux;
          aux->FilhoDir = novo;
          arvore->qtde++;
          break;
        } else {
          aux = aux->FilhoDir;
        }
      } else {
        break;
      }
    }
  }
}

void Buscar(Lista *l, char *nome, Fila *f) {
  Elista *no = l->inicio;
  while (no != NULL) {
    if (strcmp(no->dados.nome, nome) == 0) {
      printf("Paciente encontrado!\n");
      printf("Nome: %s\n", no->dados.nome);
      printf("Idade: %d\n", no->dados.idade);
      printf("RG: %s\n", no->dados.rg);
      printf("Data de entrada: %d/%d/%d\n", no->dados.entrada.dia,
             no->dados.entrada.mes, no->dados.entrada.ano);
      printf("Enfileirando..\n");
      Enfileirar(f, no->dados);
      printf("Paciente adicionado à fila com sucesso!\n");
      return;
    }
    no = no->proximo;
  }
  printf("Paciente não encontrado!\n");
}

void Atualizar(Lista *l, char *nome) {
  Elista *no = l->inicio;
  while (no != NULL) {
    if (strcmp(no->dados.nome, nome) == 0) {
      int opcao; // Corrigido: Mudar de char para int

      printf("Atualizar dados do paciente:\n");
      printf("Qual dado deseja atualizar?\n");
      printf("1 - Nome\n");
      printf("2 - Idade\n");
      printf("3 - RG\n");
      printf("4 - Entrada\n");
      scanf("%d", &opcao); // Corrigido: Ler a opção como um inteiro
      if (opcao == 1) {
        printf("Digite o novo nome: ");
        scanf("%s", no->dados.nome);
      } else if (opcao == 2) {
        printf("Digite a nova idade: ");
        scanf("%d", &(no->dados.idade));
      } else if (opcao == 3) {
        printf("Digite o novo RG: ");
        scanf("%s", no->dados.rg);
      } else if (opcao == 4) {
        printf("Digite a nova data de entrada (DD/MM/AAAA): ");
        scanf("%d/%d/%d", &(no->dados.entrada.dia), &(no->dados.entrada.mes),
              &(no->dados.entrada.ano));
      } else {
        printf("Opção inválida!\n");
      }
      return;
    }
    no = no->proximo;
  }
  printf("Paciente não encontrado!\n");
}

void Remover(Lista *l, char *nome) {
  Elista *no = l->inicio;
  Elista *anterior = NULL;
  while (no != NULL) {
    if (strcmp(no->dados.nome, nome) == 0) {
      if (anterior == NULL) {
        l->inicio = no->proximo;
      } else {
        anterior->proximo = no->proximo;
      }
      free(no);
      printf("Paciente removido com sucesso!\n");
      return;
    }
    anterior = no;
    no = no->proximo;
  }
  printf("Paciente não encontrado!\n");
}

void MostrarFila(Fila *f) {
  Efila *atual = f->head;
  while (atual != NULL) {
    printf("Nome: %s\n", atual->dados->nome);
    atual = atual->proximo;
  }
}

struct Registro Cadastrar(Lista *l, Fila *f) {
  printf("\n");
  printf("===============================\n");
  printf("||          Cadastro         ||\n");
  printf("===============================\n");
  printf("1 - Cadastrar novo paciente\n");
  printf("2 - Consultar paciente cadastrado\n");
  printf("3 - Mostrar lista completa\n");
  printf("4 - Atualizar dados do paciente\n");
  printf("5 - Remover paciente\n");

  int escolha;
  printf("\n");
  printf("Escolha uma das operações:");
  scanf("%d", &escolha);
  if (escolha == 1) {
    struct Registro registro;
    strcpy(registro.nome, "");
    registro.idade = 0;
    strcpy(registro.rg, "");
    registro.entrada.dia = 0;
    registro.entrada.mes = 0;
    registro.entrada.ano = 0;
    printf("\n");
    printf("Digite o nome:");
    scanf("%s", registro.nome);
    printf("Digite a idade:");
    scanf("%d", &registro.idade);
    printf("Digite o RG:");
    scanf("%s", registro.rg);
    printf("Digite o dia da entrada:");
    scanf("%d", &registro.entrada.dia);
    printf("Digite o mês da entrada:");
    scanf("%d", &registro.entrada.mes);
    printf("Digite o ano da entrada:");
    scanf("%d", &registro.entrada.ano);
    printf("Cadastro concluído!");
    printf("\n");
    Inserir(l, &registro);
    ABB *arvore = cria_arvore();
    inserirArvoreAno(arvore, registro.entrada.ano);
    menu(l, f);
    return registro;

  } else if (escolha == 2) {
    char paciente[255];
    printf("Digite o nome do paciente:");
    scanf("%s", &paciente);
    Consultar(l, paciente);
    menu(l, f);

  } else if (escolha == 3) {
    ImprimirLista(l);
    menu(l, f);

  } else if (escolha == 4) {
    char paciente;
    printf("Digite o nome do paciente:");
    scanf("%s", &paciente);
    Atualizar(l, &paciente);
    menu(l, f);

  } else if (escolha == 5) {
    char paciente;
    printf("Digite o nome do paciente:");
    scanf("%s", &paciente);
    Remover(l, &paciente);
    menu(l, f);
  }
}

void Atendimento(Fila *f, Lista *l) {
  printf("\n");
  printf("===============================\n");
  printf("||        Atendimento         ||\n");
  printf("===============================\n");
  printf("1 - Enfileirar paciente:\n");
  printf("2 - Desenfileirar pciente:\n");
  printf("3 - Mostrar fila:\n");
  int escolha;
  printf("Escolha uma das opções:\n");
  scanf("%d", &escolha);

  if (escolha == 1) {
    char paciente;
    printf("Digite o nome do paciente a ser adicionado na fila:");
    scanf("%s", &paciente);
    Buscar(l, &paciente, f);
    menu(l, f);

  } else if (escolha == 2) {
    Desenfileirar(f);
    menu(l, f);
  } else if (escolha == 3) {
    MostrarFila(f);
    menu(l, f);
  }
}

void Pesquisar(Fila *f, Lista *l) {
  printf("\n");
  printf("===============================\n");
  printf("||          Pesquisar         ||\n");
  printf("===============================\n");
  printf("1 - Mostrar registros ordenados por ano de registro.\n");
  printf("2 - Mostrar registros ordenados por mês de registro.\n");
  printf("3 - Mostrar registros ordenados por dia de registro.\n");
  printf("3 - Mostrar registros ordenados por idade do paciente.\n");
  int escolha;
  printf("Escolha uma das opções:\n");
  scanf("%d", &escolha);
  if (escolha == 1) {
    OrdenarPorAno(l);
    menu(l, f);

  } else if (escolha == 2) {
    OrdenarPorMes(l);
    menu(l, f);
  } else if (escolha == 3) {
    OrdenarPorDia(l);
    menu(l, f);
  } else if (escolha == 4) {
    OrdenarPorIdade(l);
    menu(l, f);
  }
}

void Sobre() {
  printf("Aléxia Suares\n");
  printf("Gabrielle Mitie\n");
  printf("Curso: Ciência da Computação\n");
  printf("Disciplina: CC4652- Estrutura de Dados\n");
  printf("Data: 17/11/2023\n");
}

void menu(Lista *l, Fila *f) {
  printf("========= Gerenciador ===========\n");
  printf("||            de               ||\n");
  printf("||     Atendimento Médico      ||\n");
  printf("=================================\n");
  printf("1 - Cadastrar\n");
  printf("2 - Atendimento\n");
  printf("3 - Pesquisa\n");
  printf("4 - Carregar/Salvar\n");
  printf("5 - Sobre\n");
  int escolha;
  printf("Escolha uma das opções:");
  scanf("%d", &escolha);

  if (escolha == 1) {
    Cadastrar(l, f);
  } else if (escolha == 2) {
    Atendimento(f, l);
  } else if (escolha == 3) {
    // Pesquisa();
  } else if (escolha == 4) {
    // Carregar(l);
    Salvar(l);
  } else if (escolha == 5) {
    Sobre();
  } else {
    printf("\n Opção inválida! \n");
    printf("\n");
    menu(l, f);
  }
}

int main(void) {
  Lista *l = inicializa_lista();
  Fila *f = cria_fila();
  menu(l, f);
  return 0;
}