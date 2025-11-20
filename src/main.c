#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#endif

#include "product.h"
#include "persistence.h"
#include "logger.h"
#include "utils.h"
#include "validation.h"

// ============================================================================
// MAIN: Sistema de Controle de Mercado - Menu Principal
// ============================================================================
// Interface principal do usuário integrando todos os módulos
// ASCII SIMPLES - compatível com Windows PowerShell
// Identificadores em inglês, snake_case; comentários em português
// ============================================================================

// banco de produtos global
static product_bank bank;

// caminho do arquivo de dados
#define DATA_FILE_PATH "data/products.dat"

// protótipos das funções de menu
void show_main_menu(void);
void handle_register_product(void);
void handle_list_products(void);
void handle_search_product(void);
void handle_update_product(void);
void handle_delete_product(void);
void handle_products_below_minimum(void);
void handle_save_data(void);
void handle_load_data(void);

// ============================================================================
// FUNÇÃO: main
// Função principal - inicializa sistema e executa loop do menu
// ============================================================================
int main(void) {
    int option;

    // ========================================================================
    // CONFIGURAÇÃO INICIAL DO SISTEMA
    // ========================================================================

    // Configura encoding UTF-8 no console do Windows (IMPORTANTE!)
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    // Inicializa sistema de logging (agora cria diretório automaticamente)
    logger_init("logs/system.log", LOG_INFO, 1);

    // Inicializa banco de produtos vazio
    initialize_product_bank(&bank);

    log_message(LOG_INFO, "MAIN", "Sistema de controle de mercado iniciado");

    // ========================================================================
    // LOOP PRINCIPAL DO MENU
    // ========================================================================

    while (1) {
        show_main_menu();

        printf("\nEscolha uma opcao: ");
        if (scanf("%d", &option) != 1) {
            clear_input_buffer();
            printf("\nEntrada invalida! Digite um numero.\n");
            pause_screen();
            continue;
        }
        clear_input_buffer();

        switch (option) {
            case 1:
                handle_register_product();
                break;
            case 2:
                handle_list_products();
                break;
            case 3:
                handle_search_product();
                break;
            case 4:
                handle_update_product();
                break;
            case 5:
                handle_delete_product();
                break;
            case 6:
                handle_products_below_minimum();
                break;
            case 7:
                handle_save_data();
                break;
            case 8:
                handle_load_data();
                break;
            case 0:
                printf("\nEncerrando sistema...\n");
                log_message(LOG_INFO, "MAIN", "Sistema encerrado pelo usuario");
                logger_close();
                return 0;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                pause_screen();
        }
    }

    return 0;
}

// ============================================================================
// FUNÇÃO: show_main_menu
// Exibe o menu principal do sistema
// ============================================================================
void show_main_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("   SISTEMA DE CONTROLE DE MERCADO\n");
    printf("========================================\n");
    printf("  Produtos cadastrados: %d\n", count_active_products(&bank));
    printf("========================================\n");
    printf("  1 - Cadastrar Produto\n");
    printf("  2 - Listar Todos os Produtos\n");
    printf("  3 - Buscar Produto\n");
    printf("  4 - Editar Produto\n");
    printf("  5 - Excluir Produto\n");
    printf("  6 - Produtos Abaixo do Minimo\n");
    printf("  7 - Salvar Dados\n");
    printf("  8 - Recarregar Dados\n");
    printf("  0 - Sair\n");
    printf("========================================\n");
}

