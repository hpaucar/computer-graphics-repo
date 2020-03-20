#pragma once
 
#include "../core/Scene.h"

class MyScene : public Scene
{
private:
	GLuint m_VertexShader;
	GLuint m_FragmentShader;
	GLuint m_ShaderProgram;

	// VBO
	GLuint m_VBO;

	// VAO
	GLuint m_VAO;

	// Array and number of vertices
	GLfloat* m_Vertices;
	GLuint m_numberVertex;
public:
	MyScene();
	void dynamicPaint();

	virtual ~MyScene();
	void update() override;
};
