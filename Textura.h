#pragma once
#include <iostream>

class Textura
{
private:
    unsigned int m_id;
    int m_largura, m_altura, m_canais;

public:
    Textura();
    ~Textura();

    void load(std::string filePath);
    void Bind();
    void UnBind();
};