// ============================================================================
// FUNÇÃO: handle_register_product
// Cadastra novo produto no sistema
// Agora usa read_float_safe() para aceitar vírgula e ponto em preços
// ============================================================================
void handle_register_product(void) {
    char name[PRODUCT_NAME_MAX_LENGTH];
    float price;
    int quantity, minimum_stock, category, unit;

    printf("\n========================================\n");
    printf("      CADASTRAR NOVO PRODUTO\n");
    printf("========================================\n");

    // Leitura do nome
    printf("Nome (min 2 caracteres): ");
    read_str_safe(name, sizeof(name));

    // Leitura do preço (CORRIGIDO: usa read_float_safe())
    printf("Preco (ex: 34,55 ou 34.55): R$ ");
    price = read_float_safe();

    // Valida se a leitura foi bem-sucedida
    if (price < 0.0f) {
        printf("\nErro ao ler preco! Operacao cancelada.\n");
        pause_screen();
        return;
    }

    // Leitura da quantidade
    printf("Quantidade em estoque: ");
    quantity = read_int_safe();

    if (quantity < 0) {
        printf("\nErro ao ler quantidade! Operacao cancelada.\n");
        pause_screen();
        return;
    }

    // Leitura do estoque mínimo
    printf("Estoque minimo: ");
    minimum_stock = read_int_safe();

    if (minimum_stock < 0) {
        printf("\nErro ao ler estoque minimo! Operacao cancelada.\n");
        pause_screen();
        return;
    }

    // Seleção de categoria
    printf("\nCategorias:\n");
    printf("  1 - Alimentos\n");
    printf("  2 - Bebidas\n");
    printf("  3 - Higiene\n");
    printf("  4 - Limpeza\n");
    printf("  5 - Outros\n");
    printf("Categoria: ");
    category = read_int_safe();

    if (category < 0) {
        printf("\nErro ao ler categoria! Operacao cancelada.\n");
        pause_screen();
        return;
    }

    // Seleção de unidade
    printf("\nUnidades:\n");
    printf("  1 - Unidade\n");
    printf("  2 - Kg\n");
    printf("  3 - Grama\n");
    printf("  4 - Litro\n");
    printf("  5 - Mililitro\n");
    printf("Unidade: ");
    unit = read_int_safe();

    if (unit < 0) {
        printf("\nErro ao ler unidade! Operacao cancelada.\n");
        pause_screen();
        return;
    }

    // Tenta registrar o produto
    int code = register_product(&bank, name, price, quantity, minimum_stock, category, unit);

    if (code > 0) {
        printf("\n========================================\n");
        printf("  PRODUTO CADASTRADO COM SUCESSO!\n");
        printf("========================================\n");
        printf("  Codigo: %d\n", code);
        printf("  Nome: %s\n", name);
        printf("  Preco: R$ %.2f\n", price);
        printf("  Quantidade: %d\n", quantity);
        printf("  Categoria: %s\n", category_to_string(category));
        printf("  Unidade: %s\n", unit_to_string(unit));
        printf("========================================\n");
        log_message(LOG_INFO, "MAIN", "Produto cadastrado com sucesso");
    } else {
        printf("\n========================================\n");
        printf("  ERRO AO CADASTRAR PRODUTO!\n");
        printf("========================================\n");
        printf("Verifique se:\n");
        printf("  - Nome tem pelo menos 2 caracteres\n");
        printf("  - Preco esta entre %.2f e %.2f\n", MIN_PRICE, MAX_PRICE);
        printf("  - Quantidade esta entre 0 e %d\n", MAX_QUANTITY);
        printf("  - Estoque minimo <= Quantidade\n");
        printf("  - Categoria entre 1 e 5\n");
        printf("  - Unidade entre 1 e 5\n");
        printf("========================================\n");
    }

    pause_screen();
}

// ============================================================================
// FUNÇÃO: handle_list_products
// Lista todos os produtos ativos do sistema
// ============================================================================
void handle_list_products(void) {
    printf("\n========================================\n");
    printf("        LISTA DE PRODUTOS\n");
    printf("========================================\n");

    product *list[MAX_PRODUCTS];
    int count = list_active_products(&bank, list, MAX_PRODUCTS);

    if (count == 0) {
        printf("Nenhum produto cadastrado.\n");
        pause_screen();
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("\n[%d] Codigo: %d\n", i + 1, list[i]->code);
        printf("    Nome: %s\n", list[i]->name);
        printf("    Preco: R$ %.2f\n", list[i]->price);
        printf("    Estoque: %d %s\n", list[i]->quantity, unit_to_string(list[i]->unit));
        printf("    Minimo: %d\n", list[i]->minimum_stock);
        printf("    Categoria: %s\n", category_to_string(list[i]->category));
        printf("----------------------------------------\n");
    }

    printf("\nTotal: %d produtos cadastrados\n", count);
    pause_screen();
}

