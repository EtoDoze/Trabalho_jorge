#ifndef DISCIPLINAS_H
#define DISCIPLINAS_H

#define MAX_DISCIPLINAS 50

typedef struct {
    int id;
    char nome[50];
    int cargaHoraria;
    int ativo; // 1 para ativo, 0 para removido/inativo
} Disciplina;

// Protótipos mantidos para o módulo de disciplinas (Escopo do Pedro)
void cadastrarDisciplina(Disciplina* disciplinas, int* qtdDisciplinas);
int cadastrarDisciplinaDados(Disciplina* disciplinas, int* qtdDisciplinas, int id, const char* nome, int cargaHoraria, char* msgErro);
void removerDisciplina(Disciplina* disciplinas, int qtdDisciplinas);
int removerDisciplinaDados(Disciplina* disciplinas, int qtdDisciplinas, int id, char* msgOut);
void salvarDisciplinas(const Disciplina* disciplinas, int qtdDisciplinas);
void carregarDisciplinas(Disciplina* disciplinas, int* qtdDisciplinas);
int buscarDisciplinaPorId(const Disciplina* disciplinas, int qtdDisciplinas, int id);
void listarDisciplinas(const Disciplina* disciplinas, int qtdDisciplinas);
#endif // DISCIPLINAS_H


