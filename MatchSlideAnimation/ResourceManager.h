/**
 * User: Bruce Washington
 * Date: 23/3/20.
 *
 */


#pragma once

#include <map>
#include <string>

#include <GL/glew.h>



#include "texture.h"
#include "shader.h"


// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager
{
public:
    // Resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture *> Textures;
    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
    // Retrieves a stored sader
    static Shader GetShader(std::string name);
    // Loads (and generates) a texture from file
    static Texture * LoadTexture(const GLchar * file, std::string name);
    static Texture * LoadVideoTexture(GLuint width, GLuint height, std::string name);
    // Retrieves a stored texture
    static Texture * GetTexture(std::string name);
    // Properly de-allocates all loaded resources


    static void Clear();
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager();

    static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile);
      // Loads a single texture from file
      // Loads a single texture from file
    static Texture * loadTextureFromFile(const GLchar *file);
    static Texture * loadvidefile(GLuint width, GLuint height);
};


