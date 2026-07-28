# Trabalho_jorge

# Sistema Acadêmico em C — Guia do Projeto & Divisão de Tarefas

Este repositório contém a implementação em **Linguagem C** de um **Sistema Acadêmico** para gerenciamento de alunos, disciplinas, matrículas e notas. 

O objetivo deste `README.md` é orientar a equipe de desenvolvimento (**Roberto**, **Pedro**, **Rennan** e **Hávila**) e fornecer contexto estruturado para o uso de assistentes de IA / agentes de código (como o Mimo Code).

---

## 1. Arquitetura Geral & Estruturas de Dados

O sistema utiliza arquivos binários (`.dat`) para persistência de dados e estruturas de dados bem definidas.

### Arquivos de Dados
- `alunos.dat`: Registro contínuo de estruturas do tipo `Aluno`.
- `disciplinas.dat`: Registro contínuo de estruturas do tipo `Disciplina`.
- `notas.dat`: Armazenamento da matriz 2D de notas (`Aluno` x `Disciplina`).

### Estruturas de Dados Propostas (`structs`)

```c
// Exemplo de referência para as estruturas do projeto

typedef struct {
    int id;
    char nome[100];
    int ativo; // 1 para ativo, 0 para removido/inativo
} Aluno;

typedef struct {
    int id;
    char nome[50];
    int cargaHoraria;
    int ativo;
} Disciplina;

// As notas serão gerenciadas por uma matriz 2D de float:
// float notas[MAX_ALUNOS][MAX_DISCIPLINAS];
```

---

## 2. Divisão de Tarefas por Membro

---

### **1. Roberto**
**Escopo:** Gestão de Alunos e Persistência de Alunos.

* **Tarefas Atribuídas:**
  1. Criar cadastro de alunos (`cadastrarAluno`).
  2. Salvar e carregar alunos (`salvarAlunos`, `carregarAlunos` em `alunos.dat`).
* **Responsabilidades Detalhadas:**
  * Implementar a função `void cadastrarAluno()` para capturar nome, ID/matrícula e demais dados relevantes.
  * Implementar funções de persistência binária (`fopen`, `fwrite`, `fread`, `fclose`) para gravar e ler no arquivo `alunos.dat`.
  * Garantir a validação dos dados de entrada (ex.: ID único, strings não vazias).

---

### **2. Pedro**
**Escopo:** Gestão de Disciplinas e Persistência de Disciplinas.

* **Tarefas Atribuídas:**
  1. Criar cadastro de disciplinas (`cadastrarDisciplina`).
  2. Salvar e carregar disciplinas (`salvarDisciplinas`, `carregarDisciplinas` em `disciplinas.dat`).
* **Responsabilidades Detalhadas:**
  * Implementar a função `void cadastrarDisciplina()` para cadastrar o código/ID e o nome da disciplina.
  * Implementar a rotina de leitura e escrita em disco no arquivo `disciplinas.dat`.
  * Disponibilizar funções para listar/buscar disciplinas cadastradas para suporte à etapa de matrícula.

---

### **3. Rennan**
**Escopo:** Matrículas, Lançamento de Notas e Persistência de Notas.

* **Tarefas Atribuídas:**
  1. Fazer matrícula dos alunos nas disciplinas (`matricularAluno`).
  2. Salvar e carregar notas (`lancarNotas`, `salvarNotas`, `carregarNotas` em `notas.dat`).
* **Responsabilidades Detalhadas:**
  * Implementar a associação entre `Aluno` e `Disciplina` (Matrícula).
  * Gerenciar a **Matriz 2D de Notas** (`Aluno x Disciplina`).
  * Criar a função `void lancarNotas()` para atribuir/atualizar as notas dos alunos nas disciplinas em que estão matriculados.
  * Implementar as funções de leitura e salvamento da matriz de notas no arquivo `notas.dat`.

---

### **4. Hávila**
**Escopo:** Cálculos de Média, Relatórios e Integração do Sistema.

* **Tarefas Atribuídas:**
  1. Calcular média dos alunos e gerar boletim/relatórios (`calcularMedia`, `listarBoletim`).
  2. Integrar tudo no menu principal e realizar testes gerais.
* **Responsabilidades Detalhadas:**
  * Implementar a função `void calcularMedia()` para processar as notas da matriz e obter a média final por disciplina e por aluno.
  * Implementar `void listarBoletim()` para exibir relatórios organizados e formatados no terminal.
  * Construir a interface em linha de comando (CLI) no `main()` unificando todos os módulos do sistema.
  * Realizar testes integrados garantindo que o fluxo completo (Cadastro -> Matrícula -> Notas -> Média -> Persistência) funcione sem erros.

---

## 3. Tabela Resumo das Funções

| Função | Responsável | Descrição / Arquivo Relacionado |
| :--- | :--- | :--- |
| `cadastrarAluno()` | **Roberto** | Insere novo aluno no sistema. |
| `salvarAlunos()` / `carregarAlunos()` | **Roberto** | Manipula o arquivo `alunos.dat`. |
| `cadastrarDisciplina()` | **Pedro** | Insere nova disciplina. |
| `salvarDisciplinas()` / `carregarDisciplinas()` | **Pedro** | Manipula o arquivo `disciplinas.dat`. |
| `matricularAluno()` | **Rennan** | Associa aluno a uma disciplina. |
| `lancarNotas()` | **Rennan** | Preenche/atualiza a matriz de notas. |
| `salvarNotas()` / `carregarNotas()` | **Rennan** | Manipula o arquivo `notas.dat`. |
| `calcularMedia()` | **Hávila** | Processa médias com base na matriz de notas. |
| `listarBoletim()` | **Hávila** | Exibe relatórios formatados na tela. |
| `main()` & Menu Interface | **Hávila** | Menu interativo e orquestração do programa. |

---

## 4. Regras de Estilo e Boas Práticas (C)

1. **Arquivos Binários:** Use `rb`, `wb`, `ab` ou `rb+` com cautela para garantir a integridade dos arquivos `.dat`.
2. **Modularização:** Mantenha os protótipos organizados em cabeçalhos (`.h`) e a implementação em arquivos `.c` ou com divisões bem comentadas.
3. **Tratamento de Arquivos:** Sempre verifique se `fopen` retornou `NULL` antes de tentar ler/escrever.
