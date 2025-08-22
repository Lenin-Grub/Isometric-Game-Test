#pragma once
#include <iostream>

class GLFWwindow;

struct Color
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;

	Color(float r, float g, float b, float a = 255.f)
		: r{ r / 255.f }, g{ g / 255.f }, b{ b / 255.f }, a{ a / 255.f }
	{
	}
};

class Window
{
public:
	Window();
	Window(const unsigned int widht, const unsigned int height, const std::string& title);
	~Window();

	bool create(const unsigned int widht, const unsigned int height, const std::string& title);
	bool close();

	void clear();
	void clear(const Color& color);

	void display();

	GLFWwindow* window = nullptr;

private:
	const std::string  m_title;
	const unsigned int m_widht;
	const unsigned int m_height;
};