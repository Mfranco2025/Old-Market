#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

// enum que representa níveis de log disponíveis
// permite filtrar logs conforme a importância da mensagem
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} log_level_t;

// inicializa o sistema de logging
// filename: nome do arquivo de log (usa stdout caso NULL)
// level_minimo: nível mínimo a registrar (LOG_INFO recomendado para produção)
// exibe_console: se verdadeiro, também escreve no terminal
void logger_init(const char *file_name, log_level_t level_minimum, int show_console);

// registra mensagem de log, incluindo timestamp, nível e módulo de origem
// Todas as mensagens devem estar em português e indicar claramente o evento
void log_message(log_level_t level, const char *module, const char *message);

// encerra o sistema de logging, fechando o arquivo se necessário
void logger_close(void);

#endif //LOGGER_H
