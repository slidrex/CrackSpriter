#include "Editor.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Viewport.h"
#include "Events.h"



Crack::Editor* Crack::Editor::s_Instance;

Crack::Editor::Editor(GLFWwindow* window)
{
	SelectedTool = EDITOR_TOOLS_BRUSH;
	m_Window = window;
	viewport = nullptr;
	tools = std::vector<std::unique_ptr<Tool>>();
	tools.push_back(std::make_unique<Brush>(this));
	tools.push_back(std::make_unique<ColorPicker>(this));
	

	s_Instance = this;
}
Crack::Editor::Editor()
{
	SelectedTool = EDITOR_TOOLS_BRUSH;
	m_Window = nullptr;
	viewport = nullptr;
}

void Crack::Editor::Init()
{
	m_CanvasWidth = 18;
	m_CanvasHeight = 18;
	glm::vec4 defaultCanvasColor = glm::vec4(0.5f, 0.7f, 0.5f, 1.0f);

	viewport = new Crack::Viewport();
	viewport->CreateCanvas(m_CanvasWidth, m_CanvasHeight, defaultCanvasColor);
	
}
void Crack::Editor::Update()
{
	int width, height;
	glfwGetWindowSize(m_Window, &width, &height);

	glm::mat4 projection = glm::ortho(-(float)width * zoomFactor / 2, (float)width * zoomFactor / 2, (float)height * zoomFactor / 2, -(float)height * zoomFactor / 2);
	glm::mat4 viewportMVP = projection;
	viewport->GetShader()->Bind();
	viewport->GetShader()->SetUniformMat4f("u_MVP", viewportMVP);
	


	bool rightMoused = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1);
	bool leftMoused = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_2);
	if (rightMoused || leftMoused)
	{
		glm::vec2 cursorPosition =  Crack::Event::GetMousePosition();
		glm::vec2 pushingPoint = viewport->ScreenToViewport(glm::vec2((float)width, (float)height), cursorPosition, projection);
		
		bool isInside = viewport->IsInsideViewport(glm::vec4(pushingPoint.x, pushingPoint.y, 0.0f, 0.0f));
		

		if (isInside)
		{
			tools[SelectedTool]->OnPush(viewport->GetPixel(pushingPoint));
		}
	}

	glPointSize(1 / zoomFactor);
	viewport->Render();
	viewport->GetShader()->Unbind();
}

void Crack::Editor::OnClose()
{

}