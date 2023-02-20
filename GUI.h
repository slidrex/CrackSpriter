#pragma once
#include "Editor.h"


namespace Crack
{
	class GUI : public Layer
	{
	private:
		Editor* m_Application;
		void Init() override;
		void Update() override;
		void OnClose() override;
		void ShowDockingWindow();
	public:
		GUI(Editor* application);
	};
}