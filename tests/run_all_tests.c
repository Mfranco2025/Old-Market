#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// RUNNER: run_all_tests.c — Executa todas as suites de testes
// ============================================================================
// Arquivo principal que coordena a execução de todos os testes unitários
// Fornece relatório consolidado ao final
// ============================================================================

// Declarações das funções main de cada suite de testes
// (cada arquivo de teste tem sua própria função main)
extern int main_test_validation(void);
extern int main_test_product(void);

// ============================================================================
// RUNNER CONSOLIDADO
// ============================================================================

int main(void) {
    int result_validation = 0;
    int result_product = 0;

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                   RUNNER DE TESTES                       ║\n");
    printf("║             Sistema de Controle de Mercado               ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n");

    // =====================================================================
    // Executar teste de validação
    // =====================================================================
    printf("\n▶ Iniciando suite: VALIDAÇÃO\n");
    printf("═══════════════════════════════════════════════════════════\n");
    result_validation = main_test_validation();

    // =====================================================================
    // Executar teste de produto
    // =====================================================================
    printf("\n▶ Iniciando suite: PRODUCT (CRUD)\n");
    printf("═══════════════════════════════════════════════════════════\n");
    result_product = main_test_product();

    // =====================================================================
    // Relatório final consolidado
    // =====================================================================
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                  RESULTADO FINAL                         ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");

    if (result_validation == 0) {
        printf("║  ✓ Validação:          PASSOU                            ║\n");
    } else {
        printf("║  ✗ Validação:          FALHOU                            ║\n");
    }

    if (result_product == 0) {
        printf("║  ✓ Product (CRUD):     PASSOU                            ║\n");
    } else {
        printf("║  ✗ Product (CRUD):     FALHOU                            ║\n");
    }

    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n");

    // Retorna 0 se todas as suites passaram, 1 caso contrário
    int overall_result = (result_validation == 0 && result_product == 0) ? 0 : 1;

    if (overall_result == 0) {
        printf("✓ TODOS OS TESTES PASSARAM COM SUCESSO!\n");
    } else {
        printf("✗ ALGUNS TESTES FALHARAM. Verifique os detalhes acima.\n");
    }
    printf("\n");

    return overall_result;
}
