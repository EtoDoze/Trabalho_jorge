#ifndef NOTAS_H
#define NOTAS_H

#include "alunos.h"
#include "disciplinas.h"

// Valor sentinela para indicar que o aluno não está matriculado na disciplina
#define NOTA_NAO_MATRICULADO -1.0f

// Inicializa toda a matriz 2D de notas com NOTA_NAO_MATRICULADO (-1.0f)
void inicializarNotas(float notas[MAX_ALUNOS][MAX_DISCIPLINAS]);

// Realiza a matrícula de um aluno em uma disciplina (associação Aluno x Disciplina)
void matricularAluno(const Aluno* alunos, int qtdAlunos, const Disciplina* disciplinas, int qtdDisciplinas, float notas[MAX_ALUNOS][MAX_DISCIPLINAS]);

// Atribui ou atualiza a nota de um aluno em uma disciplina na qual está matriculado
void lancarNotas(const Aluno* alunos, int qtdAlunos, const Disciplina* disciplinas, int qtdDisciplinas, float notas[MAX_ALUNOS][MAX_DISCIPLINAS]);

// Salva a matriz 2D de notas no arquivo binário "notas.dat"
void salvarNotas(const float notas[MAX_ALUNOS][MAX_DISCIPLINAS]);

// Carrega a matriz 2D de notas do arquivo binário "notas.dat"
void carregarNotas(float notas[MAX_ALUNOS][MAX_DISCIPLINAS]);

#endif // NOTAS_H
