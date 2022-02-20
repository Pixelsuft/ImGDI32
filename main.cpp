#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <string>
#include <vector>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_gdi.h>
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

#define WIDTH 1024
#define HEIGHT 768

using namespace std;

HWND hwnd;
int times = 0;
float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float _fps = 60.0f;
float _dt = 1000.0f / _fps;
bool is_open = true;
bool show_demo_window = false;
bool can_show_demo = true;

void DrawPixels(HWND hwnd)
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplGDI_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.Framerate = _fps;
	io.DeltaTime = _dt;
	if (ImGui::Begin(
		"Hello, world!",
		&is_open,
		ImGuiWindowFlags_NoScrollbar
	)) {
		if (ImGui::Button("Button 1")) {
			SetWindowTextA(
				hwnd,
				("Button Pressed " + to_string(++times) + " times.").c_str()
			);
		}
		if (ImGui::ColorEdit4("Background", color, 0)) {
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_WindowBg] = ImVec4(color[0], color[1], color[2], color[3]);
		}
		ImGui::Checkbox("Show Demo Window", &show_demo_window);
		if (show_demo_window) {
			can_show_demo = true;
			ImGui::ShowDemoWindow(&can_show_demo);
			if (!can_show_demo)
				show_demo_window = false;
		}
		ImGui::Text(("FPS: " + to_string((int)_fps)).c_str());
		ImGui::Text("Styles: ");
		if (ImGui::Button("Dark")) {
			ImGui::StyleColorsDark();
			ImGuiStyle& style = ImGui::GetStyle();
			color[0] = style.Colors[ImGuiCol_WindowBg].x; // Sry for shitcode
			color[1] = style.Colors[ImGuiCol_WindowBg].y;
			color[2] = style.Colors[ImGuiCol_WindowBg].z;
			color[3] = style.Colors[ImGuiCol_WindowBg].w;
		}
		if (ImGui::Button("Light")) {
			ImGui::StyleColorsLight();
			ImGuiStyle& style = ImGui::GetStyle();
			color[0] = style.Colors[ImGuiCol_WindowBg].x;
			color[1] = style.Colors[ImGuiCol_WindowBg].y;
			color[2] = style.Colors[ImGuiCol_WindowBg].z;
			color[3] = style.Colors[ImGuiCol_WindowBg].w;
		}
		if (ImGui::Button("Classic")) {
			ImGui::StyleColorsClassic();
			ImGuiStyle& style = ImGui::GetStyle();
			color[0] = style.Colors[ImGuiCol_WindowBg].x;
			color[1] = style.Colors[ImGuiCol_WindowBg].y;
			color[2] = style.Colors[ImGuiCol_WindowBg].z;
			color[3] = style.Colors[ImGuiCol_WindowBg].w;
		}
		if (ImGui::Button("My Style")) {
			ImGui::StyleColorsDark();
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowBorderSize = 0.0f;
			style.WindowRounding = 0.0f;
			style.ScrollbarRounding = 12.0f;
			style.GrabRounding = 6.0f;
			style.TabRounding = 6.0f;
			style.WindowTitleAlign.x = 0.5f;

			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 162.0f / 255.0f, 232.0f / 255.0f, 1.0f);
			style.Colors[ImGuiCol_FrameBgHovered] = style.Colors[ImGuiCol_FrameBgActive];
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 105.0f / 255.0f, 150.0f / 255.0f, 1.0f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(237.0f / 255.0f, 28.0f / 255.0f, 36.0f / 255.0f, 1.0f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
			style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_TitleBg];
			style.Colors[ImGuiCol_TitleBgCollapsed] = style.Colors[ImGuiCol_TitleBg];
			//style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			//style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 242.0f / 255.0f, 0.0f, 1.0f);
			style.Colors[ImGuiCol_CheckMark] = style.Colors[ImGuiCol_FrameBgActive];
			color[0] = color[1] = color[2] = 0.0f;
			color[3] = 1.0f;
			style.Colors[ImGuiCol_WindowBg] = ImVec4(color[0], color[1], color[2], color[3]);
		}
		
		ImVec2 window_size = ImGui::GetWindowSize();
		if (window_size.x < 320) {
			window_size.x = 320;
			ImGui::SetWindowSize(window_size);
		}
		if (window_size.y < 250) {
			window_size.y = 250;
			ImGui::SetWindowSize(window_size);
		}

		ImGui::End();
	}
	if (!is_open) {
		PostQuitMessage(0);
		ExitProcess(0);
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplGDI_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
		return true;
	}
	switch (msg) {
	case WM_CREATE:

		SetTimer(hwnd, 1, (UINT)(1000.0f / 60.0f), NULL); // 60 FPS
		break;

	case WM_PAINT:

		DrawPixels(hwnd);
		break;

	case WM_ERASEBKGND:
		return 1; // Say we handled it.

	case WM_TIMER:

		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		ExitProcess(0);
		return 0;
	}

	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow) {

	MSG  msg;
	WNDCLASSA wc = { 0 };

	DEVMODE dm;
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm)) {
		_fps = (float)dm.dmDisplayFrequency;
		_dt = 1000.0f / _fps;
	}

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = "ImGDI32";
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	RegisterClassA(&wc);
	hwnd = CreateWindowA(
		wc.lpszClassName,
		"Hello, World!",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100,
		100,
		WIDTH,
		HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;
	io.ImeWindowHandle = hwnd;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplGDI_Init();

	while (GetMessage(&msg, NULL, 0, 0) > 0) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	ImGui_ImplGDI_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return (int)msg.wParam;
}
