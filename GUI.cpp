#include "GUI.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

ImGuiIO* io = nullptr;
int canvasSize[] = { 16, 16 };
float* canvasColor;

Crack::GUI::GUI(Editor* application) : m_Application(application), showExportWindow(false) {}

void Crack::GUI::Update()
{
	BeginRenderUI();

	if (ImGui::BeginMainMenuBar())
	{
		OnMainMenu();
		ImGui::EndMainMenuBar();
	}


	if (ImGui::Begin("Toolbar"))
	{
		OnToolbar();
		ImGui::End();
	}

	if (showExportWindow)
		OnExportWindow();

	EndRenderUI();
}

void Crack::GUI::OnMainMenu() 
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New File"))
		{
			showExportWindow = true;
		}

		if (ImGui::MenuItem("Export"))
		{
			showExportWindow = true;
			m_Application->GetViewport().Export(m_Application->GetExportPath());
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Undo", "CTRL+Z")) 
		{
			m_Application->Undo();
			print("Undo!");
		}

		ImGui::Separator();
		ImGui::EndMenu();
	}
}
void Crack::GUI::OnToolbar()
{
	float sliderValue = m_Application->GetZoomFactor();
	ImGui::SliderFloat("Zoom Factor", &sliderValue, m_Application->GetMaxZoomFactor(), m_Application->GetMinZoomFactor());
	m_Application->SetZoomFactor(sliderValue);
	int currentItem = m_Application->GetSelectedTool();
	ImGui::Combo("Tools", &currentItem, items, IM_ARRAYSIZE(items));
	m_Application->SetSelectedTool(currentItem);
	float* pushColor = m_Application->GetPushColor();
	

	ImGui::ColorPicker3("Color", pushColor, ImGuiColorEditFlags_NoOptions);
	m_Application->SetPushColor(pushColor);
}
void Crack::GUI::OnExportWindow() 
{
	if (ImGui::Begin("New File"))
	{
		ImGui::InputInt2("Canvas size", canvasSize);
		ImGui::ColorPicker3("Base Canvas Color", canvasColor);
		if (ImGui::Button("Create"))
		{
			m_Application->GetViewport().CreateCanvas(canvasSize[0], canvasSize[1], glm::vec4(canvasColor[0], canvasColor[1], canvasColor[2], 1.0f));
			m_Application->Backups->ClearBackupStack();
			int canvasSize[] = { 16, 16 };
		}
		ImGui::End();
	}
}
void Crack::GUI::Init()
{
	canvasColor = new float[3];
	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;

	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
	float color[3] = { 1.0f, 1.0f, 1.0f };
	ImGui_ImplGlfw_InitForOpenGL(m_Application->GetWindow(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}
void Crack::GUI::BeginRenderUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ShowDockingWindow();
}
void Crack::GUI::EndRenderUI()
{
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

void Crack::GUI::ShowDockingWindow()
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    bool* p_open = (bool*)true;
    

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);


    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}