// ============================================================================
// FUNÇÃO: handle_search_product
// Busca produto por código
// ============================================================================
void handle_search_product(void) {
    printf("\n========================================\n");
    printf("         BUSCAR PRODUTO\n");
    printf("========================================\n");

    printf("Digite o codigo: ");
    int code = read_int_safe();

    if (code < 0) {
        printf("\nCodigo invalido!\n");
        pause_screen();
        return;
    }

    product *p = find_product_by_code(&bank, code);

    if (p) {
        printf("\n========================================\n");
        printf("       PRODUTO ENCONTRADO\n");
        printf("========================================\n");
        printf("  Codigo: %d\n", p->code);
        printf("  Nome: %s\n", p->name);
        printf("  Preco: R$ %.2f\n", p->price);
        printf("  Estoque: %d %s\n", p->quantity, unit_to_string(p->unit));
        printf("  Estoque minimo: %d\n", p->minimum_stock);
        printf("  Categoria: %s\n", category_to_string(p->category));
        printf("========================================\n");
    } else {
        printf("\nProduto nao encontrado!\n");
    }

    pause_screen();
}

// ============================================================================
// FUNÇÃO: handle_update_product
// Atualiza dados de um produto existente
// Agora usa read_float_safe() para aceitar vírgula e ponto em preços
// ============================================================================
void handle_update_product(void) {
    printf("\n========================================\n");
    printf("         EDITAR PRODUTO\n");
    printf("========================================\n");

    printf("Digite o codigo do produto: ");
    int code = read_int_safe();

    if (code < 0) {
        printf("\nCodigo invalido!\n");
        pause_screen();
        return;
    }

    product *p = find_product_by_code(&bank, code);

    if (!p) {
        printf("\nProduto nao encontrado!\n");
        pause_screen();
        return;
    }

    printf("\n========================================\n");
    printf("  Produto atual: %s\n", p->name);
    printf("========================================\n");
    printf("Pressione ENTER para manter o valor atual\n\n");

    // Editar nome
    char name[PRODUCT_NAME_MAX_LENGTH];
    printf("Novo nome [%s]: ", p->name);
    read_str_safe(name, sizeof(name));
    if (strlen(name) == 0) {
        strcpy(name, p->name);
    }

    // Editar preço (CORRIGIDO: usa read_float_safe())
    printf("Novo preco [%.2f]: R$ ", p->price);
    float price = read_float_safe();
    if (price < 0.0f) {
        price = p->price;  // mantém o valor atual se houver erro
    }

    // Editar quantidade
    printf("Nova quantidade [%d]: ", p->quantity);
    int quantity = read_int_safe();
    if (quantity < 0) {
        quantity = p->quantity;
    }

    // Editar estoque mínimo
    printf("Novo minimo [%d]: ", p->minimum_stock);
    int minimum = read_int_safe();
    if (minimum < 0) {
        minimum = p->minimum_stock;
    }

    // Atualizar produto
    if (update_product(&bank, code, name, price, quantity, minimum, p->category, p->unit)) {
        printf("\n========================================\n");
        printf("  PRODUTO ATUALIZADO COM SUCESSO!\n");
        printf("========================================\n");
        log_message(LOG_INFO, "MAIN", "Produto atualizado com sucesso");
    } else {
        printf("\nErro ao atualizar produto!\n");
    }

    pause_screen();
}

// ============================================================================
// FUNÇÃO: handle_delete_product
// Inativa um produto (exclusão lógica)
// ============================================================================
void handle_delete_product(void) {
    printf("\n========================================\n");
    printf("        EXCLUIR PRODUTO\n");
    printf("========================================\n");

    printf("Digite o codigo: ");
    int code = read_int_safe();

    if (code < 0) {
        printf("\nCodigo invalido!\n");
        pause_screen();
        return;
    }

    // Busca produto para confirmar
    product *p = find_product_by_code(&bank, code);
    if (!p) {
        printf("\nProduto nao encontrado!\n");
        pause_screen();
        return;
    }

    printf("\nProduto: %s (Codigo: %d)\n", p->name, p->code);
    printf("Confirma exclusao? (1=Sim, 0=Nao): ");
    int confirm = read_int_safe();

    if (confirm != 1) {
        printf("\nOperacao cancelada.\n");
        pause_screen();
        return;
    }

    if (deactivate_product(&bank, code)) {
        printf("\n========================================\n");
        printf("  PRODUTO EXCLUIDO COM SUCESSO!\n");
        printf("========================================\n");
        log_message(LOG_INFO, "MAIN", "Produto excluido com sucesso");
    } else {
        printf("\nErro ao excluir produto!\n");
    }

    pause_screen();
}

