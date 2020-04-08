#include "Window.h"
#include <iostream>
#include "Scene.h"

Window::Window(int width, int height , const char* title)
{
	m_ShouldClose = true;

	// inicializa a biblioca GLFW
	if (!glfwInit())
	{
		std::cerr << "Erro inicializando GLFW" << std::endl;
		return;
	}

	// Definicao dos atributos do contexto OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	glfwSetErrorCallback(Window::errorCallback);

	// Criar a janela
	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!m_Window)
	{
		std::cerr << "Erro criando janela" << std::endl;
		return;
	}

	// Torna o contexto atual
	glfwMakeContextCurrent(m_Window);

	glewExperimental = GL_TRUE;

	GLenum glewInitErr = glewInit();

	if (glewInitErr != GLEW_OK)
	{
		std::cerr << "Erro inicializando GLEW: " << glewGetErrorString(glewInitErr) << std::endl;
		return;
	}

	// Mostra dados do ambiente
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderizador: " << renderer << std::endl;
	std::cout << "Versao do opengl suportada: " << version << std::endl;

	m_ShouldClose = false;
}

void Window::updateWindow(){
	if (m_Scene)
		m_Scene->update();
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
	//m_ShouldClose = (glfwWindowShouldClose(m_Window)) ? GL_TRUE : GL_FALSE;
	if (!m_ShouldClose)
		m_ShouldClose = glfwWindowShouldClose(m_Window);// == GL_TRUE;
}

void Window::startWindow(){
	while (!glfwWindowShouldClose(m_Window)) {
		m_Scene->update();
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

Window::~Window(){
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::errorCallback(int error, const char* description){
	std::cerr << "Erro " << error << ": " << description << std::endl;
}
