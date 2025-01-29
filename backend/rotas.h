#pragma once
#ifndef ITEM_ROUTES_H
#define ITEM_ROUTES_H

#include <crow.h>  // Biblioteca para framework Crow
#include "controlador.h"  // Inclui o controlador para interagir com o banco de dados e processar lógica de negócios
#include "produto.h"  // Inclui a definição da classe Produto
#include <iostream>  // Inclui biblioteca de entrada e saída padrão
#include "hashing.h"  // Inclui a classe de hashing para gerar e verificar hashes
#include "dbManager.h"  // Inclui a classe de gerenciamento do banco de dados

DBManager dbManage;  // Instancia o gerenciador do banco de dados
hashSimples hashing_;  // Instancia a classe para gerar hashes simples

// Função para resgatar o ID de usuário de uma requisição JSON
std::string resgatarID(crow::json::rvalue json) {
    if (json["userid"].t() != crow::json::type::Null) {
        return json["userid"].s();
    }
    return "";
}

// Função para verificar se o token fornecido na requisição é válido
bool verificarTokenRequest(const crow::request& req, sqlite3* db, std::string id_user = "") {
    std::string token = req.get_header_value("Authorization");  // Obtém o valor do token no cabeçalho da requisição (Authorization: Bearer <token>)
    auto jsonBody = crow::json::load(req.body);  // Converte o corpo da requisição para JSON
    std::string userid = "";

    // Se o token estiver vazio, retorna falso
    if (token.empty()) {
        return false;
    }

    // Se o ID de usuário não foi passado como parâmetro, tenta obter do corpo JSON
    if (id_user == "") {
        if (jsonBody["userid"].t() == crow::json::type::Null) {
            return false;
        }
        else {
            userid = jsonBody["userid"].s();
        }
    }
    else {
        userid = id_user;
    }

    // Verifica se o token é válido no banco de dados
    return dbManage.verificarTokenDB(db, userid, token);  // Retorna o resultado da verificação no banco
}

// Função para converter o corpo da requisição em um objeto Produto
Produto requestToProduto(const crow::request& req) {
    try {
        // Tenta fazer o parse do corpo da requisição para JSON
        auto jsonBody = crow::json::load(req.body);

        // Se houver a chave "id" no corpo da requisição, usa seu valor para atualizar o produto
        int idTemporario = 0;
        if (jsonBody.count("id") == 1) {
            idTemporario = jsonBody["id"].i();  // A chave "id" é convertida para inteiro
        }

        // Cria um novo objeto Produto com os dados extraídos do JSON
        return Produto(idTemporario, jsonBody["nome"].s(), jsonBody["quantidade"].i(), jsonBody["categoria"].s());
    }
    catch (const std::exception& e) {
        // Em caso de erro, imprime a exceção e retorna um Produto com valores de erro
        std::cout << e.what();
        return Produto(99999, "erro1", 99999, "erro1");
    }
}

// Função para registrar as rotas no servidor
void registrarRotas(crow::SimpleApp& app, sqlite3* db) {
    // Rota para retornar produtos por ID de usuário
    CROW_ROUTE(app, "/produtos/<string>").methods(crow::HTTPMethod::GET)(
        [db](const crow::request& req, const std::string& userid) {

            // Verifica se o token é válido antes de permitir o acesso à rota
            if (!verificarTokenRequest(req, db, userid)) {
                return crow::response("Token inválido");  // Se o token for inválido, retorna erro
            }

            // Retorna os produtos associados ao usuário
            return getProdutosID(db, userid);
        });

    // Rota para buscar produtos com base em uma consulta de string
    CROW_ROUTE(app, "/produtos/<string>/<string>").methods(crow::HTTPMethod::GET)(
        [db](const crow::request& req, const std::string& userid, const std::string& query) {

            // Verifica a validade do token antes de permitir a execução
            if (!verificarTokenRequest(req, db, userid)) {
                return crow::response("Token inválido");
            }

            // Retorna os produtos encontrados pela consulta
            return searchProdutosID(db, userid, query);
        });

    // Rota para adicionar um novo produto
    CROW_ROUTE(app, "/adicionar-produto").methods(crow::HTTPMethod::POST)([db](const crow::request& req) {
        if (!verificarTokenRequest(req, db)) {
            return crow::response("Token inválido");  // Verifica o token antes de continuar
        }
        std::string userid = resgatarID(crow::json::load(req.body));  // Resgata o ID de usuário
        return addProduto(db, requestToProduto(req), userid);  // Adiciona o produto ao banco
        });

    // Rota para remover um produto baseado no ID
    CROW_ROUTE(app, "/produtos/<int>").methods(crow::HTTPMethod::Delete)([db](const crow::request& req, int idRemovido) {
        if (!verificarTokenRequest(req, db)) {
            return crow::response("Token inválido");  // Verifica o token
        }
        std::string userid = resgatarID(crow::json::load(req.body));  // Resgata o ID de usuário
        return removerProduto(db, idRemovido, userid);  // Remove o produto
        });

    // Rota para atualizar um produto
    CROW_ROUTE(app, "/produtos").methods(crow::HTTPMethod::PUT)([db](const crow::request& req) {
        if (!verificarTokenRequest(req, db)) {
            return crow::response("Token inválido");  // Verifica o token
        }
        std::string userid = resgatarID(crow::json::load(req.body));  // Resgata o ID de usuário
        return atualizarProduto(db, requestToProduto(req), userid);  // Atualiza o produto
        });

    // Rotas para o gerenciamento de usuários
    // Rota para registrar um novo usuário
    CROW_ROUTE(app, "/registrar").methods(crow::HTTPMethod::POST)(
        [db](const crow::request& req) {
            auto jsonBody = crow::json::load(req.body);  // Converte o corpo para JSON
            std::string usuario = jsonBody["usuario"].s();  // Resgata o nome do usuário
            unsigned int senha = static_cast<unsigned int>(hashing_.simples_hash(jsonBody["senha"].s()));  // Cria um hash da senha

            return registrarUsuario(db, usuario, senha);  // Registra o usuário no banco de dados
        });

    // Rota para realizar o login do usuário
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)(
        [db](const crow::request& req) {
            auto jsonBody = crow::json::load(req.body);  // Converte o corpo para JSON
            std::string usuario = jsonBody["usuario"].s();  // Resgata o nome do usuário
            std::string senha = jsonBody["senha"].s();  // Resgata a senha fornecida

            return verificarLogin(db, usuario, senha);  // Verifica se a senha é válida e retorna o resultado
        });

    // Rota para deslogar o usuário
    CROW_ROUTE(app, "/deslogar").methods(crow::HTTPMethod::POST)(
        [db](const crow::request& req) {
            auto jsonBody = crow::json::load(req.body);  // Converte o corpo para JSON
            std::string token = jsonBody["token"].s();  // Resgata o token de logout
            return deslogarUsuario(db, token);  // Desloga o usuário utilizando o token
        });
}

#endif
