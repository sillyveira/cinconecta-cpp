#include <random>   // Biblioteca para gera��o de n�meros aleat�rios
#include <sstream>  // Biblioteca para manipula��o de streams de string
#include <string>   // Biblioteca para manipula��o de strings

// Fun��o para gerar um token aleat�rio de 32 caracteres hexadecimais
std::string gerarToken() {
    // Cria��o de um dispositivo de n�meros aleat�rios com base na entropia do sistema
    std::random_device rd;   // Gera uma semente para o gerador de n�meros aleat�rios
    std::mt19937 gen(rd());  // Mersenne Twister, um gerador de n�meros pseudo-aleat�rios
    std::uniform_int_distribution<> dis(0, 15); // Define uma distribui��o uniforme entre 0 e 15 (hexadecimais)

    std::stringstream token;  // StringStream para construir o token

    // Loop para gerar 32 caracteres hexadecimais
    for (int i = 0; i < 32; ++i) {
        token << std::hex << dis(gen);  // Gera um n�mero aleat�rio e converte para hexadecimal
    }

    // Retorna o token gerado como uma string
    return token.str();
}
