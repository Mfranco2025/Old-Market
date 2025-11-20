#!/bin/bash

# ========================================
#   COMPILADOR MERCADO - Linux
# ========================================

# Configuração de diretórios (Linux usa / ao invés de \)
SRC="src"
INC="include"
OBJ="build/obj"
BIN="build/bin"
EXECUTAVEL="$BIN/mercado"

# Cores para o terminal (opcional, mas fica bonito)
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # Sem cor

clear
echo "========================================"
echo "  Iniciando Build do Old Market"
echo "========================================"
echo ""

# 1. Limpeza
echo "Limpando builds anteriores..."
rm -f "$OBJ"/*.o "$OBJ"/*.d
rm -f "$EXECUTAVEL"

# Cria diretórios se não existirem (-p cria toda a árvore necessária)
mkdir -p "$OBJ"
mkdir -p "$BIN"

echo ""

# Função para verificar erros
check_error() {
    if [ $? -ne 0 ]; then
        echo -e "${RED}ERRO: Falha na compilação de $1${NC}"
        exit 1
    fi
}

# 2. Compilação (Passo a Passo igual ao .bat)

echo "[1/6] Compilando logger.c..."
gcc -c -I"$INC" -Wall "$SRC/logger.c" -o "$OBJ/logger.o"
check_error "logger.c"

echo "[2/6] Compilando product.c..."
gcc -c -I"$INC" -Wall "$SRC/product.c" -o "$OBJ/product.o"
check_error "product.c"

echo "[3/6] Compilando persistence.c..."
gcc -c -I"$INC" -Wall "$SRC/persistence.c" -o "$OBJ/persistence.o"
check_error "persistence.c"

echo "[4/6] Compilando validation.c..."
gcc -c -I"$INC" -Wall "$SRC/validation.c" -o "$OBJ/validation.o"
check_error "validation.c"

echo "[5/6] Compilando utils.c..."
gcc -c -I"$INC" -Wall "$SRC/utils.c" -o "$OBJ/utils.o"
check_error "utils.c"

echo "[6/6] Compilando main.c..."
gcc -c -I"$INC" -Wall "$SRC/main.c" -o "$OBJ/main.o"
check_error "main.c"

echo ""

# 3. Linkagem
echo "Linkando executável..."
# O *.o pega todos os objetos na pasta, simplificando a linha
gcc "$OBJ"/*.o -o "$EXECUTAVEL"
check_error "Linkagem final"

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  BUILD CONCLUÍDO COM SUCESSO!${NC}"
echo -e "${GREEN}========================================${NC}"
echo "Executável: $EXECUTAVEL"
echo ""

# 4. Execução
read -p "Pressione ENTER para executar..."
./"$EXECUTAVEL"
