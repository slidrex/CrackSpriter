#include "Viewport.h"
#include "FilestreamParser.h"
#include "glew.h"
#include "Log.h"
#include <ostream>

void Crack::Viewport::CreateCanvas(unsigned int xCanvasSize, unsigned int yCanvasSize, glm::vec4 initialColor)
{
	attachedCanvas = new Canvas();

	attachedCanvas->xSize = xCanvasSize;
	attachedCanvas->ySize = yCanvasSize;
	attachedCanvas->shader = new Shader(GetFileString("Basicv.shader"), GetFileString("Basicf.shader"));

	attachedCanvas->canvasBounds = new int[8]
	{
		 -(int)((xCanvasSize/2)+1), -(int)((yCanvasSize/2)+1),
		-(int)((xCanvasSize /2)+1), (int)(yCanvasSize/2)+1,
		(int)(xCanvasSize/2)+1, (int)(yCanvasSize/2)+1,
		(int)(xCanvasSize/2)+1, -(int)((yCanvasSize/2)+1),
	};
	attachedCanvas->pixels = new PixelArray(xCanvasSize * yCanvasSize);

	for(int i = 0; i < attachedCanvas->xSize; i++)
	{
		for(int j = 0; j < attachedCanvas->ySize; j++)
		{
			glm::vec2 pushPos = glm::vec2((int)i - (int)(xCanvasSize / 2), j - (int)(yCanvasSize / 2));
			attachedCanvas->pixels->PushPixel(xCanvasSize, i, j, pushPos, initialColor);
		}
	}

	glGenVertexArrays(1, &attachedCanvas->vao);
	glBindVertexArray(attachedCanvas->vao);

	
	glGenBuffers(1, &attachedCanvas->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, attachedCanvas->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Pixel) * xCanvasSize * yCanvasSize, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexArrayAttrib(attachedCanvas->vao, 0);
	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(Pixel), (const void*)offsetof(Pixel, position)); //position
	glEnableVertexArrayAttrib(attachedCanvas->vao, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Pixel), (const void*)offsetof(Pixel, color)); //color
}
void Crack::Viewport::PushColor(glm::vec2 position, glm::vec4 color)
{
	int xOffset = ((int)position.x + (attachedCanvas->xSize/2));
	int yOffset = ((int)position.y + (attachedCanvas->ySize / 2));
	attachedCanvas->pixels->PushPixel(attachedCanvas->xSize, xOffset, yOffset, glm::vec2(position.x, position.y), color);
}
void Crack::Viewport::Render() const
{
	glBindVertexArray(attachedCanvas->vao);
	glBindBuffer(GL_ARRAY_BUFFER, attachedCanvas->vbo);

	glBufferSubData(GL_ARRAY_BUFFER, 0, attachedCanvas->ySize * attachedCanvas->xSize * sizeof(Pixel), attachedCanvas->pixels->pixels);
	
	attachedCanvas->shader->Bind();

	glDrawArrays(GL_POINTS, 0, attachedCanvas->xSize * attachedCanvas->ySize);
}
void Crack::Viewport::Export(const std::string &path) const
{
	std::ofstream stream(path, std::ofstream::out);
	if (stream.is_open())
	{
		 print("File is written!");
	}
	else
		print("Path is not found!");

	stream << "P3" << " " << attachedCanvas->xSize << " " << attachedCanvas->ySize << " " << "255" << std::endl;
	
	for(int i = 0; i < attachedCanvas->xSize; i++)
	{
		for(int j = 0; j < attachedCanvas->ySize; j++)
		{
			Pixel pixel = attachedCanvas->pixels->pixels[j + i * attachedCanvas->xSize];
			int r = pixel.color[0] * 255;
			int g = pixel.color[1] * 255;
			int b = pixel.color[2] * 255;
			stream << r << " " << g << " " << b << " ";
		}
	}
}
bool Crack::Viewport::IsInsideViewport(glm::vec2 v, float zoomFactor) const
{
	glm::vec4* corners = GetViewportCorners(zoomFactor);
	
	return (v.x > corners[0].x && v.x < corners[2].x && v.y > corners[0].y && v.y < corners[1].y);
}
glm::vec2 Crack::Viewport::FromScreenToViewport(int xWindowSize, int yWindowSize, glm::vec2 point) const
{
	glm::vec4 center = glm::vec4(xWindowSize / 2, yWindowSize / 2, 0.0f, 0.0f);
	glm::vec4 p = glm::vec4(point.x, point.y, 0.0f, 0.0f) - center;

	return p;
}
glm::vec4* Crack::Viewport::GetViewportCorners(float zoomFactor) const
{
	glm::vec4 *corners = new glm::vec4[4];
	for(int i = 0; i < 4; i++)
	{
		corners[i] = glm::vec4(attachedCanvas->canvasBounds[i * 2] / zoomFactor, attachedCanvas->canvasBounds[(i * 2) + 1] / zoomFactor, 0.0f, 0.0f);
	}
	
	return corners;
}
Crack::Viewport::~Viewport()
{
	delete attachedCanvas;
}
Shader* Crack::Viewport::GetShader() const
{
	return attachedCanvas->shader;
}