#include "Input.hpp"

namespace smpl
{
    GLFWwindow* Input::m_window = nullptr;
    static glm::vec2 scroll_offset{ 0.0f };

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (!window)
            return;

        scroll_offset += glm::vec2(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }

    bool Input::init(smpl::Window& window)
    {
        m_window = &window.getWindow();

        if (!m_window)
        {
            LOG_CRITICAL("Failed init Input. Window is nullptr!");
            return false;
        }

        scroll_offset = glm::vec2(0.0f);

        glfwSetScrollCallback(m_window, scrollCallback);

        return true;
    }

    bool Input::isKeyPressed(smpl::Key::Code key)
    {
        return glfwGetKey(m_window, static_cast<int>(key)) == GLFW_PRESS;
    }

    bool Input::isKeyReleased(smpl::Key::Code key)
    {
        if (!m_window) return false;
        int  m_key = static_cast<int>(key);
        bool current = glfwGetKey(m_window, m_key) == GLFW_PRESS;
        bool previous = m_previous_state[m_key];
        m_previous_state[m_key] = current;
        return !current && previous;
    }

    bool Input::isMousePressed(smpl::Mouse::Button button)
    {
        return glfwGetMouseButton(m_window, static_cast<int>(button)) == GLFW_PRESS;
    }

    bool Input::isMouseReleased(smpl::Mouse::Button button)
    {
        if (!m_window) return false;
        int  m_button = static_cast<int>(button);
        bool current = glfwGetMouseButton(m_window, m_button) == GLFW_PRESS;
        bool previous = m_previous_state[m_button];
        m_previous_state[m_button] = current;
        return !current && previous;
    }

    bool Input::isMouseScrolled(smpl::Mouse::Scroll direction)
    {
        if (!m_window)
            return false;

        bool result = false;
        float& axis = (direction == smpl::Mouse::Scroll::Up || direction == smpl::Mouse::Scroll::Down) ? scroll_offset.y : scroll_offset.x;

        switch (direction)
        {
        case smpl::Mouse::Scroll::Up:
            if (axis > 0.0f)
            {
                axis = 0.0f;
                result = true;
            }
            break;
        case smpl::Mouse::Scroll::Down:
            if (axis < 0.0f)
            {
                axis = 0.0f;
                result = true;
            }
            break;
        case smpl::Mouse::Scroll::Left:
            if (scroll_offset.x < 0.0f)
            {
                scroll_offset.x = 0.0f;
                return true;
            }
            break;
        case smpl::Mouse::Scroll::Right:
            if (scroll_offset.x > 0.0f)
            {
                scroll_offset.x = 0.0f;
                return true;
            }
        default:
            scroll_offset.x = 0.0f;
            scroll_offset.y = 0.0f;
            return false;
            break;
        }
        return result;
    }

    const float Input::getScrollOffsetX()
    {
        if (!m_window)
            return 0.0f;

        float val = scroll_offset.x;
        scroll_offset.x = 0.0f;
        return val;
    }

    const float Input::getScrollOffsetY()
    {
        if (!m_window)
            return 0.0f;

        float val = scroll_offset.y;
        scroll_offset.y = 0.0f;
        return val;
    }
}