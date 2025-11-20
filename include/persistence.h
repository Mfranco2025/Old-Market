#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "product.h"

// ============================================================================
// MODULO: persistence — Salvar e carregar dados em arquivo binário
// ============================================================================
// Gerencia persistência de produtos em disco usando arquivo binário
// Identificadores em ingles, snake_case; comentários em português
// ============================================================================

// nome padrao do arquivo de dados
#define DATA_FILE_PATH "data/products.dat"

// versao do formato de arquivo (para controle de compatibilidade)
#define FILE_FORMAT_VERSION 1

// ============================================================================
// API PUBLICA
// ============================================================================

// salva o banco de produtos em arquivo binario
// retorna 1 se sucesso, 0 se erro
int save_products_to_file(const product_bank *bank, const char *file_path);

// carrega produtos do arquivo binário para o banco
// retorna 1 se sucesso, 0 se erro (arquivo nao existe ou corrupto)
int load_products_from_file(product_bank *bank, const char *file_path);

// verifica se arquivo de dados existe
// retorna 1 se existe, 0 caso contrario
int data_file_exists(const char *file_path);

// cria backup do arquivo de dados
// retorna 1 se sucesso, 0 se erro
int backup_data_file(const char *file_path);

#endif // PERSISTENCE_H
