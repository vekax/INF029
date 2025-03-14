#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 10
#define SIZE 100

#include "EstruturaVetores.h"

int** vetorPrincipal;
int espacoVetor[10] = {};
int tamVetor[10] = {};


int criarEstruturasArquivo(FILE *stream){

    char buffer[SIZE];

    while(fgets(buffer, SIZE, stream)){

        char *token = strtok(buffer, "(,) []");
        if(token == NULL) continue;
        int posicao = atoi(token);
        token = strtok(NULL, "(,) []");
        if(token == NULL) continue;
        int tamanho = atoi(token);
        criarEstruturaAuxiliar(posicao, tamanho);

        while((token = strtok(NULL, "(,) []")) != NULL) inserirNumeroEmEstrutura(posicao, atoi(token));

    }
    
    return 0;
}

int salvarEstruturasArquivo(FILE *stream){
    int n;
    for(n = 0; n  < 10; n++){
        if(vetorPrincipal[n] != NULL) break;
    }

    for( ; n < 10; n++){
        if(vetorPrincipal[n] != NULL){
            fprintf(stream, "(%d, %d) ", n + 1, tamVetor[n]);
            fprintf(stream, "[");
            for(int i = 0; i < tamVetor[n]; i++){
                fprintf(stream, "%d", vetorPrincipal[n][i]);
                if(i < tamVetor[n] - 1) fprintf(stream, ", ");
            }
            fprintf(stream, "]\n");
        }else continue;
    }

    return 0;
}

