#include <stdio.h>
#include <stdlib.h>
#include "notas.h"

// Função auxiliar para limpar o buffer de entrada do teclado (stdin)
static void limparBufferStdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inicializarNotas(float notas[MAX_ALUNOS][MAX_DISCIPLINAS]) {
    for (int i = 0; i < MAX_ALUNOS; i++) {
        for (int j = 0; j < MAX_DISCIPLINAS; j++) {
            notas[i][j] = NOTA_NAO_MATRICULADO;
        }
    }
}

int matricularAlunoDados(const Aluno* alunos, int qtdAlunos, const Disciplina* disciplinas, int qtdDisciplinas, float notas[MAX_ALUNOS][MAX_DISCIPLINAS], int idAluno, int idDisciplina, char* msgErro) {
    if (qtdAlunos <= 0) {
        if (msgErro) sprintf(msgErro, "Erro: Nao ha alunos cadastrados no sistema.");
        return 0;
    }
    if (qtdDisciplinas <= 0) {
        if (msgErro) sprintf(msgErro, "Erro: Nao ha disciplinas cadastradas no sistema.");
        return 0;
    }

    int idxAluno = buscarAlunoPorId(alunos, qtdAlunos, idAluno);
    if (idxAluno == -1 || !alunos[idxAluno].ativo) {
        if (msgErro) sprintf(msgErro, "Erro: Aluno com ID %d nao foi encontrado ou esta inativo.", idAluno);
        return 0;
    }

    int idxDisciplina = buscarDisciplinaPorId(disciplinas, qtdDisciplinas, idDisciplina);
    if (idxDisciplina == -1 || !disciplinas[idxDisciplina].ativo) {
        if (msgErro) sprintf(msgErro, "Erro: Disciplina com ID %d nao foi encontrada ou esta inativa.", idDisciplina);
        return 0;
    }

    if (notas[idxAluno][idxDisciplina] != NOTA_NAO_MATRICULADO) {
        if (msgErro) sprintf(msgErro, "Aviso: O aluno '%s' ja esta matriculado na disciplina '%s'.", alunos[idxAluno].nome, disciplinas[idxDisciplina].nome);
        return 0;
    }

    notas[idxAluno][idxDisciplina] = 0.0f;
    if (msgErro) sprintf(msgErro, "Sucesso: Aluno '%s' (ID: %d) matriculado na disciplina '%s' (ID: %d)!",
                         alunos[idxAluno].nome, alunos[idxAluno].id,
                         disciplinas[idxDisciplina].nome, disciplinas[idxDisciplina].id);
    return 1;
}

void matricularAluno(const Aluno* alunos, int qtdAlunos, const Disciplina* disciplinas, int qtdDisciplinas, float notas[MAX_ALUNOS][MAX_DISCIPLINAS]) {
    if (qtdAlunos <= 0) {
        printf("Erro: Nao ha alunos cadastrados no sistema.\n");
        return;
    }
    if (qtdDisciplinas <= 0) {
        printf("Erro: Nao ha disciplinas cadastradas no sistema.\n");
        return;
    }

    int idAluno, idDisciplina;
    printf("Digite o ID do aluno para matricula: ");
    if (scanf("%d", &idAluno) != 1) {
        printf("Entrada invalida para o ID do aluno.\n");
        limparBufferStdin();
        return;
    }
    limparBufferStdin();

    printf("Digite o ID da disciplina para matricula: ");
    if (scanf("%d", &idDisciplina) != 1) {
        printf("Entrada invalida para o ID da disciplina.\n");
        limparBufferStdin();
        return;
    }
    limparBufferStdin();

    char msg[256];
    matricularAlunoDados(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas, idAluno, idDisciplina, msg);
    printf("%s\n", msg);
}

