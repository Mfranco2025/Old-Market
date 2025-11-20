#include "test_framework.h"
#include "product.h"
#include <string.h>

// ============================================================================
// TESTES: test_product.c — Suite de testes para o modulo product (CRUD)
// ============================================================================
// Testes completos para operacoes CRUD e gerenciamento de produtos
// ASCII SIMPLES APENAS - sem caracteres especiais
// Identificadores em ingles, snake_case; comentarios em portugues
// ============================================================================

static product_bank test_bank;

static void setup_test_bank(void) {
    initialize_product_bank(&test_bank);
}

// ============================================================================
// TESTES: initialize_product_bank
// ============================================================================

TEST_FUNCTION(test_initialize_product_bank) {
    TEST_START("Inicializacao: banco com valores zerados");

    setup_test_bank();

    ASSERT_INT_EQUAL(test_bank.count, 0);
    ASSERT_INT_EQUAL(test_bank.next_code, 1);

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: register_product (CREATE)
// ============================================================================

TEST_FUNCTION(test_register_product_successful) {
    TEST_START("Cadastro: cadastro bem-sucedido de produto");

    setup_test_bank();

    int code = register_product(&test_bank, "Arroz 5kg", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);

    ASSERT_INT_EQUAL(code, 1);
    ASSERT_INT_EQUAL(test_bank.count, 1);
    ASSERT_INT_EQUAL(test_bank.next_code, 2);

    TEST_SUCCESS();
}

TEST_FUNCTION(test_register_product_multiple) {
    TEST_START("Cadastro: multiplos produtos");

    setup_test_bank();

    int code1 = register_product(&test_bank, "Arroz", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);
    int code2 = register_product(&test_bank, "Refrigerante", 7.50f, 100, 20, CATEGORY_DRINK, UNIT_LITER);

    ASSERT_INT_EQUAL(code1, 1);
    ASSERT_INT_EQUAL(code2, 2);
    ASSERT_INT_EQUAL(test_bank.count, 2);

    TEST_SUCCESS();
}

TEST_FUNCTION(test_register_product_invalid_name) {
    TEST_START("Cadastro: rejeita nome invalido");

    setup_test_bank();

    int code = register_product(&test_bank, "1Produto", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);

    ASSERT_INT_EQUAL(code, 0);
    ASSERT_INT_EQUAL(test_bank.count, 0);

    TEST_SUCCESS();
}

TEST_FUNCTION(test_register_product_invalid_price) {
    TEST_START("Cadastro: rejeita preco invalido");

    setup_test_bank();

    int code = register_product(&test_bank, "Arroz", -5.00f, 50, 10, CATEGORY_FOOD, UNIT_KG);

    ASSERT_INT_EQUAL(code, 0);
    ASSERT_INT_EQUAL(test_bank.count, 0);

    TEST_SUCCESS();
}

TEST_FUNCTION(test_register_product_invalid_category) {
    TEST_START("Cadastro: rejeita categoria invalida");

    setup_test_bank();

    int code = register_product(&test_bank, "Arroz", 25.99f, 50, 10, 10, UNIT_KG);

    ASSERT_INT_EQUAL(code, 0);
    ASSERT_INT_EQUAL(test_bank.count, 0);

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: find_product_by_code (READ)
// ============================================================================

TEST_FUNCTION(test_find_product_by_code_found) {
    TEST_START("Busca: encontra produto existente");

    setup_test_bank();
    register_product(&test_bank, "Arroz", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);

    product *p = find_product_by_code(&test_bank, 1);

    ASSERT_NOT_NULL(p);
    ASSERT_INT_EQUAL(p->code, 1);
    ASSERT_STRING_EQUAL(p->name, "Arroz");

    TEST_SUCCESS();
}

TEST_FUNCTION(test_find_product_by_code_not_found) {
    TEST_START("Busca: retorna NULL para codigo inexistente");

    setup_test_bank();

    product *p = find_product_by_code(&test_bank, 999);

    ASSERT_NULL(p);

    TEST_SUCCESS();
}

TEST_FUNCTION(test_find_product_by_code_inactive) {
    TEST_START("Busca: ignora produtos inativos");

    setup_test_bank();
    register_product(&test_bank, "Arroz", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);
    deactivate_product(&test_bank, 1);

    product *p = find_product_by_code(&test_bank, 1);

    ASSERT_NULL(p);

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: update_product (UPDATE)
// ============================================================================

TEST_FUNCTION(test_update_product_successful) {
    TEST_START("Edicao: atualiza produto com sucesso");

    setup_test_bank();
    register_product(&test_bank, "Arroz", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);

    int result = update_product(&test_bank, 1, "Arroz Premium", 30.00f, 60, 15, CATEGORY_FOOD, UNIT_KG);

    ASSERT_INT_EQUAL(result, 1);

    product *p = find_product_by_code(&test_bank, 1);
    ASSERT_NOT_NULL(p);
    ASSERT_STRING_EQUAL(p->name, "Arroz Premium");
    ASSERT_FLOAT_EQUAL(p->price, 30.00f, 0.01f);

    TEST_SUCCESS();
}

TEST_FUNCTION(test_update_product_not_found) {
    TEST_START("Edicao: retorna erro para produto inexistente");

    setup_test_bank();

    int result = update_product(&test_bank, 999, "Novo Nome", 10.00f, 10, 5, CATEGORY_FOOD, UNIT_KG);

    ASSERT_INT_EQUAL(result, 0);

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: deactivate_product (soft DELETE)
// ============================================================================

TEST_FUNCTION(test_deactivate_product_successful) {
    TEST_START("Inativacao: marca produto como inativo");

    setup_test_bank();
    register_product(&test_bank, "Arroz", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);

    int result = deactivate_product(&test_bank, 1);

    ASSERT_INT_EQUAL(result, 1);
    ASSERT_INT_EQUAL(test_bank.list[0].active, 0);

    TEST_SUCCESS();
}

TEST_FUNCTION(test_deactivate_product_not_found) {
    TEST_START("Inativacao: retorna erro para produto inexistente");

    setup_test_bank();

    int result = deactivate_product(&test_bank, 999);

    ASSERT_INT_EQUAL(result, 0);

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: activate_product (RESTORE)
// ============================================================================

TEST_FUNCTION(test_activate_product_successful) {
    TEST_START("Ativacao: reativa produto inativo");

    setup_test_bank();
    register_product(&test_bank, "Arroz", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);
    deactivate_product(&test_bank, 1);

    int result = activate_product(&test_bank, 1);

    ASSERT_INT_EQUAL(result, 1);
    ASSERT_INT_EQUAL(test_bank.list[0].active, 1);

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: list_active_products
// ============================================================================

TEST_FUNCTION(test_list_active_products) {
    TEST_START("Listagem: retorna apenas produtos ativos");

    setup_test_bank();
    register_product(&test_bank, "Arroz", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);
    register_product(&test_bank, "Feijao", 15.50f, 30, 5, CATEGORY_FOOD, UNIT_KG);
    deactivate_product(&test_bank, 1);

    product *out_array[10];
    int count = list_active_products(&test_bank, out_array, 10);

    ASSERT_INT_EQUAL(count, 1);
    ASSERT_INT_EQUAL(out_array[0]->code, 2);

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: list_products_below_minimum
// ============================================================================

TEST_FUNCTION(test_list_products_below_minimum) {
    TEST_START("Relatorio: lista produtos abaixo do minimo");

    setup_test_bank();
    register_product(&test_bank, "Arroz", 25.99f, 50, 10, CATEGORY_FOOD, UNIT_KG);
    register_product(&test_bank, "Feijao", 15.50f, 5, 10, CATEGORY_FOOD, UNIT_KG);

    product *out_array[10];
    int count = list_products_below_minimum(&test_bank, out_array, 10);

    ASSERT_INT_EQUAL(count, 1);
    ASSERT_INT_EQUAL(out_array[0]->code, 2);

    TEST_SUCCESS();
}

// ============================================================================
// TESTES: category_to_string e unit_to_string
// ============================================================================

TEST_FUNCTION(test_category_to_string) {
    TEST_START("Conversao: categoria para string");

    ASSERT_STRING_EQUAL(category_to_string(CATEGORY_FOOD), "Alimentos");
    ASSERT_STRING_EQUAL(category_to_string(CATEGORY_DRINK), "Bebidas");
    ASSERT_STRING_EQUAL(category_to_string(CATEGORY_HYGIENE), "Higiene");
    ASSERT_STRING_EQUAL(category_to_string(CATEGORY_CLEANING), "Limpeza");
    ASSERT_STRING_EQUAL(category_to_string(CATEGORY_OTHERS), "Outros");

    TEST_SUCCESS();
}

TEST_FUNCTION(test_unit_to_string) {
    TEST_START("Conversao: unidade para string");

    ASSERT_STRING_EQUAL(unit_to_string(UNIT_PIECE), "Unidade");
    ASSERT_STRING_EQUAL(unit_to_string(UNIT_KG), "Kg");
    ASSERT_STRING_EQUAL(unit_to_string(UNIT_GRAM), "Grama");
    ASSERT_STRING_EQUAL(unit_to_string(UNIT_LITER), "Litro");
    ASSERT_STRING_EQUAL(unit_to_string(UNIT_ML), "Mililitro");

    TEST_SUCCESS();
}

// ============================================================================
// RUNNER: Executa todos os testes
// ============================================================================

int main(void) {
    printf("\n");
    printf("========================================\n");
    printf("  SUITE DE TESTES: PRODUCT (CRUD)\n");
    printf("========================================\n");

    printf("\n[GRUPO] Inicializacao:\n");
    RUN_TEST(test_initialize_product_bank);

    printf("\n[GRUPO] Cadastro (CREATE):\n");
    RUN_TEST(test_register_product_successful);
    RUN_TEST(test_register_product_multiple);
    RUN_TEST(test_register_product_invalid_name);
    RUN_TEST(test_register_product_invalid_price);
    RUN_TEST(test_register_product_invalid_category);

    printf("\n[GRUPO] Busca (READ):\n");
    RUN_TEST(test_find_product_by_code_found);
    RUN_TEST(test_find_product_by_code_not_found);
    RUN_TEST(test_find_product_by_code_inactive);

    printf("\n[GRUPO] Edicao (UPDATE):\n");
    RUN_TEST(test_update_product_successful);
    RUN_TEST(test_update_product_not_found);

    printf("\n[GRUPO] Inativacao (DELETE):\n");
    RUN_TEST(test_deactivate_product_successful);
    RUN_TEST(test_deactivate_product_not_found);

    printf("\n[GRUPO] Ativacao (RESTORE):\n");
    RUN_TEST(test_activate_product_successful);

    printf("\n[GRUPO] Listagem:\n");
    RUN_TEST(test_list_active_products);
    RUN_TEST(test_list_products_below_minimum);

    printf("\n[GRUPO] Conversao de Tipos:\n");
    RUN_TEST(test_category_to_string);
    RUN_TEST(test_unit_to_string);

    TEST_REPORT();

    return (test_failed_count == 0) ? 0 : 1;
}
