# CinConecta: Projeto de gestão de estoque de produtos com autenticação

Este projeto implementa um sistema de gerenciamento de produtos, com funcionalidades como pesquisa, adição, atualização, remoção de produtos e autenticação de usuários. O sistema utiliza um banco de dados SQLite para persistir dados e foi desenvolvido utilizando conceitos de Programação Orientada a Objetos (OOP).

## Um vídeo do projeto sendo utilizado:
[![link](https://img.youtube.com/vi/2eIc5AYp1jI/0.jpg)](https://www.youtube.com/watch?v=2eIc5AYp1jI)

### Funcionalidades:
1. **Cadastro de usuários**: Permite que novos usuários se registrem no sistema, utilizando um nome de usuário e uma senha criptografada.
2. **Autenticação de usuários**: Os usuários podem fazer login com seu nome de usuário e senha, e um token de autenticação é gerado para garantir sessões seguras.
3. **Gestão de Produtos**:
   - **Adicionar produtos**
   - **Pesquisar produtos**
   - **Atualizar produtos**
   - **Remover produtos**

### Tecnologias Utilizadas:
- **C++**: Linguagem principal utilizada para o desenvolvimento do sistema.
- **CROW**: Framework de C++ utilizado para o servidor.
- **SQLite**: Banco de dados leve utilizado para armazenar informações de usuários, tokens e produtos.
- **Programação Orientada a Objetos (OOP)**: O sistema foi projetado utilizando conceitos de OOP, organizando as funcionalidades em classes e objetos.

---

## Arquitetura

### Classes Principais:
1. **DBManager**:
   Classe responsável por gerenciar as operações no banco de dados, como buscar, inserir, atualizar e remover registros.
2. **Produto**:
   Representa os produtos armazenados no banco de dados, contendo atributos como `id`, `nome`, `quantidade` e `categoria`.

### Funcionalidades OOP:
- **Encapsulamento**: Todos os dados e operações relacionadas aos produtos e usuários estão encapsulados dentro de suas respectivas classes.
- **Abstração**: As operações no banco de dados são abstraídas na classe `DBManager`, escondendo detalhes de implementação da camada superior.


---
# Usando o sistema:
Você pode abrir a pasta release e abrir os arquivos crow.exe (para o frontend) e backend_projeto.exe (para o backend)

# Compilando o sistema:

Este é um servidor web simples implementado em C++ utilizando o framework **Crow** para a criação da API, com persistência de dados utilizando **SQLite3**. O sistema permite realizar operações de autenticação, cadastro e gestão de produtos.

## Requisitos

Antes de executar o sistema, certifique-se de que você tem as seguintes dependências instaladas:

- **C++11 ou superior**: Certifique-se de que o seu compilador suporta a versão C++11 ou superior.
- **Boost**: Biblioteca necessária para diversas operações no sistema.
- **Crow**: Framework C++ utilizado para criar servidores web.
- **Biblioteca SQLite3**: Utilizada para interação com o banco de dados.

### Instalação das Dependências

1. **Instalar o Boost**:

```bash
sudo apt-get update
sudo apt-get install libboost-all-dev
```
2. **Instalar o Crow**:

```bash
git clone https://github.com/CrowCpp/Crow.git
cd Crow
mkdir build
cd build
cmake ..
sudo make install
```

3. **Instalar o SQLite3**:

```bash
sudo apt-get install libsqlite3-dev
```

4. **Clone o repositório**:
```bash
git clone https://github.com/silly/servidorcrow.git
cd servidorcrow
```

5. **Compile e execute**:
```bash
g++ -std=c++11 main.cpp -o backend_projeto -lboost_system -lboost_filesystem -lsqlite3 -lpthread
```

# Instruções para Rodar o Frontend

Para rodar o frontend deste projeto, é necessário buildar o projeto Flutter presente na pasta `frontend/cinconecta-flutter`. Siga os passos abaixo para configurar e executar o frontend corretamente.

## Passos

### 1. Clonar o Repositório

Primeiro, clone o repositório para o seu ambiente local. Abra o Git Bash e execute o comando:

```bash
git clone https://github.com/sillyveira/cinconecta-cpp.git
```

### 2. Acesse a Pasta do Frontend
Navegue até o diretório onde o código do Flutter está localizado:

```bash
cd cinconecta-cpp/frontend/cinconecta-flutter
```

### 3. Instalar as Dependências do Flutter
Antes de compilar o projeto, instale as dependências necessárias. Execute o seguinte comando para garantir que todos os pacotes do Flutter sejam baixados:

```bash
flutter pub get
```
### 4. Buildar o Projeto
Agora, gere os arquivos necessários para rodar o frontend. Para isso, execute:
```bash
flutter build
```
### 5. Executar o Aplicativo
Para rodar o projeto em um emulador ou dispositivo conectado, use o comando:
```bash
flutter run
```
Isso irá compilar e iniciar o aplicativo Flutter.