int lancarNotasDados(const Aluno* alunos, int qtdAlunos, const Disciplina* disciplinas, int qtdDisciplinas, float notas[MAX_ALUNOS][MAX_DISCIPLINAS], int idAluno, int idDisciplina, float nota, char* msgErro) {
    if (qtdAlunos <= 0 || qtdDisciplinas <= 0) {
        if (msgErro) sprintf(msgErro, "Erro: Nao ha alunos ou disciplinas cadastrados para lancamento de notas.");
        return 0;
    }

    int idxAluno = buscarAlunoPorId(alunos, qtdAlunos, idAluno);
    if (idxAluno == -1 || !alunos[idxAluno].ativo) {
        if (msgErro) sprintf(msgErro, "Erro: Aluno com ID %d nao foi encontrado ou esta inativo.", idAluno);
        return 0;
    }

    int idxDisciplina = buscarDisciplinaPorId(disciplinas, qtdDisciplinas, idDisciplina);
    if (idxDisciplina == -1 || !disciplinas[idxDisciplina].ativo) {
        if (msgErro) sprintf(msgErro, "Erro: Disciplina com ID %d nao foi encontrada ou esta inativa.", idDisciplina);
        return 0;
    }

    if (notas[idxAluno][idxDisciplina] == NOTA_NAO_MATRICULADO) {
        if (msgErro) sprintf(msgErro, "Erro: O aluno '%s' NAO esta matriculado na disciplina '%s'. Faca a matricula primeiro.",
                             alunos[idxAluno].nome, disciplinas[idxDisciplina].nome);
        return 0;
    }

    if (nota < 0.0f || nota > 10.0f) {
        if (msgErro) sprintf(msgErro, "Erro: A nota deve estar no intervalo entre 0.0 e 10.0.");
        return 0;
    }

    notas[idxAluno][idxDisciplina] = nota;
    if (msgErro) sprintf(msgErro, "Sucesso: Nota %.2f lancada para o aluno '%s' na disciplina '%s'!",
                         nota, alunos[idxAluno].nome, disciplinas[idxDisciplina].nome);
    return 1;
}

void lancarNotas(const Aluno* alunos, int qtdAlunos, const Disciplina* disciplinas, int qtdDisciplinas, float notas[MAX_ALUNOS][MAX_DISCIPLINAS]) {
    if (qtdAlunos <= 0 || qtdDisciplinas <= 0) {
        printf("Erro: Nao ha alunos ou disciplinas cadastrados para lancamento de notas.\n");
        return;
    }

    int idAluno, idDisciplina;
    printf("Digite o ID do aluno: ");
    if (scanf("%d", &idAluno) != 1) {
        printf("Entrada invalida para o ID do aluno.\n");
        limparBufferStdin();
        return;
    }
    limparBufferStdin();

    printf("Digite o ID da disciplina: ");
    if (scanf("%d", &idDisciplina) != 1) {
        printf("Entrada invalida para o ID da disciplina.\n");
        limparBufferStdin();
        return;
    }
    limparBufferStdin();

    int idxAluno = buscarAlunoPorId(alunos, qtdAlunos, idAluno);
    int idxDisciplina = buscarDisciplinaPorId(disciplinas, qtdDisciplinas, idDisciplina);
    if (idxAluno == -1 || idxDisciplina == -1 || notas[idxAluno][idxDisciplina] == NOTA_NAO_MATRICULADO) {
        char msg[256];
        lancarNotasDados(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas, idAluno, idDisciplina, 0.0f, msg);
        printf("%s\n", msg);
        return;
    }

    float nota;
    int notaValida = 0;
    while (!notaValida) {
        printf("Digite a nota do aluno (0.0 a 10.0): ");
        if (scanf("%f", &nota) != 1) {
            printf("Entrada invalida. Digite um numero decimal ou inteiro.\n");
            limparBufferStdin();
            continue;
        }
        limparBufferStdin();

        if (nota < 0.0f || nota > 10.0f) {
            printf("Erro: A nota deve estar no intervalo entre 0.0 e 10.0.\n");
        } else {
            notaValida = 1;
        }
    }

    char msg[256];
    lancarNotasDados(alunos, qtdAlunos, disciplinas, qtdDisciplinas, notas, idAluno, idDisciplina, nota, msg);
    printf("%s\n", msg);
}


void salvarNotas(const float notas[MAX_ALUNOS][MAX_DISCIPLINAS]) {
    FILE* file = fopen("notas.dat", "wb");
    if (file == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo notas.dat para escrita.\n");
        return;
    }

    size_t totalElementos = MAX_ALUNOS * MAX_DISCIPLINAS;
    size_t gravados = fwrite(notas, sizeof(float), totalElementos, file);
    if (gravados != totalElementos) {
        printf("Erro ao salvar a matriz de notas no arquivo notas.dat.\n");
    } else {
        printf("Dados de notas salvos com sucesso em notas.dat!\n");
    }

    fclose(file);
}

void carregarNotas(float notas[MAX_ALUNOS][MAX_DISCIPLINAS]) {
    inicializarNotas(notas);

    FILE* file = fopen("notas.dat", "rb");
    if (file == NULL) {
        // Arquivo ainda não existe (primeira execução)
        return;
    }

    size_t totalElementos = MAX_ALUNOS * MAX_DISCIPLINAS;
    size_t lidos = fread(notas, sizeof(float), totalElementos, file);
    if (lidos != totalElementos) {
        printf("Aviso: Leitura incompleta do arquivo notas.dat.\n");
    }

    fclose(file);
}
