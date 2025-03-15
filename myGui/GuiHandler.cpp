//
// Created by jwhal on 1/13/2025.
//

#include "guiHandler.h"

#include <windows.h>
#include <d3d11.h>
//#pragma comment(lib, "d3d11.lib") // Links the Direct3D 11 library

// ID3D11Device* d3dDevice = nullptr;
// ID3D11DeviceContext* d3dDeviceContext = nullptr;
// IDXGISwapChain* swapChain = nullptr;
// DXGI_SWAP_CHAIN_DESC sd;
// HWND hwnd;
/*
 * will set up imgui for my laptop. This is not intended to be in final product, just for debugging.
 * must call shutDownImgui at the end of my application after calling this method.
 */
// void GuiHandler::initializeImgui() {
//  // Setup Dear ImGui context
//  IMGUI_CHECKVERSION();
//  ImGui::CreateContext();
//  ImGuiIO& io = ImGui::GetIO();
//  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//  initializeBackends();
// }

/*
 * should be called after portaudio starts listening to the mic and after Imgui has been initialized. Will
 * continue listening (and plotting) until the window has been closed.
 */
void GuiHandler::plotAudio(bool useLogarithmicScaling) {

}

// void GuiHandler::shutDownImgui() {
//  // Cleanup DirectX 11 resources
//  if (d3dDeviceContext) {
//   d3dDeviceContext->Release(); // Release the device context
//  }
//
//  if (d3dDevice) {
//   d3dDevice->Release(); // Release the device
//  }
//
//  if (swapChain) {
//   swapChain->Release(); // Release the swap chain
//  }
//
//  // Close the window
//  if (hwnd) {
//   DestroyWindow(hwnd); // Destroy the window
//  }
//
//  ImGui_ImplDX11_Shutdown();
//  ImGui_ImplWin32_Shutdown();
//  ImGui::DestroyContext();
// }
//
// void GuiHandler::initializeBackends() {
//  //initialize windows
//  HINSTANCE hInstance = GetModuleHandle(NULL);
//  hwnd = CreateWindow(
//  "GuiHandler",
//  "Your Window Title",
//  WS_OVERLAPPEDWINDOW,         // Window style
//  CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, // Position and size
//  NULL, NULL, hInstance, NULL);
//  ImGui_ImplWin32_Init(hwnd);
//
//  //initialize dx11
//  d3dDevice = nullptr;
//  d3dDeviceContext = nullptr;
//  swapChain = nullptr;
//  sd = {};
//  sd.BufferCount = 1;
//  sd.BufferDesc.Width = 800;
//  sd.BufferDesc.Height = 600;
//  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//  sd.OutputWindow = hwnd;
//  sd.SampleDesc.Count = 1;
//  sd.Windowed = TRUE;
//  D3D_FEATURE_LEVEL featureLevel;
//  D3D11CreateDeviceAndSwapChain(
//      nullptr,
//      D3D_DRIVER_TYPE_HARDWARE,
//      nullptr,
//      0,
//      nullptr,
//      0,
//      D3D11_SDK_VERSION,
//      &sd,
//      &swapChain,
//      &d3dDevice,
//      &featureLevel,
//      &d3dDeviceContext);
//
//
//  // Setup Platform/Renderer backends
//  ImGui_ImplWin32_Init(hwnd);
//  ImGui_ImplDX11_Init(d3dDevice, d3dDeviceContext);
// }
