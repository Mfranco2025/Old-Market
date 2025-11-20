#ifndef VALIDATION_H
#define VALIDATION_H

// ============================================================================
// MÓDULO: validation — Validações de entrada para o sistema de mercado
// ============================================================================
// Centraliza todas as regras de validação de dados, prevenindo entrada de
// informações inválidas ou mal-formatadas. Garante consistência e segurança
// do sistema, impedindo operações com dados corrompidos ou fora dos limites
// aceitáveis definidos pelo domínio do negócio.
// ============================================================================

// valor mínimo permitido para preço de produto (em reais)
#define MIN_PRICE 0.01f
// valor máximo permitido para preço de produto (em reais)
#define MAX_PRICE 9999.99f
// código mínimo válido para produto (1 ou superior)
#define MIN_CODE 1
// código máximo válido para produto (6 dígitos)
#define MAX_CODE 999999
// quantidade máxima de estoque permitida
#define MAX_QUANTITY 999999

// --------------------------------------------------------------------------
// Valida formato de nome de produto
// Regras:
// - Não pode ser vazio
// - Deve começar com letra (a-z ou A-Z)
// - Pode conter letras, números, espaços e hífens após o primeiro caractere
//
// Retorna: 1 se válido, 0 caso contrário
// --------------------------------------------------------------------------
int is_valid_name_format(const char *name);

// --------------------------------------------------------------------------
// Valida preço de produto
// Regras:
// - Deve estar entre MIN_PRICE e MAX_PRICE
// - Máximo de 2 casas decimais permitidas
// - Não pode ser zero ou negativo
//
// Retorna: 1 se válido, 0 caso contrário
// --------------------------------------------------------------------------
int is_valid_price(float price);

// --------------------------------------------------------------------------
// Valida código de produto
// Regras:
// - Deve ser positivo (maior que 0)
// - Máximo de 6 dígitos (não pode exceder MAX_CODE)
//
// Retorna: 1 se válido, 0 caso contrário
// --------------------------------------------------------------------------
int is_valid_code(int code);

// --------------------------------------------------------------------------
// Valida quantidade de estoque
// Regras:
// - Deve ser não-negativo (pode ser zero)
// - Não pode exceder MAX_QUANTITY
//
// Retorna: 1 se válido, 0 caso contrário
// --------------------------------------------------------------------------
int is_valid_quantity(int quantity);

// --------------------------------------------------------------------------
// Valida estoque mínimo em relação ao estoque atual
// Regras:
// - Estoque mínimo deve ser não-negativo
// - Estoque mínimo não pode ser maior que estoque atual (seria contraditório)
//
// Retorna: 1 se válido, 0 caso contrário
// --------------------------------------------------------------------------
int is_valid_minimum_stock(int minimum_stock, int current_stock);

// --------------------------------------------------------------------------
// Valida código de categoria (baseado no enum de categorias do sistema)
// Regras:
// - Deve estar no intervalo válido de categorias (1 a 5)
//
// Retorna: 1 se válido, 0 caso contrário
// --------------------------------------------------------------------------
int is_valid_category(int category);

// --------------------------------------------------------------------------
// Valida código de unidade de medida (baseado no enum de unidades)
// Regras:
// - Deve estar no intervalo válido de unidades (1 a 5)
//
// Retorna: 1 se válido, 0 caso contrário
// --------------------------------------------------------------------------
int is_valid_unit(int unit);

#endif // VALIDATION_H
