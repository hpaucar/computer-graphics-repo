#include <iostream>
#include "Window.h"
#include "MyScene.h"
#include <memory>
#include <string>


int main(int argc, char** argv)
{	
	Window w(800, 600, "Introducao OpenGL - Design Code Play Repeat!");
	w.setScene(std::unique_ptr<MyScene>(new MyScene));
	
	while (!w.shouldClose())
	{
		w.updateWindow();
	}

	return 0;
}