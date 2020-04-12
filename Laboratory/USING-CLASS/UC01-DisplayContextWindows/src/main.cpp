#include <iostream>

#include "renderEngine/DisplayManager.h"


int main(int argc, char** argv){
	
	DisplayManager dm(800, 600, "First theme");
	dm.createDisplay();

	while (!dm.isClosed()) {

		// game logic
		// render geometry
		dm.updateDisplay();
	}

	dm.closeDisplay();

	return 0;
}
