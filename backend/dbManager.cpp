// Incluindo cabe�alhos necess�rios
#include "dbManager.h" // Inclus�o do arquivo de cabe�alho dbManager.h, onde s�o declaradas as fun��es de gerenciamento do banco de dados.
#include <sqlite3.h> // Inclus�o da biblioteca SQLite3 para interagir com o banco de dados SQLite.
#include <string> // Inclus�o da biblioteca string para manipula��o de strings.
#include <vector> // Inclus�o da biblioteca vector para armazenar listas de objetos.
#include "token.h" // Inclus�o do arquivo de cabe�alho token.h, que cont�m declara��es relacionadas ao token.
#include "horarios.h" // Inclus�o do arquivo de cabe�alho horarios.h, provavelmente relacionado � manipula��o de hor�rios (n�o mostrado no c�digo).

using namespace std; // Utiliza��o do espa�o de nomes padr�o da linguagem C++.

// Fun��o para iniciar o banco de dados e criar as tabelas se n�o existirem
void DBManager::iniciarBanco(const string& nome_do_banco, sqlite3** db) {
	// Abre o banco de dados. Se houver um erro, exibe mensagem de erro e encerra o programa.
	if (sqlite3_open(nome_do_banco.c_str(), db)) {
		cerr << "Erro ao abrir o banco de dados: " << sqlite3_errmsg(*db) << endl;
		exit(1);
	}

	cout << "Banco aberto com sucesso." << endl;

	// Cria��o da tabela 'produtos' se ela n�o existir
	string sql_produtos = "CREATE TABLE IF NOT EXISTS produtos ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"nome TEXT NOT NULL, "
		"quantidade INTEGER NOT NULL, "
		"categoria TEXT, "
		"userid TEXT NOT NULL);";

	sqlite3_stmt* stmt = nullptr; // Ponteiro para preparar e executar a consulta SQL.

	// Prepara a consulta SQL para cria��o da tabela 'produtos'.
	if (sqlite3_prepare_v2(*db, sql_produtos.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		// Caso ocorra erro ao preparar a consulta, exibe a mensagem de erro e encerra o programa.
		cerr << "Erro ao preparar a cria��o da tabela usuarios: " << sqlite3_errmsg(*db) << endl;
		sqlite3_finalize(stmt); // Libera os recursos do statement
		exit(1); // Encerra o programa em caso de erro.
	}

	// Executa a consulta para criar a tabela 'produtos'.
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		// Caso ocorra erro ao executar a consulta, exibe a mensagem de erro e encerra o programa.
		cerr << "Erro ao criar a tabela usuarios: " << sqlite3_errmsg(*db) << endl;
		sqlite3_finalize(stmt); // Libera os recursos do statement
		exit(1);
	}
	sqlite3_finalize(stmt); // Finaliza o statement ap�s execu��o.

	stmt = nullptr; // Redefine o ponteiro para reutilizar na pr�xima tabela.

	// Cria��o da tabela 'usuarios' se ela n�o existir
	string sql_usuarios = "CREATE TABLE IF NOT EXISTS usuarios ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"usuario TEXT NOT NULL UNIQUE, "
		"senha INTEGER NOT NULL);";

	// Prepara a consulta SQL para cria��o da tabela 'usuarios'.
	if (sqlite3_prepare_v2(*db, sql_usuarios.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		// Caso ocorra erro ao preparar a consulta, exibe a mensagem de erro e encerra o programa.
		cerr << "Erro ao preparar a cria��o da tabela usuarios: " << sqlite3_errmsg(*db) << endl;
		sqlite3_finalize(stmt); // Libera os recursos do statement
		exit(1);
	}

	// Executa a consulta para criar a tabela 'usuarios'.
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		// Caso ocorra erro ao executar a consulta, exibe a mensagem de erro e encerra o programa.
		cerr << "Erro ao criar a tabela usuarios: " << sqlite3_errmsg(*db) << endl;
		sqlite3_finalize(stmt); // Libera os recursos do statement
		exit(1);
	}
	sqlite3_finalize(stmt); // Finaliza o statement ap�s execu��o.

	stmt = nullptr; // Redefine o ponteiro para reutilizar na pr�xima tabela.

	// Cria��o da tabela 'tokens' se ela n�o existir
	string sql_tokens = "CREATE TABLE IF NOT EXISTS tokens ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"userid TEXT NOT NULL, "
		"token TEXT NOT NULL, "
		"data_expiracao TEXT);";

	// Prepara a consulta SQL para cria��o da tabela 'tokens'.
	if (sqlite3_prepare_v2(*db, sql_tokens.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		// Caso ocorra erro ao preparar a consulta, exibe a mensagem de erro e encerra o programa.
		cerr << "Erro ao preparar a cria��o da tabela tokens: " << sqlite3_errmsg(*db) << endl;
		sqlite3_finalize(stmt); // Libera os recursos do statement
		exit(1);
	}

	// Executa a consulta para criar a tabela 'tokens'.
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		// Caso ocorra erro ao executar a consulta, exibe a mensagem de erro e encerra o programa.
		cerr << "Erro ao criar a tabela tokens: " << sqlite3_errmsg(*db) << endl;
		sqlite3_finalize(stmt); // Libera os recursos do statement
		exit(1);
	}

	sqlite3_finalize(stmt); // Finaliza o statement ap�s execu��o.
}

