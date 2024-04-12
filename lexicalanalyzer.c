/*
    Professora: Jacqueline Midlej do Espírito Santo
    Aluno: Bruno Santos Costa
*/

// Importa as bibliotecas que serão usadas no programa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enumeração dos estados do automato
enum STATE{
    Q0, Q1, Q2, Q3, Q4, TK_INT, Q14, Q5, Q6, Q7, Q10, Q9, TK_FLOAT
};

// Declaração das funções
int lexical_analyzer(char str[], int size);
enum STATE acept_tk(enum STATE current, char character, char* word);

// Função principal
int main(void){
    // Abre o arquivo onde está o codigo de teste, o nome do arquivo de teste é hello.cic
    FILE* file = fopen("hello.cic", "r");

    //Se o arquivo não existir retorna um erro
    if(file == NULL){
        printf("Error opening file!!!\n");
        return 1;
    }

    // Calcula o tamanho do arquivo
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Vetor onde o conteudo do arquivo será guardado
    char* contentfile = (char*)malloc(size + 1);

    // Se não puder alocar memória retorna um erro
    if(contentfile == NULL){
        printf("Error to allocate memory!!!\n");
        return 1;
    }

    // Faz  leitura do arquivo e guarda no array contentfile
    fread(contentfile, 1, size, file);
    contentfile[size] = '\0';

    // Fecha o arquivo
    fclose(file);

    // Chama a funão que fara a leitura de cada carctere do array
    lexical_analyzer(contentfile, size);

    // Limpa a memória alocada para contentfile
    free(contentfile);

    // Retorno final
    return 0;
}// Fim main

// Função que vai ler cada caractere do vetor str onde tem a string do arquivo
int lexical_analyzer(char str[], int size){
    // Caso a variavel que guarda o tamanho do arquivo seja zero
    //O progrma retorna uma menssagem de erro e retorna para a função principal
    if(size == 0){
        return 0;
    }

    // O estado da simulação do automato sempre começa em Q0 que é o estado inicial
    enum STATE current = Q0; //Sempre vai começar no estado Q0
    int i = 0; // Contador de posições do vetor
    char* word = (char*)malloc((size + 1) * sizeof(char)); // Cria uma variavel onde será guardado a palavra que acabou de ser lida
	word[0] = '\0'; // String "limpa"

    // Laço para iterar nas posições do array
    while(i <= size){
        current = acept_tk(current, str[i], word); // Retorna o estado em que o eutomato está

        // Retorna uma menssagem de erro se cair em um estado que o eutomato não reconhece
        if(current == -1){
            printf("%s -> Rejected!!!\n", word); // Imprime a palavra que teve um erro
			word[0] = '\0'; // Limpa a variavel que guarda a palavra que teve o erro
			current = Q0; // Retorna pqra o estado Q0 para poder ler outra palavra
        }

        // Imprime o token que foi reconhecido ou retorna o erro
        if(current == TK_INT){
            printf("%s -> TK_INT\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna pqra o estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_FLOAT){
            printf("%s -> TK_FLOAT\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }

		i++; // Incrementa aposição do vetor
    }

    // Limpa a memória alocada para word
    free(word);

    return 0; // Retorna quando percorre todos os caracteres do arquivo
}

// Função que simula o funcionamento do automato
// A função retorna o estado atual do automato
enum STATE acept_tk(enum STATE current, char character, char* word){
    // Entra no estado atual e faz a ação que o estado em que ele está pode fazer
    // O switch case faz os desvios com base no automato real
    // Caso ele entre em algum estado mas o caractere que ele recebeu não leve ele a nenhum outro estado, será retornado uma menssagem de erro
    // Por exemplo no estado Q0, caso o caractere que ele receba não seja um espaço, '\n', um caractere entre '0' e '9' ou um ponto
    // Ele retorna -1 que significa um erro, pois ele não reconheceu o caractere
    // O mesmo acontece com todos os outros estados

    // Variavel que vai formar a palavra que está sendo lida, esta variavel vai se concatenada com word
    // strcat vai concatenar c com word, no final uma palavra aceita ou recusada vai ser formada
    char c[2] = {character, '\0'};

    switch(current){
        case Q0:
            if(character == ' ' || character == '\n' || character == '\t' || character == '\0'){
                return Q0;
            }else if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q1;
            }else if(character == '.'){
                strcat(word, c);
                return Q14;
            }
            break;
        case Q1:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q2;
            }else if(character == '.'){
                strcat(word, c);
                return Q5;
            }else if(!(character >= '0' && character <= '9')){
                return TK_INT;
            }
            break;
        case Q2:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q3;
            }else if(character == '.'){
                strcat(word, c);
                return Q5;
            }else if(!(character >= '0' && character <= '9')){
                return TK_INT;
            }
            break;
        case Q3:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q4;
            }else if(character == '.'){
                strcat(word, c);
                return Q5;
            }else if(!(character >= '0' && character <= '9')){
                return TK_INT;
            }
            break;
        case Q4:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q4;
            }else if(!(character >= '0' && character <= '9')){
                return TK_INT;
            }
            break;
        case Q5:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q6;
            }else if(!(character >= '0' && character <= '9')){
                return TK_FLOAT;
            }
            break;
        case Q6:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q6;
            }else if(character == 'e'){
                strcat(word, c);
                return Q7;
            }else if(!(character >= '0' && character <= '9')){
                return TK_FLOAT;
            }
            break;
        case Q7:
            if(character == '-'){
                strcat(word, c);
                return Q10;
            }else if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q9;
            }
            break;
        case Q10:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q9;
            }
            break;
        case Q14:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q6;
            }
            break;
        case Q9:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q9;
            }else if(!(character >= '0' && character <= '9')){
                return TK_FLOAT;
            }
            break;
        default:
            // Case ele receba algum estado que não existe nessa versão do eutomato também será retornado um erro
            return -1;
    }

    strcat(word, c);

    // Retorno do erro
    return -1;
}