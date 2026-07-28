#ifndef ALUNOS_H
#define ALUNOS_H

#define MAX_ALUNOS 100

typedef struct {
    int id;
    char nome[100];
    int ativo; // 1 para ativo, 0 para removido/inativo
} Aluno;

void cadastrarAluno(Aluno* alunos, int* qtdAlunos);
void salvarAlunos(const Aluno* alunos, int qtdAlunos);
void carregarAlunos(Aluno* alunos, int* qtdAlunos);
int buscarAlunoPorId(const Aluno* alunos, int qtdAlunos, int id);

#endif // ALUNOS_H
