#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
/*
    Código em C para a implementação de linked lists
    Estruturas de Dados - UFS 
    Pedro Vinícius de Araújo Barreto
*/

FILE* input = 0;
FILE* output = 0;

typedef struct element element;

struct element{
    // Nome da pessoa na rede social
    char nome[50];
    // Ponteiro para o próximo element
    element *P;
};

// Por ser lista cíclica, será interessante ter função para identificar o ultimo elemento atual da lista:

element* last(element* cabeca_lista){
    element* temp = cabeca_lista;
    do {
        temp = temp->P;
    } while (temp->P != cabeca_lista);
    return temp;
}

// Função para checar se tem pessoa repetida
int contains_name(element* elem, char* searched_name){
    element* temp = elem;
    if(!elem->P){return 0;}
    do {
            if(strcmp(temp->nome, searched_name) == 0){
                return 1;
            }
            temp = temp->P;
    }while (temp != elem);
    return 0;
}

// Função para adicionar uma nova pessoa na lista, inclusive no meio dela
// lista atual refere ao elemento ao qual vc quer posicionar o novo elemento imediatamente a direita dele
element* create_linked_list(char *nome_novo){
    element* prim_elem = malloc(sizeof(element));
    strcpy(prim_elem->nome, nome_novo);
    return prim_elem;
}

// Aqui, elem_inicial representa o elemento em que a iteração irá começar. Não precisa ser o primeiro elemento da lista
// Assume-se que o nome da pessoa removida está presente na lista ligada
void remove_elem(element *elem_inicial, char *nome_pessoa_removida){
    if(contains_name(elem_inicial, nome_pessoa_removida) == 0){
        fprintf(output, "[ERROR] REMOVE %s\n", nome_pessoa_removida);
        return;
    }
    element* temp = elem_inicial;
    element* predecessor;
    do
    {
        predecessor = temp;
        temp = temp->P;
    } while (strcmp(temp->nome, nome_pessoa_removida) != 0);
    // Armazenamos o endereço do sucessor de temp
    element *sucessor = temp->P;
    // Desalocamos/removemos o elemento de predecessor e apontamos para um novo, sucessor.
    predecessor->P = sucessor;
    fprintf(output, "[ OK  ] REMOVE %s\n", nome_pessoa_removida);
}

void add_elem(element *elem_atual, char *nome_novo){
    // caso o elemento atual não estiver inicializado corretamente, criamos uma nova lista
    if(!(elem_atual->P)) {
        strcpy(elem_atual->nome, nome_novo);
        elem_atual->P = elem_atual;
        //fprintf(output, "[ OK  ] ADD %s\n", nome_novo);
        fprintf(output, "[ OK  ] ADD %s\n", nome_novo);
        return;
        } 
    element *nova_pessoa = malloc(sizeof(element));
    // Primeiro colocamos o nome desse elemento como o nome passado como argumento
    // Depois, essa nova pessoa (p2) irá apontar (dar as mãos) para a pessoa (p3) a qual o elem/pessoa atual (p1) estava apontando
    // p1 -> p3    queremos adicionar p2 após p1:
    // p2 -> p3
    // p1 -> p2 
    // p1 -> p2 -> p3     feito  
    strcpy(nova_pessoa->nome, nome_novo);
    nova_pessoa->P = elem_atual->P;
    elem_atual->P = nova_pessoa;
    fprintf(output, "[ OK  ] ADD %s\n", nova_pessoa->nome);
    return;
}

void show_elem(element* elem, char* name_to_show){
    element* temp = elem;
    // se for o único elemento da lista
    if(temp->P == temp) {goto final;}
    // se não estiver na lista
    else if( contains_name(elem, name_to_show) == 0){
        fprintf(output, "[ERROR] ?<-%s->?\n", name_to_show);
        return;
    }
    // Caso esteja na lista e possua mais de um elem, itere até achar
    while (strcmp(temp->P->nome, name_to_show) != 0){
        temp = temp->P;
    }
    final:
        fprintf(output, "[ OK  ] %s<-%s->%s\n", temp->nome, temp->P->nome, temp->P->P->nome);
        return;
}


int main(int argc, char **argv){
    input = fopen(argv[1],"r");
    output = fopen(argv[2], "w");
    if (!input){
        fprintf(stdout, "NAO ABRIU\n");
        exit(EXIT_FAILURE);
    } 
    // sp é separador. command é o comando e name é o nome do indivídu
    element* current = malloc(sizeof(element));
    char* linha_atual = 0;
    size_t len = 0;
    while(!feof(input)){
        getline(&linha_atual, &len, input);
        if(feof(input)){
            break;
        }
        linha_atual[strlen(linha_atual) - 1] = '\0';
        // formatação da linha de input
        char* sp = strchr(linha_atual, ' ');
        if (!sp) {
            exit(EXIT_FAILURE);}
        char* command = strndup(linha_atual, sp-linha_atual);
        char* name = sp+1;
        // comandos
        if (strcmp(command, "ADD") == 0) {
            if (contains_name(current, name) == 1){
                fprintf(output, "[ERROR] ADD %s\n", name);
            }
            else {
            add_elem(current, name);
            current = current->P;
            }
        }
        else if (strcmp(command, "REMOVE") == 0)
        {
            //current = current->P;
            remove_elem(current, name);
            //current = last(current);
        }
        else {
            show_elem(current, name);
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}