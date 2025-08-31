#pragma once

#include <Graphics/Color/Color.hpp>
#include <Event/Event.hpp>
#include <queue>

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

        bool pollEvent(smpl::Event& event);
        bool isOpen();

        void clear();
        void clear(const Color& color);

        void display();

        //void processInput(GLFWwindow* window);

        GLFWwindow& getWindow() const;

    private:
        GLFWwindow* window = nullptr;
        const std::string  m_title;
        const unsigned int m_width;
        const unsigned int m_height;

        std::queue<smpl::Event> m_event_queue;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void windowCloseCallback(GLFWwindow* window);
        static void windowSizeCallback(GLFWwindow* window, int width, int height);
        static void scrollCallback(GLFWwindow* window, float xoffset, float yoffset);
    };
}