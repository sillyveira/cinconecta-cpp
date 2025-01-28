#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <iostream>
#include "produto.h"

class DBManager {
public:
    //método para iniciar o banco de dados
    static void iniciarBanco(const std::string& nome_do_banco, sqlite3** db);

    //métodos CRUD
    static std::vector<Produto> getProdutos(sqlite3* db);
    std::vector<Produto> getProdutosID(sqlite3* db, const std::string& userid);
    std::vector<Produto> searchProdutosID(sqlite3* db, const std::string& userid, const std::string& query);
    static bool addProduto(sqlite3* db, const Produto& item, const std::string& userid);
    static bool atualizarProduto(sqlite3* db, const Produto& produto, const std::string& userid);
    static bool removerProduto(sqlite3* db, int id, const std::string& userid);
    
    //métodos para usuários
    static bool registrarUsuario(sqlite3* db, const std::string& usuario, unsigned int senha_hash);
    std::string getUserID(sqlite3* db, const std::string& usuario);
    std::vector<std::string> verificarUsuarioSenha(sqlite3* db, const std::string& usuario);
    
    //métodos para tokens
    std::string gerarTokenDB(sqlite3* db, const std::string& usuario);
    static bool verificarTokenDB(sqlite3* db, const std::string& userid, const std::string& token);
    static bool revogarTokenDB(sqlite3* db, const std::string& token);
};

#endif // DBMANAGER_H