// ============================================================================
// FUNÇÃO: handle_products_below_minimum
// Lista produtos com estoque abaixo do mínimo
// ============================================================================
void handle_products_below_minimum(void) {
    printf("\n========================================\n");
    printf("  PRODUTOS ABAIXO DO ESTOQUE MINIMO\n");
    printf("========================================\n");

    product *list[MAX_PRODUCTS];
    int count = list_products_below_minimum(&bank, list, MAX_PRODUCTS);

    if (count == 0) {
        printf("\nTodos os produtos estao com estoque adequado!\n");
        printf("Nenhuma reposicao necessaria.\n");
        pause_screen();
        return;
    }

    printf("\nATENCAO! %d produto(s) precisam de reposicao:\n\n", count);

    for (int i = 0; i < count; i++) {
        printf("[%d] Codigo: %d\n", i + 1, list[i]->code);
        printf("    Nome: %s\n", list[i]->name);
        printf("    Estoque atual: %d %s\n", list[i]->quantity, unit_to_string(list[i]->unit));
        printf("    Estoque minimo: %d %s\n", list[i]->minimum_stock, unit_to_string(list[i]->unit));
        printf("    DEFICIT: %d %s\n", list[i]->minimum_stock - list[i]->quantity, unit_to_string(list[i]->unit));
        printf("----------------------------------------\n");
    }

    pause_screen();
}

// ============================================================================
// FUNÇÃO: handle_save_data
// Salva todos os produtos em arquivo binário
// ============================================================================
void handle_save_data(void) {
    printf("\n========================================\n");
    printf("         SALVAR DADOS\n");
    printf("========================================\n");
    printf("Salvando dados em arquivo...\n");

    if (save_products_to_file(&bank, DATA_FILE_PATH)) {
        printf("\n========================================\n");
        printf("  DADOS SALVOS COM SUCESSO!\n");
        printf("========================================\n");
        printf("  Arquivo: %s\n", DATA_FILE_PATH);
        printf("  Produtos salvos: %d\n", count_active_products(&bank));
        printf("========================================\n");
        log_message(LOG_INFO, "MAIN", "Dados salvos em arquivo com sucesso");
    } else {
        printf("\nErro ao salvar dados!\n");
        printf("Verifique as permissoes do diretorio.\n");
    }

    pause_screen();
}

// ============================================================================
// FUNÇÃO: handle_load_data
// Recarrega produtos do arquivo binário
// ============================================================================
void handle_load_data(void) {
    printf("\n========================================\n");
    printf("       RECARREGAR DADOS\n");
    printf("========================================\n");
    printf("ATENCAO: Dados nao salvos serao perdidos!\n");
    printf("Confirma recarregamento? (1=Sim, 0=Nao): ");

    int confirm = read_int_safe();

    if (confirm != 1) {
        printf("\nOperacao cancelada.\n");
        pause_screen();
        return;
    }

    // Reinicializa banco de produtos
    initialize_product_bank(&bank);

    printf("\nCarregando dados do arquivo...\n");

    if (load_products_from_file(&bank, DATA_FILE_PATH)) {
        printf("\n========================================\n");
        printf("  DADOS RECARREGADOS COM SUCESSO!\n");
        printf("========================================\n");
        printf("  Arquivo: %s\n", DATA_FILE_PATH);
        printf("  Produtos carregados: %d\n", bank.count);
        printf("========================================\n");
        log_message(LOG_INFO, "MAIN", "Dados recarregados do arquivo com sucesso");
    } else {
        printf("\nErro ao recarregar dados!\n");
        printf("O arquivo pode nao existir ou estar corrompido.\n");
    }

    pause_screen();
}
