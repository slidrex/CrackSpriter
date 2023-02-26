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
		 -(int)((xCanvasSize/2)+1), -(int)((yCanvasSize/2)+0.5),
		-(int)((xCanvasSize /2)+1), (int)(yCanvasSize/2)+1,
		(int)((xCanvasSize/2)+0.5), (int)(yCanvasSize/2)+1,
		(int)((xCanvasSize/2)+0.5), -(int)((yCanvasSize/2)+1),
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
	position.y *= -1;
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
bool Crack::Viewport::IsInsideViewport(glm::vec2 p, glm::mat4 mvp) const
{
	glm::vec4* corners = GetViewportCorners(mvp);
	
	return (p.x > corners[0].x && p.x < corners[2].x && p.y > corners[0].y && p.y < corners[1].y);
}
glm::vec2 Crack::Viewport::ScreenToViewport(glm::vec2 screenSize, glm::vec2 point, glm::mat4 projection) const
{
	glm::vec2 rawPointPosition = point;
	//Lerps (0; screenSize) raw point position into (-1;1) coordinate system
	glm::vec2 fromScreenToStandartCoordinateSystem = glm::vec2((point.x * 2) / screenSize.x, (point.y * 2) / screenSize.y) - glm::vec2(1);

	return glm::round(glm::inverse(projection) * glm::vec4(fromScreenToStandartCoordinateSystem.x, fromScreenToStandartCoordinateSystem.y, 0.0f, 0.0f));
}
glm::vec4* Crack::Viewport::GetViewportCorners(glm::mat4 mvp) const
{
	glm::vec4 *corners = new glm::vec4[4];
	for(int i = 0; i < 4; i++)
	{
		corners[i] = glm::inverse(mvp) * glm::vec4(attachedCanvas->canvasBounds[i * 2], attachedCanvas->canvasBounds[(i * 2) + 1], 0.0f, 0.0f);
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