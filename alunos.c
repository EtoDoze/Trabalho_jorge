#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alunos.h"

int buscarAlunoPorId(const Aluno* alunos, int qtdAlunos, int id) {
    for (int i = 0; i < qtdAlunos; i++) {
        if (alunos[i].id == id) {
            return i;
        }
    }
    return -1;
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
            // Limpar o buffer do teclado
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        // Limpar o buffer do teclado apos leitura do inteiro
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
            // Remover \n do final da string
            size_t len = strlen(nome);
            if (len > 0 && nome[len - 1] == '\n') {
                nome[len - 1] = '\0';
                len--;
            }

            // Validar se o nome nao e vazio ou composto apenas por espacos
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

    alunos[*qtdAlunos].id = id;
    strcpy(alunos[*qtdAlunos].nome, nome);
    alunos[*qtdAlunos].ativo = 1;
    (*qtdAlunos)++;

    printf("Aluno cadastrado com sucesso!\n");
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
