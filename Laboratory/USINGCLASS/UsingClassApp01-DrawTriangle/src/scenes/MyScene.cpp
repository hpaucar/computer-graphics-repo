#include "MyScene.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <sstream>
#include "../core/Utils.h"

void MyScene::dynamicPaint(){
	glClearColor(0.5f, 0.5f, 1, 0);

	// Utils
	m_ShaderProgram = Utils::createShaderProgram("src/scenes/vertex.shader", "src/scenes/fragment.shader");

	/**************** Configuracao dos Dados **********************/
	// Prepara os dados para renderizar
	// Vertices	(X, Y, Z)	Cores (R, G, B)
	m_numberVertex = 18;
	m_Vertices = new GLfloat[18] {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

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
		18 * sizeof(GLfloat),	// tamanho do buffer
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
	dynamicPaint();
}

void MyScene::update(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Use este shader
	glUseProgram(m_ShaderProgram);
	// Use este VAO e suas configurações
	glBindVertexArray(m_VAO);	

	glDrawArrays(GL_TRIANGLE_FAN, 0, m_numberVertex);
	glBindVertexArray(0);
}

MyScene::~MyScene()
{
	// Libera memoria e deleta os objetos especificados por cada shader
	glDeleteProgram(m_ShaderProgram);
	delete[] m_Vertices;
}
