#ifndef ITEM_CONTROLLER_H
#define ITEM_CONTROLLER_H

#include "crow.h"
#include <string>
#include <sqlite3.h>
#include "produto.h"
crow::response getProdutos(sqlite3* db);
crow::response getProdutosID(sqlite3* db, const std::string& userid);
crow::response searchProdutosID(sqlite3* db, const std::string& userid, const std::string& query);
crow::response addProduto(sqlite3* db, const Produto& produto, const std::string& userid);
crow::response atualizarProduto(sqlite3* db, const Produto& produto, const std::string& userid);
crow::response removerProduto(sqlite3* db, int id, const std::string& userid);
crow::response registrarUsuario(sqlite3* db, const std::string& usuario, unsigned int senha_hash);
crow::response verificarLogin(sqlite3* db, const std::string& usuario, const std::string& senha);
crow::response deslogarUsuario(sqlite3* db, const std::string& token);
#endif
