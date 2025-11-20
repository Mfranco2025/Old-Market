@echo off
chcp 65001 >nul
cls

echo ========================================
echo   COMPILADOR MERCADO - Windows 11
echo ========================================
echo.

set SRC=src
set INC=include
set OBJ=build\obj
set BIN=build\bin

echo Limpando builds anteriores...
if exist "%OBJ%\*.o" del /q "%OBJ%\*.o" >nul 2>&1
if exist "%OBJ%\*.d" del /q "%OBJ%\*.d" >nul 2>&1
if exist "%BIN%\mercado.exe" del "%BIN%\mercado.exe" >nul 2>&1

if not exist "%OBJ%" mkdir "%OBJ%"
if not exist "%BIN%" mkdir "%BIN%"

echo.
echo [1/6] Compilando logger.c...
gcc -c -I"%INC%" -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall "%SRC%\logger.c" -o "%OBJ%\logger.o"
if errorlevel 1 goto erro

echo [2/6] Compilando product.c...
gcc -c -I"%INC%" -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall "%SRC%\product.c" -o "%OBJ%\product.o"
if errorlevel 1 goto erro

echo [3/6] Compilando persistence.c...
gcc -c -I"%INC%" -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall "%SRC%\persistence.c" -o "%OBJ%\persistence.o"
if errorlevel 1 goto erro

echo [4/6] Compilando validation.c...
gcc -c -I"%INC%" -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall "%SRC%\validation.c" -o "%OBJ%\validation.o"
if errorlevel 1 goto erro

echo [5/6] Compilando utils.c...
gcc -c -I"%INC%" -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall "%SRC%\utils.c" -o "%OBJ%\utils.o"
if errorlevel 1 goto erro

echo [6/6] Compilando main.c...
gcc -c -I"%INC%" -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall "%SRC%\main.c" -o "%OBJ%\main.o"
if errorlevel 1 goto erro

echo.
echo Linkando executavel...
gcc "%OBJ%\logger.o" "%OBJ%\product.o" "%OBJ%\persistence.o" "%OBJ%\validation.o" "%OBJ%\utils.o" "%OBJ%\main.o" -o "%BIN%\mercado.exe"
if errorlevel 1 goto erro

echo.
echo ========================================
echo   BUILD CONCLUIDO COM SUCESSO!
echo ========================================
echo Executavel: %BIN%\mercado.exe
echo.
echo Pressione qualquer tecla para executar...
pause >nul
"%BIN%\mercado.exe"
exit /b 0

:erro
echo.
echo ========================================
echo   ERRO NA COMPILACAO!
echo ========================================
echo Verifique os erros acima.
echo.
pause
exit /b 1
