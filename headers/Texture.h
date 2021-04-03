#pragma once
#include <iostream>

// Header for Texture class (note: commetns about implementation are in the cpp file)
class Texture
{
private:
    unsigned int m_id;
    int m_width, m_height, m_channels;

public:
    Texture();
    ~Texture();

    void load(std::string filePath);
    void Bind();
    void UnBind();
};