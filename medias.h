#ifndef MEDIAS_H
#define MEDIAS_H

#include "alunos.h"
#include "disciplinas.h"
#include "notas.h"

typedef struct {
    char nomeDisciplina[50];
    float nota;
} ItemBoletim;

typedef struct {
    int idAluno;
    char nomeAluno[100];
    ItemBoletim itens[MAX_DISCIPLINAS];
    int qtdItens;
    float mediaGeral;
    int aprovado; // 1 para aprovado, 0 para reprovado
} BoletimAluno;

/*
 * Calcula a média geral do aluno considerando
 * todas as disciplinas em que ele está matriculado.
 */
float calcularMediaAluno(
    int indiceAluno,
    const float notas[MAX_ALUNOS][MAX_DISCIPLINAS],
    int qtdDisciplinas
);

/*
 * Preenche a estrutura BoletimAluno com os dados do aluno.
 */
int gerarBoletimDados(
    const Aluno* alunos,
    int qtdAlunos,
    const Disciplina* disciplinas,
    int qtdDisciplinas,
    const float notas[MAX_ALUNOS][MAX_DISCIPLINAS],
    int idAluno,
    BoletimAluno* boletim,
    char* msgErro
);

/*
 * Exibe o boletim do aluno, contendo disciplinas,
 * notas, média geral e situação.
 */
void listarBoletim(
    const Aluno* alunos,
    int qtdAlunos,
    const Disciplina* disciplinas,
    int qtdDisciplinas,
    const float notas[MAX_ALUNOS][MAX_DISCIPLINAS]
);

#endif // MEDIAS_H