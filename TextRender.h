#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include <ResourceHandle.h>
#include <ShaderVision.h>
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // size of glyph
    glm::ivec2   Bearing;   // offset from baseline to left/top of glyph
    unsigned int Advance;   // horizontal offset to advance to next glyph
};

class TextRender {
public:
    TextRender();
    void PrepareText();
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
    void PrepareTextVS();
    void RenderTextVS(std::string text, float x, float y, float scale, glm::vec3 color);
    void DeleteBuffer();

    std::map<char, Character> Characters;
    Shader TextShader;
    ShaderVision TS;

private:

    unsigned int VAO, VBO;
};



#endif 
