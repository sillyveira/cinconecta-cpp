#pragma once
#include <string>

// Classe para uma implementa��o simples de hash
class hashSimples {
public:
    // Fun��o de hash simples que gera um valor hash a partir de uma string
    unsigned int simples_hash(const std::string& input) {
        unsigned int hash = 0;  // Inicializa o valor do hash

        // Percorre cada caractere da string de entrada
        for (char c : input) {
            // Soma o valor ASCII de cada caractere multiplicado pelo primo 31
            hash += c * 31;  // O n�mero primo 31 ajuda a distribuir melhor os valores de hash
        }

        return hash;  // Retorna o valor calculado do hash
    }

    // Fun��o para verificar se o hash de uma senha corresponde ao hash fornecido
    bool verificar_hash(unsigned int hash, const std::string& senha) {
        // Calcula o hash da senha fornecida
        unsigned int hashVerificado = simples_hash(senha);

        // Compara o hash calculado com o hash fornecido
        if (hashVerificado == hash) {
            return true;  // Se os hashes coincidirem, a senha � v�lida
        }
        else {
            return false;  // Se n�o coincidirem, a senha est� incorreta
        }
    }
};
