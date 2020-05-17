/**
 * User: Bruce Washington
 * Date: 11/4/20.
 *
 */

#ifndef EVENT_H
#define EVENT_H
#pragma once

#include <vector>
#include <tuple>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "NetworkAPI/JSONAPI.h"
#include "VideoRender.h"
#include "Effects.h"
#include <vector>

class EVTVideo {
public:
    GLuint                 Width, Height;
    // Constructor/Destructor
    EVTVideo(GLuint width, GLuint height);
    ~EVTVideo();

    // Initialize game state (load all shaders/textures/levels)
    void Init();
    void Render();
    void updateRight();
    void updateleft();
    void updatebuttom();

private:
    TextRenderer      *Text;
    JSONAPI *network;
    PostProcessor *effect;
    bool direction = true;
    std::string display = "BoY, I Killed Many Who Were Deserving...";
    std::string display1="And Many Who Were Not.";

    //leftside background
    glm::vec3 Movement = glm::vec3((double)Width/1.95, (double)Width,0.0f);
    glm::vec3 Videospeed= glm::vec3(100.0f,100.0f,100.0f);
    //Match Slide
    AppData data[4];
    VideoRender * videolayer=NULL;
    VideoRender * videolayer1=NULL;
    Sprite_Renderer  *Renderer;
    // video file paths
    std::vector<std::string> filename;
    double timeframe=0.0;
    int64_t pts;
    std::thread * VideoThread= nullptr;

};


#endif //RENDERVIDEO_EVENT_H
