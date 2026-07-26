void cadastrarDisciplina() {
    printf("\n--- CADASTRO DE DISCIPLINA ---\n");

    if (totalDisciplinas >= MAX_DISCIPLINAS) {
        printf("Erro: Limite maximo de disciplinas atingido!\n");
        return;
    }

    disciplinas[totalDisciplinas].id = totalDisciplinas;

    printf("Digite o nome da disciplina: ");
    fgets(disciplinas[totalDisciplinas].nome, MAX_NOME, stdin);
    disciplinas[totalDisciplinas].nome[strcspn(disciplinas[totalDisciplinas].nome, "\n")] = '\0';

    printf("Disciplina '%s' cadastrada com sucesso! (ID: %d)\n", 
           disciplinas[totalDisciplinas].nome, totalDisciplinas);

    totalDisciplinas++;
}