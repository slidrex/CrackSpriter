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
Crack::Editor* application;
void reshapeFunction(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	application->zoomFactor = glm::clamp(application->zoomFactor + -(float)yoffset * application->zoomSensitivity, application->minZoomFactor, application->maxZoomFactor);
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
	application = new Crack::Editor(window);
	Crack::GUI* gui = new Crack::GUI(application);

	layerStack.PushLayer(application);
	layerStack.PushLayer(gui);

	for(Crack::Layer* layer : layerStack.layers)
	{
		layer->Init();
	}
	
	glClearColor(GLclampf(0.2f), GLclampf(0.2f), GLclampf(0.2f), GLclampf(1.0f));
	while (glfwWindowShouldClose(window) == false)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		

		for (Crack::Layer* layer : layerStack.layers)
		{
			layer->Update();
		}


		glfwSetWindowSizeCallback(window, reshapeFunction);
		glfwSetScrollCallback(window, scrollCallback);
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