#pragma once
#include "Editor.h"

namespace Crack
{
	class Editor;

	class Tool
	{
	protected:
		Editor* m_Editor;
		Tool(Editor* editor);
	public:
		virtual void OnPushBegin(Crack::Pixel& pushedPixel) {}
		virtual void OnPush(Crack::Pixel& pushedPixel) {}
		virtual void OnPushEnd(Crack::Pixel& pushedPixel) {}
	};
	class Brush : public Tool
	{
	public:
		Brush(Editor* editor) : Tool(editor) {}
	public:
		void OnPushBegin(Crack::Pixel& pixel) override;
		void OnPushEnd(Crack::Pixel& pixel) override;
		void OnPush(Crack::Pixel& pixel) override;
	};
	class ColorPicker : public Tool
	{
	public:
		ColorPicker(Editor* editor) : Tool(editor) {}
	public:
		void OnPush(Crack::Pixel &pixel) override;
	};
}