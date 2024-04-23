/*
    Professora: Jacqueline Midlej do Espírito Santo
    Aluno: Bruno Santos Costa
*/

// Importa as bibliotecas que serão usadas no programa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enumeração dos estados do automato com alguns enums adicionais para auxiliar nos retornos do código
enum STATE{
    Q0, Q1, Q2, Q3, Q4, TK_INT, Q14, Q5, Q6, Q7, Q10, Q9, TK_FLOAT, Q32, Q33, Q34, TK_ID, Q71, TK_IDENTIFIER, Q16, Q17, Q18, Q19, Q20, Q21, Q22, Q23, Q24, Q25, Q26, Q27, TK_DATA, Q28, Q29, Q30, TK_END, Q50, Q51, COMMENTLINE, Q12, Q13, Q15, TK_CADEIA, Q37, Q44, Q45, Q46, Q47, Q48, Q49, COMMENTMULTLINES, Q40, Q43, TK_GREATERTHAN, TK_GREATEREQUAL, TK_LESSTHAN, Q39, TK_LESSEQUAL, Q42, TK_ASSIGNMENT, Q38, TK_NOTEQUAL, Q41, Q67, TK_EQUAL, Q54, TK_OPENPAR, Q53, TK_TOWDOTS, Q55, TK_CLOSEPAR, Q70, TK_OR, Q69, TK_AND, Q36, TK_ADD, Q56, TK_SUB, Q57, TK_MULT, Q58, TK_DIV, Q59, TK_MOD, Q60, TK_NOT, INITIAL_ERROR, QUOTATION_ERROR, KEYWORD_ERROR, ID_ERROR, DATAT_ERROR
};

// Declaração das funções
int lexical_analyzer(char str[], int size, char*** TK, int** row, int** column, char*** lexem, int* count, int* qtd);
enum STATE acept_tk(enum STATE current, char character, char* word);
int haveKeyWord(char* word);
int allocMemory(char*** TK, int** row, int** column, char*** lexem, int count);
void asignValues(int j, int* k, int* count, char** TK, int row[], int column[], char** lexem, char* word, enum STATE* current, int* i, char* tk_x);
void printTable(char** TK, int* row, int* column, char** lexem, int count, int qtd[]);
void initializeWithZero(int* var, int size);
void errorMessage(int* row, int* column, char* program, enum STATE* error);
int allocMemoryError(int** r, int** c, enum STATE** s, int count);
void asignValuesError(char* word, int** errorRow, int** errorColumn, enum STATE** errorState, enum STATE* current, int* c, int* i, int j, int k, enum STATE errorT);

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

    // Variaveis para guardar informações do codigo fonte que será testado
    char** TK = NULL; // TOKEM
    int* row = NULL; // Linha
    int* column = NULL; // Coluna
    char** lexem = NULL; // Lexema
    int count = 0; // Contador
    int qtd[25]; // Vetor para guardar a quantidade de cada token no código

    // Inicializa o vetor de quantidade de tokens da linguagem
    initializeWithZero(qtd, 25);

    // Chama a função que faz a analize lexica da linguagem
    lexical_analyzer(contentfile, size, &TK, &row, &column, &lexem, &count, qtd);

    // Imprime as tabelas com os tokens usados no codigo e a quantidade de vezes que cada um foi usado
    printTable(TK, row, column, lexem, count, qtd);

    // Limpa a memória alocada para contentfile
    free(contentfile);

    // Retorno final
    return 0;
}// Fim main

