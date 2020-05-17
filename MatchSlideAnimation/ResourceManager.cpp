/**
 * User: Bruce Washington
 * Date: 23/3/20.
 *
 */


#define STB_IMAGE_IMPLEMENTATION
#include "ResourceManager.h"
#include <iostream>
#include <map>
#include <string>
#include "basicimage.h"
#include "stb_image.h"
// Instantiate static variables
std::map<std::string, Shader>       ResourceManager::Shaders;
std::map<std::string, Texture *>    ResourceManager::Textures;



Shader ResourceManager::LoadShader(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture * ResourceManager::LoadTexture(const GLchar * file, std::string name)
{
    Textures[name] = loadTextureFromFile(file);
    return Textures[name];
}

Texture * ResourceManager::LoadVideoTexture(GLuint width, GLuint height, std::string name)
{
    Textures[name] = loadvidefile(width,height);
    return Textures[name];
}

Texture * ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.Program);
    // (Properly) delete all textures
    for (auto iter : Textures)
        delete(iter.second);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture * ResourceManager::loadTextureFromFile(const GLchar * file) {
    Texture * texture = new Texture(GL_BGRA);

    std::vector<RGBA> pix;
    std::uint32_t w;
    std::uint32_t h;
    std::uint16_t bpp;

    Helper::BasicImage img;
    try {

        img.loadimage(file, pix, w, h, bpp);
        texture->Generate(w, h, (unsigned char *)pix.data()); // XXX use (GLvoid *)
    } catch (std::exception &e) {
       std::cout<<"Failed to load sprite asset with filename={0}"<< file<<std::endl;
    }
    return texture;
/*
    Texture texture;

    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;

    int width, height, nrComponents;
    unsigned char *data = stbi_load(file, &width, &height, &nrComponents, 0);
    if (data) {
        texture.Generate(width, height, data);
        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << file << std::endl;
        stbi_image_free(data);
    }
    return texture;*/
}

Texture * ResourceManager::loadvidefile(GLuint width, GLuint height) {
   Texture * texture = new Texture(GL_BGRA);

    try {
        texture->GeneratevideoTexture( width,  height); // XXX use (GLvoid *)
    } catch (std::exception &e) {
        std::cout<<"Failed to load sprite asset with filename={0}"<< texture->TextureID<<std::endl;
    }
    return texture;
}
