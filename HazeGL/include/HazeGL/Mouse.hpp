#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdint.h>


namespace HazeGL {
    class Window;

    class Mouse {
    public:
        void getPosition(float*, float*);
        void getPosition(double*, double*);
        void getPosition(uint32_t*, uint32_t*);

        bool isInScreen() const;
    private:
        Mouse(const HazeGL::Window*);
        double mLastX;
        double mLastY;
        // not ownwer
        const HazeGL::Window* mWindow;
    friend class HazeGL::Window;
    };
}