#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 50
#define MAX_DISCIPLINAS 10
#define MAX_NOME 50

// --- Estruturas ---
typedef struct {
    int id;
    char nome[MAX_NOME];
    int matriculado; // 1 = sim, 0 = não
} Aluno;

typedef struct {
    int id;
    char nome[MAX_NOME];
} Disciplina;

// --- Variáveis Globais (Memória) ---
Aluno alunos[MAX_ALUNOS];
Disciplina disciplinas[MAX_DISCIPLINAS];

// Matriz 2D: Linhas = Alunos, Colunas = Disciplinas
// Armazena a nota do aluno X na disciplina Y (-1.0 indica sem nota/não matriculado)
float notas[MAX_ALUNOS][MAX_DISCIPLINAS];

int totalAlunos = 0;
int totalDisciplinas = 0;

// --- Protótipos das Funções ---
void inicializarSistema();
void cadastrarAluno();
void cadastrarDisciplina();
void matricularAluno();
void lancarNotas();
void calcularMedia();
void listarBoletim();
void exibirMenu();

// --- Função Principal ---
int main() {
    int opcao;

    inicializarSistema();

    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1: cadastrarAluno(); break;
            case 2: cadastrarDisciplina(); break;
            case 3: matricularAluno(); break;
            case 4: lancarNotas(); break;
            case 5: calcularMedia(); break;
            case 6: listarBoletim(); break;
            case 0: printf("\nSaindo do programa...\n"); break;
            default: printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

void inicializarSistema() {
    // Preenche a matriz de notas com -1 para indicar ausencia de nota
    for (int i = 0; i < MAX_ALUNOS; i++) {
        for (int j = 0; j < MAX_DISCIPLINAS; j++) {
            notas[i][j] = -1.0f;
        }
    }
}

void exibirMenu() {
    printf("\n========================================\n");
    printf("         SISTEMA ACADEMICO              \n");
    printf("========================================\n");
    printf("1. Cadastrar Aluno\n");
    printf("2. Cadastrar Disciplina\n");
    printf("3. Matricular Aluno em Disciplina\n");
    printf("4. Lancar Notas\n");
    printf("5. Calcular Media de um Aluno\n");
    printf("6. Gerar Boletim\n");
    printf("0. Sair\n");
    printf("========================================\n");
}

void cadastrarAluno() {
    if (totalAlunos >= MAX_ALUNOS) {
        printf("\n[Erro] Limite maximo de alunos atingido!\n");
        return;
    }

    alunos[totalAlunos].id = totalAlunos;
    printf("\nDigite o nome do aluno: ");
    fgets(alunos[totalAlunos].nome, MAX_NOME, stdin);
    alunos[totalAlunos].nome[strcspn(alunos[totalAlunos].nome, "\n")] = '\0'; // Remove o '\n'

    alunos[totalAlunos].matriculado = 1;
    printf("-> Aluno '%s' cadastrado com ID %d!\n", alunos[totalAlunos].nome, totalAlunos);
    totalAlunos++;
}

void cadastrarDisciplina() {
    if (totalDisciplinas >= MAX_DISCIPLINAS) {
        printf("\n[Erro] Limite maximo de disciplinas atingido!\n");
        return;
    }

    disciplinas[totalDisciplinas].id = totalDisciplinas;
    printf("\nDigite o nome da disciplina: ");
    fgets(disciplinas[totalDisciplinas].nome, MAX_NOME, stdin);
    disciplinas[totalDisciplinas].nome[strcspn(disciplinas[totalDisciplinas].nome, "\n")] = '\0';

    printf("-> Disciplina '%s' cadastrada com ID %d!\n", disciplinas[totalDisciplinas].nome, totalDisciplinas);
    totalDisciplinas++;
}

void matricularAluno() {
    int idAluno, idDisc;

    if (totalAlunos == 0 || totalDisciplinas == 0) {
        printf("\n[Erro] Cadastre pelo menos 1 aluno e 1 disciplina antes.\n");
        return;
    }

    printf("\n--- Matricular Aluno ---\n");
    printf("ID do Aluno (0 a %d): ", totalAlunos - 1);
    scanf("%d", &idAluno);
    printf("ID da Disciplina (0 a %d): ", totalDisciplinas - 1);
    scanf("%d", &idDisc);

    if (idAluno >= 0 && idAluno < totalAlunos && idDisc >= 0 && idDisc < totalDisciplinas) {
        // Marcamos como nota 0.0 inicialmente para representar matriculado sem nota lançada
        notas[idAluno][idDisc] = 0.0f;
        printf("-> Aluno %s matriculado em %s com sucesso!\n", alunos[idAluno].nome, disciplinas[idDisc].nome);
    } else {
        printf("\n[Erro] ID de Aluno ou Disciplina invalido!\n");
    }
}

void lancarNotas() {
    int idAluno, idDisc;
    float nota;

    printf("\n--- Lançar Nota ---\n");
    printf("ID do Aluno: ");
    scanf("%d", &idAluno);
    printf("ID da Disciplina: ");
    scanf("%d", &idDisc);

    if (idAluno >= 0 && idAluno < totalAlunos && idDisc >= 0 && idDisc < totalDisciplinas) {
        if (notas[idAluno][idDisc] == -1.0f) {
            printf("\n[Aviso] Aluno nao esta matriculado nesta disciplina! Matricule-o primeiro.\n");
            return;
        }

        printf("Digite a nota (0.0 a 10.0): ");
        scanf("%f", &nota);

        if (nota >= 0.0f && nota <= 10.0f) {
            notas[idAluno][idDisc] = nota;
            printf("-> Nota %.2f lancada com sucesso!\n", nota);
        } else {
            printf("\n[Erro] Nota invalida! Deve ser entre 0.0 e 10.0.\n");
        }
    } else {
        printf("\n[Erro] IDs invalidos.\n");
    }
}

void calcularMedia() {
    int idAluno;
    float soma = 0.0f;
    int qtdDisciplinas = 0;

    printf("\n--- Calcular Media Geral ---\n");
    printf("ID do Aluno: ");
    scanf("%d", &idAluno);

    if (idAluno >= 0 && idAluno < totalAlunos) {
        for (int j = 0; j < totalDisciplinas; j++) {
            if (notas[idAluno][j] != -1.0f) {
                soma += notas[idAluno][j];
                qtdDisciplinas++;
            }
        }

        if (qtdDisciplinas > 0) {
            printf("\n-> A media geral do aluno %s e: %.2f\n", alunos[idAluno].nome, soma / qtdDisciplinas);
        } else {
            printf("\n-> O aluno %s nao esta cursando nenhuma disciplina com nota.\n", alunos[idAluno].nome);
        }
    } else {
        printf("\n[Erro] Aluno nao encontrado.\n");
    }
}

void listarBoletim() {
    int idAluno;

    printf("\n--- Boletim Escolar ---\n");
    printf("ID do Aluno: ");
    scanf("%d", &idAluno);

    if (idAluno >= 0 && idAluno < totalAlunos) {
        printf("\n========================================\n");
        printf("BOLETIM DE: %s\n", alunos[idAluno].nome);
        printf("========================================\n");

        int matriculas = 0;
        for (int j = 0; j < totalDisciplinas; j++) {
            if (notas[idAluno][j] != -1.0f) {
                printf("Disciplina: %-20s | Nota: %.2f\n", disciplinas[j].nome, notas[idAluno][j]);
                matriculas++;
            }
        }

        if (matriculas == 0) {
            printf("Nenhuma disciplina vinculada a este aluno.\n");
        }
        printf("========================================\n");
    } else {
        printf("\n[Erro] Aluno nao encontrado.\n");
    }
}
