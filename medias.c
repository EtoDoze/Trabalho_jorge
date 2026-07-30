#include <stdio.h>
#include "medias.h"

/*
 * Função: calcularMediaAluno
 * ----------------------------------------
 * Calcula a média geral de um aluno considerando
 * apenas as disciplinas em que ele está matriculado.
 *
 * Parâmetros:
 * indiceAluno    -> posição do aluno no vetor de alunos.
 * notas          -> matriz contendo as notas.
 * qtdDisciplinas -> quantidade de disciplinas cadastradas.
 *
 * Retorno:
 * Média geral do aluno.
 */
float calcularMediaAluno(
    int indiceAluno,
    const float notas[MAX_ALUNOS][MAX_DISCIPLINAS],
    int qtdDisciplinas
) {
    float soma = 0.0f;
    int quantidadeNotas = 0;

    // Percorre todas as disciplinas
    for (int i = 0; i < qtdDisciplinas; i++) {

        // Verifica se o aluno está matriculado na disciplina
        if (notas[indiceAluno][i] != NOTA_NAO_MATRICULADO) {

            // Soma a nota encontrada
            soma += notas[indiceAluno][i];

            // Conta quantas disciplinas possuem nota
            quantidadeNotas++;
        }
    }

    // Caso o aluno não possua nenhuma disciplina
    if (quantidadeNotas == 0) {
        return 0.0f;
    }

    // Retorna a média geral
    return soma / quantidadeNotas;
}

/*
 * Função: listarBoletim
 * ----------------------------------------
 * Exibe no terminal todas as disciplinas em que
 * o aluno está matriculado, suas respectivas notas,
 * a média geral e sua situação.
 */
void listarBoletim(
    const Aluno* alunos,
    int qtdAlunos,
    const Disciplina* disciplinas,
    int qtdDisciplinas,
    const float notas[MAX_ALUNOS][MAX_DISCIPLINAS]
) {
    int idAluno;

    // Solicita ao usuário o ID do aluno
    printf("\nDigite o ID do aluno: ");

    // Valida a entrada
    if (scanf("%d", &idAluno) != 1) {

        printf("\nID inválido!\n");

        // Limpa o buffer do teclado
        while (getchar() != '\n');

        return;
    }

    /*
     * Procura o aluno pelo ID utilizando
     * a função existente no módulo alunos.
     */
    int indiceAluno = buscarAlunoPorId(
        alunos,
        qtdAlunos,
        idAluno
    );

    // Verifica se o aluno existe e está ativo
    if (indiceAluno == -1 || alunos[indiceAluno].ativo == 0) {

        printf("\nAluno não encontrado ou inativo.\n");

        return;
    }

    // Cabeçalho do boletim
    printf("\n=============================================\n");
    printf("              BOLETIM DO ALUNO\n");
    printf("=============================================\n");

    printf("ID: %d\n", alunos[indiceAluno].id);
    printf("Nome: %s\n", alunos[indiceAluno].nome);

    printf("---------------------------------------------\n");
    printf("%-25s %10s\n", "Disciplina", "Nota");
    printf("---------------------------------------------\n");

    int quantidadeDisciplinas = 0;

    /*
     * Percorre todas as disciplinas procurando
     * aquelas em que o aluno possui matrícula.
     */
    for (int i = 0; i < qtdDisciplinas; i++) {

        if (
            disciplinas[i].ativo == 1 &&
            notas[indiceAluno][i] != NOTA_NAO_MATRICULADO
        ) {

            printf(
                "%-25s %10.2f\n",
                disciplinas[i].nome,
                notas[indiceAluno][i]
            );

            quantidadeDisciplinas++;
        }
    }

    // Caso o aluno não esteja matriculado em nenhuma disciplina
    if (quantidadeDisciplinas == 0) {

        printf("\nAluno não possui disciplinas matriculadas.\n");
        printf("=============================================\n");

        return;
    }

    // Calcula a média geral do aluno
    float media = calcularMediaAluno(
        indiceAluno,
        notas,
        qtdDisciplinas
    );

    printf("---------------------------------------------\n");
    printf("%-25s %10.2f\n", "Média Geral", media);

    /*
     * Exibe a situação do aluno.
     * Neste projeto foi considerado:
     * média maior ou igual a 7 = aprovado.
     */
    if (media >= 7.0f) {

        printf("%-25s %10s\n", "Situação", "APROVADO");

    } else {

        printf("%-25s %10s\n", "Situação", "REPROVADO");

    }

    printf("=============================================\n");
}