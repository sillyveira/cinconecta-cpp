#include <random>   // Biblioteca para geração de números aleatórios
#include <sstream>  // Biblioteca para manipulação de streams de string
#include <string>   // Biblioteca para manipulação de strings

// Função para gerar um token aleatório de 32 caracteres hexadecimais
std::string gerarToken() {
    // Criação de um dispositivo de números aleatórios com base na entropia do sistema
    std::random_device rd;   // Gera uma semente para o gerador de números aleatórios
    std::mt19937 gen(rd());  // Mersenne Twister, um gerador de números pseudo-aleatórios
    std::uniform_int_distribution<> dis(0, 15); // Define uma distribuição uniforme entre 0 e 15 (hexadecimais)

    std::stringstream token;  // StringStream para construir o token

    // Loop para gerar 32 caracteres hexadecimais
    for (int i = 0; i < 32; ++i) {
        token << std::hex << dis(gen);  // Gera um número aleatório e converte para hexadecimal
    }

    // Retorna o token gerado como uma string
    return token.str();
}
