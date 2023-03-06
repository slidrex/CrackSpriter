#include "EditorTools.h"


void Crack::Brush::OnPushBegin(Crack::Pixel& pixel)
{
	m_Editor->Backups->BeginWritePixels();
}

void Crack::Brush::OnPushEnd(Crack::Pixel& pixel)
{
	m_Editor->Backups->EndWritePixels();
}

void Crack::Brush::OnPush(Crack::Pixel& pixel)
{
	m_Editor->Backups->WritePixel(pixel);
	pixel.color[0] = m_Editor->GetPushColor()[0];
	pixel.color[1] = m_Editor->GetPushColor()[1];
	pixel.color[2] = m_Editor->GetPushColor()[2];
}
void Crack::ColorPicker::OnPush(Crack::Pixel& pixel)
{
	m_Editor->GetPushColor()[0] = pixel.color[0];
	m_Editor->GetPushColor()[1] = pixel.color[1];
	m_Editor->GetPushColor()[2] = pixel.color[2];
}
Crack::Tool::Tool(Editor* editor)
{
	m_Editor = editor;
}