#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <string>

#include <stdint.h>

namespace HazeGL {

    class Window {
    public:
        Window(uint32_t width, uint32_t height, std::string title);

        void initialize(uint32_t version=450, uint32_t profile=GLFW_OPENGL_CORE_PROFILE);
        void initializeGLFW(uint32_t version=450, uint32_t profile=GLFW_OPENGL_CORE_PROFILE);
        void initializeWindow();
        void createGLContext();
        void initializeGLAD();

        void setDebug(bool);

        void enableBlending();

        void getMousePosition(float*, float*);
        void getMousePosition(double*, double*);
        void getMousePosition(uint32_t*, uint32_t*);

        bool shouldClose() const;
        void swapBuffers() const;

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        std::string getTitle() const;
        GLFWwindow* getWindow() const;
    private:
        GLFWwindow* mWindow;
        uint32_t mWidth;
        uint32_t mHeight;
        std::string mTitle;
        bool mDebug;
    };
}