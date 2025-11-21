# 🛒 Old Market

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/C-Standard%20C99-00599C.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg)
![Status](https://img.shields.io/badge/status-active-success.svg)

> **Gerencie seu estoque com leveza, precisão e sem dependências pesadas.**

O **Old Market** é um sistema de controle de estoque desenvolvido puramente em C. O objetivo deste projeto foi criar uma solução que unisse a performance da linguagem C com práticas modernas de engenharia de software, como arquitetura modular, persistência binária e logs detalhados.

Ideal para estudos de caso sobre sistemas embarcados, aplicações de console de alta performance ou para pequenos comércios que precisam de uma solução que rode em qualquer hardware.

---

## ✨ O que ele faz?

O sistema resolve o problema de gestão de produtos oferecendo:

- 📦 **Gestão Completa de Produtos:** Cadastro, leitura, edição e exclusão (CRUD) com validação rigorosa de dados.
- ⚠️ **Alertas Inteligentes:** Monitoramento automático de produtos abaixo do estoque mínimo.
- 💾 **Persistência Customizada:** Os dados são salvos em formato binário (`.dat`), garantindo que o banco de dados seja compacto e rápido, além de possuir sistema de versionamento interno.
- 📝 **Rastreabilidade (Logs):** Cada ação dentro do sistema (sucesso ou erro) é registrada com timestamp, permitindo auditoria completa.
- 🌎 **Suporte Brasileiro:** Interface e tratamento de strings preparados para UTF-8 (acentuação funciona corretamente no terminal).

---

## 🛠️ A Tecnologia por Trás

O projeto segue o padrão **C99** e foi estruturado para simular uma arquitetura corporativa:

| Componente       | Descrição                                                                 |
| :--------------- | :------------------------------------------------------------------------ |
| **Core**         | Lógica pura em C, sem dependências externas (apenas StdLib).              |
| **Arquitetura**  | Modularização em camadas (UI, Logic, Persistence, Validation).            |
| **Build System** | Scripts automatizados para Windows (`.bat`) e Linux (`Makefile`).         |
| **Database**     | Arquivo binário estruturado com _Soft Delete_ (preservação de histórico). |

---

## 🚀 Como Rodar o Projeto

### Pré-requisitos

Você precisa apenas de um compilador C (GCC é o recomendado).

- **Windows:** msys instalado via ucrt64.
- **Linux:** Pacote `build-essential` ou `gcc`.

### 1. Clone e Entre

````bash
git clone https://github.com/Mfranco2025/Old-Market
cd old_market

### 2\. Compile e Execute

Simplificamos o processo de build com scripts prontos:

**No Windows:**

```powershell
# Basta rodar o script de build (ele compila e gera a pasta build)
.\build_mercado.bat

# Para executar:
.\build\bin\mercado.exe
````

**No Linux:**

```bash
# 1º Transforme o script em executável e execute:
chmod +x build_mercado.sh
./build_mercado.sh

# Para executar:
./build/bin/mercado
```

---

## 📖 Guia de Uso Rápido

Ao iniciar o sistema, você verá o menu principal. A navegação é feita via números.

```text
========================================
     SISTEMA DE CONTROLE DE MERCADO
========================================
Produtos cadastrados: 42
========================================
1 - Cadastrar Produto
...
6 - Relatório de Reposição (Estoque Baixo)
...
0 - Sair
```

### Dicas de Preenchimento

- **Preços:** O sistema aceita tanto vírgula (`5,90`) quanto ponto (`5.90`).
- **Backup:** Seus dados ficam salvos em `data/products.dat`. Para fazer um backup, basta copiar este arquivo.

---

## 📂 Estrutura de Pastas

Para quem deseja estudar o código, esta é a organização:

```
old_market/
├── src/           # Onde a mágica acontece (código fonte .c)
├── include/       # Contratos e definições (headers .h)
├── data/          # Banco de dados binário (gerado pelo sistema)
├── logs/          # Arquivos de log para auditoria
├── build/         # Executáveis e objetos (gerado na compilação)
└── docs/          # Documentação complementar
```

### Módulos Principais

- `product.c`: Regras de negócio (cálculos, structs).
- `persistence.c`: Toda a lógica de ler/escrever bits no disco.
- `validation.c`: Garante que ninguém digite texto no lugar de preço.
- `logger.c`: O "gravador" do sistema.

---

## 🤝 Contribuição

Este é um projeto Open Source ideal para iniciantes em C. Quer contribuir?

1.  Faça um Fork.
2.  Crie uma branch (`git checkout -b feature/nova-melhoria`).
3.  Commit suas mudanças (`git commit -m 'Adiciona nova validação'`).
4.  Push para a branch e abra um Pull Request.

---

## 📝 Licença

Distribuído sob a licença MIT. Sinta-se livre para usar, modificar e distribuir.

---

<div align="center">
<sub>Desenvolvido com ☕ e C por <a href="https://github.com/Mfranco2025">Matheus Franco</a></sub>
</div>
