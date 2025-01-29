#include "controlador.h"
#include "crow.h"
#include <vector>
#include <string>
#include "dbManager.h"
#include "produto.h"
#include "hashing.h"

using namespace std;

hashSimples hashing;  // Inst�ncia de hashing para valida��o de senhas
DBManager dbManagement;  // Inst�ncia para interagir com o banco de dados

// Fun��o para gerar uma resposta padr�o no formato JSON
crow::response criarResposta(int status, const std::string& mensagem, crow::json::wvalue dados = {}) {
    crow::json::wvalue responseBody;

    // Define se o status � sucesso ou erro com base no c�digo de status HTTP
    responseBody["status"] = (status >= 200 && status < 300) ? "sucesso" : "erro";
    responseBody["mensagem"] = mensagem;

    // Se houver dados, adiciona-os � resposta
    if (!dados.dump().empty()) {
        responseBody["dados"] = std::move(dados);
    }

    // Cria uma resposta HTTP com o status e corpo em JSON
    crow::response res{ status };
    res.set_header("Content-Type", "application/json");
    res.body = responseBody.dump();  // Converte o corpo para formato JSON

    return res;
}

// Fun��o para obter a lista de produtos de um usu�rio espec�fico, dado o seu ID
crow::response getProdutosID(sqlite3* db, const std::string& userid) {
    try {
        vector<Produto> listaProdutos = dbManagement.getProdutosID(db, userid);
        crow::json::wvalue listaJson;
        listaJson = crow::json::wvalue::list();  // Cria uma lista vazia no formato JSON
        std::vector<crow::json::wvalue> produtosJson;

        // Para cada produto da lista, converte os dados para JSON
        for (const auto& produto : listaProdutos) {
            crow::json::wvalue produtoJson;
            produtoJson["id"] = produto.id;
            produtoJson["nome"] = produto.nome;
            produtoJson["quantidade"] = produto.quantidade;
            produtoJson["categoria"] = produto.categoria;

            produtosJson.push_back(move(produtoJson));  // Adiciona o produto na lista
        }

        listaJson["corpo"] = move(produtosJson);  // Atribui a lista de produtos � chave "corpo"

        return crow::response(listaJson);  // Retorna a resposta com a lista de produtos em JSON
    }
    catch (const std::exception& e) {
        string erro = "Deu errado: ";
        erro += e.what();
        return crow::response(erro);  // Retorna uma resposta de erro caso ocorra uma exce��o
    }
}

// Fun��o para buscar produtos de um usu�rio por um crit�rio de busca
crow::response searchProdutosID(sqlite3* db, const std::string& userid, const std::string& query) {
    try {
        vector<Produto> listaProdutos = dbManagement.searchProdutosID(db, userid, query);
        crow::json::wvalue listaJson;
        listaJson = crow::json::wvalue::list();
        std::vector<crow::json::wvalue> produtosJson;

        // Converte cada produto da lista em formato JSON
        for (const auto& produto : listaProdutos) {
            crow::json::wvalue produtoJson;
            produtoJson["id"] = produto.id;
            produtoJson["nome"] = produto.nome;
            produtoJson["quantidade"] = produto.quantidade;
            produtoJson["categoria"] = produto.categoria;

            produtosJson.push_back(move(produtoJson));
        }

        listaJson["corpo"] = move(produtosJson);  // Adiciona a lista de produtos ao JSON

        return crow::response(listaJson);  // Retorna a resposta com a lista de produtos encontrados
    }
    catch (const std::exception& e) {
        string erro = "Deu errado: ";
        erro += e.what();
        return crow::response(erro);  // Retorna erro se ocorrer uma exce��o
    }
}

// Fun��o para obter todos os produtos do banco de dados
crow::response getProdutos(sqlite3* db) {
    try {
        vector<Produto> listaProdutos = dbManagement.getProdutos(db);
        crow::json::wvalue listaJson;
        listaJson = crow::json::wvalue::list();
        std::vector<crow::json::wvalue> produtosJson;

        // Converte cada produto em formato JSON
        for (const auto& produto : listaProdutos) {
            crow::json::wvalue produtoJson;
            produtoJson["id"] = produto.id;
            produtoJson["nome"] = produto.nome;
            produtoJson["quantidade"] = produto.quantidade;
            produtoJson["categoria"] = produto.categoria;

            produtosJson.push_back(move(produtoJson));  // Adiciona o produto � lista JSON
        }

        listaJson["corpo"] = move(produtosJson);  // Adiciona a lista de produtos ao JSON

        return criarResposta(200, "Sucesso", listaJson);  // Retorna sucesso e a lista de produtos
    }
    catch (const std::exception& e) {
        string erro = "Deu errado: ";
        erro += e.what();
        return crow::response(erro);  // Retorna erro caso ocorra uma exce��o
    }
}

