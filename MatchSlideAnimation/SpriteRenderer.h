/**
 * User: Bruce Washington
 * Date: 23/3/20.
 *
 */

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class Sprite_Renderer
{
public:
    Sprite_Renderer(Shader &shader);
    ~Sprite_Renderer();
    void DrawSprite(Texture* texture, glm::vec2 position,
                    glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec4 Movement = glm::vec4(0.0f, 0.0f ,0.0f, 0.0f)); // initializes and configures the quad's buffer and vertex attributes renders a defined quad textured with given sprite
private:
    Shader shader;
    GLuint quadVAO;
    void initRenderData();
};

#endif