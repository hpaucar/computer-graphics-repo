#pragma once
 
#include "Scene.h"

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

	GLfloat* m_Vertices;
public:
	MyScene();
	void dynamicPaint(int currentTime);

	virtual ~MyScene();
	void update() override;
};
