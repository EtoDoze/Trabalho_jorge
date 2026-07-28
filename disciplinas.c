#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disciplinas.h"

int buscarDisciplinaPorId(const Disciplina* disciplinas, int qtdDisciplinas, int id) {
    for (int i = 0; i < qtdDisciplinas; i++) {
        if (disciplinas[i].id == id) {
            return i;
        }
    }
    return -1;
}

/*
 * Observação: As funções abaixo pertencem ao escopo do Pedro e serão implementadas por ele:
 * - cadastrarDisciplina
 * - salvarDisciplinas
 * - carregarDisciplinas
 */
