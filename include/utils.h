#ifndef UTILS_H
#define UTILS_H

// ============================================================================
// MÓDULO: utils — Funções utilitárias de I/O seguro e manipulação de texto
// ============================================================================
// Aqui são centralizadas funções reutilizáveis para entrada e manipulação
// de strings e números, garantindo SEGURANÇA contra buffer overflow e
// facilitando a padronização da interface. Todos os identificadores seguem
// snake_case; todas as macros, SCREAMING_SNAKE_CASE. As mensagens e
// comentários SEMPRE em português brasileiro, para máximo aprendizado.
// ============================================================================

// tamanho máximo de buffer temporário para leitura de entrada
#define MAX_INPUT_BUFFER_SIZE 100

// --------------------------------------------------------------------------
// Limpa o buffer de entrada do teclado (stdin).
// Evita problemas causados por entradas maiores que o esperado.
// --------------------------------------------------------------------------
void clear_input_buffer(void);

// --------------------------------------------------------------------------
// Lê um inteiro de forma segura (só aceita número isolado, evita lixo).
// Retorna o inteiro digitado ou -1 em erro (entrada inválida, EOF).
// Garante que não há caracteres digitados a mais.
// Mensagens de erro para o usuário SEMPRE em português.
// --------------------------------------------------------------------------
int read_int_safe(void);

// --------------------------------------------------------------------------
// Lê um número decimal (float) de forma segura.
// Aceita vírgula (,) ou ponto (.) como separador decimal (padrão brasileiro).
// Retorna o float digitado ou -1.0f em caso de erro.
// Exemplo: 255,70 ou 255.70 retornam 255.70
// --------------------------------------------------------------------------
float read_float_safe(void);

// --------------------------------------------------------------------------
// Lê uma string da entrada padrão sem causar buffer overflow.
// Remove o '\n' final se presente, garantindo string terminada.
// str: buffer de destino
// max_length: comprimento máximo permitido (incluindo '\0')
// --------------------------------------------------------------------------
void read_str_safe(char *str, int max_length);

// --------------------------------------------------------------------------
// Converte uma string para letras maiúsculas (in-place).
// Usada para normalização em buscas case-insensitive.
// --------------------------------------------------------------------------
void str_to_upper(char *str);

// --------------------------------------------------------------------------
// Pausa a execução e aguarda o usuário pressionar ENTER.
// Usado para manter menus e mensagens visíveis antes de limpar a tela.
// --------------------------------------------------------------------------
void pause_screen(void);

#endif // UTILS_H
