#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "alunos.h"
#include "disciplinas.h"
#include "notas.h"
#include "medias.h"

int main(void) {
    printf("=== TESTE AUTOMATIZADO DAS NOVAS FUNCIONALIDADES DO SISTEMA ===\n");

    Aluno alunos[MAX_ALUNOS];
    Disciplina disciplinas[MAX_DISCIPLINAS];
    float notas[MAX_ALUNOS][MAX_DISCIPLINAS];
    int qtdAlunos = 0;
    int qtdDisciplinas = 0;
    char msg[256];

    inicializarNotas(notas);

    // Teste 1: Cadastro
    assert(cadastrarAlunoDados(alunos, &qtdAlunos, 101, "Rennan Silva", msg) == 1);
    assert(cadastrarAlunoDados(alunos, &qtdAlunos, 102, "Roberto Souza", msg) == 1);
    assert(cadastrarDisciplinaDados(disciplinas, &qtdDisciplinas, 1, "Estrutura de Dados", 80, msg) == 1);
    assert(matricularAlunoDados(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas, 101, 1, msg) == 1);
    assert(lancarNotasDados(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas, 101, 1, 9.5f, msg) == 1);
    printf("[PASS] Cadastro, Matricula e Lancamento de Nota OK.\n");

    // Teste 2: Verificar Nome no Boletim
    BoletimAluno bol;
    assert(gerarBoletimDados(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas, 101, &bol, msg) == 1);
    assert(strcmp(bol.nomeAluno, "Rennan Silva") == 0);
    assert(bol.idAluno == 101);
    assert(bol.mediaGeral == 9.5f);
    printf("[PASS] Boletim contendo o NOME do Aluno ('%s') OK.\n", bol.nomeAluno);

    // Teste 3: Remoção de Aluno
    assert(removerAlunoDados(alunos, qtdAlunos, 102, msg) == 1);
    assert(alunos[1].ativo == 0);
    // Tentar remover aluno inativo deve falhar
    assert(removerAlunoDados(alunos, qtdAlunos, 102, msg) == 0);
    printf("[PASS] Remocao de Aluno da Memoria OK.\n");

    // Teste 4: Remoção de Disciplina
    assert(removerDisciplinaDados(disciplinas, qtdDisciplinas, 1, msg) == 1);
    assert(disciplinas[0].ativo == 0);
    assert(removerDisciplinaDados(disciplinas, qtdDisciplinas, 1, msg) == 0);
    printf("[PASS] Remocao de Disciplina da Memoria OK.\n");

    printf("\n=== TODOS OS TESTES DAS NOVAS REVISOES PASSARAM! ===\n");
    return 0;
}
