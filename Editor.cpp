#include "Editor.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Viewport.h"


void Crack::Editor::Init()
{
	m_CanvasWidth = 18;
	m_CanvasHeight = 18;
	viewportOffset = new float[2] {0.0f, 0.0f};
	glm::vec4 defaultCanvasColor = glm::vec4(0.5f, 0.7f, 0.5f, 1.0f);

	viewport = new Crack::Viewport();
	viewport->CreateCanvas(m_CanvasWidth, m_CanvasHeight, defaultCanvasColor);
	
}
void Crack::Editor::Update()
{
	int width, height;
	glfwGetWindowSize(m_Window, &width, &height);

	glm::mat4 view = glm::translate(glm::mat4(1), glm::vec3(viewportOffset[0], viewportOffset[1], 0.0f));
	glm::mat4 projection = glm::ortho(-(float)width * zoomFactor / 2, (float)width * zoomFactor / 2, (float)height * zoomFactor / 2, -(float)height * zoomFactor / 2);
	glm::mat4 viewportMVP = projection * view;
	viewport->GetShader()->Bind();
	viewport->GetShader()->SetUniformMat4f("u_MVP", viewportMVP);

	double xCur, yCur;
	glfwGetCursorPos(m_Window, &xCur, &yCur);


	bool rightMoused = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1);
	bool leftMoused = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_2);
	if (rightMoused || leftMoused)
	{
		double x, y;
		glfwGetCursorPos(m_Window, &x, &y);
		glm::vec2 pushingPoint = viewport->ScreenToViewport(glm::vec2((float)width, (float)height), glm::vec2((float)x, (float)y), projection);
		print(pushingPoint.x << " " << pushingPoint.y);
		bool isInside = viewport->IsInsideViewport(glm::vec4(pushingPoint.x, pushingPoint.y, 0.0f, 0.0f), viewportMVP);
		
			print(isInside);
		if (isInside)
		{
			viewport->PushColor(pushingPoint, glm::vec4(pushColor[0], pushColor[1], pushColor[2], 1.0f));
		}
	}

	glPointSize(1 / zoomFactor);
	viewport->Render();
	viewport->GetShader()->Unbind();
}

void Crack::Editor::OnClose()
{
	delete viewportOffset;
}
