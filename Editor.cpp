#include "Editor.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Viewport.h"
#include "Events.h"

bool rightMoused;
bool leftMoused;

bool undo;
glm::vec2 previousFramePushPosition;

Crack::Editor* Crack::Editor::s_Instance;

Crack::Editor::Editor(GLFWwindow* window)
	:selectedTool(EDITOR_TOOLS_BRUSH), m_Window(window), viewport(new Viewport()), maxZoomFactor(1.0f), minZoomFactor(0.03f), zoomFactor(0.1f), m_CanvasHeight(16), m_CanvasWidth(16), zoomSensitivity(0.04f)
{
	pushColor[0] = 0.0f;
	pushColor[1] = 0.0f;
	pushColor[2] = 0.0f;
	pushColor[3] = 1.0f;
	tools.push_back(std::make_unique<Brush>(this));
	tools.push_back(std::make_unique<ColorPicker>(this));
	
	Backups = new BackupStack();
	s_Instance = this;
}
Crack::Editor::Editor() : selectedTool(EDITOR_TOOLS_BRUSH), m_Window(nullptr), viewport(nullptr), maxZoomFactor(0), minZoomFactor(0), zoomFactor(0.0f), m_CanvasHeight(0), m_CanvasWidth(0), zoomSensitivity(0.0f)
{
	pushColor[0] = 0.0f;
	pushColor[1] = 0.0f;
	pushColor[2] = 0.0f;
	pushColor[3] = 1.0f;
	Backups = nullptr;
}

void Crack::Editor::Init()
{
	m_CanvasWidth = 18;
	m_CanvasHeight = 18;
	glm::vec4 defaultCanvasColor = glm::vec4(0.5f, 0.7f, 0.5f, 1.0f);

	viewport = new Crack::Viewport();

	viewport->CreateCanvas(m_CanvasWidth, m_CanvasHeight, defaultCanvasColor);
	
}
void Crack::Editor::SetPushColor(float* color)
{
	pushColor[0] = color[0];
	pushColor[1] = color[1];
	pushColor[2] = color[2];
	pushColor[3] = color[3];
}
void Crack::Editor::Undo()
{
	if(Backups->GetBackupPixelsStackCount() > 0)
	{
		auto pixels = Backups->GetLastBackupedPixels();
		for (int i = 0; i < pixels.writtenPixelsCount; i++)
		{
			Crack::Pixel pix = pixels.pixels[i];
			viewport->PushColor(glm::vec2(pix.position[0], -pix.position[1]), glm::vec4(pix.color[0], pix.color[1], pix.color[2], 1.0f));

		}
		delete pixels.pixels;
	}
}
void Crack::Editor::Update()
{
	int width, height;
	glfwGetWindowSize(m_Window, &width, &height);

	glm::mat4 projection = glm::ortho(-(float)width * zoomFactor / 2, (float)width * zoomFactor / 2, (float)height * zoomFactor / 2, -(float)height * zoomFactor / 2);
	glm::mat4 viewportMVP = projection;
	viewport->GetShader()->Bind();
	viewport->GetShader()->SetUniformMat4f("u_MVP", viewportMVP);
	
	bool down = rightMoused == false && leftMoused == false;
	bool up = rightMoused || leftMoused;
	rightMoused = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1);
	leftMoused = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_2);
	if (up == true && rightMoused == false && leftMoused == false) up = true;
	else up = false;
	bool curUndo = glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(m_Window, GLFW_KEY_Z);
	if (!undo && curUndo) Undo();

	if (rightMoused || leftMoused)
	{
		glm::vec2 cursorPosition =  Crack::Event::GetMousePosition();
		glm::vec2 pushingPoint = viewport->ScreenToViewport(glm::vec2((float)width, (float)height), cursorPosition, projection);
		bool isInside = viewport->IsInsideViewport(glm::vec4(pushingPoint.x, pushingPoint.y, 0.0f, 0.0f));
		

		if (isInside && pushingPoint != previousFramePushPosition)
		{
			if(down)
			{
				tools[selectedTool]->OnPushBegin(viewport->GetPixel(pushingPoint));
			}
			tools[selectedTool]->OnPush(viewport->GetPixel(pushingPoint));
		}
		previousFramePushPosition = pushingPoint;
	}
	if (up)
	{
		glm::vec2 cursorPosition = Crack::Event::GetMousePosition();
		glm::vec2 pushingPoint = viewport->ScreenToViewport(glm::vec2((float)width, (float)height), cursorPosition, projection);
		tools[selectedTool]->OnPushEnd(viewport->GetPixel(pushingPoint));
	}
		
	undo = curUndo;

	glPointSize(1 / zoomFactor);
	viewport->Render();
	viewport->GetShader()->Unbind();
}

void Crack::Editor::OnClose()
{

}