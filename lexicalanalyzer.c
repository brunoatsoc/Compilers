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
    Q0, Q1, Q2, Q3, Q4, TK_INT, Q14, Q5, Q6, Q7, Q10, Q9, TK_FLOAT, Q32, Q33, Q34, TK_ID, Q71, TK_IDENTIFIER, Q16, Q17, Q18, Q19, Q20, Q21, Q22, Q23, Q24, Q25, Q26, Q27, TK_DATA, Q28, Q29, Q30, TK_END, Q50, Q51, COMMENTLINE, Q12, Q13, Q15, TK_CADEIA, Q37, Q44, Q45, Q46, Q47, Q48, Q49, COMMENTMULTLINES, Q40, Q43, TK_GREATERTHAN, TK_GREATEREQUAL, TK_LESSTHAN, Q39, TK_LESSEQUAL, Q42, TK_ASSIGNMENT, Q38, TK_NOTEQUAL, Q41, Q67, TK_EQUAL, Q54, TK_OPENPAR, Q53, TK_TOWDOTS, Q55, TK_CLOSEPAR, Q70, TK_OR, Q69, TK_AND, Q36, TK_ADD, Q56, TK_SUB, Q57, TK_MULT, Q58, TK_DIV, Q59, TK_MOD, Q60, TK_NOT, INITIAL_ERROR
};

