/**
 * User: Bruce Washington
 * Date: 23/3/20.
 *
 */

#include <GL/glew.h>
#include "SpriteRenderer.h"


Sprite_Renderer::Sprite_Renderer(Shader & shader)
{
    this->shader = shader;
    this->initRenderData();
}

Sprite_Renderer::~Sprite_Renderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}
void Sprite_Renderer::DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec4 Movement)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Prepare transformations
    this->shader.Use();
    glm :: mat4 model = glm :: mat4 (1.0);
    model = glm::translate(model, glm::vec3(position, 0.0f)); // First translate
    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    //model = glm::translate(model, glm::vec3((-0.5f * size.x-Movement.x)+Movement.y, (-0.5f * size.y-Movement.z)+Movement.w, 0.0f)); // Move origin back
    model = glm::scale(model, glm::vec3(size, 1.0f));
    this->shader.SetMatrix4("model", model);
    
	glActiveTexture(GL_TEXTURE0);
    texture->Bind();
	shader.SetInteger("image", 0);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite_Renderer::initRenderData()
{
    GLuint vbo;
    // layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
    GLfloat vertices[] = {
            // Positions	// Texture Coords
            0.0f, 1.0f,	0.0f, 1.0f,
            1.0f, 0.0f,	1.0f, 0.0f,
            0.0f, 0.0f,	0.0f, 0.0f,

            0.0f, 1.0f,	0.0f, 1.0f,
            1.0f, 1.0f,	1.0f, 1.0f,
            1.0f, 0.0f,	1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// Allocate data for the buffer. 4-channel 8-bit image
    //register the buffer object for access by CUDA
    glBindVertexArray(this->quadVAO);
    GLint attrib = glGetAttribLocation(this->shader.Program, "vertex");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
