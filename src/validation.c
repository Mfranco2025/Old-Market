#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "validation.h"

// ============================================================================
// MÓDULO: validation — Implementação das funções de validação
// ============================================================================
// Validações de entrada de dados do sistema
// Identificadores em inglês, snake_case; comentários em português
// ============================================================================

// valida formato de nome: não pode ser vazio, deve começar com letra
// aceita letras (incluindo acentuadas), números, espaço e hífen
// CORRIGIDO: agora aceita caracteres UTF-8 (acentuação brasileira)
int is_valid_name_format(const char *name) {
    if (!name || strlen(name) < 2) {
        return 0; // nome muito curto ou nulo
    }

    // Verifica se não é apenas espaços
    int has_valid_char = 0;
    for (size_t i = 0; name[i] != '\0'; ++i) {
        if (name[i] != ' ' && name[i] != '\t') {
            has_valid_char = 1;
            break;
        }
    }

    if (!has_valid_char) {
        return 0; // nome vazio ou só espaços
    }

    // CORREÇÃO: aceita qualquer caractere que não seja controle ou símbolo problemático
    // permite acentuação (ã, á, é, í, ó, ú, ç, etc)
    for (size_t i = 0; name[i] != '\0'; ++i) {
        unsigned char c = (unsigned char)name[i];

        // Permite:
        // - Letras ASCII (A-Z, a-z)
        // - Números (0-9)
        // - Espaço, hífen, apóstrofo
        // - Caracteres acentuados (valores >= 128 em UTF-8)
        if (!(isalpha(c) || isdigit(c) || c == ' ' || c == '-' || c == '\'' || c >= 128)) {
            return 0; // caractere inválido
        }
    }

    return 1;
}

// valida preço: entre MIN_PRICE e MAX_PRICE, máximo duas casas decimais
int is_valid_price(float price) {
    if (price < MIN_PRICE || price > MAX_PRICE) {
        return 0;
    }

    // Verifica se tem no máximo 2 casas decimais
    float scaled = price * 100.0f;
    if (fabsf(scaled - roundf(scaled)) > 0.001f) {
        // Valor tem mais que duas casas decimais
        return 0;
    }

    return 1;
}

// valida código: inteiro positivo, até MAX_CODE
int is_valid_code(int code) {
    if (code < MIN_CODE || code > MAX_CODE) {
        return 0;
    }
    return 1;
}

// valida quantidade: não-negativo, até MAX_QUANTITY
int is_valid_quantity(int quantity) {
    if (quantity < 0 || quantity > MAX_QUANTITY) {
        return 0;
    }
    return 1;
}

// valida estoque mínimo (deve ser >= 0 e menor que estoque atual)
int is_valid_minimum_stock(int minimum_stock, int current_stock) {
    if (minimum_stock < 0) {
        return 0;
    }
    if (minimum_stock > current_stock) {
        return 0;
    }
    return 1;
}

// valida categoria: código entre 1 e 5
int is_valid_category(int category) {
    if (category < 1 || category > 5) {
        return 0;
    }
    return 1;
}

// valida unidade: código entre 1 e 5
int is_valid_unit(int unit) {
    if (unit < 1 || unit > 5) {
        return 0;
    }
    return 1;
}
