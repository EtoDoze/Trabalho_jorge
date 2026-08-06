#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alunos.h"

int buscarAlunoQualquerPorId(const Aluno* alunos, int qtdAlunos, int id) {
    for (int i = 0; i < qtdAlunos; i++) {
        if (alunos[i].id == id) {
            return i;
        }
    }
    return -1;
}

int buscarAlunoPorId(const Aluno* alunos, int qtdAlunos, int id) {
    for (int i = 0; i < qtdAlunos; i++) {
        if (alunos[i].ativo && alunos[i].id == id) {
            return i;
        }
    }
    return -1;
}

int cadastrarAlunoDados(Aluno* alunos, int* qtdAlunos, int id, const char* nome, char* msgErro) {
    if (id <= 0) {
        if (msgErro) sprintf(msgErro, "Erro: O ID deve ser um numero positivo.");
        return 0;
    }
    if (buscarAlunoPorId(alunos, *qtdAlunos, id) != -1) {
        if (msgErro) sprintf(msgErro, "Erro: Ja existe um aluno ativo cadastrado com o ID %d.", id);
        return 0;
    }
    if (nome == NULL || strlen(nome) == 0) {
        if (msgErro) sprintf(msgErro, "Erro: O nome do aluno nao pode ser vazio.");
        return 0;
    }

    int apenasEspaco = 1;
    size_t len = strlen(nome);
    for (size_t i = 0; i < len; i++) {
        if (nome[i] != ' ' && nome[i] != '\t' && nome[i] != '\n' && nome[i] != '\r') {
            apenasEspaco = 0;
            break;
        }
    }
    if (apenasEspaco) {
        if (msgErro) sprintf(msgErro, "Erro: O nome do aluno nao pode conter apenas espacos.");
        return 0;
    }

    int idxQualquer = buscarAlunoQualquerPorId(alunos, *qtdAlunos, id);
    int targetIdx;
    if (idxQualquer != -1) {
        targetIdx = idxQualquer;
    } else {
        if (*qtdAlunos >= MAX_ALUNOS) {
            if (msgErro) sprintf(msgErro, "Erro: Limite maximo de alunos cadastrados (%d) atingido.", MAX_ALUNOS);
            return 0;
        }
        targetIdx = (*qtdAlunos);
        (*qtdAlunos)++;
    }

    alunos[targetIdx].id = id;
    strncpy(alunos[targetIdx].nome, nome, sizeof(alunos[targetIdx].nome) - 1);
    alunos[targetIdx].nome[sizeof(alunos[targetIdx].nome) - 1] = '\0';

    size_t nlen = strlen(alunos[targetIdx].nome);
    while (nlen > 0 && (alunos[targetIdx].nome[nlen - 1] == '\n' || alunos[targetIdx].nome[nlen - 1] == '\r')) {
        alunos[targetIdx].nome[nlen - 1] = '\0';
        nlen--;
    }

    alunos[targetIdx].ativo = 1;

    if (msgErro) sprintf(msgErro, "Aluno '%s' (ID: %d) cadastrado com sucesso!", alunos[targetIdx].nome, id);
    return 1;
}


void cadastrarAluno(Aluno* alunos, int* qtdAlunos) {
    if (*qtdAlunos >= MAX_ALUNOS) {
        printf("Erro: Limite maximo de alunos cadastrados (%d) atingido.\n", MAX_ALUNOS);
        return;
    }

    int id;
    int idValido = 0;
    while (!idValido) {
        printf("Digite o ID do aluno (numero positivo): ");
        if (scanf("%d", &id) != 1) {
            printf("Entrada invalida. Digite um numero inteiro.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (id <= 0) {
            printf("O ID deve ser um numero positivo.\n");
        } else if (buscarAlunoPorId(alunos, *qtdAlunos, id) != -1) {
            printf("Erro: Ja existe um aluno cadastrado com este ID.\n");
        } else {
            idValido = 1;
        }
    }

    char nome[100];
    int nomeValido = 0;
    while (!nomeValido) {
        printf("Digite o nome do aluno: ");
        if (fgets(nome, sizeof(nome), stdin) != NULL) {
            size_t len = strlen(nome);
            if (len > 0 && nome[len - 1] == '\n') {
                nome[len - 1] = '\0';
                len--;
            }

            int apenasEspaco = 1;
            for (size_t i = 0; i < len; i++) {
                if (nome[i] != ' ' && nome[i] != '\t') {
                    apenasEspaco = 0;
                    break;
                }
            }

            if (len == 0 || apenasEspaco) {
                printf("Erro: O nome nao pode ser vazio.\n");
            } else {
                nomeValido = 1;
            }
        } else {
            printf("Erro ao ler o nome. Tente novamente.\n");
        }
    }

    char msg[256];
    if (cadastrarAlunoDados(alunos, qtdAlunos, id, nome, msg)) {
        printf("%s\n", msg);
    } else {
        printf("%s\n", msg);
    }
}

int removerAlunoDados(Aluno* alunos, int qtdAlunos, int id, char* msgOut) {
    int idx = buscarAlunoPorId(alunos, qtdAlunos, id);
    if (idx == -1 || !alunos[idx].ativo) {
        if (msgOut) sprintf(msgOut, "Erro: Aluno com ID %d nao foi encontrado ou ja esta inativo.", id);
        return 0;
    }
    alunos[idx].ativo = 0;
    if (msgOut) sprintf(msgOut, "Sucesso: Aluno '%s' (ID: %d) foi removido da memoria com sucesso!", alunos[idx].nome, id);
    return 1;
}

void removerAluno(Aluno* alunos, int qtdAlunos) {
    if (qtdAlunos <= 0) {
        printf("Erro: Nao ha alunos cadastrados no sistema.\n");
        return;
    }
    int id;
    printf("Digite o ID do aluno que deseja remover: ");
    if (scanf("%d", &id) != 1) {
        printf("ID invalido.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    char msg[256];
    removerAlunoDados(alunos, qtdAlunos, id, msg);
    printf("%s\n", msg);
}



void salvarAlunos(const Aluno* alunos, int qtdAlunos) {
    FILE* file = fopen("alunos.dat", "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo alunos.dat para escrita.\n");
        return;
    }

    size_t gravados = fwrite(alunos, sizeof(Aluno), qtdAlunos, file);
    if (gravados != (size_t)qtdAlunos) {
        printf("Erro ao salvar todos os registros de alunos.\n");
    } else {
        printf("Dados de alunos salvos com sucesso!\n");
    }

    fclose(file);
}

void carregarAlunos(Aluno* alunos, int* qtdAlunos) {
    *qtdAlunos = 0;
    FILE* file = fopen("alunos.dat", "rb");
    if (file == NULL) {
        // Arquivo nao existe ainda (comum na primeira execucao)
        return;
    }

    while (*qtdAlunos < MAX_ALUNOS && fread(&alunos[*qtdAlunos], sizeof(Aluno), 1, file) == 1) {
        (*qtdAlunos)++;
    }

    if (!feof(file) && *qtdAlunos >= MAX_ALUNOS) {
        printf("Aviso: Limite maximo de alunos atingido ao carregar do arquivo.\n");
    }

    fclose(file);
}
