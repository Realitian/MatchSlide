/**
 * User: Bruce Washington
 * Date: 23/3/20.
 *
 */


#include "texture.h"


Texture::Texture()
        : Width(0), Height(0), Internal_Format(GL_RGBA), Image_Format(GL_RGBA),
          Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Wrap_R(GL_REPEAT),
          Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->TextureID);
}

Texture::Texture(GLuint format)
        : Width(0), Height(0), Internal_Format(GL_RGBA), Image_Format(format),
          Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Wrap_R(GL_REPEAT),
          Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->TextureID);
}
Texture::~Texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &this->TextureID);
}

void Texture::Generate(GLuint width, GLuint height, unsigned char* data)
{
    // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    // Load image, create texture and generate mipmaps
    //NULL means reserve texture memory, but texels are undefined
    glTexImage2D(GL_TEXTURE_2D, 0,
                 this->Internal_Format,
                 width, height, 0,
                 this->Image_Format,
                 GL_UNSIGNED_BYTE, (GLvoid *) data);
    // Set the texture wrapping & filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    // Unbind texture when done, so we won't accidentily mess up our texture.
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
void Texture::GenerateCubeMap(GLuint width, GLuint height,  unsigned char* data)
{
    // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Load image, create texture and generate mipmaps
    //NULL means reserve texture memory, but texels are undefined
    glTexImage2D(GL_TEXTURE_2D, 0,
                 this->Internal_Format,
                 width, height, 0,
                 this->Image_Format,
                 GL_UNSIGNED_BYTE, data);

    // Set the texture wrapping & filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // Unbind texture when done, so we won't accidentily mess up our texture.
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Update(GLuint width, GLuint height, const void* data)
{
    // XXX glTextureSubImage2D() requires GL4.5
    this->Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
        width/2, height/2,
        GL_RGBA, // this->Image_Format,
        GL_UNSIGNED_BYTE, data);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
}

void Texture::BindCubeMap() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->TextureID);
}

void Texture::GeneratevideoTexture(GLuint width, GLuint height){
	Width = width;
	Height = height;

// All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    //  // Set the texture wrapping & filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //NULL means reserve texture memory, but texels are undefined
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // Unbind texture when done, so we won't accidentily mess up our texture.
    glBindTexture(GL_TEXTURE_2D, 0);
}