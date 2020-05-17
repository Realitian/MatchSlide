/**
 * User: Bruce Washington
 * Date: 31/3/20.
 *
 */

#ifndef JSONAPI_H
#define JSONAPI_H
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <GL/glew.h> // Include glew to get all the required OpenGL headers
#include <glm/glm.hpp>

struct SuperImageData{
    std:: string ImagePath;
    std:: string Filetype;

    struct SuperImageSize{
        GLfloat Img_Height;
        GLfloat Img_Width;
    }ImageSize;

    struct SuperImagePosition{
        GLfloat X_origin;
        GLfloat Y_origin;
        GLfloat Z_origin;
    }ImagePosition;

    struct SuperImage_End{
        GLfloat X_end;
        GLfloat Y_end;
        GLfloat Z_end;
    }Image_End;

    bool SuperImagRotation;

    struct SuperImgrotation{
        GLfloat X_Rot;
        GLfloat Y_Rot;
        GLfloat Z_Rot;
        std::string details;
    }rotation;

    bool SuperImgAnimation;
    struct SuperImganimation{
        GLfloat img_Speed;
        GLfloat img_delay;
        std::string details;
    }Imgaimagation;

    GLfloat Scale;
    GLfloat Opacity;
};

struct SuperFontData{
    std:: string super_string;
    std:: string super_Fontname;
    GLfloat font_size;

    struct SuperFontPosition{
        GLfloat X_origin;
        GLfloat Y_origin;
        GLfloat Z_origin;
    }FontPosition;

    struct SuperFontColor{
        GLfloat X;
        GLfloat Y;
        GLfloat Z;
    }FontColor;

    struct SuperFontRotation{
        GLfloat angle;
        std::string details;
    }rotation;

    bool SuperFontanimation;

    struct SuperFontanimation{
        GLfloat font_delay;
        std::string details;
    }Fontanimation;

    GLfloat Scale;
};

class JSONAPI {
public:
    JSONAPI();
    ~JSONAPI();
    void Start()
    {
        the_thread = std::thread(&JSONAPI::ThreadMain, this);
    }

    std::vector<struct SuperImageData>& getSuperImageDetails();
    std::vector<struct SuperFontData>& getSuperFontDetails();
    std::vector<struct demo>& getdemo();
private:
    std::thread the_thread;
    bool stop_thread = false;
    void ThreadMain();
};


#endif //RENDERIMAGE_JSONAPI_H
