#pragma once
#include "Layer.h"
#include "glfw3.h"
#include "Viewport.h"

#define CRACK_BRUSH 0
#define CRACK_COLOR_PICKER 1

const static char* items[] { "Brush", "Color picker" };
namespace Crack
{
	class Editor : public Layer
	{
	private:
		void Init() override;
		void Update() override;
		void OnClose() override;
	public:
		void reshapeFunction(GLFWwindow* window, int w, int h);
		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		Editor(GLFWwindow* window)
		{
			selectedTool = CRACK_BRUSH; 
			m_Window = window; 
			viewport = nullptr; }
		std::string exportPath = "C:\\Users\\artem\\OneDrive\\Рабочий стол\\images\\imgvideo.ppm";
		unsigned int m_CanvasWidth = 0;
		unsigned int m_CanvasHeight = 0;
		float pushColor[3] = { 0.0f, 0.0f, 0.0f };
		float zoomFactor = 0.1f;
		float minZoomFactor = 0.03f;
		float maxZoomFactor = 1.0f;
		float zoomSensitivity = 0.04f;
		int selectedTool;
		Crack::Viewport* viewport;
		GLFWwindow *m_Window;
	};
}