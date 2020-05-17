/**
 * User: Bruce Washington
 * Date: 11/4/20.
 *
 */
#include "Event.h"
#include <algorithm>
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include <iostream>

EVTVideo::EVTVideo(GLuint width, GLuint height)
        : Width(width), Height(height) {

}
EVTVideo::~EVTVideo() {
    delete Renderer;
    delete Text;
    delete network;
    delete effect;
    direction = true;
    Movement = glm::vec3(Width/1.95, (double)Width,0);
    filename.clear();
    for (auto & i : data) {
        avformat_close_input(&i.fmt_ctx);
        videolayer->clearAppData(&i);
    }
    delete videolayer;
    timeframe=0;
    Videospeed= glm::vec3(100.0f,100.0f,100.0f);
    pts=0;
    delete VideoThread;
}

void EVTVideo::Init() {
    // Load shaders
    ResourceManager::LoadShader("/home/nirmalgajera/Documents/RenderVideo/sprite.vs",
                                "/home/nirmalgajera/Documents/RenderVideo/sprite.fs", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                      static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    //define video file name
    filename.emplace_back(
            "/home/nirmalgajera/Documents/Render Engine Requirement Examples/MatchSlide/Rugby World Cup/Background - Wales - Right.mov");
    filename.emplace_back(
            "/home/nirmalgajera/Documents/Render Engine Requirement Examples/MatchSlide/Rugby World Cup/Background - Australia - Left.mov");
    filename.emplace_back(
            "/home/nirmalgajera/Documents/Render Engine Requirement Examples/MatchSlide/Rugby World Cup/Rising Sun.mov");
    filename.emplace_back("/home/nirmalgajera/Documents/Render Engine Requirement Examples/MatchSlide/Rugby World Cup/Shield - Australia.mov");
    filename.emplace_back("/home/nirmalgajera/Documents/Render Engine Requirement Examples/MatchSlide/Rugby World Cup/Shield - Wales.mov");
    filename.emplace_back("/home/nirmalgajera/Documents/Render Engine Requirement Examples/MatchSlide/Rugby World Cup/Composite Example.mov");

    // Printing the vector
    for (auto it = filename.begin(); it != filename.end(); ++it)
        std::cout << ' ' << *it << std::endl;

    //video Threading
    videolayer=new VideoRender();
    videolayer1=new VideoRender();
    videolayer->initializeAppData(&data[0]);
    videolayer1->initializeAppData(&data[1]);

    videolayer->openVideoFrame(&data[0],filename[0]);
    videolayer1->openVideoFrame(&data[1],filename[1]);

    // Load up the vector with MyClass objects
   /* for (auto i = 0; i < 4; i++) {
        videovector = new VideoRender();
        videovector->initializeAppData(&data[i]);
        videovector->openVideoFrame(&data[i], filename[i]);
        videolayer.push_back(videovector);
    }*/

  // Load textures
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new Sprite_Renderer(myShader);
    if (!Renderer)
        std::cout << "Error" << std::endl;
    else {
        ResourceManager::LoadVideoTexture(this->Width,this->Height, "MatchSideBackGroundsRight");
    }
    // Set render-specific controls
    Text = new TextRenderer(this->Width, this->Height);
//    Text->Load("/home/nirmalgajera/Documents/Super/Font/demo.ttf", 50);
    auto ppef = ResourceManager::GetShader("postprocessing");
    ResourceManager::LoadShader("/home/nirmalgajera/Documents/RenderVideo/effect.vs",
                                "/home/nirmalgajera/Documents/RenderVideo/effect.fs", nullptr, "effect");
    effect = new PostProcessor(ppef, this->Width, this->Height);
    effect = new PostProcessor(ResourceManager::GetShader("effect"), this->Width, this->Height);

    /*  network = new JSONAPI();
      network->Start();*/
    // convert string to upper case
    // using transform() function and ::toupper in STL
    transform(display.begin(), display.end(), display.begin(), ::toupper);
    transform(display1.begin(), display1.end(), display1.begin(), ::toupper);

}

std::mutex m;
void EVTVideo::Render() {
   /* auto currentSuperFontData = network->getSuperFontDetails();
    for (std::vector<struct SuperFontData>::iterator it = currentSuperFontData.begin(); it != currentSuperFontData.end(); ++it) {
        std::string horsePosition = it->super_string;
    }*/
     effect->BeginRender();

    /*--------------------------------------------------------------------------*/
    static bool first_frame = true;
    if (first_frame) {
        glfwSetTime(0.0);
        first_frame = false;
    }
    /*--------------------------------------------------------------------------------------------------------*/
    //Background thread for Animation
    std::thread t1(&EVTVideo::updateRight,this);
    std::thread t2(&EVTVideo::updateleft,this);
    std::thread t3(&EVTVideo::updatebuttom,this);
    /*--------------------------------------------------------------------------------------------------------*/

    //left side Background
#pragma omp parallel
    videolayer->readFrame(&data[0]);
#pragma omp parallel
        Renderer->DrawSprite(*ResourceManager::GetTexture("MatchSideBackGroundsRight"),
                             glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f,
                             glm::vec4(Movement.x, this->Width, 0.0f, 0.0f)
        );
    /*--------------------------------------------------------------------------------------------------------
    //Right side Background
    videolayer1->readFrame(&data[1]);
    Renderer->DrawSprite(*ResourceManager::GetTexture("MatchSideBackGroundsRight"),
                         glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f,
                         glm::vec4(Movement.y,0.0f, 0.0f, 0.0f)
    );
    /*--------------------------------------------------------------------------------------------------------*/
    //frame timing


    t1.join();
    t2.join();
    t3.join();
    effect->EndRender();
}

void EVTVideo::updateRight() {
    if (direction) {
        //left side BackGround
        Movement.x += Videospeed.x *(GLfloat)1.5;
        if (Movement.x >= this->Width)
            Movement.x = this->Width;
    }
}

void EVTVideo::updatebuttom() {
    if (direction) {
        //Right Side BackGround
        Movement.y -= Videospeed.x*(GLfloat)2.7;
        if (Movement.y <= 0)
            Movement.y = 0;
    }
}

void EVTVideo::updateleft() {
    if (direction) {
    //Buttom Side BackGround
        Movement.z += Videospeed.x *(GLfloat)2;
        if (Movement.z >= this->Height)
            Movement.z = this->Height;
    }
}
