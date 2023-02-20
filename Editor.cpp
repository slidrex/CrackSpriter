#include "Editor.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Viewport.h"

void Crack::Editor::reshapeFunction(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}
void Crack::Editor::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoomFactor = glm::clamp(zoomFactor + -(float)yoffset * zoomSensitivity, minZoomFactor, maxZoomFactor);
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

	glm::mat4 viewportVP = projection;
	viewport->GetShader()->Bind();
	viewport->GetShader()->SetUniformMat4f("u_MVP", viewportVP);


	bool rightMoused = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1);
	bool leftMoused = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_2);
	if (rightMoused || leftMoused)
	{
		double x, y;
		glfwGetCursorPos(m_Window, &x, &y);
		glm::vec2 newCoord = viewport->FromScreenToViewport(width, height, glm::vec2((float)x, (float)y));

		bool isInside = viewport->IsInsideViewport(glm::vec4(newCoord.x, newCoord.y, 0.0f, 0.0f), zoomFactor);
		glm::vec2 pushingPoint = glm::vec2(glm::round(newCoord.x * zoomFactor), glm::round(newCoord.y * zoomFactor));
		if (isInside && glm::abs(pushingPoint.x) <= m_CanvasWidth && glm::abs(pushingPoint.y) <= m_CanvasHeight)
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
}