// Fun��o para adicionar um novo produto ao banco de dados
crow::response addProduto(sqlite3* db, const Produto& produto, const std::string& userid) {
    try {
        dbManagement.addProduto(db, produto, userid);  // Chama o m�todo para adicionar o produto
        return criarResposta(200, "O produto foi adicionado.");  // Retorna sucesso
    }
    catch (const std::exception& e) {
        string erro = "Deu errado ";
        erro += e.what();
        return crow::response(erro);  // Retorna erro caso algo d� errado
    }
}

// Fun��o para remover um produto do banco de dados
crow::response removerProduto(sqlite3* db, int id, const std::string& userid) {
    try {
        dbManagement.removerProduto(db, id, userid);  // Chama o m�todo para remover o produto
        return criarResposta(200, "O produto foi removido.");  // Retorna sucesso
    }
    catch (const std::exception& e) {
        string erro = "Deu errado ";
        erro += e.what();
        return crow::response(erro);  // Retorna erro caso algo d� errado
    }
}

// Fun��o para atualizar um produto no banco de dados
crow::response atualizarProduto(sqlite3* db, const Produto& produto, const std::string& userid) {
    try {
        dbManagement.atualizarProduto(db, produto, userid);  // Chama o m�todo para atualizar o produto
        return criarResposta(200, "O produto foi atualizado.");  // Retorna sucesso
    }
    catch (const std::exception& e) {
        string erro = "Deu errado ";
        erro += e.what();
        return crow::response(erro);  // Retorna erro caso algo d� errado
    }
}

// Fun��o para registrar um novo usu�rio
crow::response registrarUsuario(sqlite3* db, const std::string& usuario, unsigned int senha_hash) {
    try {
        if (dbManagement.registrarUsuario(db, usuario, senha_hash)) {
            return criarResposta(200, "O usu�rio foi registrado.");  // Retorna sucesso se o usu�rio foi registrado
        }
        else {
            return criarResposta(500, "Um erro aconteceu. O usu�rio j� deve existir.");  // Retorna erro se o usu�rio j� existir
        }
    }
    catch (const std::exception& e) {
        string erro = "Deu errado ";
        erro += e.what();
        return criarResposta(500, erro);  // Retorna erro se ocorrer uma exce��o
    }
}

// Fun��o para verificar o login de um usu�rio
crow::response verificarLogin(sqlite3* db, const std::string& usuario, const std::string& senha) {
    try {
        // Busca o hash da senha do usu�rio no banco
        std::vector<std::string> listaUseridHash = dbManagement.verificarUsuarioSenha(db, usuario);
        std::string hash_str = "";
        std::string userid = "";

        // Se o usu�rio foi encontrado, recupera o hash e o ID
        if (!listaUseridHash.empty()) {
            hash_str = listaUseridHash[0];
            userid = listaUseridHash[1];
        }

        // Se o hash estiver vazio, significa que o usu�rio n�o foi encontrado
        if (hash_str.empty()) {
            return criarResposta(500, "Usu�rio n�o encontrado");
        }

        unsigned int hash_uint = std::stoul(hash_str);

        // Verifica se a senha fornecida corresponde ao hash armazenado
        if (hashing.verificar_hash(hash_uint, senha)) {
            // Gera um token para o login
            string loginToken = dbManagement.gerarTokenDB(db, userid);

            crow::json::wvalue loginInfo;
            loginInfo["token"] = loginToken;
            loginInfo["userid"] = userid;
            return criarResposta(200, "Sucesso", loginInfo);  // Retorna o token e o ID do usu�rio
        }
        else {
            return criarResposta(500, "Senha incorreta.");  // Retorna erro se a senha estiver incorreta
        }
    }
    catch (const std::exception& e) {
        string erro = "Erro ao verificar login: ";
        erro += e.what();
        return criarResposta(500, erro);  // Retorna erro em caso de exce��o
    }
}

// Fun��o para deslogar um usu�rio
crow::response deslogarUsuario(sqlite3* db, const std::string& token) {
    try {
        dbManagement.revogarTokenDB(db, token);  // Revoga o token do usu�rio
        return criarResposta(200, "O usu�rio foi deslogado com sucesso.");  // Retorna sucesso
    }
    catch (const std::exception& e) {
        return criarResposta(500, "N�o foi poss�vel deslogar o usu�rio.");  // Retorna erro caso falhe
    }
}