// Fun��o para obter todos os produtos no banco de dados
vector<Produto> DBManager::getProdutos(sqlite3* db) {
	vector<Produto> produtos; // Lista que ser� retornada com os produtos do banco.

	// Consulta SQL para selecionar todos os produtos.
	string sql = "SELECT * FROM produtos;";

	sqlite3_stmt* stmt; // Statement usado para proteger a consulta SQL contra inje��es.

	// Prepara a consulta SQL
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		// Caso ocorra erro ao preparar a consulta, exibe a mensagem de erro e retorna uma lista vazia.
		cerr << "Erro ao preparar a consulta: " << sqlite3_errmsg(db) << endl;
		return produtos;
	}

	// Executa a consulta e obt�m os resultados
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		// Extrai os dados de cada linha da tabela e cria um objeto Produto.
		int id = sqlite3_column_int(stmt, 0);
		const unsigned char* nome = sqlite3_column_text(stmt, 1);
		int quantidade = sqlite3_column_int(stmt, 2);
		const unsigned char* categoria = sqlite3_column_text(stmt, 3);

		// Adiciona o produto � lista de produtos.
		produtos.push_back(Produto(id, string(reinterpret_cast<const char*>(nome)), quantidade, string(reinterpret_cast<const char*>(categoria))));
	}

	sqlite3_finalize(stmt); // Libera os recursos do statement
	return produtos; // Retorna a lista de produtos recuperados.
}

// Fun��o para obter os produtos de um usu�rio espec�fico com base no seu userid
vector<Produto> DBManager::getProdutosID(sqlite3* db, const std::string& userid) {
	vector<Produto> produtos; // Lista de produtos para retornar.

	// Verifica se o 'userid' n�o est� vazio
	if (userid.empty()) {
		// Se o 'userid' for vazio, retorna a lista vazia.
		return produtos;
	}

	// Consulta SQL para selecionar os produtos filtrados pelo 'userid'
	string sql = "SELECT * FROM produtos WHERE userid = ?;";

	sqlite3_stmt* stmt; // Statement usado para proteger a consulta SQL contra inje��es.

	// Prepara a consulta SQL
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		// Caso ocorra erro ao preparar a consulta, exibe a mensagem de erro e retorna a lista vazia.
		cerr << "Erro ao preparar a consulta: " << sqlite3_errmsg(db) << endl;
		return produtos;
	}

	// Realiza o "bind" do par�metro 'userid' na consulta para evitar inje��es de SQL.
	sqlite3_bind_text(stmt, 1, userid.c_str(), -1, SQLITE_TRANSIENT);

	// Executa a consulta e obt�m os resultados
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		// Extrai os dados de cada linha e cria um objeto Produto
		int id = sqlite3_column_int(stmt, 0);
		const unsigned char* nome = sqlite3_column_text(stmt, 1);
		int quantidade = sqlite3_column_int(stmt, 2);
		const unsigned char* categoria = sqlite3_column_text(stmt, 3);

		// Adiciona o produto � lista
		produtos.push_back(Produto(id, string(reinterpret_cast<const char*>(nome)), quantidade, string(reinterpret_cast<const char*>(categoria))));
	}

	sqlite3_finalize(stmt); // Libera os recursos do statement
	return produtos; // Retorna a lista de produtos do usu�rio.
}

