#include "MyScene.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <sstream>
#include "Util.h"

#include<time.h>

GLint checkShaderCompilation(GLuint shader)
{
	GLint success = 0;
	GLchar infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cerr << infoLog << std::endl;
	}

	return success;
}

GLint checkProgramLink(GLuint program)
{
	GLint success = 0;
	GLchar infoLog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
		std::cerr << "ERROR: PROGRAM LINKING FAILED:" << infoLog << std::endl;
	}

	return success;
}

void MyScene::dynamicPaint(int currentTime){
	glClearColor(0.5f, 0.5f, 1, 0);
		std::stringstream vertexSourceStream;
		std::stringstream fragmentSourceStream;


		LoadFile("src/vertex.shader", vertexSourceStream);
		LoadFile("src/fragment.shader", fragmentSourceStream);
		std::string vertexSource = vertexSourceStream.str();
		std::string fragmentSource = fragmentSourceStream.str();
		const char* buff;

		// Create shader Program
		m_ShaderProgram = glCreateProgram();

		// Vertex shader
		buff = vertexSource.c_str();
		m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_VertexShader, 1, &buff, NULL);
		glCompileShader(m_VertexShader);
		checkShaderCompilation(m_VertexShader);
		glAttachShader(m_ShaderProgram, m_VertexShader);
		glDeleteShader(m_VertexShader);

		// Fragment shader
		buff = fragmentSource.c_str();
		m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_FragmentShader, 1, &buff, NULL);
		glCompileShader(m_FragmentShader);
		checkShaderCompilation(m_FragmentShader);
		glAttachShader(m_ShaderProgram, m_FragmentShader);
		glDeleteShader(m_FragmentShader);

		// Link shader program
		glLinkProgram(m_ShaderProgram);
		checkProgramLink(m_ShaderProgram);
		/***************************************************************/
		srand(time(0));
		int n1 = rand()%100+3;
		std::cout<<"N1: "<<n1<<std::endl;
		m_Vertices = new GLfloat[n1*6];
		for (int i = 0; i < n1*6; ++i) {
			m_Vertices[i] = (rand()%101)/100.0;
		}


		/*************************************** Configuracao dos Dados **********************************/
		// Prepara os dados para renderizar
		// Vertices	(X, Y, Z)	Cores (R, G, B)
	/*	float temp = (currentTime%2==0)?1.0:0.0;
		std::cout<<" TEMP color: "<<temp<<std::endl;
		m_Vertices = new GLfloat[18] {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.0f, 0.0f, 1.0f, temp,
				0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		};
*/
		// Cria um ID na GPU para nosso buffer
		glGenBuffers(1, &m_VBO);
		// Cria um ID na GPU para um array de  buffers
		glGenVertexArrays(1, &m_VAO);


		glBindVertexArray(m_VAO);

		// https://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
		// Associa esse ID a um TARGET cujo proposito e armazenar Vertex attributes
		// GL_ARRAY_BUFFER = Vertex attributes
		// Quaisquer operações nesse TARGET a este bufferID.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		// Reserva memoria na GPU para um TARGET receber dados
		// Copia esses dados pra essa área de memoria
		glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			n1 * sizeof(GLfloat),	// tamanho do buffer
			m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW		// Política de acesso aos dados, para otimização
			);

		// Nesse ponto, ja copiamos nossos dados pra GPU.
		// Mas, OpenGL nao faz ideia do que eles representam
		// Sao 6 vértices ? 3 vérices e 3 cores ?
		// Os vertices vem antes ou depois das cores ?
		// Ou estão intercalados vértices depois cores, depois vertices, depois cores novamente....
		// Precisamos dizer com nossos dados precisam ser interpretados:
		glVertexAttribPointer(
			0,			// Lembra do (layout = 0 ) no vertex shader ? Esse valor indica qual atributo estamos indicando
			3,			// cada vertice é composto de 3 valores
			GL_FLOAT,	// cada valor do vértice é do tipo GLfloat
			GL_FALSE,	// Quer normalizar os dados e converter tudo pra NDC ? ( no nosso caso, já esta tudo correto, então deixamos como FALSE)
			6 * sizeof(GLfloat),	// De quantos em quantos bytes, este atributo é encontrado no buffer ? No nosso caso 3 floats pros vertices + 3 floats pra cor = 6 floats
			(GLvoid*)0	// Onde está o primeiro valor deste atributo no buffer. Nesse caso, está no início do buffer
			);

		glEnableVertexAttribArray(0);			// Habilita este atributo

		// Faremos a mesma coisa pra cor de cada vértice

		glVertexAttribPointer(
			1,			// Lembra do (layout = 0 ) no vertex shader ? Esse valor indica qual atributo estamos indicando
			3,			// cada vertice é composto de 3 valores
			GL_FLOAT,	// cada valor do vértice é do tipo GLfloat
			GL_FALSE,	// Quer normalizar os dados e converter tudo pra NDC ? ( no nosso caso, já esta tudo correto, então deixamos como FALSE)
			6 * sizeof(GLfloat),	// De quantos em quantos bytes, este atributo é encontrado no buffer ? No nosso caso 3 floats pros vertices + 3 floats pra cor = 6 floats
			(GLvoid*)(3 * sizeof(GLfloat))	// Onde está o primeiro valor deste atributo no buffer. Nesse caso, 3 floats após o início do buffer
			);


		glEnableVertexAttribArray(1);		// Habilita este atributo
		glBindVertexArray(0);
}
MyScene::MyScene()
{
	//dynamicPaint();
}

void MyScene::update()
{
	srand(time(0));
	dynamicPaint(rand());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Use este shader
	glUseProgram(m_ShaderProgram);

	// Use este VAO e suas configurações
	glBindVertexArray(m_VAO);	
	//glPointSize(10.0f);
	glDrawArrays(GL_TRIANGLE_FAN, 0, (int)m_Vertices);
	glBindVertexArray(0);
}

MyScene::~MyScene()
{
	// Libera memoria e deleta os objetos especificados por cada shader

	glDeleteProgram(m_ShaderProgram);
	delete[] m_Vertices;
}
