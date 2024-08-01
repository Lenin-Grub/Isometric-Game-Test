#pragma once

class GLFWwindow;

class Window
{
public:
	Window();
	Window(const unsigned int widht, const unsigned int height, const std::string title);
	~Window();

	bool create(const unsigned int widht, const unsigned int height, const std::string title);
	bool close();
	void clear();

	GLFWwindow* m_window;
private:

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
	const std::string  m_title;
	const unsigned int m_widht;
	const unsigned int m_height;
};