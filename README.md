# 🐚 Minishell

> As beautiful as a shell — 42 Network project

[![42 School](https://img.shields.io/badge/42-School-000000?style=for-the-badge&logo=42&logoColor=white)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)

---

## 📖 About

**Minishell** is a 42 school project that challenges you to create your own simplified version of **bash**. It involves building a fully functional shell from scratch in C, capable of parsing user input, managing processes, and executing commands — just like a real terminal.

This project is a deep dive into how shells work under the hood: **lexing**, **parsing**, **process creation**, **file descriptor management**, and **signal handling**.

## ✨ Features

- 🔍 **Command Execution** — Run any system binary via `PATH` resolution or absolute paths
- 🔀 **Pipes** — Chain commands with `|` (e.g., `ls -la | grep .c | wc -l`)
- 📁 **Redirections** — Input `<`, output `>`, append `>>`, and heredoc `<<`
- 💲 **Environment Variables** — Expand `$VAR` and `$?` (last exit status)
- 🏠 **Built-in Commands**:
  - `echo` (with `-n` flag)
  - `cd` (relative and absolute paths)
  - `pwd`
  - `export` / `unset`
  - `env`
  - `exit`
- 🔔 **Signal Handling** — `Ctrl+C`, `Ctrl+D`, `Ctrl+\` behave like in bash
- 📝 **Command History** — Navigate previous commands with arrow keys
- 🔗 **Quote Handling** — Single `'` and double `"` quotes with proper parsing

## 🚀 Getting Started

### Prerequisites

- GCC compiler
- Make
- `readline` library (`sudo apt install libreadline-dev` on Ubuntu)
- A UNIX-based OS (Linux / macOS)

### Build

```bash
git clone https://github.com/JMADIL/MINISHELL.git
cd MINISHELL
make
```

### Usage

```bash
./minishell
```

You'll be greeted with a prompt where you can type commands just like in bash:

```
minishell$ echo "Hello World"
Hello World
minishell$ ls -la | grep .c | wc -l
3
minishell$ export MY_VAR="42"
minishell$ echo $MY_VAR
42
minishell$ exit
```

## 📂 Project Structure

```
MINISHELL/
├── Makefile            # Build system
├── minishell.c         # Entry point & main loop (prompt, read, execute)
├── minishell.h         # Header with structs, prototypes & includes
├── cleaning.c          # Memory cleanup & free functions
├── pars/               # Parsing module (lexer, tokenizer, AST)
└── exec/               # Execution module (builtins, pipes, redirections)
```

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────┐
│                     MINISHELL                       │
├─────────────────────────────────────────────────────┤
│                                                     │
│   Input ──► Lexer ──► Parser ──► Executor ──► Output│
│     │                    │            │              │
│  readline()         Token List    fork/execve        │
│                     + AST         + pipes            │
│                                   + redirections     │
│                                                     │
├─────────────────────────────────────────────────────┤
│  Built-ins: echo, cd, pwd, export, unset, env, exit │
└─────────────────────────────────────────────────────┘
```

## 🔑 Key Concepts Learned

| Concept | Description |
|---------|-------------|
| **Lexing & Parsing** | Tokenizing raw input and building an abstract syntax tree |
| **Process Management** | Creating child processes with `fork()` and `execve()` |
| **Pipes** | Inter-process communication via `pipe()` and file descriptors |
| **Redirections** | Manipulating `stdin`/`stdout` with `dup2()` |
| **Signals** | Handling `SIGINT`, `SIGQUIT`, `SIGTERM` in parent and child |
| **Environment** | Managing environment variables as a linked list |
| **Memory Management** | Preventing leaks with proper cleanup routines |

## 👤 Author

**Adil Jamoun** — [@JMADIL](https://github.com/JMADIL) & [@ilyasrf](https://github.com/ilyasrf)

> 🏫 1337 Coding School (42 Network) — Morocco
