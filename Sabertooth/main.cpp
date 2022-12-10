#include "System.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

int main() {

	System system;

	if (system.GLFWInit() != 0) {
		return EXIT_FAILURE;
	}
	if (system.SystemSetup() != 0) {
		return EXIT_FAILURE;
	}
	
	system.Run();

	system.Finish();

	return EXIT_SUCCESS;
}