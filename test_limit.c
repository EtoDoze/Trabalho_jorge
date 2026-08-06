#include <stdio.h>
#include <assert.h>
#include "alunos.h"

int main(void) {
    Aluno a[MAX_ALUNOS];
    int qtd = 0;
    char msg[256];

    printf("Testando cadastro continuo de 10 alunos...\n");
    for (int i = 1; i <= 10; i++) {
        char nome[50];
        sprintf(nome, "Aluno Teste %d", i);
        assert(cadastrarAlunoDados(a, &qtd, i, nome, msg) == 1);
    }
    assert(qtd == 10);
    printf("[PASS] 10 alunos cadastrados com sucesso!\n");

    // Remover aluno de ID 3
    assert(removerAlunoDados(a, qtd, 3, msg) == 1);

    // Re-cadastrar aluno com ID 3 (reutilização de slot inativo)
    assert(cadastrarAlunoDados(a, &qtd, 3, "Aluno Novo Reutilizado", msg) == 1);
    printf("[PASS] Reutilizacao de ID desativado (ID 3) OK!\n");

    printf("=== TESTE DE LIMITE E ID OK ===\n");
    return 0;
}
