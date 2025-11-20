#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

// ============================================================================
// MÓDULO: utils — Implementação das funções utilitárias
// ============================================================================
// Funções de I/O seguro e manipulação de strings
// Identificadores em inglês, snake_case; comentários em português
// ============================================================================

// limpa o buffer de entrada do teclado (stdin)
// descarta todos os caracteres até encontrar '\n' ou EOF
// evita problemas com entradas maiores que o esperado ou caracteres indesejados
void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // descarta caractere por caractere até limpar o buffer
    }
}

// lê um inteiro de forma segura da entrada padrão
// valida que a entrada contém apenas um número (sem lixo após)
// retorna o inteiro lido ou -1 em caso de erro
int read_int_safe(void) {
    int value;
    char extra;
    char buffer[MAX_INPUT_BUFFER_SIZE];

    // lê uma linha completa do usuário
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Erro na leitura. Tente novamente.\n");
        return -1;
    }

    // valida que só há um número na linha (sem caracteres extras)
    // sscanf retorna 1 se conseguiu ler apenas o inteiro
    if (sscanf(buffer, "%d %c", &value, &extra) != 1) {
        printf("Entrada invalida. Digite apenas numeros.\n");
        return -1;
    }

    return value;
}

// lê um número decimal (float) de forma segura
// aceita tanto vírgula quanto ponto como separador decimal
// substitui vírgula por ponto para compatibilidade com atof()
// retorna o float lido ou -1.0f em caso de erro
float read_float_safe(void) {
    char buffer[MAX_INPUT_BUFFER_SIZE];

    // lê uma linha completa do usuário
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Erro na leitura. Tente novamente.\n");
        return -1.0f;
    }

    // remove o '\n' final se presente
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }

    // valida que contém apenas dígitos, vírgula, ponto e espaços
    int has_separator = 0;
    int has_digit = 0;

    for (size_t i = 0; i < len; i++) {
        if (isdigit((unsigned char)buffer[i])) {
            has_digit = 1;
        } else if (buffer[i] == ',' || buffer[i] == '.') {
            if (has_separator) {
                // mais de um separador decimal
                printf("Entrada invalida. Use apenas um separador decimal (virgula ou ponto).\n");
                return -1.0f;
            }
            has_separator = 1;
            // substitui vírgula por ponto para atof()
            buffer[i] = '.';
        } else if (!isspace((unsigned char)buffer[i])) {
            // caractere inválido
            printf("Entrada invalida. Digite apenas numeros e virgula/ponto.\n");
            return -1.0f;
        }
    }

    if (!has_digit) {
        printf("Entrada invalida. Digite um numero.\n");
        return -1.0f;
    }

    // converte string para float
    float value = (float)atof(buffer);

    if (value < 0.0f) {
        printf("Entrada invalida. Digite um valor positivo.\n");
        return -1.0f;
    }

    return value;
}

// lê uma string da entrada padrão sem causar buffer overflow
// remove o '\n' final se presente
// garante que a string será sempre terminada com '\0'ndle
void read_str_safe(char *str, int max_length) {
    if (fgets(str, max_length, stdin) == NULL) {
        printf("Erro na leitura. Tente novamente.\n");
        str[0] = '\0';
        return;
    }

    // remove o caractere '\n' que fgets() adiciona, se presente
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// converte todos os caracteres de uma string para maiúsculas (in-place)
// útil para comparações case-insensitive
void str_to_upper(char *str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        // cast para unsigned char previne comportamento indefinido com ASCII estendido
        str[i] = (char) toupper((unsigned char) str[i]);
    }
}

// pausa a execução aguardando o usuário pressionar ENTER
// usado para manter telas de menu e mensagens visíveis
void pause_screen(void) {
    printf("\nPressione ENTER para continuar...");
    clear_input_buffer();
}
