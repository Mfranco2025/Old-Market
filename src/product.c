#include <stdio.h>
#include <string.h>
#include "product.h"
#include "validation.h"

// inicializa o banco de produtos: zera contagem e códigos automáticos
void initialize_product_bank(product_bank *bank) {
    if (!bank) return;
    memset(bank->list, 0, sizeof(bank->list));
    bank->count = 0;
    bank->next_code = 1;
}

// cadastra novo produto, retorna 1 se sucesso, 0 se erro de validação ou cheio
int register_product(product_bank *bank, const char *name, float price, int quantity, int minimum_stock, int category, int unit) {
    if (!bank || !name) return 0;
    if (bank->count >= MAX_PRODUCTS) {
        printf("Limite máximo de produtos atingido.\n");
        return 0;
    }
    // valida todos os campos
    if (!is_valid_name_format(name)) {
        printf("Nome do produto inválido.\n");
        return 0;
    }
    if (!is_valid_price(price)) {
        printf("Preço inválido.\n");
        return 0;
    }
    if (!is_valid_quantity(quantity)) {
        printf("Quantidade inválida.\n");
        return 0;
    }
    if (!is_valid_minimum_stock(minimum_stock, quantity)) {
        printf("Estoque mínimo inválido.\n");
        return 0;
    }
    if (!is_valid_category(category)) {
        printf("Categoria inválida.\n");
        return 0;
    }
    if (!is_valid_unit(unit)) {
        printf("Unidade de medida inválida.\n");
        return 0;
    }
    // preenche o novo produto
    product *p = &bank->list[bank->count];
    p->code = bank->next_code++;
    strncpy(p->name, name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';
    p->price = price;
    p->quantity = quantity;
    p->minimum_stock = minimum_stock;
    p->category = category;
    p->unit = unit;
    p->active = 1;
    bank->count++;
    printf("Produto cadastrado com sucesso!\n");
    return 1;
}

// busca produto pelo código
product *find_product_by_code(product_bank *bank, int code) {
    if (!bank) return NULL;
    for (int i = 0; i < bank->count; ++i) {
        if (bank->list[i].active && bank->list[i].code == code) {
            return &bank->list[i];
        }
    }
    return NULL;
}

// lista produtos ativos (até max_out)
int list_active_products(const product_bank *bank, product *out_array[], size_t max_out) {
    if (!bank || !out_array) return 0;
    int count = 0;
    for (int i = 0; i < bank->count && count < (int)max_out; ++i) {
        if (bank->list[i].active) {
            out_array[count++] = (product *)&bank->list[i];
        }
    }
    return count;
}

// edita produto identificado pelo código
int update_product(product_bank *bank, int code, const char *new_name, float new_price, int new_quantity, int new_minimum_stock, int new_category, int new_unit) {
    product *p = find_product_by_code(bank, code);
    if (!p) {
        printf("Produto não encontrado.\n");
        return 0;
    }
    if (new_name && is_valid_name_format(new_name)) {
        strncpy(p->name, new_name, sizeof(p->name) - 1);
        p->name[sizeof(p->name) - 1] = '\0';
    }
    if (is_valid_price(new_price)) p->price = new_price;
    if (is_valid_quantity(new_quantity)) p->quantity = new_quantity;
    if (is_valid_minimum_stock(new_minimum_stock, new_quantity)) p->minimum_stock = new_minimum_stock;
    if (is_valid_category(new_category)) p->category = new_category;
    if (is_valid_unit(new_unit)) p->unit = new_unit;
    printf("Produto atualizado com sucesso.\n");
    return 1;
}

// inativa (soft delete) produto
int deactivate_product(product_bank *bank, int code) {
    product *p = find_product_by_code(bank, code);
    if (!p) {
        printf("Produto não encontrado.\n");
        return 0;
    }
    p->active = 0;
    printf("Produto inativado.\n");
    return 1;
}

// ativa produto inativo
int activate_product(product_bank *bank, int code) {
    if (!bank) return 0;
    for (int i = 0; i < bank->count; ++i) {
        if (!bank->list[i].active && bank->list[i].code == code) {
            bank->list[i].active = 1;
            printf("Produto reativado.\n");
            return 1;
        }
    }
    printf("Produto não encontrado ou já ativo.\n");
    return 0;
}

// lista produtos abaixo do estoque mínimo
int list_products_below_minimum(const product_bank *bank, product *out_array[], size_t max_out) {
    if (!bank || !out_array) return 0;
    int count = 0;
    for (int i = 0; i < bank->count && count < (int)max_out; ++i) {
        const product *p = &bank->list[i];
        if (p->active && p->quantity <= p->minimum_stock) {
            out_array[count++] = (product *)p;
        }
    }
    return count;
}

// converte código de categoria para string amigável
const char *category_to_string(int category) {
    switch (category) {
        case CATEGORY_FOOD: return "Alimentos";
        case CATEGORY_DRINK: return "Bebidas";
        case CATEGORY_HYGIENE: return "Higiene";
        case CATEGORY_CLEANING: return "Limpeza";
        case CATEGORY_OTHERS: return "Outros";
        default: return "Desconhecida";
    }
}

// converte código de unidade para string amigável
const char *unit_to_string(int unit) {
    switch (unit) {
        case UNIT_PIECE: return "Unidade";
        case UNIT_KG: return "Kg";
        case UNIT_GRAM: return "Grama";
        case UNIT_LITER: return "Litro";
        case UNIT_ML: return "Mililitro";
        default: return "Outra";
    }
}

// conta quantidade de produtos ativos
int count_active_products(const product_bank *bank) {
    if (!bank) return 0;
    int count = 0;
    for (int i = 0; i < bank->count; i++) {
        if (bank->list[i].active) {
            count++;
        }
    }
    return count;
}

// calcula valor total do estoque (somando preço × quantidade dos ativos)
float calculate_total_stock_value(const product_bank *bank) {
    if (!bank) return 0.0f;
    float total = 0.0f;
    for (int i = 0; i < bank->count; i++) {
        const product *p = &bank->list[i];
        if (p->active) {
            total += p->price * p->quantity;
        }
    }
    return total;
}
