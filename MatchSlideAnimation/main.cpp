/**
 * User: Bruce Washington
 * Date: 23/3/20.
 *
 */

// Headers

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Event.h"
#include "ResourceManager.h"
#include <cstdio>
#include <cstring>

    int main(int argc, char **argv) {
        if (glfwInit() != GL_TRUE) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            return -1;
        }

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "SuperProvision", monitor, NULL);
        //GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", NULL, NULL);
        glfwMakeContextCurrent(window);
        // print out some info about the graphics drivers
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        // Init GLEW
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            return -1;
        }

        EVTVideo* evtVideo = new EVTVideo(mode->width, mode->height);
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, mode->width, mode->height);
       // Set OpenGL options
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // Initialize game
        evtVideo->Init();
        while (!glfwWindowShouldClose(window))
        {
            // Calculate delta time
            glfwPollEvents();
            // Render
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            evtVideo->Render();
            glfwSwapBuffers(window);
            glFlush();
        }
        ResourceManager::Clear();
        delete evtVideo;
        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return 0;
    }
