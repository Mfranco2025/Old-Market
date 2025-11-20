#include "test_framework.h"
#include "validation.h"
#include <math.h>

// ============================================================================
// TESTES: test_validation.c — Suite de testes para o modulo validation
// ============================================================================
// Testes completos para todas as funcoes de validacao do sistema
// ASCII SIMPLES APENAS - sem caracteres especiais
// Identificadores em ingles, snake_case; comentarios em portugues
// ============================================================================

// ============================================================================
// TESTES: is_valid_name_format
// ============================================================================

TEST_FUNCTION(test_is_valid_name_format_valid_names) {
    TEST_START("Validacao de nome: nomes validos");

    ASSERT_TRUE(is_valid_name_format("Arroz"));
    ASSERT_TRUE(is_valid_name_format("Refrigerante 2L"));
    ASSERT_TRUE(is_valid_name_format("Pao-de-forma"));
    ASSERT_TRUE(is_valid_name_format("Produto123"));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_name_format_invalid_names) {
    TEST_START("Validacao de nome: nomes invalidos");

    ASSERT_FALSE(is_valid_name_format(""));
    ASSERT_FALSE(is_valid_name_format("1Produto"));
    ASSERT_FALSE(is_valid_name_format("@Produto"));
    ASSERT_FALSE(is_valid_name_format("-Produto"));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_name_format_edge_cases) {
    TEST_START("Validacao de nome: casos de borda");

    ASSERT_FALSE(is_valid_name_format("a"));
    ASSERT_TRUE(is_valid_name_format("ab"));

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: is_valid_price
// ============================================================================

TEST_FUNCTION(test_is_valid_price_valid_prices) {
    TEST_START("Validacao de preco: precos validos");

    ASSERT_TRUE(is_valid_price(0.01f));
    ASSERT_TRUE(is_valid_price(19.99f));
    ASSERT_TRUE(is_valid_price(100.50f));
    ASSERT_TRUE(is_valid_price(9999.99f));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_price_invalid_prices) {
    TEST_START("Validacao de preco: precos invalidos");

    ASSERT_FALSE(is_valid_price(0.00f));
    ASSERT_FALSE(is_valid_price(-5.00f));
    ASSERT_FALSE(is_valid_price(10000.00f));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_price_decimal_places) {
    TEST_START("Validacao de preco: casas decimais");

    ASSERT_TRUE(is_valid_price(19.99f));
    ASSERT_TRUE(is_valid_price(19.90f));
    ASSERT_TRUE(is_valid_price(19.9f));

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: is_valid_code
// ============================================================================

TEST_FUNCTION(test_is_valid_code_valid_codes) {
    TEST_START("Validacao de codigo: codigos validos");

    ASSERT_TRUE(is_valid_code(1));
    ASSERT_TRUE(is_valid_code(123456));
    ASSERT_TRUE(is_valid_code(50000));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_code_invalid_codes) {
    TEST_START("Validacao de codigo: codigos invalidos");

    ASSERT_FALSE(is_valid_code(0));
    ASSERT_FALSE(is_valid_code(-1));
    ASSERT_FALSE(is_valid_code(1000000));

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: is_valid_quantity
// ============================================================================

TEST_FUNCTION(test_is_valid_quantity_valid_quantities) {
    TEST_START("Validacao de quantidade: quantidades validas");

    ASSERT_TRUE(is_valid_quantity(0));
    ASSERT_TRUE(is_valid_quantity(1));
    ASSERT_TRUE(is_valid_quantity(500));
    ASSERT_TRUE(is_valid_quantity(999999));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_quantity_invalid_quantities) {
    TEST_START("Validacao de quantidade: quantidades invalidas");

    ASSERT_FALSE(is_valid_quantity(-1));
    ASSERT_FALSE(is_valid_quantity(-500));
    ASSERT_FALSE(is_valid_quantity(1000000));

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: is_valid_minimum_stock
// ============================================================================

TEST_FUNCTION(test_is_valid_minimum_stock_valid) {
    TEST_START("Validacao de estoque minimo: validos");

    ASSERT_TRUE(is_valid_minimum_stock(0, 100));
    ASSERT_TRUE(is_valid_minimum_stock(10, 100));
    ASSERT_TRUE(is_valid_minimum_stock(100, 100));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_minimum_stock_invalid) {
    TEST_START("Validacao de estoque minimo: invalidos");

    ASSERT_FALSE(is_valid_minimum_stock(-1, 100));
    ASSERT_FALSE(is_valid_minimum_stock(101, 100));
    ASSERT_FALSE(is_valid_minimum_stock(200, 100));

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: is_valid_category
// ============================================================================

TEST_FUNCTION(test_is_valid_category_valid) {
    TEST_START("Validacao de categoria: categorias validas");

    ASSERT_TRUE(is_valid_category(1));
    ASSERT_TRUE(is_valid_category(2));
    ASSERT_TRUE(is_valid_category(3));
    ASSERT_TRUE(is_valid_category(4));
    ASSERT_TRUE(is_valid_category(5));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_category_invalid) {
    TEST_START("Validacao de categoria: categorias invalidas");

    ASSERT_FALSE(is_valid_category(0));
    ASSERT_FALSE(is_valid_category(6));
    ASSERT_FALSE(is_valid_category(-1));
    ASSERT_FALSE(is_valid_category(10));

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: is_valid_unit
// ============================================================================

TEST_FUNCTION(test_is_valid_unit_valid) {
    TEST_START("Validacao de unidade: unidades validas");

    ASSERT_TRUE(is_valid_unit(1));
    ASSERT_TRUE(is_valid_unit(2));
    ASSERT_TRUE(is_valid_unit(3));
    ASSERT_TRUE(is_valid_unit(4));
    ASSERT_TRUE(is_valid_unit(5));

    TEST_SUCCESS();
}

TEST_FUNCTION(test_is_valid_unit_invalid) {
    TEST_START("Validacao de unidade: unidades invalidas");

    ASSERT_FALSE(is_valid_unit(0));
    ASSERT_FALSE(is_valid_unit(6));
    ASSERT_FALSE(is_valid_unit(-1));

    TEST_SUCCESS();
}

// ============================================================================
// RUNNER: Executa todos os testes
// ============================================================================

int main(void) {
    printf("\n");
    printf("========================================\n");
    printf("  SUITE DE TESTES: VALIDACAO\n");
    printf("========================================\n");

    printf("\n[GRUPO] Validacao de Nome:\n");
    RUN_TEST(test_is_valid_name_format_valid_names);
    RUN_TEST(test_is_valid_name_format_invalid_names);
    RUN_TEST(test_is_valid_name_format_edge_cases);

    printf("\n[GRUPO] Validacao de Preco:\n");
    RUN_TEST(test_is_valid_price_valid_prices);
    RUN_TEST(test_is_valid_price_invalid_prices);
    RUN_TEST(test_is_valid_price_decimal_places);

    printf("\n[GRUPO] Validacao de Codigo:\n");
    RUN_TEST(test_is_valid_code_valid_codes);
    RUN_TEST(test_is_valid_code_invalid_codes);

    printf("\n[GRUPO] Validacao de Quantidade:\n");
    RUN_TEST(test_is_valid_quantity_valid_quantities);
    RUN_TEST(test_is_valid_quantity_invalid_quantities);

    printf("\n[GRUPO] Validacao de Estoque Minimo:\n");
    RUN_TEST(test_is_valid_minimum_stock_valid);
    RUN_TEST(test_is_valid_minimum_stock_invalid);

    printf("\n[GRUPO] Validacao de Categoria:\n");
    RUN_TEST(test_is_valid_category_valid);
    RUN_TEST(test_is_valid_category_invalid);

    printf("\n[GRUPO] Validacao de Unidade:\n");
    RUN_TEST(test_is_valid_unit_valid);
    RUN_TEST(test_is_valid_unit_invalid);

    TEST_REPORT();

    return (test_failed_count == 0) ? 0 : 1;
}