// Declaração das funções
int lexical_analyzer(char str[], int size);
enum STATE acept_tk(enum STATE current, char character, char* word);
int haveKeyWord(char* word);

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

        if(current == INITIAL_ERROR){
            printf("%s -> Rejected!!!\n", word); // Imprime a palavra que teve um erro
			word[0] = '\0'; // Limpa a variavel que guarda a palavra que teve o erro
			current = Q0; // Retorna pqra o estado Q0 para poder ler outra palavra
            //i = i - 1;
        }

        // Retorna uma menssagem de erro se cair em um estado que o eutomato não reconhece
        if(current == -1){
            printf("%s -> Rejected!!!\n", word); // Imprime a palavra que teve um erro
			word[0] = '\0'; // Limpa a variavel que guarda a palavra que teve o erro
			current = Q0; // Retorna pqra o estado Q0 para poder ler outra palavra
            i = i - 1;
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
        }else if(current == TK_ID){
            printf("%s -> TK_ID\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_IDENTIFIER){
            printf("%s -> TK_IDENTIFIER\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_DATA){
            printf("%s -> TK_DATA\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_END){
            printf("%s -> TK_END\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == COMMENTLINE){
            printf("%s -> COMMENTLINE\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_CADEIA){
            printf("%s -> TK_CADEIA\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == COMMENTMULTLINES){
            printf("%s -> COMMENTMUTLINES\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_GREATERTHAN){
            printf("%s -> TK_GREATERTHAN\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_GREATEREQUAL){
            printf("%s -> TK_GREATEREQUAL\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_LESSTHAN){
            printf("%s -> TK_LESSTHAN\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_LESSEQUAL){
            printf("%s -> TK_LESSEQUAL\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_ASSIGNMENT){
            printf("%s -> TK_ASSIGNMENT\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_NOTEQUAL){
            printf("%s -> TK_NOTEQUAL\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_EQUAL){
            printf("%s -> TK_EQUAL\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_OPENPAR){
            printf("%s -> TK_OPENPAR\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_CLOSEPAR){
            printf("%s -> TK_CLOSEPAR\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_TOWDOTS){
            printf("%s -> TK_TOWDOTS\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_OR){
            printf("%s -> TK_OR\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_AND){
            printf("%s -> TK_AND\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_ADD){
            printf("%s -> TK_ADD\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_SUB){
            printf("%s -> TK_SUB\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_MULT){
            printf("%s -> TK_MULT\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_DIV){
            printf("%s -> TK_DIV\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_MOD){
            printf("%s -> TK_MOD\n", word); // Imprime a palavra e o token que foi aceito
            word[0] = '\0'; // Limpa a variavel que guarda a palavra que foi lida
            current = Q0; // Retorna ao estado Q0 para poder ler outra palavra
			i = i - 1; // Volta uma posição para que quando a variavel i for incrementada o array fique na mesma posição que está agora
        }else if(current == TK_NOT){
            printf("%s -> TK_NOT\n", word); // Imprime a palavra e o token que foi aceito
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

    /*
        TK_INT, TK_FLOAT, TK_ID, TK_IDENTIFIER, TK_DATA, TK_END, COMMENTLINE, TK_CADEIA, COMMENTMULTLINES
    */
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
            }else if(character >= 'a' && character <= 'z'){
                strcat(word, c);
                return Q32;
            }else if(character >= 'A' && character <= 'F'){
                strcat(word, c);
                return Q28;
            }else if(character == '#'){
                strcat(word, c);
                return Q50;
            }else if(character == '"'){
                strcat(word, c);
                return Q12;
            }else if(character == '<'){
                strcat(word, c);
                return Q37;
            }else if(character == '>'){
                strcat(word, c);
                return Q40;
            }else if(character == '='){
                strcat(word, c);
                return Q41;
            }else if(character == '('){
                strcat(word, c);
                return Q54;
            }else if(character == ':'){
                strcat(word, c);
                return Q53;
            }else if(character == ')'){
                strcat(word, c);
                return Q55;
            }else if(character == '|'){
                strcat(word, c);
                return Q70;
            }else if(character == '&'){
                strcat(word, c);
                return Q69;
            }else if(character == '+'){
                strcat(word, c);
                return Q36;
            }else if(character == '-'){
                strcat(word, c);
                return Q56;
            }else if(character == '*'){
                strcat(word, c);
                return Q57;
            }else if(character == '/'){
                strcat(word, c);
                return Q58;
            }else if(character == '%'){
                strcat(word, c);
                return Q59;
            }else if(character == '~'){
                strcat(word, c);
                return Q60;
            }
            strcat(word, c);
            return INITIAL_ERROR;
        case Q1:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q2;
            }else if(character == '.'){
                strcat(word, c);
                return Q5;
            }else if(character == 'X'){
                strcat(word, c);
                return Q29;
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
            }else if(character == '_'){
                strcat(word, c);
                return Q16;
            }else if(character == '/'){
                strcat(word, c);
                return Q24;
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
        case Q32:
            if(character >= 'A' && character <= 'Z'){
                strcat(word, c);
                return Q33;
            }else if(character >= 'a' && character <= 'z'){
                strcat(word, c);
                return Q71;
            }
            break;
        case Q33:
            if(character >= 'a' && character <= 'z'){
                strcat(word, c);
                return Q34;
            }else if(!(character >= 'a' && character >= 'z')){
                return TK_ID;
            }
            break;
        case Q34:
            if(character >= 'A' && character <= 'Z'){
                strcat(word, c);
                return Q33;
            }else if(!(character >= 'A' && character >= 'Z')){
                return TK_ID;
            }
            break;
        case Q71:
            if((character >= 'a' && character <= 'z') || character == '_'){
                strcat(word, c);
                return Q71;
            }else if(!(character >= 'a' && character <= 'z') && character != '_'){
                if(haveKeyWord(word) == 1){
                    return TK_IDENTIFIER;
                }
            }
            break;
        case Q16:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q17;
            }
            break;
        case Q17:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q18;
            }
            break;
        case Q18:
            if(character == '_'){
                strcat(word, c);
                return Q19;
            }
            break;
        case Q19:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q20;
            }
            break;
        case Q20:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q21;
            }
            break;
        case Q21:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q22;
            }
            break;
        case Q22:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q23;
            }
            break;
        case Q23:
            return TK_DATA;
        case Q24:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q25;
            }
            break;
        case Q25:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q26;
            }
            break;
        case Q26:
            if(character == '/'){
                strcat(word, c);
                return Q27;
            }
            break;
        case Q27:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q20;
            }
            break;
        case Q28:
            if(character == 'X'){
                strcat(word, c);
                return Q29;
            }
            break;
        case Q29:
            if((character >= '0' && character <= '9') || (character >= 'A' && character <= 'F')){
                strcat(word, c);
                return Q30;
            }
            break;
        case Q30:
            if((character >= '0' && character <= '9') || (character >= 'A' && character <= 'F')){
                strcat(word, c);
                return Q30;
            }else{
                return TK_END;
            }
        case Q50:
            if(character != '\n'){
                strcat(word, c);
                return Q51;
            }
            return COMMENTLINE;
        case Q51:
            if(character == '\n' || character == '\0'){
                return COMMENTLINE;
            }
            strcat(word, c);
            return Q51;
        case Q12:
            if((character >= ' ' && character <= '~') && character != '"'){
                strcat(word, c);
                return Q13;
            }else if(character == '"'){
                strcat(word, c);
                return Q15;
            }
            break;
        case Q13:
            if((character >= ' ' && character <= '~') && character != '"'){
                strcat(word, c);
                return Q13;
            }else if(character == '"'){
                strcat(word, c);
                return Q15;
            }
            break;
        case Q15:
            return TK_CADEIA;
        case Q37:
            if(character == '<'){
                strcat(word, c);
                return Q44;
            }else if(character == '='){
                strcat(word, c);
                return Q39;
            }else if(character == '>'){
                strcat(word, c);
                return Q38;
            }
            return TK_LESSTHAN;
        case Q44:
            if(character == '<'){
                strcat(word, c);
                return Q45;
            }
            break;
        case Q45:
            if(character == '>'){
                strcat(word, c);
                return Q47;
            }else if(character != '<' && character != '\0'){
                strcat(word, c);
                return Q46;
            }
            break;
        case Q46:
            if(character == '>'){
                strcat(word, c);
                return Q47;
            }else if(character != '<' && character != '\0'){
                strcat(word, c);
                return Q46;
            }
            break;
        case Q47:
            if(character == '>'){
                strcat(word, c);
                return Q48;
            }
            break;
        case Q48:
            if(character == '>'){
                strcat(word, c);
                return Q49;
            }
            break;
        case Q49:
            return COMMENTMULTLINES;
        case Q40:
            if(character == '='){
                strcat(word, c);
                return Q43;
            }
            return TK_GREATERTHAN;
        case Q43:
            return TK_GREATEREQUAL;
        case Q39:
            if(character == '='){
                strcat(word, c);
                return Q42;
            }
            return TK_LESSEQUAL;
        case Q42:
            return TK_ASSIGNMENT;
        case Q38:
            return TK_NOTEQUAL;
        case Q41:
            if(character == '='){
                strcat(word, c);
                return Q67;
            }
            break;
        case Q67:
            return TK_EQUAL;
        case Q54:
            return TK_OPENPAR;
        case Q53:
            return TK_TOWDOTS;
        case Q55:
            return TK_CLOSEPAR;
        case Q70:
            return TK_OR;
        case Q69:
            return TK_AND;
        case Q36:
            return TK_ADD;
        case Q56:
            return TK_SUB;
        case Q57:
            return TK_MULT;
        case Q58:
            return TK_DIV;
        case Q59:
            return TK_MOD;
        case Q60:
            return TK_NOT;
        default:
            // Caso ele receba algum estado que não existe nessa versão do eutomato também será retornado um erro
            return -1;
    }

    //strcat(word, c);
    // Retorno do erro
    return -1;
}

// Função para verificar se o TK_IDENTIFIER que foi reconhecido faz parte das palavras reservadas da linguagem
int haveKeyWord(char* word){
    char* keyWords[] = {"rotina", "fim_rotina", "se", "senao", "imprima", "leia", "para", "enquanto"}; // Lista de palavras reservadas
    int size = sizeof(keyWords) / sizeof(keyWords[0]); // Tamanho da lista
    int i = 0;

    // Laço que verifica se alguma das palavras são iguais a que está no parametro da função
    for(i = 0; i < size; i++){
        if(strcmp(word, keyWords[i]) == 0){
            return 1; // Retorna 1 se a palavras faz parte da linguagem
        }
    }
    return 0; // Retorna zero se a palavra não faz parte da linguagem
}