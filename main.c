#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "alunos.h"
#include "disciplinas.h"
#include "notas.h"
#include "medias.h"

// Cores do Console Windows
#define COR_PADRAO   7
#define COR_TITULO   11   // Cyan Claro
#define COR_OPCAO    14   // Amarelo Claro
#define COR_SUCESSO  10  // Verde
#define COR_ERRO     12  // Vermelho
#define COR_DESTAQUE 11 // Azul Claro

void definirCor(WORD cor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, cor);
}

void limparTela(void) {
    system("cls");
}

void limparBufferEntrada(void) {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

void exibirMenuEstilizado(int qtdAlunos, int qtdDisciplinas) {
    definirCor(COR_TITULO);
    printf("\n========================================================\n");
    printf("        SISTEMA DE GESTÃO ESCOLAR — MENU EM C          \n");
    printf("========================================================\n");
    definirCor(COR_PADRAO);
    printf(" Status do Sistema: [%d Aluno(s) | %d Disciplina(s)]\n", qtdAlunos, qtdDisciplinas);
    printf("--------------------------------------------------------\n");
    
    definirCor(COR_OPCAO);
    printf(" [1] "); definirCor(COR_PADRAO); printf("Cadastrar novo aluno\n");
    definirCor(COR_OPCAO);
    printf(" [2] "); definirCor(COR_PADRAO); printf("Cadastrar nova disciplina\n");
    definirCor(COR_OPCAO);
    printf(" [3] "); definirCor(COR_PADRAO); printf("Matricular aluno em disciplina\n");
    definirCor(COR_OPCAO);
    printf(" [4] "); definirCor(COR_PADRAO); printf("Lançar ou atualizar nota\n");
    definirCor(COR_OPCAO);
    printf(" [5] "); definirCor(COR_PADRAO); printf("Consultar boletim do aluno\n");
    definirCor(COR_OPCAO);
    printf(" [6] "); definirCor(COR_PADRAO); printf("Listar disciplinas cadastradas\n");
    definirCor(COR_OPCAO);
    printf(" [7] "); definirCor(COR_PADRAO); printf("Remover aluno da memória\n");
    definirCor(COR_OPCAO);
    printf(" [8] "); definirCor(COR_PADRAO); printf("Remover disciplina da memória\n");
    definirCor(COR_DESTAQUE);
    printf(" [9] "); definirCor(COR_TITULO);  printf("Abrir Interface Gráfica (Janela Desktop)\n");
    definirCor(COR_ERRO);
    printf(" [0] "); definirCor(COR_PADRAO); printf("Salvar dados e Sair\n");
    
    definirCor(COR_TITULO);
    printf("========================================================\n");
    definirCor(COR_PADRAO);
    printf(" Escolha uma opção: ");
}

int main(void) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Aluno alunos[MAX_ALUNOS];
    Disciplina disciplinas[MAX_DISCIPLINAS];
    float notas[MAX_ALUNOS][MAX_DISCIPLINAS];

    int qtdAlunos = 0;
    int qtdDisciplinas = 0;
    int opcao;

    inicializarNotas(notas);

    carregarAlunos(alunos, &qtdAlunos);
    carregarDisciplinas(disciplinas, &qtdDisciplinas);
    carregarNotas(notas);

    do {
        exibirMenuEstilizado(qtdAlunos, qtdDisciplinas);

        if (scanf("%d", &opcao) != 1) {
            definirCor(COR_ERRO);
            printf("\nOpção inválida! Digite apenas números inteiros.\n");
            definirCor(COR_PADRAO);
            limparBufferEntrada();
            opcao = -1;
            printf("\nPressione ENTER para continuar...");
            getchar();
            limparTela();
            continue;
        }
        limparBufferEntrada();

        printf("\n");

        switch (opcao) {
            case 1:
                definirCor(COR_TITULO);
                printf("--- [1] CADASTRO DE ALUNO ---\n");
                definirCor(COR_PADRAO);
                cadastrarAluno(alunos, &qtdAlunos);
                break;

            case 2:
                definirCor(COR_TITULO);
                printf("--- [2] CADASTRO DE DISCIPLINA ---\n");
                definirCor(COR_PADRAO);
                cadastrarDisciplina(disciplinas, &qtdDisciplinas);
                break;

            case 3:
                definirCor(COR_TITULO);
                printf("--- [3] MATRÍCULA DE ALUNO ---\n");
                definirCor(COR_PADRAO);
                matricularAluno(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas);
                break;

            case 4:
                definirCor(COR_TITULO);
                printf("--- [4] LANÇAMENTO DE NOTAS ---\n");
                definirCor(COR_PADRAO);
                lancarNotas(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas);
                break;

            case 5:
                definirCor(COR_TITULO);
                printf("--- [5] BOLETIM DO ALUNO ---\n");
                definirCor(COR_PADRAO);
                listarBoletim(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas);
                break;

            case 6:
                definirCor(COR_TITULO);
                printf("--- [6] LISTA DE DISCIPLINAS ---\n");
                definirCor(COR_PADRAO);
                listarDisciplinas(disciplinas, qtdDisciplinas);
                break;

            case 7:
                definirCor(COR_TITULO);
                printf("--- [7] REMOVER ALUNO ---\n");
                definirCor(COR_PADRAO);
                removerAluno(alunos, qtdAlunos);
                break;

            case 8:
                definirCor(COR_TITULO);
                printf("--- [8] REMOVER DISCIPLINA ---\n");
                definirCor(COR_PADRAO);
                removerDisciplina(disciplinas, qtdDisciplinas);
                break;

            case 9:
                definirCor(COR_SUCESSO);
                printf("Iniciando Interface Gráfica de Software (Janela Desktop)...\n");
                definirCor(COR_PADRAO);
                
                salvarAlunos(alunos, qtdAlunos);
                salvarDisciplinas(disciplinas, qtdDisciplinas);
                salvarNotas(notas);

                system("start sistema_gui.exe");
                break;

            case 0:
                salvarAlunos(alunos, qtdAlunos);
                salvarDisciplinas(disciplinas, qtdDisciplinas);
                salvarNotas(notas);

                definirCor(COR_SUCESSO);
                printf("\nDados salvos com sucesso nos arquivos binários .dat.\n");
                printf("Sistema encerrado com sucesso. Até logo!\n");
                definirCor(COR_PADRAO);
                break;

            default:
                definirCor(COR_ERRO);
                printf("Opção inexistente. Tente novamente.\n");
                definirCor(COR_PADRAO);
                break;
        }

        if (opcao != 0 && opcao != 9) {
            printf("\nPressione ENTER para voltar ao menu principal...");
            getchar();
            limparTela();
        }

    } while (opcao != 0);

    return 0;
}