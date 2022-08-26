#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct livro{
    char isbn[20], titulo[101], autor[51];
} livro;

typedef struct dupla{
    int32_t indice, chamadas;
} dupla;

// Função de cortar e alocar parte de uma string
void slice(const char *str, char *result, size_t start, size_t end)
{
    strncpy(result, str + start, end - start);
}

// Função de busca binária iterativa
dupla binaria ( livro* V , uint32_t n, char* isbn_procurado ) {
    fflush(stdout);
    int32_t i = 0 , j = n - 1;
    uint32_t chamadas = 1;
    int32_t p = ( i + j ) / 2;
    while ( j >= i && (strcmp(V[p].isbn, isbn_procurado) != 0)) {
        // se o V[p].isbn for MAIOR que o isbn q eu procuro, vamos à esquerda 
        if( strcmp(V[p].isbn, isbn_procurado) > 0)
            j = p - 1;
        else
            i = p + 1;
        p = ( i + j ) / 2;
        chamadas++;
    }
    
    int32_t indice = ( strcmp(V[p].isbn, isbn_procurado) == 0 ) ? ( p ) : ( -1);
    dupla resultado = {indice, chamadas};

    return resultado;
}


uint32_t heuristica(livro* V, uint32_t i, uint32_t j){
    char* temp_pointer = malloc(20);
    uint64_t isbn_i = strtol(V[i].isbn, &temp_pointer, 10);
    uint64_t isbn_j = strtol(V[j].isbn, &temp_pointer, 10);
    return i + (isbn_j - isbn_i) % (j - i + 1);
}


dupla interpolada(livro* V, uint32_t n, char* isbn_procurado){
    int i = 0, j = n - 1;
    int p = heuristica(V, i, j);
    dupla resultado;
    resultado.chamadas = 1;
    while ( j >= i && (strcmp(V[p].isbn, isbn_procurado) != 0) && p != 0) {
        // se o V[p].isbn for MAIOR que o isbn q eu procuro, vamos à esquerda 
        if( strcmp(V[p].isbn, isbn_procurado) > 0)
            j = p - 1;
        else
            i = p + 1;
        if( j >= i){p = heuristica(V, i, j);}
        else{
            break;
        }
        resultado.chamadas++;
    }

    resultado.indice = ( strcmp(V[p].isbn, isbn_procurado) == 0 ) ? ( p ) : ( -1);
    return resultado;
}

FILE* input = NULL, *output = NULL;

/*
int main(void){
    livro* arr_livros = malloc(5 * sizeof(livro));
    strcpy(arr_livros[0].autor, "Niklaus Wirt");
    strcpy(arr_livros[0].isbn, "9780130224187");
    strcpy(arr_livros[0].titulo,"Algorithms + Data Structures = Programs");
    strcpy(arr_livros[1].autor, "Gaston Gonnet");
    strcpy(arr_livros[1].isbn, "9780201416077");
    strcpy(arr_livros[1].titulo,"Handbook of Algorithms and Data Structures");
    strcpy(arr_livros[2].isbn, "9780262033848");
    strcpy(arr_livros[2].autor, "Thomas Cormen");
    strcpy(arr_livros[2].titulo, "Introduction to Algorithms");
    strcpy(arr_livros[3].autor, "Donald Knuth");
    strcpy(arr_livros[3].isbn, "9780321751041");
    strcpy(arr_livros[3].titulo, "The Art of Computer Programming");
    strcpy(arr_livros[4].autor, "Dinesh Mehta");
    strcpy(arr_livros[4].titulo, "Handbook of Data Structures and Applications");
    strcpy(arr_livros[4].isbn, "9781584884354");

    dupla resultado_bin = binaria(arr_livros, 5, 0, "9780130224187");
    dupla resultado_int = interpolada(arr_livros, 5, "9780130224187");
    printf("%d %d\n", resultado_bin.chamadas, resultado_bin.indice);
    printf("%d %d\n", resultado_int.chamadas, resultado_int.indice);
}
*/

// retorna 0 se a binária tiver vencido, 1 se a interpolada
int quem_ganhou(int chamadas_binaria, int chamadas_interpolada){
    int result = (chamadas_interpolada <= chamadas_binaria) ? 1 : 0;
    return result;
}


int main(int argc, char** argv){
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if(!input){
        fprintf(stdout, "O arquivo de entrada tem algum problema\n");
        exit(EXIT_FAILURE);
    }

    uint32_t tamanho_vetor, quant_livros_a_procurar, i = 0;
    fscanf(input, "%d\n", &tamanho_vetor);
    livro* arr_livros = malloc(sizeof(livro) * tamanho_vetor);
    
    // preenchimento da array de livros
    
    char* livro_atual = 0;
    size_t buf = 0;
    while (i < tamanho_vetor)
    {
        getline(&livro_atual, &buf, input);
        // indice em que começa o titulo do livro
        int indice_titulo = strcspn(livro_atual, "&");
        memset(arr_livros[i].autor, '\0', sizeof(arr_livros[i].autor));
        memset(arr_livros[i].isbn, '\0', sizeof(arr_livros[i].isbn));
        memset(arr_livros[i].titulo, '\0', sizeof(arr_livros[i].titulo));

        // os 13 primeiros caracteres do livro são o isbn
        slice(livro_atual, arr_livros[i].isbn, 0, 13);
        // do 14º até o & temos o nome do autor
        slice(livro_atual, arr_livros[i].autor, 14, indice_titulo);
        // e finalmente, a partir do & até o \n final temos o titulo do livro
        slice(livro_atual, arr_livros[i].titulo, indice_titulo + 1, strlen(livro_atual) - 1);
        i++;
    }

    fscanf(input, "%d\n", &quant_livros_a_procurar);
    i = 0;
    int vitorias_binaria = 0, vitorias_interpolada = 0;
    int chamadas_binaria_total = 0, chamadas_interpolada_total = 0;

    char * isbn_atual = NULL;
    size_t len = 0;
    while(i < quant_livros_a_procurar){
        getline(&isbn_atual, &len, input);
        isbn_atual[strlen(isbn_atual) - 1] = '\0';
        dupla resultado_binaria = binaria(arr_livros, tamanho_vetor, isbn_atual);
        dupla resultado_interpolada = interpolada(arr_livros, tamanho_vetor, isbn_atual);


        chamadas_binaria_total += resultado_binaria.chamadas;
        chamadas_interpolada_total += resultado_interpolada.chamadas;

        fprintf(output, "[%s]B=%d,I=%d:", isbn_atual, resultado_binaria.chamadas, resultado_interpolada.chamadas);
        if(resultado_binaria.indice == -1){
            fprintf(output, "ISBN_NOT_FOUND\n");
        }

        else {fprintf(output, "Author:%s,Title:%s\n", arr_livros[resultado_binaria.indice].autor, arr_livros[resultado_binaria.indice].titulo);}
        // se o resultado da função for 1, a interpolada ganhou esse round
        if(quem_ganhou(resultado_binaria.chamadas, resultado_interpolada.chamadas)) {vitorias_interpolada++;}
        else {vitorias_binaria++;}
        i++;
        }


    fclose(input);

    fprintf(output, "BINARY=%d:%d\n", vitorias_binaria, chamadas_binaria_total/quant_livros_a_procurar);
    fprintf(output, "INTERPOLATION=%d:%d\n", vitorias_interpolada, chamadas_interpolada_total/quant_livros_a_procurar);
    fclose(output);

    return 0;
}