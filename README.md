# Estruturas de dados em C

Mini-projetos da disciplina de Estruturas de Dados da UFS. Cada mini-projeto se trata de fazer uso de uma estrutura de dados aplicada a um problema da empresa [Poxim Tech](https://pt.wikipedia.org/wiki/Rio_Poxim).

## Rodando cada código

```bash
gcc -Wall -O3 arquivo.c
./a.out <arquivo de input> <arquivo de output>
```

Cada mini-projeto vem com:
* Um arquivo markdown explicando o problema e como a estrutura de dados é esperada se comportar
* Um arquivo de input pequeno, o `light.input`;
* Um arquivo de output correspondente ao `light.input`, chamado `light.output`;
* Um arquivo de input mediano pra grande (2 - 5 MB), o `heavy.input`;
* O código fonte do programa em C, o `arquivo.c`.

**O programa deve ser capaz de rodar esses inputs em menos de 1 segundo**. Se ele não for capaz de fazer isso, abra um issue detalhando o seu caso.
