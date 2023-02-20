#include "GUI.h"
#include "imgui.h"
#include "imgui-docking/backends/imgui_impl_glfw.h"
#include "imgui-docking/backends/imgui_impl_opengl3.h"

bool showExportWindow;
ImGuiIO* io = nullptr;

Crack::GUI::GUI(Editor* application)
{
	m_Application = application;
}

void Crack::GUI::Init()
{
	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;

	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
	float color[3] = { 1.0f, 1.0f, 1.0f };
	ImGui_ImplGlfw_InitForOpenGL(m_Application->m_Window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Crack::GUI::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New File"))
			{
				showExportWindow = true;
			}

			if (ImGui::MenuItem("Export"))
			{
				m_Application->viewport->Export(m_Application->exportPath);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}

			ImGui::Separator();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	if (ImGui::Begin("Toolbar"))
	{
		ImGui::SliderFloat("Zoom Factor", &m_Application->zoomFactor, m_Application->maxZoomFactor, m_Application->minZoomFactor);

		ImGui::Combo("Tools", &m_Application->selectedTool, items, IM_ARRAYSIZE(items));

		ImGui::ColorPicker3("Color", m_Application->pushColor, ImGuiColorEditFlags_NoOptions);
		ImGui::End();
	}

	if (showExportWindow)
	{
		if (ImGui::Begin("New File"))
		{
			int canvasSize[] = { 16, 16 };


			ImGui::InputInt2("Canvas size", canvasSize);
			ImGui::ColorPicker3("Base Canvas Color", m_Application->pushColor);
			if (ImGui::Button("Create"))
			{

			}
			ImGui::End();
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Crack::GUI::OnClose()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
