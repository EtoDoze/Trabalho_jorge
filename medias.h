#ifndef MEDIAS_H
#define MEDIAS_H

#include "alunos.h"
#include "disciplinas.h"
#include "notas.h"

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