#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disciplinas.h"

#ifdef _WIN32
#include <windows.h>
static void garantirUtf8(char* str, size_t maxLen) {
    if (!str || str[0] == '\0') return;
    wchar_t wbuf[1024];
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, wbuf, 1024) == 0) {
        if (MultiByteToWideChar(CP_ACP, 0, str, -1, wbuf, 1024) > 0) {
            WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, str, (int)maxLen, NULL, NULL);
        }
    }
}
#else
static void garantirUtf8(char* str, size_t maxLen) { (void)str; (void)maxLen; }
#endif

/*
 * Procura uma disciplina pelo ID.
 *
 * Retorna:
 * - o índice da disciplina no vetor, caso seja encontrada;
 * - -1, caso não exista.
 */
int buscarDisciplinaQualquerPorId(
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

int buscarDisciplinaPorId(
    const Disciplina* disciplinas,
    int qtdDisciplinas,
    int id
) {
    for (int i = 0; i < qtdDisciplinas; i++) {
        if (disciplinas[i].ativo && disciplinas[i].id == id) {
            return i;
        }
    }

    return -1;
}

int cadastrarDisciplinaDados(
    Disciplina* disciplinas,
    int* qtdDisciplinas,
    int id,
    const char* nome,
    int cargaHoraria,
    char* msgErro
) {
    if (id <= 0) {
        if (msgErro) sprintf(msgErro, "Erro: O ID da disciplina deve ser positivo.");
        return 0;
    }
    if (buscarDisciplinaPorId(disciplinas, *qtdDisciplinas, id) != -1) {
        if (msgErro) sprintf(msgErro, "Erro: Ja existe uma disciplina ativa cadastrada com o ID %d.", id);
        return 0;
    }
    if (nome == NULL || strlen(nome) == 0) {
        if (msgErro) sprintf(msgErro, "Erro: O nome da disciplina nao pode ser vazio.");
        return 0;
    }
    if (cargaHoraria <= 0) {
        if (msgErro) sprintf(msgErro, "Erro: A carga horaria deve ser maior que zero.");
        return 0;
    }

    int idxQualquer = buscarDisciplinaQualquerPorId(disciplinas, *qtdDisciplinas, id);
    int targetIdx;
    if (idxQualquer != -1) {
        targetIdx = idxQualquer;
    } else {
        if (*qtdDisciplinas >= MAX_DISCIPLINAS) {
            if (msgErro) sprintf(msgErro, "Erro: Limite maximo de disciplinas (%d) atingido.", MAX_DISCIPLINAS);
            return 0;
        }
        targetIdx = (*qtdDisciplinas);
        (*qtdDisciplinas)++;
    }

    disciplinas[targetIdx].id = id;
    strncpy(disciplinas[targetIdx].nome, nome, sizeof(disciplinas[targetIdx].nome) - 1);
    disciplinas[targetIdx].nome[sizeof(disciplinas[targetIdx].nome) - 1] = '\0';
    garantirUtf8(disciplinas[targetIdx].nome, sizeof(disciplinas[targetIdx].nome));

    size_t len = strlen(disciplinas[targetIdx].nome);
    while (len > 0 && (disciplinas[targetIdx].nome[len - 1] == '\n' || disciplinas[targetIdx].nome[len - 1] == '\r')) {
        disciplinas[targetIdx].nome[len - 1] = '\0';
        len--;
    }

    disciplinas[targetIdx].cargaHoraria = cargaHoraria;
    disciplinas[targetIdx].ativo = 1;

    if (msgErro) sprintf(msgErro, "Disciplina '%s' (ID: %d, %dh) cadastrada com sucesso!", disciplinas[targetIdx].nome, id, cargaHoraria);
    return 1;
}


/*
 * Cadastra uma nova disciplina no vetor.
 */
void cadastrarDisciplina(
    Disciplina* disciplinas,
    int* qtdDisciplinas
) {
    Disciplina novaDisciplina;

    if (*qtdDisciplinas >= MAX_DISCIPLINAS) {
        printf("\nLimite maximo de disciplinas atingido.\n");
        return;
    }

    printf("\n========================================\n");
    printf("        CADASTRO DE DISCIPLINA\n");
    printf("========================================\n");

    printf("Digite o ID da disciplina: ");
    if (scanf("%d", &novaDisciplina.id) != 1) {
        printf("\nID invalido.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (buscarDisciplinaPorId(disciplinas, *qtdDisciplinas, novaDisciplina.id) != -1) {
        printf("\nJa existe uma disciplina com esse ID.\n");
        return;
    }

    printf("Digite o nome da disciplina: ");
    fgets(novaDisciplina.nome, sizeof(novaDisciplina.nome), stdin);
    novaDisciplina.nome[strcspn(novaDisciplina.nome, "\n")] = '\0';

    if (strlen(novaDisciplina.nome) == 0) {
        printf("\nO nome da disciplina nao pode ficar vazio.\n");
        return;
    }

    printf("Digite a carga horaria: ");
    if (scanf("%d", &novaDisciplina.cargaHoraria) != 1) {
        printf("\nCarga horaria invalida.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    char msg[256];
    if (cadastrarDisciplinaDados(disciplinas, qtdDisciplinas, novaDisciplina.id, novaDisciplina.nome, novaDisciplina.cargaHoraria, msg)) {
        printf("\n%s\n", msg);
    } else {
        printf("\n%s\n", msg);
    }
}

int removerDisciplinaDados(Disciplina* disciplinas, int qtdDisciplinas, int id, char* msgOut) {
    int idx = buscarDisciplinaPorId(disciplinas, qtdDisciplinas, id);
    if (idx == -1 || !disciplinas[idx].ativo) {
        if (msgOut) sprintf(msgOut, "Erro: Disciplina com ID %d nao foi encontrada ou ja esta inativa.", id);
        return 0;
    }
    disciplinas[idx].ativo = 0;
    if (msgOut) sprintf(msgOut, "Sucesso: Disciplina '%s' (ID: %d) foi removida da memoria com sucesso!", disciplinas[idx].nome, id);
    return 1;
}

void removerDisciplina(Disciplina* disciplinas, int qtdDisciplinas) {
    if (qtdDisciplinas <= 0) {
        printf("Erro: Nao ha disciplinas cadastradas no sistema.\n");
        return;
    }
    int id;
    printf("Digite o ID da disciplina que deseja remover: ");
    if (scanf("%d", &id) != 1) {
        printf("ID invalido.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    char msg[256];
    removerDisciplinaDados(disciplinas, qtdDisciplinas, id, msg);
    printf("%s\n", msg);
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

    for (int i = 0; i < *qtdDisciplinas; i++) {
        garantirUtf8(disciplinas[i].nome, sizeof(disciplinas[i].nome));
    }

    fclose(arquivo);
}
/*
 * Lista todas as disciplinas cadastradas.
 */
void listarDisciplinas(
    const Disciplina* disciplinas,
    int qtdDisciplinas
) {
    printf("\n========================================\n");
    printf("         LISTA DE DISCIPLINAS\n");
    printf("========================================\n");

    if (qtdDisciplinas == 0) {
        printf("Nenhuma disciplina cadastrada.\n");
        printf("========================================\n");
        return;
    }

    printf("%-5s %-30s %-10s\n", "ID", "NOME", "CARGAHORÁRIA");

    int encontrou = 0;

    for (int i = 0; i < qtdDisciplinas; i++) {
        if (disciplinas[i].ativo) {
            printf("%-5d %-30s %-10d\n",
                   disciplinas[i].id,
                   disciplinas[i].nome,
                   disciplinas[i].cargaHoraria);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhuma disciplina ativa cadastrada.\n");
    }

    printf("========================================\n");
}
