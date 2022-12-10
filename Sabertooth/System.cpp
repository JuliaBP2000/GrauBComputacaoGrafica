#include "System.h"


double x = 0.0, y = 0.0;
bool create = false;
bool draw = false;

System::System()
{
}
System::~System()
{
}

int System::GLFWInit()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//cria janela
	window = glfwCreateWindow(WIDTH, HEIGHT, "Editor de Curva", nullptr, nullptr);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//verifica se é criada
	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

		
	//inicia a glew
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	glfwSetMouseButtonCallback(window, MouseEvent);

	return EXIT_SUCCESS;

}

int System::SystemSetup()
{
	//add shader
	Shader* shader = new Shader("Shaders/Core/vertexShader.vert", "Shaders/Core/fragmentShader.frag");
	shaders.push_back(shader);	

	//create obj reference
	Object* obj = new Object();
	objects.push_back(obj);
	objects[objects.size() - 1]->setShader(shaders[shaders.size() - 1]);

	//make mtl
	MTLWriter MTLWriter;
	MTLWriter.createMtlFile();

	//make obj
	OBJWriter OBJWriter;
	OBJWriter.createOBJFile();
	
	return EXIT_SUCCESS;
}

void System::Run()
{
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		update();

		render();

		glfwSwapBuffers(window);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		shaders[i]->Delete();;
	}

}

void System::Finish()
{
	glfwTerminate();
}

void System::MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

			double xpos, ypos;

			glfwGetCursorPos(window, &xpos, &ypos);

			x = xpos;
			y = ypos;

			//td::cout << "Pontos antes divisão:" + (int)x + (int)y << std::endl;
			
			// printf("[%f , %f] ", x, y);

		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			create = true;
		}
}

void System::update()
{
	if (x != 0.0 || y != 0.0)
	{
		//normaliza os pontos e adciona numa lista 
		objects[0]->addNewPoint(x, y);
		x = 0.0;
		y = 0.0;
	}
	if (create)
	{
		objects[0]->startCurve();		
		create = false;		
		draw = true;		
	}
}

void System::render() {
	//draw triangles with shader
	glClearColor(0.98f, 0.75f, 0.75f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();


	if (draw)
	{
		objects[0]->draw(*shaders[0]);
	}
}