// Função que vai ler cada caractere do vetor str onde tem a string do arquivo
int lexical_analyzer(char str[], int size, char*** TK, int** row, int** column, char*** lexem, int* count, int* qtd){
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

    int j = 0, k = 0; // j é uma linha e k uma coluna

    // Variaveis que vão guardar iformações sobre os possiveis erros do código
    int* errorRow = NULL; // Linhas
    int* errorColumn = NULL; // Colunas
    enum STATE* errorState = NULL; // Estado do erro
    int c = 0; // Contador para os indices dos erros

    // Laço para iterar nas posições do array
    while(i <= size){
        current = acept_tk(current, str[i], word); // Retorna o estado em que o eutomato está

        // Guardam o erro que foi cometido
        // Aqui vai ser guardado a linha, coluna e tipo de erro para no final da leitura do código retornar as menssagens de erro
        if(current == INITIAL_ERROR){
            asignValuesError(word, &errorRow, &errorColumn, &errorState, &current, &c, &i, j, k, INITIAL_ERROR);
            i++;
        }else if(current == QUOTATION_ERROR){
            asignValuesError(word, &errorRow, &errorColumn, &errorState, &current, &c, &i, j, k, QUOTATION_ERROR);
        }else if(current == ID_ERROR){
            asignValuesError(word, &errorRow, &errorColumn, &errorState, &current, &c, &i, j, k, ID_ERROR);
            k = k - 1;
        }else if(current == KEYWORD_ERROR){
            asignValuesError(word, &errorRow, &errorColumn, &errorState, &current, &c, &i, j, k, KEYWORD_ERROR);
            k = k - 1;
        }else if(current == DATAT_ERROR){
            asignValuesError(word, &errorRow, &errorColumn, &errorState, &current, &c, &i, j, k, DATAT_ERROR);
            k = k - 1;
        }else if(current == -1){
            asignValuesError(word, &errorRow, &errorColumn, &errorState, &current, &c, &i, j, k, -1);
            k = k - 1;
        }

        // Guarda as informações corretas do código para no final retornar as duas tabelas
        // com linha, coluna, token e lexema de cada palavra reconhecida pelo analizador
        if(current == TK_INT){
            allocMemory(TK, row, column, lexem, *count); // Aloca memória para os vetores onde estarão as informações
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_INT"); // Faz a atribuição dos valores
            qtd[0] += 1; // Incrementa o vetor com as quantidades de cada token
        }else if(current == TK_FLOAT){
            allocMemory(TK, row, column, lexem, *count);
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_FLOAT");
            qtd[1] += 1;
        }else if(current == TK_ID){
            allocMemory(TK, row, column, lexem, *count);
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_ID");
            qtd[2] += 1;
        }else if(current == TK_IDENTIFIER){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_IDENTIFIER");
            qtd[3] += 1;
        }else if(current == TK_DATA){
            allocMemory(TK, row, column, lexem, *count);
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_DATA");
            qtd[4] += 1;
        }else if(current == TK_END){
            allocMemory(TK, row, column, lexem, *count);
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_END");
            qtd[5] += 1;
        }else if(current == TK_CADEIA){
            allocMemory(TK, row, column, lexem, *count);
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_CADEIA");
            qtd[6] += 1;
        }else if(current == TK_GREATERTHAN){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_GREATERTHAN");
            qtd[7] += 1;
        }else if(current == TK_GREATEREQUAL){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_GREATEREQUAL");
            qtd[8] += 1;
        }else if(current == TK_LESSTHAN){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_LESSTHAN");
            qtd[9] += 1;
        }else if(current == TK_LESSEQUAL){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_LESSEQUAL");
            qtd[10] += 1;
        }else if(current == TK_ASSIGNMENT){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_ASSIGNMENT");
            qtd[11] += 1;
        }else if(current == TK_NOTEQUAL){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_NOTEQUAL");
            qtd[12] += 1;
        }else if(current == TK_EQUAL){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_EQUAL");
            qtd[13] += 1;
        }else if(current == TK_OPENPAR){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_OPENPAR");
            qtd[14] += 1;
        }else if(current == TK_CLOSEPAR){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_CLOSEPAR");
            qtd[15] += 1;
        }else if(current == TK_TOWDOTS){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_TOWDOTS");
            qtd[16] += 1;
        }else if(current == TK_OR){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_OR");
            qtd[17] += 1;
        }else if(current == TK_AND){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_AND");
            qtd[18] += 1;
        }else if(current == TK_ADD){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_ADD");
            qtd[19] += 1;
        }else if(current == TK_SUB){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_SUB");
            qtd[20] += 1;
        }else if(current == TK_MULT){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_MULT");
            qtd[21] += 1;
        }else if(current == TK_DIV){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_DIV");
            qtd[22] += 1;
        }else if(current == TK_MOD){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_MOD");
            qtd[23] += 1;
        }else if(current == TK_NOT){
            allocMemory(TK, row, column, lexem, *count);
            word[0] = '\0';
            asignValues(j, &k, count, *TK, *row, *column, *lexem, word, &current, &i, "TK_NOT");
            qtd[24] += 1;
        }else if(current == COMMENTLINE){
            current = Q0;
            i = i - 1;
            k = k - 1;
            word[0] = '\0';
        }else if(current == COMMENTMULTLINES){
            current = Q0;
            i = i - 1;
            k = k - 1;
            word[0] = '\0';
        }

        k++; // Colina em que o codigo está

        // Caso o ponteiro esteja lendo um caractere de proxima linha, ele zera a coluna e incrementa a linha
        if(str[i] == '\n'){
            k = 0; // Coluna
            j++; // Linha
        }

		i++; // Incrementa aposição do vetor
    }

    // Se a quantidade de erros no código for maior que zero as menssagens serão mostradas sinalizando os erros
    if(c > 0){
        errorMessage(errorRow, errorColumn, str, errorState);
    }

    // Limpa a memória alocada para word
    free(word);

    return 0; // Retorna quando percorre todos os caracteres do arquivo
}// Fim lexical_analyzer

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
            return DATAT_ERROR;
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
            return DATAT_ERROR;
        case Q10:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q9;
            }
            return DATAT_ERROR;
        case Q14:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q6;
            }
            return DATAT_ERROR;
        case Q9:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q9;
            }else if(!(character >= '0' && character <= '9')){
                return TK_FLOAT;
            }
            return DATAT_ERROR;
        case Q32:
            if(character >= 'A' && character <= 'Z'){
                strcat(word, c);
                return Q33;
            }else if(character >= 'a' && character <= 'z'){
                strcat(word, c);
                return Q71;
            }
            return ID_ERROR;
        case Q33:
            if(character >= 'a' && character <= 'z'){
                strcat(word, c);
                return Q34;
            }else if(!(character >= 'a' && character >= 'z')){
                return TK_ID;
            }
            return ID_ERROR;
        case Q34:
            if(character >= 'A' && character <= 'Z'){
                strcat(word, c);
                return Q33;
            }else if(!(character >= 'A' && character >= 'Z')){
                return TK_ID;
            }
            return ID_ERROR;
        case Q71:
            if((character >= 'a' && character <= 'z') || character == '_'){
                strcat(word, c);
                return Q71;
            }else if(!(character >= 'a' && character <= 'z') && character != '_'){
                if(haveKeyWord(word) == 1){
                    return TK_IDENTIFIER;
                }
                return KEYWORD_ERROR;
            }
            break;
        case Q16:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q17;
            }
            return DATAT_ERROR;
        case Q17:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q18;
            }
            return DATAT_ERROR;
        case Q18:
            if(character == '_'){
                strcat(word, c);
                return Q19;
            }
            return DATAT_ERROR;
        case Q19:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q20;
            }
            return DATAT_ERROR;
        case Q20:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q21;
            }
            return DATAT_ERROR;
        case Q21:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q22;
            }
            return DATAT_ERROR;
        case Q22:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q23;
            }
            return DATAT_ERROR;
        case Q23:
            return TK_DATA;
        case Q24:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q25;
            }
            return DATAT_ERROR;
        case Q25:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q26;
            }
            return DATAT_ERROR;
        case Q26:
            if(character == '/'){
                strcat(word, c);
                return Q27;
            }
            return DATAT_ERROR;
        case Q27:
            if(character >= '0' && character <= '9'){
                strcat(word, c);
                return Q20;
            }
            return DATAT_ERROR;
        case Q28:
            if(character == 'X'){
                strcat(word, c);
                return Q29;
            }
            return DATAT_ERROR;
        case Q29:
            if((character >= '0' && character <= '9') || (character >= 'A' && character <= 'F')){
                strcat(word, c);
                return Q30;
            }
            return DATAT_ERROR;
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

            return QUOTATION_ERROR;
        case Q13:
            if((character >= ' ' && character <= '~') && character != '"'){
                strcat(word, c);
                return Q13;
            }else if(character == '"'){
                strcat(word, c);
                return Q15;
            }
            return QUOTATION_ERROR;
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

    // Retorno do erro
    return -1;
}// Fim acept_tk

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
}// Fim haveKeyWord

