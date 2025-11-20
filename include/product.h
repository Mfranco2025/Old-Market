#ifndef PRODUCT_H
#define PRODUCT_H

#include <stddef.h>

// ============================================================================
// MÓDULO: product — Gerenciamento de produtos do sistema de mercado
// ============================================================================
// Define a estrutura de dados principal do sistema (produto) e as funções
// para cadastro, consulta, edição e remoção (CRUD). Todos os produtos são
// armazenados em memória (mini banco de dados) com código único auto-incrementado.
// Identificadores em inglês, snake_case; comentários em português.
// ============================================================================

// tamanho máximo do array de produtos no sistema (capacidade total)
#define MAX_PRODUCTS 500
// tamanho máximo do nome do produto (incluindo terminador nulo)
#define PRODUCT_NAME_MAX_LENGTH 64

// ============================================================================
// ENUMERAÇÕES
// ============================================================================

// categorias de produtos disponíveis no sistema
typedef enum {
    CATEGORY_FOOD = 1,      // alimentos em geral
    CATEGORY_DRINK,         // bebidas
    CATEGORY_HYGIENE,       // produtos de higiene pessoal
    CATEGORY_CLEANING,      // produtos de limpeza
    CATEGORY_OTHERS         // outros itens
} category_codes;

// unidades de medida para os produtos
typedef enum {
    UNIT_PIECE = 1,         // unidade (un)
    UNIT_KG,                // quilograma (kg)
    UNIT_GRAM,              // grama (g)
    UNIT_LITER,             // litro (L)
    UNIT_ML                 // mililitro (mL)
} unit_codes;

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================

// estrutura que representa um produto individual
typedef struct {
    int code;                           // código único do produto (auto-incrementado)
    char name[PRODUCT_NAME_MAX_LENGTH]; // nome do produto
    float price;                        // preço unitário de venda
    int quantity;                       // quantidade em estoque
    int minimum_stock;                  // estoque mínimo de segurança (alerta)
    int category;                       // categoria (enum category_codes)
    int unit;                           // unidade de medida (enum unit_codes)
    int active;                         // 1 = ativo, 0 = inativo (deleção lógica)
} product;

// estrutura que representa o banco de produtos em memória
typedef struct {
    product list[MAX_PRODUCTS];         // array de produtos cadastrados
    int count;                          // quantidade atual de produtos (ativos + inativos)
    int next_code;                      // próximo código a ser atribuído (auto-increment)
} product_bank;

// ============================================================================
// API PÚBLICA - INICIALIZAÇÃO
// ============================================================================

// inicializa o banco de produtos
// - zera o array de produtos
// - count = 0, next_code = 1
void initialize_product_bank(product_bank *bank);

// ============================================================================
// API PÚBLICA - CRUD (Create, Read, Update, Delete)
// ============================================================================

// cadastra um novo produto no banco
// - valida os dados de entrada
// - atribui código único automaticamente
// - retorna o código do produto cadastrado, ou -1 em caso de erro
int register_product(product_bank *bank, const char *name, float price,
                    int quantity, int minimum_stock, int category, int unit);

// busca produto pelo código
// - retorna ponteiro para o produto encontrado, ou NULL se não existir
product *find_product_by_code(product_bank *bank, int code);

// busca produto pelo nome (busca parcial, case-insensitive)
// - retorna código do primeiro produto encontrado, ou -1 se não existir
int find_product_by_name(product_bank *bank, const char *name);

// atualiza dados de um produto existente
// - permite alterar todos os campos exceto o código
// - retorna 1 se sucesso, 0 se produto não encontrado
int update_product(product_bank *bank, int code, const char *new_name,
                  float new_price, int new_quantity, int new_minimum_stock,
                  int new_category, int new_unit);

// inativa um produto (deleção lógica)
// - produto não é removido do array, apenas marcado como inativo
// - retorna 1 se sucesso, 0 se não encontrado
int deactivate_product(product_bank *bank, int code);

// ativa novamente um produto inativo
// - útil para recuperar produtos removidos por engano
// - retorna 1 se sucesso, 0 se não encontrado ou já ativo
int activate_product(product_bank *bank, int code);

// ============================================================================
// API PÚBLICA - CONSULTAS E RELATÓRIOS
// ============================================================================

// lista todos os produtos ativos
// - preenche array de ponteiros para produtos
// - retorna quantidade de produtos listados
int list_active_products(const product_bank *bank, product *out_array[], size_t max_out);

// lista produtos com estoque abaixo do mínimo
// - identifica produtos que precisam de reposição
// - retorna quantidade de produtos em situação crítica
int list_products_below_minimum(const product_bank *bank, product *out_array[], size_t max_out);

// lista produtos de uma categoria específica
// - filtra produtos ativos pela categoria
// - retorna quantidade de produtos encontrados
int list_products_by_category(const product_bank *bank, int category,
                              product *out_array[], size_t max_out);

// ============================================================================
// API PÚBLICA - UTILITÁRIOS
// ============================================================================

// converte código da categoria em string descritiva
// - retorna string estática (não precisa liberar memória)
const char *category_to_string(int category);

// converte código da unidade em string abreviada
// - retorna string estática (não precisa liberar memória)
const char *unit_to_string(int unit);

// verifica se um código de produto existe no banco
// - retorna 1 se existe, 0 caso contrário
int product_exists(const product_bank *bank, int code);

// conta quantidade de produtos ativos
// - retorna número de produtos com active = 1
int count_active_products(const product_bank *bank);

// calcula valor total do estoque
// - soma (preço × quantidade) de todos produtos ativos
// - retorna valor total em reais
float calculate_total_stock_value(const product_bank *bank);

#endif // PRODUCT_H
