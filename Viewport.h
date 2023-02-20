#pragma once
#include "Shader.h"
#include <vector>
#include "Log.h"

namespace Crack
{
	struct Pixel
	{
		int position[2];
		float color[4];
		Pixel(glm::vec2 pos, glm::vec4 col)
		{
			color[0] = col.x;
			color[1] = col.y;
			color[2] = col.z;
			color[3] = col.w;
			position[0] = pos.x;
			position[1] = pos.y;
		}
		Pixel()
		{
			position[0] = 0.0f;
			position[1] = 0.0f;
			color[0] = 0.5f;
			color[1] = 0.3f;
			color[2] = 0.8f;
			color[3] = 1.0f;
		}
	};
	struct PixelArray
	{
		Pixel* pixels;
		PixelArray(unsigned int size)
		{
			pixels = new Pixel[size];
		}
		void PushPixel(int width, int x, int y, glm::vec2 position, glm::vec4 color)
		{
			pixels[x + y * width] = Pixel(position, color);
		}
	};
	struct Canvas
	{
		~Canvas() { delete shader; delete pixels; }
		int* canvasBounds;
		Shader* shader;
		unsigned int vao;
		unsigned int vbo;
		unsigned int xSize;
		unsigned int ySize;
		PixelArray* pixels;
	};
	class Viewport
	{
	public:
		~Viewport();
		void CreateCanvas(unsigned int xCanvasSize, unsigned int yCanvasSize, glm::vec4 initialColor);
		void Render() const;
		Shader* GetShader() const;
		glm::vec4* GetViewportCorners(float zoomFactor) const;
		glm::vec2 FromScreenToViewport(int xWindowSize, int yWindowSize, glm::vec2 v) const;
		bool IsInsideViewport(glm::vec2 v, float zoomFactor) const;
		void PushColor(glm::vec2 position, glm::vec4 color);
		void Export(const std::string& path) const;
	private:
		Canvas* attachedCanvas;
	};
}