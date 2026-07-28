#include <stdio.h>
#include "alunos.h"

int main() {
    Aluno alunos[MAX_ALUNOS];
    int qtdAlunos = 0;

    printf("--- Carregando alunos existentes ---\n");
    carregarAlunos(alunos, &qtdAlunos);
    printf("Total de alunos carregados: %d\n", qtdAlunos);

    printf("\n--- Cadastrando um novo aluno ---\n");
    cadastrarAluno(alunos, &qtdAlunos);

    printf("\n--- Salvando alunos ---\n");
    salvarAlunos(alunos, qtdAlunos);

    printf("\n--- Listando alunos em memoria ---\n");
    for (int i = 0; i < qtdAlunos; i++) {
        printf("ID: %d | Nome: %s | Ativo: %d\n", alunos[i].id, alunos[i].nome, alunos[i].ativo);
    }

    return 0;
}
