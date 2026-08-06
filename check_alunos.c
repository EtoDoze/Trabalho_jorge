#include <stdio.h>
#include "alunos.h"

int main(void) {
    Aluno a[MAX_ALUNOS];
    int n = 0;
    carregarAlunos(a, &n);
    printf("Total de alunos em alunos.dat: %d\n", n);
    for (int i = 0; i < n; i++) {
        printf("Index %d -> ID: %d, Nome: '%s', Ativo: %d\n", i, a[i].id, a[i].nome, a[i].ativo);
    }
    return 0;
}
