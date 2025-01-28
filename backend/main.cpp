#include "crow.h"  // Inclui a biblioteca Crow para criar o servidor da API em C++
#include "rotas.h"  // Inclui o arquivo de rotas onde as rotas HTTP serão definidas
#include <sqlite3.h>  // Inclui a biblioteca SQLite3 para interagir com o banco de dados
#include <string>  // Biblioteca para manipulação de strings
#include "dbManager.h"  // Inclui o arquivo dbManager.h, responsável pela interação com o banco de dados

using namespace std;

// Ponteiro para o banco de dados SQLite
sqlite3* db;
// Instância do gerenciador do banco de dados
DBManager dbManager;

int main()
{
    // Criação do objeto Crow para o servidor da API
    crow::SimpleApp app;

    // Inicializa o banco de dados com o nome "dbProdutos.db" e obtém a referência do banco no ponteiro 'db'
    dbManager.iniciarBanco("dbProdutos.db", &db);

    // Registra todas as rotas HTTP definidas no arquivo rotas.h
    registrarRotas(app, db);

    // Inicia o servidor na porta 18080 e aguarda as requisições
    app.port(18080).run();

    // Fecha a conexão com o banco de dados após a execução do servidor
    sqlite3_close(db);

    // Retorna 0 para indicar que o programa terminou com sucesso
    return 0;
}