// Fun��o para buscar produtos pelo ID e nome relacionados a um usu�rio espec�fico
vector<Produto> DBManager::searchProdutosID(sqlite3* db, const std::string& userid, const std::string& query) {
    vector<Produto> produtos; // Lista a ser retornada de produtos.

    if (userid.empty()) {
        // Caso o usu�rio n�o tenha sido encontrado
        return produtos;
    }
    // Modificar a consulta para filtrar pelo userid e a query
    string sql = "SELECT * FROM produtos WHERE userid = ? AND nome LIKE ?;";

    sqlite3_stmt* stmt; // statement: usado para proteger as consultas SQL de injetores

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erro ao preparar a consulta: " << sqlite3_errmsg(db) << endl;
        return produtos;
    }

    // Bind do usuario_id
    sqlite3_bind_text(stmt, 1, userid.c_str(), -1, SQLITE_TRANSIENT);
    // O bind da query com % ao redor � para buscar qualquer ocorr�ncia.
    string nomeComCoringas = "%" + query + "%";  // Exemplo: "%exemplo%"
    sqlite3_bind_text(stmt, 2, nomeComCoringas.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* nome = sqlite3_column_text(stmt, 1);
        int quantidade = sqlite3_column_int(stmt, 2);
        const unsigned char* categoria = sqlite3_column_text(stmt, 3);

        // Criando o produto e adicionando na lista
        produtos.push_back(Produto(id, string(reinterpret_cast<const char*>(nome)), quantidade, string(reinterpret_cast<const char*>(categoria))));
    }

    sqlite3_finalize(stmt);
    return produtos;
}

