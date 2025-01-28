#pragma once  // Previne a inclus�o m�ltipla deste arquivo de cabe�alho

#include <iostream>  // Biblioteca para entrada e sa�da de dados
#include <chrono>    // Biblioteca para manipula��o de tempo e data
#include <ctime>     // Biblioteca para manipula��o de tempo
#include <iomanip>   // Biblioteca para formata��o de sa�da
#include <sstream>   // Biblioteca para manipula��o de streams de string

// Fun��o para obter a hora atual do sistema como uma string formatada
std::string obterHoraAtualComoString() {
    // Obtendo o tempo atual do sistema
    auto agora = std::chrono::system_clock::now();
    std::time_t tempo_atual = std::chrono::system_clock::to_time_t(agora);

    struct tm tempo_info;
    // Converte o tempo para a estrutura 'tm', que armazena informa��es sobre data e hora
    localtime_s(&tempo_info, &tempo_atual); // Uso seguro da fun��o localtime

    // Formatando a hora como string (ano-m�s-dia hora:minuto:segundo)
    std::ostringstream stream;
    stream << std::put_time(&tempo_info, "%Y-%m-%d %H:%M:%S");
    return stream.str();  // Retorna a hora atual como string formatada
}

// Fun��o para converter uma string no formato "ano-m�s-dia hora:minuto:segundo" para std::tm
std::tm stringParaTime(const std::string& data_str) {
    std::tm tm = {};  // Estrutura para armazenar informa��es de tempo
    std::istringstream ss(data_str);  // Cria um stream de entrada a partir da string de data
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");  // Converte a string para a estrutura tm com o formato adequado

    // Verifica se ocorreu algum erro na convers�o
    if (ss.fail()) {
        throw std::runtime_error("Erro ao converter string para time.");
    }
    return tm;  // Retorna a estrutura 'tm' com a data e hora convertidas
}

// Fun��o para comparar duas datas em formato string
bool compararDatas(const std::string& data1, const std::string& data2) {
    std::tm tm1 = stringParaTime(data1);  // Converte a primeira string para a estrutura tm
    std::tm tm2 = stringParaTime(data2);  // Converte a segunda string para a estrutura tm

    // Converte a estrutura tm para time_t, que � o n�mero de segundos desde 1970
    std::time_t tempo1 = std::mktime(&tm1);
    std::time_t tempo2 = std::mktime(&tm2);

    // Compara as duas datas: retorna true se data1 for maior ou igual a data2
    return tempo1 >= tempo2;
}