// Aloca memória para as variaveis que vão guardar informações validas do código, o retorno é feito pelos ponteiros
int allocMemory(char*** TK, int** row, int** column, char*** lexem, int count){
    *TK = (char**)realloc(*TK, (count + 1) * sizeof(char*));
    *row = (int*)realloc(*row, (count + 1) * sizeof(int));
    *column = (int*)realloc(*column, (count + 1) * sizeof(int));
    *lexem = (char**)realloc(*lexem, (count + 1) * sizeof(char*));

    if(TK == NULL || row == NULL || column == NULL || lexem == NULL){
        printf("Error allocating memory!!!\n");

        return -1;
    }
}// Fim allocMemory

// Aloca memória para os dados incorretos do código, o retorno é feito pelos ponteiros
int allocMemoryError(int** r, int** c, enum STATE** s, int count){
    *r = (int*)realloc(*r, (count + 1) * sizeof(int));
    *c = (int*)realloc(*c, (count + 1) * sizeof(int));
    *s = (enum STATE*)realloc(*s, (count + 1) * sizeof(enum STATE));

    if(r == NULL || c == NULL || s == NULL){
        printf("Error allocating memory!!!\n");

        return -1;
    }
}// Fim allocMemoryError

// Atribui os valores para os dados corretos do código, o retorno das atribuições e feita pelos ponteiros
void asignValues(int j, int* k, int* count, char** TK, int* row, int* column, char** lexem, char* word, enum STATE* current, int* i, char* tk_x){
    row[*(count)] = j;
    column[*(count)] = (*k) - 1;
    TK[*(count)] = tk_x;
    lexem[*(count)] = strdup(word);
    word[0] = '\0';
    (*current) = Q0;
    (*i) = (*i) - 1;
    (*count)++;
    *(k) = (*k) - 1;
}// Fim asignValues

