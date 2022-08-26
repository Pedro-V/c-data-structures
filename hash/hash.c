#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE* input = NULL;
FILE* output = NULL;

// FUNCOES GLOBAIS

uint8_t checksum(char* padrao){
    uint8_t val = 0;
    while(*padrao){
        if(*padrao != ' '){val = val ^ *padrao;}
        padrao++;
    }
    return val;
}

uint32_t doubleHash(char* padrao, uint32_t tamanho, uint32_t i){
    return (7919 * checksum(padrao) + i*(104729 * checksum(padrao) + 123)) % tamanho;
}

uint8_t get_quant_strs(char* str){
    char* val = malloc(5);
    int indice = 0;
    while(*str != ' '){
        val[indice] = * str;
        indice++;
        str++;
    }

    return atoi(val);
}


int main(int argc, char** argv){
    // ABERTURA DE ARQUIVOS
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if(!input){
        printf("O arquivo de entrada ou saída tem algum problema\n");
        exit(EXIT_FAILURE);
    }
    

    // DEFINICAO DO AMBIENTE E PRINCIPAIS VALORES
    uint32_t num_servs, i = 0, j = 0, lim_max_geral, operacoes;
    fscanf(input, "%d %d\n", &num_servs, &lim_max_geral);
    fscanf(input, "%d\n", &operacoes);
    //
    typedef struct servidor {
        char nome[50];
        uint8_t quant_atual;
        char** buscas;
    } servidor;

    // servidor* serv1 = malloc( sizeof(servidor) )
    // serv1->buscas = malloc( 2 * sizeof(char *))
    // serv1->buscas[0] = mallloc(100)

    
    
    // PREENCHENDO A ARRAY DE SERVIDORES
    servidor* arr_servidores = malloc(sizeof(servidor) * num_servs);
    while(i < num_servs){
        arr_servidores[i].quant_atual = 0;
        arr_servidores[i].buscas = malloc( lim_max_geral * sizeof(char*));
        char temp_serv_name[50] = "S";
        char str_num[50];
        sprintf(str_num, "%d", i);
        strcat(temp_serv_name, str_num);
        // nome do servidor será "Si"
        strcpy(arr_servidores[i].nome, temp_serv_name);

        i++;
    }

    // EXECUTANDO O HASHING PARA CADA STRING
    i = 0;
    uint32_t k = 0;

    char* operacao_corrente = 0;
    size_t len = 0;
    char* sp = 0;

    while(i < operacoes){
        getline(&operacao_corrente, &len, input);
        uint8_t quant_padroes = get_quant_strs(operacao_corrente);
        sp = strchr(operacao_corrente, ' ');
        sp++;
        sp[strlen(sp) - 1] = '\0';
        uint32_t indice;
        uint32_t h1 = doubleHash(sp, num_servs, 0);
        k = 0;
        do
        {
            indice = doubleHash(sp, num_servs, k);
            k++;
        } while (arr_servidores[indice].quant_atual == lim_max_geral);

        if (k-1)
        {
            fprintf(output, "S%d->S%d\n", h1, indice);
        }
        
        
        arr_servidores[indice].buscas[arr_servidores[indice].quant_atual] = malloc(quant_padroes * 100);
        strcpy(arr_servidores[indice].buscas[arr_servidores[indice].quant_atual], sp);
        fprintf(output, "[%s] ", arr_servidores[indice].nome);
        for(j = 0; j < arr_servidores[indice].quant_atual; j++){
            fprintf(output, "%s, ", arr_servidores[indice].buscas[j]);
        }
        fprintf(output, "%s\n", arr_servidores[indice].buscas[arr_servidores[indice].quant_atual]);
        arr_servidores[indice].quant_atual++;
        i++;
    }

    return 0;
}
