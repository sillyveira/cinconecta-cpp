#ifndef ITEM_H
#define ITEM_H

#include <string>

struct Produto {
    int id;
    std::string nome;
    int quantidade;
    std::string categoria;

    // Construtor
    Produto(int id_, const std::string& nome_, int quantidade_, const std::string& categoria_)
        : id(id_), nome(nome_), quantidade(quantidade_), categoria(categoria_) {
    }
};

#endif