// Fun��o para adicionar um produto ao banco de dados
bool DBManager::addProduto(sqlite3* db, const Produto& produto, const std::string& userid) {
    string sql = "INSERT INTO produtos (nome, quantidade, categoria, userid) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erro ao preparar a consulta: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, produto.nome.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, produto.quantidade);
    sqlite3_bind_text(stmt, 3, produto.categoria.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, userid.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Erro ao executar a consulta: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Fun��o para atualizar as informa��es de um produto no banco de dados
bool DBManager::atualizarProduto(sqlite3* db, const Produto& produto, const std::string& userid) {
    std::string sql = "UPDATE produtos SET nome = ?, quantidade = ?, categoria = ? WHERE id = ? AND userid = ?;";
    sqlite3_stmt* stmt;

    // Prepara a consulta SQL
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erro ao preparar a consulta: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind dos par�metros
    sqlite3_bind_text(stmt, 1, produto.nome.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, produto.quantidade);
    sqlite3_bind_text(stmt, 3, produto.categoria.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, produto.id);
    sqlite3_bind_text(stmt, 5, userid.c_str(), -1, SQLITE_TRANSIENT);

    // Executa a consulta
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Erro ao atualizar produto: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Fun��o para remover um produto do banco de dados
bool DBManager::removerProduto(sqlite3* db, int id, const std::string& userid) {
    std::string sql = "DELETE FROM produtos WHERE id = ? AND userid = ?;";
    sqlite3_stmt* stmt;

    // Prepara a instru��o SQL
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erro ao preparar a consulta para remover produto: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Vincula o valor do ID
    if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK) {
        std::cerr << "Erro ao vincular o ID: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_bind_text(stmt, 2, userid.c_str(), -1, SQLITE_TRANSIENT);

    // Executa a instru��o
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Erro ao executar a remo��o do produto: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finaliza o statement
    sqlite3_finalize(stmt);
    return true;
}

// Fun��o para obter o ID do usu�rio a partir do nome de usu�rio
std::string DBManager::getUserID(sqlite3* db, const std::string& usuario) {
    std::string userid = ""; // Valor padr�o, caso o usu�rio n�o seja encontrado.

    // Consulta SQL para buscar o userid pelo nome do usu�rio
    std::string sql = "SELECT id FROM usuarios WHERE usuario = ?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erro ao preparar a consulta: " << sqlite3_errmsg(db) << std::endl;
        return userid;
    }

    // Bind do nome do usu�rio na consulta
    sqlite3_bind_text(stmt, 1, usuario.c_str(), -1, SQLITE_TRANSIENT);

    // Executar a consulta e verificar se encontramos o usu�rio
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Recuperar o id do usu�rio
        userid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return userid;
}

// Fun��o para registrar um novo usu�rio no banco de dados
bool DBManager::registrarUsuario(sqlite3* db, const std::string& usuario, unsigned int senha_hash) {
    string sql = "INSERT INTO usuarios (usuario, senha) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erro ao preparar a consulta: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, usuario.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, senha_hash);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Erro ao executar a consulta: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Fun��o para verificar a senha e ID de um usu�rio
std::vector<std::string> DBManager::verificarUsuarioSenha(sqlite3* db, const std::string& usuario) {
    std::vector<std::string> resultado; // Lista para armazenar hash e userid
    std::string sql = "SELECT senha, id FROM usuarios WHERE usuario = ?;"; // Consulta para buscar senha e userid

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erro ao preparar a consulta: " << sqlite3_errmsg(db) << endl;
        return resultado; // Retorna lista vazia se ocorrer erro
    }

    sqlite3_bind_text(stmt, 1, usuario.c_str(), -1, SQLITE_STATIC);

    // Executa a consulta
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Pega o hash da senha armazenado e o userid
        const unsigned char* hash_db = sqlite3_column_text(stmt, 0);
        const unsigned char* userid_db = sqlite3_column_text(stmt, 1);

        if (hash_db && userid_db) {
            resultado.push_back(reinterpret_cast<const char*>(hash_db));  // Armazena o hash da senha
            resultado.push_back(reinterpret_cast<const char*>(userid_db)); // Armazena o userid
        }
    }

    sqlite3_finalize(stmt);
    return resultado; // Retorna lista com hash e userid ou vazia se n�o encontrado
}

// Fun��o para gerar um token para o usu�rio no banco de dados
std::string DBManager::gerarTokenDB(sqlite3* db, const std::string& userid) {
    // Consulta para verificar se o token j� existe
    std::string sql_verificar = "SELECT token FROM tokens WHERE userid = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql_verificar.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erro ao preparar consulta de verifica��o: " << sqlite3_errmsg(db) << endl;
        return ""; // Retorna string vazia em caso de erro
    }

    sqlite3_bind_text(stmt, 1, userid.c_str(), -1, SQLITE_TRANSIENT);

    // Se j� existir um token, retorna ele
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* token_existente = sqlite3_column_text(stmt, 0);
        std::string token_str = reinterpret_cast<const char*>(token_existente);
        sqlite3_finalize(stmt);
        return token_str; // Retorna o token existente
    }
    sqlite3_finalize(stmt);

    // Caso contr�rio, cria um novo token
    std::string token = gerarToken();
    std::string sql_inserir = "INSERT INTO tokens (userid, token, data_expiracao) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql_inserir.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erro ao preparar consulta de inser��o: " << sqlite3_errmsg(db) << endl;
        return ""; // Retorna string vazia em caso de erro
    }

    std::string hora_atual = obterHoraAtualComoString();
    sqlite3_bind_text(stmt, 1, userid.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, token.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, hora_atual.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Erro ao inserir o token: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return ""; // Retorna string vazia se ocorrer erro
    }

    sqlite3_finalize(stmt);
    return token; // Retorna o novo token gerado
}

// Fun��o para verificar a validade de um token
bool DBManager::verificarTokenDB(sqlite3* db, const std::string& userid, const std::string& token) {
    // Consulta SQL para verificar se o token corresponde ao usu�rio
    std::string sql_verificar = "SELECT 1 FROM tokens WHERE userid = ? AND token = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql_verificar.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Erro ao preparar consulta de verifica��o: " << sqlite3_errmsg(db) << endl;
        return false; // Retorna false em caso de erro
    }

    sqlite3_bind_text(stmt, 1, userid.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, token.c_str(), -1, SQLITE_TRANSIENT);

    // Se a consulta retornar uma linha, o token � v�lido
    bool token_existe = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt); // Finaliza a consulta para liberar recursos

    return token_existe;
}

// Fun��o para revogar um token do banco de dados
bool DBManager::revogarTokenDB(sqlite3* db, const std::string& token) {
    std::string sql = "DELETE FROM tokens WHERE token = ?;";
    sqlite3_stmt* stmt;

    // Prepara a instru��o SQL
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erro ao preparar a consulta para remover token: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Vincula o valor do ID
    if (sqlite3_bind_text(stmt, 1, token.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        std::cerr << "Erro ao vincular o token: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Executa a instru��o
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Erro ao executar a revoga��o do token: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finaliza o statement
    sqlite3_finalize(stmt);
    return true;
}
