#pragma once
#include <memory>
#include "glfw3.h"
#include "Viewport.h"
#include "EditorTools.h"
#include "Layer.h"
#include "BackupStack.h"

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
		float GetZoomFactor() { return zoomFactor; }
		void SetZoomFactor(float zoom) { zoomFactor = zoom; }
		float GetMaxZoomFactor() { return maxZoomFactor; }
		float GetMinZoomFactor() { return minZoomFactor; }
		unsigned int GetCanvasWidth() { return m_CanvasWidth; }
		unsigned int GetCanvasHeight() { return m_CanvasHeight; }
		void SetCanvasSize(unsigned int width, unsigned int height) { m_CanvasHeight = height; m_CanvasWidth = width; }
		int GetSelectedTool() { return selectedTool; }
		void SetSelectedTool(int tool) { selectedTool = tool; }
		float* GetPushColor() { return pushColor; }
		void SetPushColor(float* color);
		float GetZoomSensitivity() { return zoomSensitivity; }
		Viewport& GetViewport() { return *viewport; }
		std::string GetExportPath() { return exportPath; }
		BackupStack* Backups;
		void Undo();
	private:
		void UpdateCanvas();
		void Init() override;
		void Update() override;
		void OnClose() override;
		std::string exportPath;
		unsigned int m_CanvasWidth;
		unsigned int m_CanvasHeight;
		int selectedTool;
		float pushColor[4];
		float zoomFactor;
		float minZoomFactor;
		float maxZoomFactor;
		float zoomSensitivity;
		GLFWwindow *m_Window;
		Crack::Viewport* viewport;
		std::vector<std::unique_ptr<Tool>> tools;
		static Editor* s_Instance;

	};
}