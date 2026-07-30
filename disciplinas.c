#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disciplinas.h"

/*
 * Procura uma disciplina pelo ID.
 *
 * Retorna:
 * - o índice da disciplina no vetor, caso seja encontrada;
 * - -1, caso não exista.
 */
int buscarDisciplinaPorId(
    const Disciplina* disciplinas,
    int qtdDisciplinas,
    int id
) {
    for (int i = 0; i < qtdDisciplinas; i++) {
        if (disciplinas[i].id == id) {
            return i;
        }
    }

    return -1;
}

/*
 * Cadastra uma nova disciplina no vetor.
 */
void cadastrarDisciplina(
    Disciplina* disciplinas,
    int* qtdDisciplinas
) {
    Disciplina novaDisciplina;

    /*
     * Verifica se o vetor já atingiu
     * a quantidade máxima permitida.
     */
    if (*qtdDisciplinas >= MAX_DISCIPLINAS) {
        printf("\nLimite máximo de disciplinas atingido.\n");
        return;
    }

    printf("\n========================================\n");
    printf("        CADASTRO DE DISCIPLINA\n");
    printf("========================================\n");

    /*
     * Solicita o ID da disciplina.
     */
    printf("Digite o ID da disciplina: ");

    if (scanf("%d", &novaDisciplina.id) != 1) {
        printf("\nID inválido.\n");

        while (getchar() != '\n') {
        }

        return;
    }

    while (getchar() != '\n') {
    }

    /*
     * Verifica se já existe uma disciplina
     * com o mesmo ID.
     */
    if (
        buscarDisciplinaPorId(
            disciplinas,
            *qtdDisciplinas,
            novaDisciplina.id
        ) != -1
    ) {
        printf("\nJá existe uma disciplina com esse ID.\n");
        return;
    }

    /*
     * Solicita o nome da disciplina.
     */
    printf("Digite o nome da disciplina: ");

    fgets(
        novaDisciplina.nome,
        sizeof(novaDisciplina.nome),
        stdin
    );

    /*
     * Remove o caractere de quebra de linha
     * colocado pelo fgets.
     */
    novaDisciplina.nome[
        strcspn(novaDisciplina.nome, "\n")
    ] = '\0';

    /*
     * Verifica se o nome ficou vazio.
     */
    if (strlen(novaDisciplina.nome) == 0) {
        printf("\nO nome da disciplina não pode ficar vazio.\n");
        return;
    }

    /*
     * Solicita a carga horária.
     */
    printf("Digite a carga horária: ");

    if (scanf("%d", &novaDisciplina.cargaHoraria) != 1) {
        printf("\nCarga horária inválida.\n");

        while (getchar() != '\n') {
        }

        return;
    }

    while (getchar() != '\n') {
    }

    /*
     * A carga horária deve ser maior que zero.
     */
    if (novaDisciplina.cargaHoraria <= 0) {
        printf("\nA carga horária deve ser maior que zero.\n");
        return;
    }

    /*
     * A nova disciplina é cadastrada como ativa.
     */
    novaDisciplina.ativo = 1;

    /*
     * Adiciona a disciplina na próxima posição livre.
     */
    disciplinas[*qtdDisciplinas] = novaDisciplina;

    /*
     * Atualiza a quantidade de disciplinas cadastradas.
     */
    (*qtdDisciplinas)++;

    printf("\nDisciplina cadastrada com sucesso.\n");
}

/*
 * Salva as disciplinas em um arquivo binário.
 */
void salvarDisciplinas(
    const Disciplina* disciplinas,
    int qtdDisciplinas
) {
    /*
     * Abre ou cria o arquivo disciplinas.dat
     * no modo de escrita binária.
     */
    FILE* arquivo = fopen("disciplinas.dat", "wb");

    if (arquivo == NULL) {
        printf("\nErro ao abrir disciplinas.dat para gravação.\n");
        return;
    }

    /*
     * Salva primeiro a quantidade de disciplinas.
     */
    fwrite(
        &qtdDisciplinas,
        sizeof(int),
        1,
        arquivo
    );

    /*
     * Salva o vetor de disciplinas.
     */
    if (qtdDisciplinas > 0) {
        fwrite(
            disciplinas,
            sizeof(Disciplina),
            qtdDisciplinas,
            arquivo
        );
    }

    fclose(arquivo);
}

/*
 * Carrega as disciplinas salvas no arquivo binário.
 */
void carregarDisciplinas(
    Disciplina* disciplinas,
    int* qtdDisciplinas
) {
    /*
     * Abre o arquivo no modo de leitura binária.
     */
    FILE* arquivo = fopen("disciplinas.dat", "rb");

    /*
     * Caso o arquivo ainda não exista,
     * inicia a quantidade com zero.
     */
    if (arquivo == NULL) {
        *qtdDisciplinas = 0;
        return;
    }

    /*
     * Lê a quantidade de disciplinas salvas.
     */
    if (
        fread(
            qtdDisciplinas,
            sizeof(int),
            1,
            arquivo
        ) != 1
    ) {
        printf("\nErro ao ler a quantidade de disciplinas.\n");

        *qtdDisciplinas = 0;

        fclose(arquivo);

        return;
    }

    /*
     * Proteção contra arquivo inválido ou corrompido.
     */
    if (
        *qtdDisciplinas < 0 ||
        *qtdDisciplinas > MAX_DISCIPLINAS
    ) {
        printf("\nArquivo disciplinas.dat inválido.\n");

        *qtdDisciplinas = 0;

        fclose(arquivo);

        return;
    }

    /*
     * Carrega o vetor de disciplinas.
     */
    if (*qtdDisciplinas > 0) {
        size_t quantidadeLida = fread(
            disciplinas,
            sizeof(Disciplina),
            *qtdDisciplinas,
            arquivo
        );

        /*
         * Se não conseguir carregar todas,
         * utiliza somente a quantidade lida.
         */
        if (quantidadeLida != (size_t)*qtdDisciplinas) {
            printf(
                "\nAviso: nem todas as disciplinas foram carregadas.\n"
            );

            *qtdDisciplinas = (int)quantidadeLida;
        }
    }

    fclose(arquivo);
}