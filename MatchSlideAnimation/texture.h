/**
 * User: Bruce Washington
 * Date: 23/3/20.
 *
 */


#ifndef TEXTURE_H
#define TEXTURE_H

#include <Windows.h>
#include <GL/glew.h>
#include <vector>
// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture {
public:
    Texture();
    Texture(GLuint format);
    ~Texture();

    void Generate(GLuint width, GLuint height, unsigned char* data);
    void GenerateCubeMap(GLuint width, GLuint height,  unsigned char* data);
    void GeneratevideoTexture(GLuint width, GLuint height);
        
	void Update(GLuint width, GLuint height, const void* data);
    void Bind() const;
    void BindCubeMap() const;

    // Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    GLuint TextureID;
private:
    // Texture image dimensions
    GLuint Width, Height; // Width and height of loaded image in pixels
    // Texture Format
    GLuint Internal_Format; // Format of texture object
    GLuint Image_Format; // Format of loaded image
    // Texture configuration
    GLuint Wrap_S; // Wrapping mode on S axis
    GLuint Wrap_T; // Wrapping mode on T axis
    GLuint Wrap_R; // Wrapping mode on R axis
    GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
    GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
};

#endif
