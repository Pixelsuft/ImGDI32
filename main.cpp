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

#define WIDTH 640
#define HEIGHT 480

using namespace std;

HWND hwnd;

void DrawPixels(HWND hwnd)
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplGDI_NewFrame();
	ImGui::NewFrame();
	if (ImGui::Begin("Hello, world!"), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize) {
		ImGui::Button("Button 1");
		ImGui::Button("Button 2");
		ImGui::Button("Button 3");
		ImGui::Button("Button 4");
		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplGDI_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
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

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow) {

	MSG  msg;
	WNDCLASSW wc = { 0 };

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"ImGDI32";
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	RegisterClassW(&wc);
	hwnd = CreateWindowW(
		wc.lpszClassName,
		L"Hello, World!",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		200,
		200,
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
	// Apply My Style
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.WindowRounding = 0.0f;
	style.ScrollbarRounding = 12.0f;
	style.GrabRounding = 6.0f;
	style.TabRounding = 6.0f;
	style.WindowTitleAlign.x = 0.5f;

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 162.0f / 255.0f, 232.0f / 255.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = style.Colors[ImGuiCol_FrameBgActive];
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 105.0f / 255.0f, 150.0f / 255.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(237.0f / 255.0f, 28.0f / 255.0f, 36.0f / 255.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_TitleBg];
	style.Colors[ImGuiCol_TitleBgCollapsed] = style.Colors[ImGuiCol_TitleBg];
	//style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	//style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 242.0f / 255.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = style.Colors[ImGuiCol_FrameBgActive];

	// "Fix" Bug (Convert all RGB to BGR)
	for (size_t i = 0; i < ImGuiCol_COUNT; i++) {
		float temp_color = style.Colors[i].x;
		style.Colors[i].x = style.Colors[i].z;
		style.Colors[i].z = temp_color;
	}

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
