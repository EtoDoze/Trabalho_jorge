#include <stdio.h>
#include <stdlib.h>

#include "alunos.h"
#include "disciplinas.h"
#include "notas.h"
#include "medias.h"

/*
 * Limpa os caracteres restantes no buffer do teclado.
 * Evita problemas depois do uso de scanf.
 */
void limparBufferEntrada(void)
{
    int caractere;

    while ((caractere = getchar()) != '\n' && caractere != EOF)
    {
        // Apenas remove os caracteres restantes
    }
}

/*
 * Exibe as opções disponíveis no sistema.
 */
void exibirMenu(void)
{
    printf("\n========================================\n");
    printf("       SISTEMA DE GESTAO ESCOLAR\n");
    printf("========================================\n");
    printf("1 - Cadastrar aluno\n");
    printf("2 - Cadastrar disciplina\n");
    printf("3 - Matricular aluno em disciplina\n");
    printf("4 - Lancar ou atualizar nota\n");
    printf("5 - Listar boletim do aluno\n");
    printf("6 - Listar disciplinas\n");
    printf("0 - Sair\n");
    printf("========================================\n");
    printf("Escolha uma opcao: ");
}

int main(void)
{
    /*
     * Vetor que armazena os alunos cadastrados.
     */
    Aluno alunos[MAX_ALUNOS];

    /*
     * Vetor que armazena as disciplinas cadastradas.
     */
    Disciplina disciplinas[MAX_DISCIPLINAS];

    /*
     * Matriz de notas:
     *
     * Cada linha representa um aluno.
     * Cada coluna representa uma disciplina.
     */
    float notas[MAX_ALUNOS][MAX_DISCIPLINAS];

    /*
     * Quantidade atual de alunos e disciplinas.
     */
    int qtdAlunos = 0;
    int qtdDisciplinas = 0;

    int opcao;

    /*
     * Inicializa todas as posições da matriz com
     * NOTA_NAO_MATRICULADO.
     */
    inicializarNotas(notas);

    /*
     * Carrega os dados existentes dos arquivos.
     *
     * Caso os arquivos ainda não existam, as funções
     * devem manter os vetores vazios.
     */
    carregarAlunos(alunos, &qtdAlunos);

    carregarDisciplinas(
        disciplinas,
        &qtdDisciplinas
    );

    carregarNotas(notas);

    /*
     * Repete o menu até que o usuário escolha sair.
     */
    do
    {
        exibirMenu();

        /*
         * Verifica se o usuário digitou um número.
         */
        if (scanf("%d", &opcao) != 1)
        {
            printf("\nOpcao invalida! Digite apenas numeros.\n");

            limparBufferEntrada();

            opcao = -1;

            continue;
        }

        limparBufferEntrada();

        switch (opcao)
        {
            case 1:
                /*
                 * Cadastra um novo aluno.
                 */
                cadastrarAluno(
                    alunos,
                    &qtdAlunos
                );

                break;

            case 2:
                /*
                 * Cadastra uma nova disciplina.
                 */
                cadastrarDisciplina(
                    disciplinas,
                    &qtdDisciplinas
                );

                break;

            case 3:
                /*
                 * Realiza a matrícula de um aluno
                 * em uma disciplina.
                 */
                matricularAluno(
                    alunos,
                    qtdAlunos,
                    disciplinas,
                    qtdDisciplinas,
                    notas
                );

                break;

            case 4:
                /*
                 * Permite lançar ou atualizar a nota
                 * de um aluno matriculado.
                 */
                lancarNotas(
                    alunos,
                    qtdAlunos,
                    disciplinas,
                    qtdDisciplinas,
                    notas
                );

                break;

            case 5:
                /*
                 * Exibe as disciplinas, as notas,
                 * a média geral e a situação do aluno.
                 */
                listarBoletim(
                    alunos,
                    qtdAlunos,
                    disciplinas,
                    qtdDisciplinas,
                    notas
                );

                break;
           case 6:
               /*
               * Exibe todas as disciplinas       cadastradas.
               */
              listarDisciplinas(
                  disciplinas,
                  qtdDisciplinas
                );

                 break;
            case 0:
                /*
                 * Salva todos os dados antes de encerrar.
                 */
                salvarAlunos(
                    alunos,
                    qtdAlunos
                );

                salvarDisciplinas(
                    disciplinas,
                    qtdDisciplinas
                );

                salvarNotas(notas);

                printf("\nDados salvos com sucesso.\n");
                printf("Sistema encerrado.\n");

                break;

            default:
                printf("\nOpcao inexistente. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}