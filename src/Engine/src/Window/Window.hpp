#pragma once
#include <iostream>
#include <Graphics/Color/Color.hpp>

class GLFWwindow;

namespace smpl
{
    class Window
    {
    public:
        Window();
        Window(const unsigned int width, const unsigned int height, const std::string& title);
        ~Window();

        bool create(const unsigned int width, const unsigned int height, const std::string& title);
        bool close();

        void clear();
        void clear(const Color& color);

        void display();

        void processInput(GLFWwindow* window);

        GLFWwindow& getWindow() const;


    private:
        GLFWwindow* window = nullptr;
        const std::string  m_title;
        const unsigned int m_width;
        const unsigned int m_height;
    };
}