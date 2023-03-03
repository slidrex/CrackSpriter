#pragma once
#include <vector>
#include "Log.h"
#include "Canvas.h"

namespace Crack
{
	class Viewport
	{
	public:
		~Viewport();
		void CreateCanvas(unsigned int xCanvasSize, unsigned int yCanvasSize, glm::vec4 initialColor);
		void Render() const;
		Shader* GetShader() const;
		glm::vec4* GetViewportCorners() const;
		glm::vec2 ScreenToViewport(glm::vec2 screenPosition, glm::vec2 point, glm::mat4 projection) const;
		bool IsInsideViewport(glm::vec2 p) const;
		void PushColor(glm::vec2 position, glm::vec4 color);
		Pixel& GetPixel(glm::vec2 position);
		void Export(const std::string& path) const;
	private:
		Canvas* attachedCanvas;
	};
}