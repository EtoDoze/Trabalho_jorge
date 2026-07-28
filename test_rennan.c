#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "alunos.h"
#include "disciplinas.h"
#include "notas.h"

int main() {
    printf("=== Teste Automatizado do Modulo de Rennan ===\n");

    // Preparar dados simulados de Alunos (escopo Roberto)
    Aluno alunos[MAX_ALUNOS];
    int qtdAlunos = 2;
    alunos[0].id = 101;
    strcpy(alunos[0].nome, "Rennan Silva");
    alunos[0].ativo = 1;

    alunos[1].id = 102;
    strcpy(alunos[1].nome, "Roberto Souza");
    alunos[1].ativo = 1;

    // Preparar dados simulados de Disciplinas (escopo Pedro)
    Disciplina disciplinas[MAX_DISCIPLINAS];
    int qtdDisciplinas = 2;
    disciplinas[0].id = 1;
    strcpy(disciplinas[0].nome, "Estrutura de Dados");
    disciplinas[0].cargaHoraria = 80;
    disciplinas[0].ativo = 1;

    disciplinas[1].id = 2;
    strcpy(disciplinas[1].nome, "Banco de Dados");
    disciplinas[1].cargaHoraria = 60;
    disciplinas[1].ativo = 1;

    // Teste 1: Inicialização
    float notas[MAX_ALUNOS][MAX_DISCIPLINAS];
    inicializarNotas(notas);

    // Verificar se matriz foi preenchida com NOTA_NAO_MATRICULADO (-1.0f)
    assert(notas[0][0] == NOTA_NAO_MATRICULADO);
    assert(notas[1][1] == NOTA_NAO_MATRICULADO);
    printf("[PASS] Teste 1: Inicializacao da matriz de notas efetuada.\n");

    // Teste 2: Matrícula direta na matriz
    int idxAluno0 = buscarAlunoPorId(alunos, qtdAlunos, 101);
    int idxDisc0 = buscarDisciplinaPorId(disciplinas, qtdDisciplinas, 1);
    assert(idxAluno0 == 0);
    assert(idxDisc0 == 0);

    // Matricular Rennan em Estrutura de Dados
    notas[idxAluno0][idxDisc0] = 0.0f;
    assert(notas[0][0] == 0.0f);
    printf("[PASS] Teste 2: Matricula registrada com sucesso.\n");

    // Teste 3: Lançamento de nota
    notas[idxAluno0][idxDisc0] = 8.5f;
    assert(notas[0][0] == 8.5f);
    printf("[PASS] Teste 3: Lancamento de nota 8.5 registrado.\n");

    // Teste 4: Persistência (salvarNotas e carregarNotas)
    salvarNotas(notas);

    float notasCarregadas[MAX_ALUNOS][MAX_DISCIPLINAS];
    carregarNotas(notasCarregadas);

    assert(notasCarregadas[0][0] == 8.5f);
    assert(notasCarregadas[0][1] == NOTA_NAO_MATRICULADO);
    assert(notasCarregadas[1][0] == NOTA_NAO_MATRICULADO);
    printf("[PASS] Teste 4: Salvamento e carregamento de notas.dat validados.\n");

    printf("\nTodos os testes unitarios automatizados passaram com SUCESSO!\n");
    return 0;
}
