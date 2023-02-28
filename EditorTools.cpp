#include "EditorTools.h"


void Crack::Brush::OnPush(Crack::Pixel& pixel)
{
	print("Brush push!");
	*pixel.color = *Editor::Get().pushColor;
}
void Crack::ColorPicker::OnPush(Crack::Pixel& pixel)
{
	print("Color picker push!");
}
Crack::Tool::Tool(Editor* editor)
{
	m_Editor = editor;
}