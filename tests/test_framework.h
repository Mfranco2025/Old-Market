#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ============================================================================
// MÓDULO: test_framework — Framework minimalista para testes unitários
// ============================================================================
// Framework leve e autocontido para testes em C, sem dependências externas.
// Fornece macros para assertions, execução de testes e relatório.
// UI: ASCII simples (compatível com Windows/PowerShell)
// Identificadores em inglês, snake_case; comentários em português.
// ============================================================================

// ============================================================================
// VARIÁVEIS GLOBAIS DO FRAMEWORK (acessadas pelas macros)
// ============================================================================

static int test_total_count = 0;        // total de testes executados
static int test_passed_count = 0;       // testes que passaram
static int test_failed_count = 0;       // testes que falharam
static const char *test_current_name = NULL;  // nome do teste atual

// ============================================================================
// MACROS PRINCIPAIS
// ============================================================================

// inicia um novo teste com descrição em português
// deve ser chamado no início de cada função de teste
#define TEST_START(test_name) \
    do { \
        test_current_name = test_name; \
        test_total_count++; \
    } while (0)

// finaliza um teste com sucesso
#define TEST_END_SUCCESS() \
    do { \
        printf("  [PASS] %s\n", test_current_name); \
        test_passed_count++; \
    } while (0)

// registra falha em um teste
#define TEST_END_FAILURE() \
    do { \
        printf("  [FAIL] %s\n", test_current_name); \
        test_failed_count++; \
    } while (0)

// ============================================================================
// MACROS DE ASSERTION
// ============================================================================

// verifica se uma condição é verdadeira
#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            printf("    [ERROR] Condicao falsa: %s (linha %d)\n", #condition, __LINE__); \
            TEST_END_FAILURE(); \
            return 0; \
        } \
    } while (0)

// verifica se uma condição é falsa
#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            printf("    [ERROR] Condicao verdadeira quando deveria ser falsa: %s (linha %d)\n", #condition, __LINE__); \
            TEST_END_FAILURE(); \
            return 0; \
        } \
    } while (0)

// verifica se dois inteiros são iguais
#define ASSERT_INT_EQUAL(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            printf("    [ERROR] Esperado %d, obtido %d (linha %d)\n", (expected), (actual), __LINE__); \
            TEST_END_FAILURE(); \
            return 0; \
        } \
    } while (0)

// verifica se dois floats são iguais (com margem de tolerância)
#define ASSERT_FLOAT_EQUAL(actual, expected, tolerance) \
    do { \
        float diff = (actual) - (expected); \
        if (diff < 0) diff = -diff; \
        if (diff > (tolerance)) { \
            printf("    [ERROR] Esperado %.2f, obtido %.2f (linha %d)\n", (expected), (actual), __LINE__); \
            TEST_END_FAILURE(); \
            return 0; \
        } \
    } while (0)

// verifica se dois strings são iguais
#define ASSERT_STRING_EQUAL(actual, expected) \
    do { \
        if (strcmp((actual), (expected)) != 0) { \
            printf("    [ERROR] Esperado \"%s\", obtido \"%s\" (linha %d)\n", (expected), (actual), __LINE__); \
            TEST_END_FAILURE(); \
            return 0; \
        } \
    } while (0)

// verifica se um ponteiro é nulo
#define ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != NULL) { \
            printf("    [ERROR] Esperado NULL, obtido ponteiro valido (linha %d)\n", __LINE__); \
            TEST_END_FAILURE(); \
            return 0; \
        } \
    } while (0)

// verifica se um ponteiro não é nulo
#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            printf("    [ERROR] Esperado ponteiro valido, obtido NULL (linha %d)\n", __LINE__); \
            TEST_END_FAILURE(); \
            return 0; \
        } \
    } while (0)

// ============================================================================
// MACROS AUXILIARES
// ============================================================================

// declare uma função de teste (retorna 1 se sucesso, 0 se falha)
#define TEST_FUNCTION(test_name) \
    static int test_name(void)

// executa uma função de teste e registra o resultado
#define RUN_TEST(test_func) \
    do { \
        if (test_func()) { \
            TEST_END_SUCCESS(); \
        } \
    } while (0)

// imprime relatório resumido dos testes (ASCII simples)
#define TEST_REPORT() \
    do { \
        printf("\n"); \
        printf("========================================\n"); \
        printf("      RELATORIO DE TESTES UNITARIOS    \n"); \
        printf("========================================\n"); \
        printf("  Total executado:    %3d\n", test_total_count); \
        printf("  Sucessos:           %3d [PASS]\n", test_passed_count); \
        printf("  Falhas:             %3d [FAIL]\n", test_failed_count); \
        printf("  Taxa de sucesso:    %3d%%\n", (test_total_count > 0) ? ((test_passed_count * 100) / test_total_count) : 0); \
        printf("========================================\n"); \
        printf("\n"); \
    } while (0)

// reseta contadores para próxima suite de testes
#define TEST_RESET() \
    do { \
        test_total_count = 0; \
        test_passed_count = 0; \
        test_failed_count = 0; \
        test_current_name = NULL; \
    } while (0)

// retorna 1 (sucesso) ao final de um teste que passou
#define TEST_SUCCESS() \
    return 1

#endif // TEST_FRAMEWORK_H
