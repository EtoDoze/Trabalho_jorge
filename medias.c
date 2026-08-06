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

int gerarBoletimDados(

    const Aluno* alunos,
    int qtdAlunos,
    const Disciplina* disciplinas,
    int qtdDisciplinas,
    const float notas[MAX_ALUNOS][MAX_DISCIPLINAS],
    int idAluno,
    BoletimAluno* boletim,
    char* msgErro
) {
    if (boletim == NULL) return 0;
    boletim->qtdItens = 0;

    int idx = buscarAlunoPorId(alunos, qtdAlunos, idAluno);
    if (idx == -1 || !alunos[idx].ativo) {
        if (msgErro) sprintf(msgErro, "Erro: Aluno com ID %d nao foi encontrado ou esta inativo.", idAluno);
        return 0;
    }

    boletim->idAluno = alunos[idx].id;
    snprintf(boletim->nomeAluno, sizeof(boletim->nomeAluno), "%s", alunos[idx].nome);

    for (int i = 0; i < qtdDisciplinas; i++) {
        if (disciplinas[i].ativo && notas[idx][i] != NOTA_NAO_MATRICULADO) {
            snprintf(boletim->itens[boletim->qtdItens].nomeDisciplina, sizeof(boletim->itens[boletim->qtdItens].nomeDisciplina), "%s", disciplinas[i].nome);
            boletim->itens[boletim->qtdItens].nota = notas[idx][i];
            boletim->qtdItens++;
        }
    }

    if (boletim->qtdItens == 0) {
        boletim->mediaGeral = 0.0f;
        boletim->aprovado = 0;
        if (msgErro) sprintf(msgErro, "Aviso: O aluno '%s' nao esta matriculado em nenhuma disciplina.", alunos[idx].nome);
        return 1;
    }

    boletim->mediaGeral = calcularMediaAluno(idx, notas, qtdDisciplinas);
    boletim->aprovado = (boletim->mediaGeral >= 7.0f) ? 1 : 0;
    if (msgErro) sprintf(msgErro, "Boletim de '%s' gerado com sucesso.", alunos[idx].nome);
    return 1;
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

    printf("\nDigite o ID do aluno: ");
    if (scanf("%d", &idAluno) != 1) {
        printf("\nID invalido!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    BoletimAluno boletim;
    char msg[256];
    if (!gerarBoletimDados(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas, idAluno, &boletim, msg)) {
        printf("\n%s\n", msg);
        return;
    }

    printf("\n=============================================\n");
    printf("              BOLETIM DO ALUNO\n");
    printf("=============================================\n");
    printf("ID: %d\n", boletim.idAluno);
    printf("Nome: %s\n", boletim.nomeAluno);
    printf("---------------------------------------------\n");
    printf("%-25s %10s\n", "Disciplina", "Nota");
    printf("---------------------------------------------\n");

    if (boletim.qtdItens == 0) {
        printf("\nAluno nao possui disciplinas matriculadas.\n");
        printf("=============================================\n");
        return;
    }

    for (int i = 0; i < boletim.qtdItens; i++) {
        printf("%-25s %10.2f\n", boletim.itens[i].nomeDisciplina, boletim.itens[i].nota);
    }

    printf("---------------------------------------------\n");
    printf("%-25s %10.2f\n", "Media Geral", boletim.mediaGeral);
    printf("%-25s %10s\n", "Situacao", boletim.aprovado ? "APROVADO" : "REPROVADO");
    printf("=============================================\n");
}