#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Incluir headers específicos do Windows para criar diretório e configurar UTF-8
#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define PATH_SEPARATOR '\\'
    #define mkdir_portable(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define PATH_SEPARATOR '/'
    #define mkdir_portable(path) mkdir(path, 0755)
#endif

// ============================================================================
// MÓDULO: logger — Implementação do sistema de logging
// ============================================================================
// Sistema de registro de eventos com níveis de severidade
// Suporta output para arquivo e console simultaneamente
// Identificadores em inglês, snake_case; comentários em português
// ============================================================================

// ponteiro para o arquivo de log aberto (NULL se não configurado)
static FILE *log_file = NULL;

// nível mínimo de log a ser registrado (eventos abaixo são ignorados)
static log_level_t current_level = LOG_INFO;

// flag que define se logs também devem aparecer no console
static int show_console = 1;

// strings descritivas para cada nível de log (para formatação)
static const char *level_names[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

// ============================================================================
// FUNÇÃO: extract_directory_path
// Extrai o caminho do diretório de um caminho de arquivo completo
// ============================================================================
static void extract_directory_path(const char *file_path, char *dir_path, size_t size) {
    strncpy(dir_path, file_path, size - 1);
    dir_path[size - 1] = '\0';

    // Procura o último separador de diretório (/ ou \)
    char *last_sep = strrchr(dir_path, '/');
    char *last_sep_win = strrchr(dir_path, '\\');

    // Usa o separador que aparecer por último
    char *separator = last_sep;
    if (last_sep_win && (!last_sep || last_sep_win > last_sep)) {
        separator = last_sep_win;
    }

    if (separator) {
        *separator = '\0';  // Trunca no separador
    } else {
        dir_path[0] = '\0';  // Sem diretório, apenas nome de arquivo
    }
}

// ============================================================================
// FUNÇÃO: create_directory_if_needed
// Cria o diretório especificado se ele não existir (Windows e Linux)
// ============================================================================
static int create_directory_if_needed(const char *file_path) {
    char dir_path[512];
    extract_directory_path(file_path, dir_path, sizeof(dir_path));

    // Se não há diretório (arquivo no diretório atual), não faz nada
    if (dir_path[0] == '\0') {
        return 1;  // Sucesso (não precisa criar)
    }

    // Tenta criar o diretório
    // mkdir retorna 0 em sucesso, -1 se falhar (mas pode falhar porque já existe)
    if (mkdir_portable(dir_path) == 0) {
        return 1;  // Diretório criado com sucesso
    }

    // Se falhou, verifica se é porque o diretório já existe
    // Tenta abrir um arquivo de teste no diretório
    char test_path[600];
    snprintf(test_path, sizeof(test_path), "%s%c.test", dir_path, PATH_SEPARATOR);
    FILE *test = fopen(test_path, "w");
    if (test) {
        fclose(test);
        remove(test_path);
        return 1;  // Diretório existe e é acessível
    }

    return 0;  // Falhou ao criar e diretório não existe
}

// ============================================================================
// FUNÇÃO: logger_init
// Inicializa o sistema de logging
// ============================================================================
void logger_init(const char *file_name, log_level_t level_minimum, int show_console_flag) {
    // Configura encoding UTF-8 no console do Windows
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    current_level = level_minimum;
    show_console = show_console_flag;

    // Se não foi especificado arquivo, usa apenas stdout
    if (!file_name) {
        log_file = NULL;
        return;
    }

    // Cria o diretório se necessário
    if (!create_directory_if_needed(file_name)) {
        fprintf(stderr, "Aviso: nao foi possivel criar diretorio para logs\n");
        fprintf(stderr, "Continuando sem arquivo de log...\n");
        log_file = NULL;
        return;
    }

    // Abre arquivo em modo append (adiciona no final, preserva conteúdo anterior)
    log_file = fopen(file_name, "a");
    if (!log_file) {
        fprintf(stderr, "Aviso: nao foi possivel abrir arquivo de log: %s\n", file_name);
        fprintf(stderr, "Continuando sem arquivo de log...\n");
        return;
    }

    // Registra inicialização do sistema
    log_message(LOG_INFO, "LOGGER", "Sistema de logging inicializado");
}

// ============================================================================
// FUNÇÃO: log_message
// Registra uma mensagem de log com timestamp e nível
// ============================================================================
void log_message(log_level_t level, const char *module, const char *message) {
    // Ignora mensagens abaixo do nível mínimo configurado
    if (level < current_level) {
        return;
    }

    // Obtém timestamp atual
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    // Formata mensagem: [TIMESTAMP] [NIVEL] [MODULO] mensagem
    char formatted[1024];
    snprintf(formatted, sizeof(formatted), "[%s] [%-7s] [%s] %s\n",
             timestamp, level_names[level], module, message);

    // Escreve no arquivo de log (se configurado)
    if (log_file) {
        fputs(formatted, log_file);
        fflush(log_file);  // Força escrita imediata (importante para debug de crashes)
    }

    // Escreve no console (se habilitado)
    if (show_console) {
        fputs(formatted, stdout);
    }
}

// ============================================================================
// FUNÇÃO: logger_close
// Finaliza o sistema de logging e fecha o arquivo
// ============================================================================
void logger_close(void) {
    if (log_file) {
        log_message(LOG_INFO, "LOGGER", "Sistema de logging finalizado");
        fclose(log_file);
        log_file = NULL;
    }
}
