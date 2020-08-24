#include "Window.hpp"
#include "../Log/Log.hpp"

#include <GLFW/glfw3.h>
#include <cassert>

namespace
{
	GLFWwindow* m_Window = nullptr;
}

bool Pine::Window::Internal::Create()
{
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(1024, 768, "Pine", nullptr, nullptr);
	if (m_Window == nullptr)
	{
		Log::Fatal("Failed to create window.");
		return false;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1); 
	
	return true;
}

void Pine::Window::Internal::Destroy()
{
	glfwDestroyWindow(m_Window);
}

GLFWwindow* Pine::Window::Internal::GetWindowPointer()
{
	return m_Window;
}

void Pine::Window::Show()
{
	assert(m_Window != nullptr);
	
	glfwShowWindow(m_Window);
}

void Pine::Window::Hide()
{
	assert(m_Window != nullptr);

	glfwHideWindow(m_Window);
}

void Pine::Window::SetPosition(int x, int y)
{
	glfwSetWindowPos(m_Window, x, y);
}

void Pine::Window::SetSize(int w, int h)
{
	glfwSetWindowSize(m_Window, w, h);
}

void Pine::Window::SetTitle(const std::string& str)
{
	glfwSetWindowTitle(m_Window, str.c_str());
}

glm::ivec2 Pine::Window::GetSize() {
	glm::ivec2 vec;

	glfwGetWindowSize(m_Window, &vec.x, &vec.y);

	return vec;
}