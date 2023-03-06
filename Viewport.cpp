#include "Viewport.h"
#include "FilestreamParser.h"
#include "glew.h"
#include "Log.h"
#include <ostream>
#include <png.h>

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
Crack::Pixel& Crack::Viewport::GetPixel(glm::vec2 position)
{
	position.y *= -1;
	int xOffset = ((int)position.x + (attachedCanvas->xSize / 2));
	int yOffset = ((int)position.y + (attachedCanvas->ySize / 2));
	return attachedCanvas->pixels->GetPixel(attachedCanvas->xSize, xOffset, yOffset);
}
void Crack::Viewport::PushColor(glm::vec2 position, glm::vec4 color)
{
	position.y *= -1;
	int xOffset = ((int)position.x + (attachedCanvas->xSize/2));
	int yOffset = ((int)position.y + (attachedCanvas->ySize / 2));
	attachedCanvas->pixels->PushPixel(attachedCanvas->xSize, xOffset, yOffset, position, color);
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
	FILE* fp;
	errno_t err = fopen_s(&fp, path.c_str(), "wb");
	if (err != 0)
	{
		print("Path is not found!");
		return;
	}

	// Initialize PNG structures
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(fp);
		print("Failed to create PNG write struct!");
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, NULL);
		fclose(fp);
		print("Failed to create PNG info struct!");
		return;
	}

	// Set error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		print("Failed to write PNG file!");
		return;
	}

	// Set PNG file header
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, attachedCanvas->xSize, attachedCanvas->ySize,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	// Write PNG pixel data
	png_bytep row_pointer = (png_bytep)malloc(3 * attachedCanvas->xSize);
	for (int y = 0; y < attachedCanvas->ySize; ++y)
	{
		for (int x = 0; x < attachedCanvas->xSize; ++x)
		{
			Pixel pixel = attachedCanvas->pixels->pixels[y * attachedCanvas->xSize + x];
			png_bytep ptr = &row_pointer[x * 3];
			ptr[0] = static_cast<png_byte>(pixel.color[0] * 255.0f);
			ptr[1] = static_cast<png_byte>(pixel.color[1] * 255.0f);
			ptr[2] = static_cast<png_byte>(pixel.color[2] * 255.0f);
		}
		png_write_row(png_ptr, row_pointer);
	}
	free(row_pointer);

	// Finish writing PNG file
	png_write_end(png_ptr, NULL);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);

	print("PNG file written!");
}
bool Crack::Viewport::IsInsideViewport(glm::vec2 p) const
{
	glm::vec4* corners = GetViewportCorners();
	
	return (p.x > corners[0].x && p.x < corners[2].x && p.y > corners[0].y && p.y < corners[1].y);
}
glm::vec2 Crack::Viewport::ScreenToViewport(glm::vec2 screenSize, glm::vec2 point, glm::mat4 projection) const
{
	glm::vec2 rawPointPosition = point;
	//Lerps (0; screenSize) raw point position into (-1;1) coordinate system
	glm::vec2 fromScreenToStandartCoordinateSystem = glm::vec2((point.x * 2) / screenSize.x, (point.y * 2) / screenSize.y) - glm::vec2(1);

	return glm::round(glm::inverse(projection) * glm::vec4(fromScreenToStandartCoordinateSystem.x, fromScreenToStandartCoordinateSystem.y, 0.0f, 0.0f));
}
glm::vec4* Crack::Viewport::GetViewportCorners() const
{
	glm::vec4 *corners = new glm::vec4[4];
	for(int i = 0; i < 4; i++)
	{
		corners[i] = glm::vec4(attachedCanvas->canvasBounds[i * 2], attachedCanvas->canvasBounds[(i * 2) + 1], 0.0f, 0.0f);
	}
	
	return corners;
}
Crack::Viewport::~Viewport()
{
	delete attachedCanvas;
}
bool Crack::Viewport::HasCanvas()
{
	return attachedCanvas != nullptr;
}
Shader* Crack::Viewport::GetShader() const
{
	return attachedCanvas->shader;
}