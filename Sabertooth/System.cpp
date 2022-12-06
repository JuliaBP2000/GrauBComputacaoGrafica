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
	/*glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);*/


	//cria janela
	window = glfwCreateWindow(WIDTH, HEIGHT, "Sabertooth", nullptr, nullptr);

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

int System::OpenGLSetup()
{

	glEnable(GL_BLEND);	// Enables blending ( glBlendFunc )
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{

	coreShader = Shader("Shaders/Core/vertexShader.vert", "Shaders/Core/fragmentShader.frag");
	coreShader.Use();

	return EXIT_SUCCESS;
}

void System::Run()
{

	//coreShader.Use();

	//GLfloat vertices[] =
	//{
	//	// Positions         // Textures

	//	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
	//	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left

	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
	//	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // Top Left
	//	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
	//};

	//GLuint VBO, VAO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// Position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);

	//// Texture attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	//glBindVertexArray(0); // Unbind VAO

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		update();

		glfwSwapBuffers(window);
	}


}

void System::Finish()
{
	coreShader.Delete();

	glfwTerminate();
}

void System::MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

			double xpos, ypos;

			glfwGetCursorPos(window, &xpos, &ypos);

			x = xpos;
			y = ypos;

			std::cout << "Pontos antes divisão:" + (int)x + (int)y << std::endl;
			
			// printf("[%f , %f] ", x, y);

		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			create = true;
		}
}

void System::update()
{
	/*if (keys[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}*/
	if (x != 0.0 || y != 0.0)
	{
		

		objects[0]->addNewPoint(x, y);
		x = 0.0;
		y = 0.0;
	}
	if (create)
	{

		//set initial points

		// calculate other points

		//make the final lap

		//draw the curve

		//add curve on a VBO
		/*objects[0]->newCurve();
		create = false;
		draw = true;*/
	}

	
}

void System::createCurve() {
	

}

void System::addNewPoint(double x, double y)
{
	double width = WIDTH / 2;
	double height = HEIGHT/ 2;

	if (x > width) {
		x = ((x - width) / width);
	}
	else if (x == width) {
		x = 0;
	}
	else {
		x = -((width - x) / width);
	}

	if (y > height) {
		y = ((y - height) / height) * (-1);
	}
	else if (y == height) {
		y = 0;
	}
	else {
		y = -((height - y) / height) * (-1);
	}

	glm::vec3* point = new glm::vec3(x, y, 0.0);

	printf("Pontos apos divisão: [%f , %f]", x, y);

	initialPoints->push_back(point);

	if (x > 0.0 && y > 0.0) {
		x += 0.5;
		y += 0.5;
	}
	else if (x > 0.0 && y < 0.0) {
		x += 0.5;
		y -= 0.5;
	}
	else if (x < 0.0 && y < 0.0) {
		x -= 0.5;
		y -= 0.5;
	}
	else {
		x -= 0.5;
		y += 0.5;
	}
}

void System::render() {
	//draw triangles with shader

}