// Atribui palores para guardar as informações errados do código
void asignValuesError(char* word, int** errorRow, int** errorColumn, enum STATE** errorState, enum STATE* current, int* c, int* i, int j, int k, enum STATE errorT){
    word[0] = '\0'; // Limpa a variavel que guarda a palavra que teve o erro
    *current = Q0; // Retorna pqra o estado Q0 para poder ler outra palavra
    allocMemoryError(errorRow, errorColumn, errorState, *c);
    (*errorRow)[*c] = j;
    (*errorColumn)[*c] = k;
    (*errorState)[*c] = errorT;
    (*i) = (*i) - 1;
    (*c)++;
}// Fim asignValuesError

// Iprime a tabela de tokens com a linha, coluna, tipo de token e seu lexema
// Também imprime a tabela com a quantidade de vezes que cada token foi usado
void printTable(char** TK, int* row, int* column, char** lexem, int count, int qtd[]){
    int i;
    char* TOKEN[] = {"TK_INT", "TK_FLOAT", "TK_ID", "TK_IDENTIFIER", "TK_DATA", "TK_END", "TK_CADEIA", "TK_GREATERTHAN", "TK_GREATEREQUAL", "TK_LESSTHAN", "TK_LESSEQUAL", "TK_ASSIGNMENT", "TK_NOTEQUAL", "TK_EQUAL", "TK_OPENPAR", "TK_CLOSEPAR", "TK_TOWDOTS", "TK_OR", "TK_AND", "TK_ADD", "TK_SUB", "TK_MULT", "TK_DIV", "TK_MOD", "TK_NOT"};
    int size = 25;

    printf("+-----+-----+----------------+--------------------+\n");
    printf("| LIN | COL | TOKEN          | LEXEMA             |\n");
    printf("+-----+-----+----------------+--------------------+\n");

    for(i = 0; i < count; i++){
        printf("|%3d  |%3d  |%-15s |%-19s |\n", row[i] + 1, column[i] + 1, TK[i], lexem[i]);
        printf("+-----+-----+----------------+--------------------+\n");
    }

    printf("\n\n");

    printf("+----------------+------+\n");
    printf("| TOKEN          | USOS |\n");
    printf("+----------------+------+\n");

    for(i = 0; i < size; i++){
        if(qtd[i] != 0){
            printf("|%-15s | %3d  |\n", TOKEN[i], qtd[i]);
            printf("+----------------+------+\n");
        }
    }
}// Fim printTable

// Inicializa um vetor de inteiros com zeros
void initializeWithZero(int* var, int size){
    int i;

    for(i = 0; i < size; i++){
        var[i] = 0;
    }
}// Fim initializeWithZero

// Imprime as menssagens de erro do código
void errorMessage(int* row, int* column, char* program, enum STATE* error){
    int i = 0;
    int j = 0, k = 0, l = 0;

    char* newProgram = calloc(strlen(program) + 2, sizeof(char));
    strcat(newProgram, program);
    strcat(newProgram, "\n");

    printf("[%3d] ", j + 1);

    while(newProgram[i] != '\0'){
        printf("%c", newProgram[i]);

        if(newProgram[i] == '\n'){
            k = 0;
            j++;
        }

        if(newProgram[i] == '\n' && row[l] != j - 1 && strlen(newProgram) - 1 != i){
            printf("\n[%3d] ", j + 1);
        }

        if((row[l] + 1) == j){
            printf("\n");
            int count;

            printf("      ");
            for(count = 0; count < column[l]; count++){
                printf("-");
            }

            printf("^\n");

            if(error[l] == INITIAL_ERROR || error[l] == -1){
                printf("      Erro na Linha %d Coluna %d: Não reconhece TOKEN!!!\n", row[l] + 1, column[l] + 1);
            }else if(error[l] == QUOTATION_ERROR){
                printf("      Erro na Linha %d Coluna %d: Cadeia sem fechamento!!!\n", row[l] + 1, column[l] + 1);
            }else if(error[l] == ID_ERROR){
                printf("      Erro na Linha %d Coluna %d: Identificador não valido!!!\n", row[l] + 1, column[l] + 1);
            }else if(error[l] == KEYWORD_ERROR){
                printf("      Erro na Linha %d Coluna %d: Palavra Reservada não encontrada!!!\n", row[l] + 1, column[l] + 1);
            }else if(error[l] == DATAT_ERROR){
                printf("      Erro na Linha %d Coluna %d: Tipo de dado mal formatado!!!\n", row[l] + 1, column[l] + 1);
            }
            if((row[l + 1] + 1) != j){
                printf("\n[%3d] ", j + 1);
            }
            l++;
        }
        i++;
    }
    printf("\n\n");
}// Fim errorMessage