int main(){

    inicializar();

    char filename[] = "structs.in";
    FILE *stream = fopen(filename, "r");

    if (stream == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    criarEstruturasArquivo(stream);


    for(int i = 0; i < 10; i++){
        for(int j = 0; j < tamVetor[i]; j++){
            printf("%d ", vetorPrincipal[i][j]);
        }
        printf("\n");
    }

    FILE *saida = fopen("output.out", "w");
    if (saida == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    salvarEstruturasArquivo(saida);

    fclose(saida);
    fclose(stream);
    finalizar();

    return 0;
}


/*
Objetivo: criar estrutura auxiliar na posição 'posicao'.
com tamanho 'tamanho'

Rertono (int)
    SUCESSO - criado com sucesso
    JA_TEM_ESTRUTURA_AUXILIAR - já tem estrutura na posição
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
    SEM_ESPACO_DE_MEMORIA - Sem espaço de memória
    TAMANHO_INVALIDO - o tamanho deve ser maior ou igual a 1
*/
int criarEstruturaAuxiliar(int posicao, int tamanho)
{
    posicao--;
    int retorno = 0;

    if(posicao < 0 || posicao > 9) retorno = POSICAO_INVALIDA;
    else if(vetorPrincipal[posicao] != NULL) retorno = JA_TEM_ESTRUTURA_AUXILIAR;
    else if(tamanho < 1) retorno = TAMANHO_INVALIDO;
    else{

        vetorPrincipal[posicao] = (int*) malloc(tamanho * sizeof(int));
        if(vetorPrincipal[posicao] != NULL){
            tamVetor[posicao] = tamanho; 
            retorno = SUCESSO;
        }
        else retorno = SEM_ESPACO_DE_MEMORIA;

    }

    return retorno;
}

/*
Objetivo: inserir número 'valor' em estrutura auxiliar da posição 'posicao'
Rertono (int)
    SUCESSO - inserido com sucesso
    SEM_ESPACO - não tem espaço
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
CONSTANTES
*/
int inserirNumeroEmEstrutura(int posicao, int valor)
{
    posicao--;
    int retorno = 0;
    int existeEstruturaAuxiliar = 0;
    int temEspaco = 0;
    int posicao_invalida = 0;
    

    if(vetorPrincipal[posicao] != NULL) existeEstruturaAuxiliar = 1;
    
    if(espacoVetor[posicao] < tamVetor[posicao]) temEspaco = 1;

    if(posicao < 0 || posicao > 9) posicao_invalida = 1;

    if (posicao_invalida)
        retorno = POSICAO_INVALIDA;
    else
    {
        // testar se existe a estrutura auxiliar
        if (existeEstruturaAuxiliar)
        {
            if (temEspaco)
            {
                vetorPrincipal[posicao][espacoVetor[posicao]] = valor;
                espacoVetor[posicao]++;
                retorno = SUCESSO;
            }
            else
            {
                retorno = SEM_ESPACO;
            }
        }
        else
        {
            retorno = SEM_ESTRUTURA_AUXILIAR;
        }
    }

    return retorno;
}

/*
Objetivo: excluir o numero 'valor' da estrutura auxiliar no final da estrutura.
ex: suponha os valores [3, 8, 7, 9,  ,  ]. Após excluir, a estrutura deve ficar da seguinte forma [3, 8, 7,  ,  ,  ].
Obs. Esta é uma exclusão lógica

Rertono (int)
    SUCESSO - excluido com sucesso
    ESTRUTURA_AUXILIAR_VAZIA - estrutura vazia
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
*/
int excluirNumeroDoFinaldaEstrutura(int posicao)
{
    int retorno = 0;
    posicao--;
    if(posicao < 0 || posicao > 9) retorno = POSICAO_INVALIDA;
    else if(vetorPrincipal[posicao] == NULL) retorno = SEM_ESTRUTURA_AUXILIAR;
    else if(espacoVetor[posicao] == 0) retorno = ESTRUTURA_AUXILIAR_VAZIA;
    else{
        espacoVetor[posicao]--;
        retorno = SUCESSO;
    }
    return retorno;
}

/*
Objetivo: excluir o numero 'valor' da estrutura auxiliar da posição 'posicao'.
Caso seja excluido, os números posteriores devem ser movidos para as posições anteriores
ex: suponha os valores [3, 8, 7, 9,  ,  ] onde deve ser excluido o valor 8. A estrutura deve ficar da seguinte forma [3, 7, 9,  ,  ,  ]
Obs. Esta é uma exclusão lógica
Rertono (int)
    SUCESSO - excluido com sucesso 'valor' da estrutura na posição 'posicao'
    ESTRUTURA_AUXILIAR_VAZIA - estrutura vazia
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    NUMERO_INEXISTENTE - Número não existe
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar

*/
int excluirNumeroEspecificoDeEstrutura(int posicao, int valor)
{
    int retorno = 0;
    posicao--;
    if(posicao < 0 || posicao > 9) retorno = POSICAO_INVALIDA;
    else if(vetorPrincipal[posicao] == NULL) retorno = SEM_ESTRUTURA_AUXILIAR;
    else if(espacoVetor[posicao] == 0) retorno = ESTRUTURA_AUXILIAR_VAZIA;
    else{
        int aux = 0;
        for(int i = 0; i < espacoVetor[posicao]; i++){
            if(vetorPrincipal[posicao][i] == valor){
                aux++;
                for(int j = i+1; j < espacoVetor[posicao]; j++)  vetorPrincipal[posicao][j-1] = vetorPrincipal[posicao][j]; //shift
                espacoVetor[posicao]--;
            }
        }
        if(aux == 0) retorno = NUMERO_INEXISTENTE;
        else retorno = SUCESSO;
    }
    return retorno;
}

// se posição é um valor válido {entre 1 e 10}
int ehPosicaoValida(int posicao)
{
    int retorno = 0;
    if (posicao < 1 || posicao > 10)
    {
        retorno = POSICAO_INVALIDA;
    }
    else
        retorno = SUCESSO;

    return retorno;
}
/*
Objetivo: retorna os números da estrutura auxiliar da posição 'posicao (1..10)'.
os números devem ser armazenados em vetorAux

Retorno (int)
    SUCESSO - recuperado com sucesso os valores da estrutura na posição 'posicao'
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
*/
int getDadosEstruturaAuxiliar(int posicao, int vetorAux[])
{
    int retorno = 0;
    posicao--;

    if(posicao < 0 || posicao > 9) retorno = POSICAO_INVALIDA;
    else if(vetorPrincipal[posicao] == NULL) retorno = SEM_ESTRUTURA_AUXILIAR;
    else if(espacoVetor[posicao] == 0) retorno = ESTRUTURA_AUXILIAR_VAZIA;
    else{
        for(int i = 0; i < espacoVetor[posicao]; i++){
            vetorAux[i] = vetorPrincipal[posicao][i];
        }
        retorno = SUCESSO;
    }

    return retorno;
}

/*
Objetivo: retorna os números ordenados da estrutura auxiliar da posição 'posicao (1..10)'.
os números devem ser armazenados em vetorAux

Rertono (int)
    SUCESSO - recuperado com sucesso os valores da estrutura na posição 'posicao (1..10)'
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
*/
int getDadosOrdenadosEstruturaAuxiliar(int posicao, int vetorAux[])
{
    int retorno = 0;
    posicao--;

    if(posicao < 0 || posicao > 9) retorno = POSICAO_INVALIDA;
    else if(vetorPrincipal[posicao] == NULL) retorno = SEM_ESTRUTURA_AUXILIAR;
    else if(espacoVetor[posicao] == 0) retorno = ESTRUTURA_AUXILIAR_VAZIA;
    else{
        for(int i = 0; i < espacoVetor[posicao]; i++){
            vetorAux[i] = vetorPrincipal[posicao][i];
        }

        int tam = espacoVetor[posicao];
        int aux;

        for(int i = 0; i < tam; i++){
            for(int j = i+1; j < tam; j++){
                if(vetorAux[i] > vetorAux[j]){
                    aux = vetorAux[i];
                    vetorAux[i] = vetorAux[j];
                    vetorAux[j] = aux;
                }
            }
        }
        retorno = SUCESSO;
    }

    return retorno;
}

/*
Objetivo: retorna os números de todas as estruturas auxiliares.
os números devem ser armazenados em vetorAux

Rertono (int)
    SUCESSO - recuperado com sucesso os valores da estrutura na posição 'posicao'
    TODAS_ESTRUTURAS_AUXILIARES_VAZIAS - todas as estruturas auxiliares estão vazias
*/
int getDadosDeTodasEstruturasAuxiliares(int vetorAux[])
{
    int retorno = 0;
    int n;
    for(n = 0; n  < 10; n++){
        if(vetorPrincipal[n] != NULL) break;
    }

    if(n == 10){
        retorno = TODAS_ESTRUTURAS_AUXILIARES_VAZIAS;
    }else{
        int count;
        for( ; n <  10; n++){
            if(vetorPrincipal[n] != NULL){
                for(int i = 0; i < espacoVetor[n]; i++){
                    vetorAux[count] = vetorPrincipal[n][i];
                    count++;
                }
            }else{
                continue;
            }
        }
        retorno = SUCESSO;
    }

    return retorno;
}

/*
Objetivo: retorna os números ordenados de todas as estruturas auxiliares.
os números devem ser armazenados em vetorAux

Rertono (int)
    SUCESSO - recuperado com sucesso os valores da estrutura na posição 'posicao'
    TODAS_ESTRUTURAS_AUXILIARES_VAZIAS - todas as estruturas auxiliares estão vazias
*/
int getDadosOrdenadosDeTodasEstruturasAuxiliares(int vetorAux[])
{
    int retorno = 0;
    int n;
    for(n = 0; n  < 10; n++){
        if(vetorPrincipal[n] != NULL) break;
    }

    if(n == 10){
        retorno = TODAS_ESTRUTURAS_AUXILIARES_VAZIAS;
    }else{
        int count = 0;
        for( ; n <  10; n++){
            if(vetorPrincipal[n] != NULL){
                for(int i = 0; i < espacoVetor[n]; i++){
                    vetorAux[count] = vetorPrincipal[n][i];
                    count++;
                }
            }else{
                continue;
            }
        }

        int aux = 0;

        //sort

        for(int i = 0; i < count; i++){
            for(int j = i+1; j < count; j++){
                if(vetorAux[i] > vetorAux[j]){
                    aux = vetorAux[i];
                    vetorAux[i] = vetorAux[j];
                    vetorAux[j] = aux;
                }
            }
        }

        retorno = SUCESSO;
    }

    return retorno;
}

/*
Objetivo: modificar o tamanho da estrutura auxiliar da posição 'posicao' para o novo tamanho 'novoTamanho' + tamanho atual
Suponha o tamanho inicial = x, e novo tamanho = n. O tamanho resultante deve ser x + n. Sendo que x + n deve ser sempre >= 1

Rertono (int)
    SUCESSO - foi modificado corretamente o tamanho da estrutura auxiliar
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
    NOVO_TAMANHO_INVALIDO - novo tamanho não pode ser negativo
    SEM_ESPACO_DE_MEMORIA - erro na alocação do novo valor
*/
int modificarTamanhoEstruturaAuxiliar(int posicao, int novoTamanho)
{

    posicao--;
    int retorno = 0;


    if(posicao < 0 || posicao > 9) retorno = POSICAO_INVALIDA;
    else if(vetorPrincipal[posicao] == NULL) retorno = SEM_ESTRUTURA_AUXILIAR;
    else if(tamVetor[posicao] + novoTamanho < 0) retorno = NOVO_TAMANHO_INVALIDO;
    else{
        int novoTamanhoTotal = tamVetor[posicao] + novoTamanho;
        int *novoVetor = (int *) realloc(vetorPrincipal[posicao], novoTamanhoTotal * sizeof(int));

        if(novoVetor == NULL) retorno = SEM_ESPACO_DE_MEMORIA;
        else{
            if(espacoVetor[posicao] > tamVetor[posicao]) espacoVetor[posicao] = tamVetor[posicao];
            vetorPrincipal[posicao] = novoVetor;
            tamVetor[posicao] = novoTamanhoTotal;
            retorno = SUCESSO;
        }

    }

    return retorno;
}

/*
Objetivo: retorna a quantidade de elementos preenchidos da estrutura auxiliar da posição 'posicao'.

Retorno (int)
    POSICAO_INVALIDA - posição inválida
    SEM_ESTRUTURA_AUXILIAR - sem estrutura auxiliar
    ESTRUTURA_AUXILIAR_VAZIA - estrutura auxiliar vazia
    Um número int > 0 correpondente a quantidade de elementos preenchidos da estrutura
*/
int getQuantidadeElementosEstruturaAuxiliar(int posicao)
{

    posicao--;

    int retorno = 0;

    if(posicao < 0 || posicao > 9) retorno = POSICAO_INVALIDA;
    else if(vetorPrincipal[posicao] == NULL) retorno = SEM_ESTRUTURA_AUXILIAR;
    else if(espacoVetor[posicao] == 0) retorno = ESTRUTURA_AUXILIAR_VAZIA;
    else{

        retorno = espacoVetor[posicao];

    }

    return retorno;
}

/*
Objetivo: montar a lista encadeada com cabeçote com todos os números presentes em todas as estruturas.

Retorno (No*)
    NULL, caso não tenha nenhum número nas listas
    No*, ponteiro para o início da lista com cabeçote
*/
No *montarListaEncadeadaComCabecote()
{

    int retorno = 0;
    int n;
    for(n = 0; n  < 10; n++){
        if(vetorPrincipal[n] != NULL) break;
    }

    if(n == 10){
        retorno = TODAS_ESTRUTURAS_AUXILIARES_VAZIAS;
    }else{
        int count = 0;
        No* temp;
        No* head;


        for(n++ ; n <  10; n++){
            if(vetorPrincipal[n] != NULL){
                for(int i = 0; i < espacoVetor[n]; i++){
                    No* aux = (No*) malloc(sizeof(No));
                    aux->conteudo = vetorPrincipal[n][i];
                    aux->prox = NULL;
                    
                    if(head == NULL){
                        head = aux;
                        temp = aux;
                    }else{
                        temp->prox = aux;
                        temp = aux;
                    }
                }
            }else{
                continue;
            }
        }

        return head;
    }

    return NULL;
}

/*
Objetivo: retorna os números da lista enceada com cabeçote armazenando em vetorAux.
Retorno void
*/
void getDadosListaEncadeadaComCabecote(No *inicio, int vetorAux[])
{
    No* temp = inicio;
    int i = 0;
    while(temp != NULL){
        vetorAux[i] = temp->conteudo;
        i++;
        temp = temp->prox;
    }
}

/*
Objetivo: Destruir a lista encadeada com cabeçote a partir de início.
O ponteiro inicio deve ficar com NULL.

Retorno 
    void.
*/
void destruirListaEncadeadaComCabecote(No **inicio) {
    No *temp = *inicio;
    while (temp != NULL) {
        No *prox = temp->prox; // Armazenar o próximo nó antes de liberar
        free(temp);            // Liberar o nó atual
        temp = prox;           // Passar para o próximo nó
    }

    *inicio = NULL; // Atualizar o ponteiro de início para NULL
}


/*
Objetivo: inicializa o programa. deve ser chamado ao inicio do programa 

*/

void inicializar()
{

    vetorPrincipal = (int**) malloc(TAM * sizeof(int*));

    for(int i = 0; i < TAM; i++){
        vetorPrincipal[i] = NULL;
    }

}

/*
Objetivo: finaliza o programa. deve ser chamado ao final do programa 
para poder liberar todos os espaços de memória das estruturas auxiliares.

*/

void finalizar()
{
    for(int i = 0; i < 10; i++){
        free(vetorPrincipal[i]);
    }

    free(vetorPrincipal);

}