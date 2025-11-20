#include "persistence.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// ============================================================================
// MODULO: persistence — Implementacao da persistencia em arquivo binario
// ============================================================================
// Salva e carrega produtos usando formato binario para eficiencia
// Identificadores em ingles, snake_case; comentarios em portugues
// ============================================================================
// ============================================================================

// estrutura de cabecalho do arquivo (metadados)
typedef struct {
    int version;        // versao do formato
    int product_count;  // quantidade de produtos salvos
    int next_code;      // proximo codigo disponivel
} file_header;

// salva banco de produtos em arquivo binario
int save_products_to_file(const product_bank *bank, const char *file_path) {
    if (!bank || !file_path) {
        log_message(LOG_ERROR, "persistence", "Parametros invalidos para salvar");
        return 0;
    }

    FILE *file = fopen(file_path, "wb");
    if (!file) {
        log_message(LOG_ERROR, "persistence", "Nao foi possivel abrir arquivo para escrita");
        return 0;
    }

    // escreve cabecalho
    file_header header;
    header.version = FILE_FORMAT_VERSION;
    header.product_count = bank->count;
    header.next_code = bank->next_code;

    if (fwrite(&header, sizeof(file_header), 1, file) != 1) {
        log_message(LOG_ERROR, "persistence", "Erro ao escrever cabecalho");
        fclose(file);
        return 0;
    }

    // escreve array de produtos
    if (bank->count > 0) {
        size_t written = fwrite(bank->list, sizeof(product), bank->count, file);
        if (written != (size_t)bank->count) {
            log_message(LOG_ERROR, "persistence", "Erro ao escrever produtos");
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    log_message(LOG_INFO, "persistence", "Dados salvos com sucesso");
    return 1;
}

// carrega produtos do arquivo binario
int load_products_from_file(product_bank *bank, const char *file_path) {
    if (!bank || !file_path) {
        log_message(LOG_ERROR, "persistence", "Parametros invalidos para carregar");
        return 0;
    }

    FILE *file = fopen(file_path, "rb");
    if (!file) {
        log_message(LOG_WARNING, "persistence", "Arquivo de dados nao encontrado");
        return 0;
    }

    // le cabecalho
    file_header header;
    if (fread(&header, sizeof(file_header), 1, file) != 1) {
        log_message(LOG_ERROR, "persistence", "Erro ao ler cabecalho");
        fclose(file);
        return 0;
    }

    // verifica versao do arquivo
    if (header.version != FILE_FORMAT_VERSION) {
        log_message(LOG_ERROR, "persistence", "Versao de arquivo incompativel");
        fclose(file);
        return 0;
    }

    // verifica limites
    if (header.product_count > MAX_PRODUCTS) {
        log_message(LOG_ERROR, "persistence", "Arquivo corrompido: muitos produtos");
        fclose(file);
        return 0;
    }

    // le produtos
    if (header.product_count > 0) {
        size_t read_count = fread(bank->list, sizeof(product), header.product_count, file);
        if (read_count != (size_t)header.product_count) {
            log_message(LOG_ERROR, "persistence", "Erro ao ler produtos");
            fclose(file);
            return 0;
        }
    }

    // atualiza contadores do banco
    bank->count = header.product_count;
    bank->next_code = header.next_code;

    fclose(file);
    log_message(LOG_INFO, "persistence", "Dados carregados com sucesso");
    return 1;
}

// verifica se arquivo existe
int data_file_exists(const char *file_path) {
    if (!file_path) return 0;

    struct stat buffer;
    return (stat(file_path, &buffer) == 0);
}

// cria backup do arquivo de dados
int backup_data_file(const char *file_path) {
    if (!file_path || !data_file_exists(file_path)) {
        return 0;
    }

    // cria nome do arquivo de backup
    char backup_path[256];
    snprintf(backup_path, sizeof(backup_path), "%s.backup", file_path);

    // abre arquivo original
    FILE *source = fopen(file_path, "rb");
    if (!source) {
        log_message(LOG_ERROR, "persistence", "Erro ao abrir arquivo original para backup");
        return 0;
    }

    // cria arquivo de backup
    FILE *dest = fopen(backup_path, "wb");
    if (!dest) {
        log_message(LOG_ERROR, "persistence", "Erro ao criar arquivo de backup");
        fclose(source);
        return 0;
    }

    // copia dados
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, bytes, dest) != bytes) {
            log_message(LOG_ERROR, "persistence", "Erro ao escrever backup");
            fclose(source);
            fclose(dest);
            return 0;
        }
    }

    fclose(source);
    fclose(dest);

    log_message(LOG_INFO, "persistence", "Backup criado com sucesso");
    return 1;
}
