#include <iostream>
#include "core/Window.h"
#include "scenes/MyScene.h"
#include <memory>
#include <string>


int main(int argc, char** argv)
{	
	Window w(800, 800, "UCApp 01: Display Triangle");
	w.setScene(std::unique_ptr<MyScene>(new MyScene));
	
	w.startWindow();

/*
	while (!w.shouldClose())
	{
		w.update();
	}
*/
	return 0;
}
