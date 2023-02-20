#include "glew.h"
#include "glfw3.h"
#include "Shader.h"
#include "Log.h"
#include "Layer.h"
#include "GUI.h"


static void GLAPIENTRY error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	print(message);
}

void InitDebugger()
{
	glDebugMessageCallback(error_callback, nullptr);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}


int main()
{
	if (glfwInit() == false)
	{
		print("glew is not setup!");
	}
	GLFWwindow* window = glfwCreateWindow(600, 400, "Crack Spriter", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		print("glew is not setup!");
	}
	Crack::LayerStack layerStack = Crack::LayerStack();
	Crack::Editor* application = new Crack::Editor(window);
	Crack::GUI* gui = new Crack::GUI(application);

	layerStack.PushLayer(application);
	layerStack.PushLayer(gui);

	for(Crack::Layer* layer : layerStack.layers)
	{
		layer->Init();
	}
	
	glClearColor(0.2, 0.2, 0.2, 1);
	while (glfwWindowShouldClose(window) == false)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		

		for (Crack::Layer* layer : layerStack.layers)
		{
			layer->Update();
		}


		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	for (Crack::Layer* layer : layerStack.layers)
	{
		layer->OnClose();
	}
	glfwTerminate();
	return 0;
}