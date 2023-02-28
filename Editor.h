#pragma once
#include <memory>
#include "glfw3.h"
#include "Viewport.h"
#include "EditorTools.h"
#include "Layer.h"

#define EDITOR_TOOLS_BRUSH 0
#define EDITOR_TOOLS_COLOR_PICKER 1

const static char* items[] { "Brush", "Color picker" };
namespace Crack
{
	class Tool;
	class Editor : public Layer
	{
	public:
		Editor(GLFWwindow* window);
		Editor();
		GLFWwindow* GetWindow() { return m_Window; }
		static Editor& Get() { return *s_Instance; }
		void reshapeFunction(GLFWwindow* window, int w, int h);
		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		std::string exportPath = "C:\\Users\\artem\\OneDrive\\Рабочий стол\\imgvideo.ppm";
		unsigned int m_CanvasWidth = 0;
		unsigned int m_CanvasHeight = 0;
		float pushColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float zoomFactor = 0.1f;
		float minZoomFactor = 0.03f;
		float maxZoomFactor = 1.0f;
		float zoomSensitivity = 0.04f;
		Crack::Viewport* viewport;
		int SelectedTool;
	private:
		std::vector<std::unique_ptr<Tool>> tools;
		GLFWwindow *m_Window;
		void Init() override;
		void Update() override;
		void OnClose() override;
		static Editor* s_Instance;

	};
}