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
		void OnMainMenu();
		void OnToolbar();
		void BeginRenderUI();
		void EndRenderUI();
		void OnExportWindow();
		bool showExportWindow;
		bool showNewFileWindow;
	public:
		GUI(Editor* application);
	};
}