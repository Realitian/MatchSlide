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
        : Width(width), Height(height),
	network(0){	

}
EVTVideo::~EVTVideo() {
    delete Renderer;
    //delete Text;
	if (network)
		delete network;
    //delete effect;
    direction = true;
    Movement = glm::vec3(Width/1.95, (double)Width,0);
    filename.clear();
    
	for ( int i = 0 ; i < 5 ; i++ ) {
        avformat_close_input( &data[i].fmt_ctx );
		videolayers[i]->clearAppData(&data[i]);
		delete videolayers[i];
	}
    timeframe=0;
    Videospeed= glm::vec3(100.0f,100.0f,100.0f);
    pts=0;
    delete VideoThread;
}

void EVTVideo::Init() {
    // Load shaders
    ResourceManager::LoadShader("D:\\8\\MatchSlide\\sprite.vs",
                                "D:\\8\\MatchSlide\\sprite.fs", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                      static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use();
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    //define video file name
    filename.emplace_back("D:/8/V_S/Background - Australia - Left.mov");
	filename.emplace_back("D:/8/V_S/Background - Wales - Right.mov");
	filename.emplace_back("D:/8/V_S/Rising Sun.mov");
	filename.emplace_back("D:/8/V_S/Shield - Australia.mov");
	filename.emplace_back("D:/8/V_S/Shield - Wales.mov");

	Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new Sprite_Renderer(myShader);

	for (int i = 0; i < 5; i++) {
		videolayers[i] = new VideoRender();
		videolayers[i]->initializeAppData(&data[i]);
		videolayers[i]->openVideoFrame(&data[i],filename[i]);
		ResourceManager::LoadVideoTexture(this->Width,this->Height, filename[i]);
	}

	// Load textures
    
    if (!Renderer)
        std::cout << "Error" << std::endl;
    else {
    }

#if 0
	// Set render-specific controls
    Text = new TextRenderer(this->Width, this->Height);
//    Text->Load("/home/nirmalgajera/Documents/Super/Font/demo.ttf", 50);
	auto ppef = ResourceManager::GetShader("postprocessing");
	ResourceManager::LoadShader("D:\\8\\MatchSlide\\effect.vs",
                                "D:\\8\\MatchSlide\\effect.fs", nullptr, "effect");
	
	effect = new PostProcessor(ppef, this->Width, this->Height);
	effect = new PostProcessor(ResourceManager::GetShader("effect"), this->Width, this->Height);

    /*  network = new JSONAPI();
      network->Start();*/
    // convert string to upper case
    // using transform() function and ::toupper in STL
    transform(display.begin(), display.end(), display.begin(), ::toupper);
    transform(display1.begin(), display1.end(), display1.begin(), ::toupper);
#endif
}

std::mutex m;
void EVTVideo::Render() {
#if 0
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
    //std::thread t1(&EVTVideo::updateRight,this);
    //std::thread t2(&EVTVideo::updateleft,this);
    //std::thread t3(&EVTVideo::updatebuttom,this);
    /*--------------------------------------------------------------------------------------------------------*/
#endif

	for (int i = 0; i < 5; i++) {
		Texture* texture = ResourceManager::GetTexture(filename[i]);
		videolayers[i]->readFrame(&data[i], texture);

		Renderer->DrawSprite(texture,
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f,
			glm::vec4(0, 0, 0.0f, 0.0f)
		);
	}

#if 0
    /*--------------------------------------------------------------------------------------------------------
    //Right side Background
    videolayer1->readFrame(&data[1]);
    Renderer->DrawSprite(*ResourceManager::GetTexture("MatchSideBackGroundsRight"),
                         glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f,
                         glm::vec4(Movement.y,0.0f, 0.0f, 0.0f)
    );
    /*--------------------------------------------------------------------------------------------------------*/
    //frame timing


    //t1.join();
    //t2.join();
    //t3.join();
    effect->EndRender();
#endif
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
