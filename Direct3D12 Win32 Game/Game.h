//
// Game.h
//

//GEP:: This project was created by braodly the following the docs for the DitrectXTK12 on their GitHub site.
// https://github.com/Microsoft/DirectXTK12/wiki 

#pragma once

#include "AllScenes.h"
#include "StepTimer.h"
#include "Audio.h"
#include "EventHandler.h"
#include "InputSystem.h"
#include "CharacterManager.h"
#include <vector>
using std::vector;

class SceneHandler;
class MusicHandler;

struct RenderData;
struct GameStateData;

enum SceneEnum
{
	GAME_SCENE,
	TEST_SCENE,
	PHYSICS_SCENE,
	MENU_SCENE
};

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game
{
public:

    Game();
    ~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

//GEP:: These are the update and render fucntions for the main game class
//as called by the application system
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void WaitForGpu() noexcept;
    void MoveToNextFrame();
    void GetAdapter(IDXGIAdapter1** ppAdapter);

    void OnDeviceLost();

	bool SwitchToScene(SceneEnum _scene, bool _reset);

    // Application state
    HWND                                                m_window;
    int                                                 m_outputWidth;
    int                                                 m_outputHeight;

    // Direct3D Objects
    D3D_FEATURE_LEVEL                                   m_featureLevel;
    static const UINT                                   c_swapBufferCount = 2;
    UINT                                                m_backBufferIndex;
    UINT                                                m_rtvDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12Device>                m_d3dDevice;
    Microsoft::WRL::ComPtr<IDXGIFactory4>               m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_rtvDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
    Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence;
    UINT64                                              m_fenceValues[c_swapBufferCount];
    Microsoft::WRL::Wrappers::Event                     m_fenceEvent;

    // Rendering resources
    Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_depthStencil;

    // Game state
    DX::StepTimer                                       m_timer;

	std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

	RenderData* m_RD;

	GameStateData* m_GSD;

	Scene* m_activeScene;
	SceneEnum m_current_scene;

	GameScene* m_gameScene;
	TestScene* m_testScene;
	PhysicsScene* m_physScene;
	MenuScene* m_menuScene;


	std::vector<Scene*> m_all_scenes;

	//GEP:: Keyboard and Mouse Abstractions for basic input system
	void ReadInput();

	InputSystem m_input;
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	DirectX::Keyboard::State m_prev_keyboard;

	std::unique_ptr<DirectX::Mouse> m_mouse;

	std::unique_ptr<DirectX::GamePad> m_gamePad;
	DirectX::GamePad::ButtonStateTracker m_buttons[4];

	//audio system
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	//Vector of managers
	std::unique_ptr<MusicHandler> m_musicListener = nullptr;
	std::unique_ptr<SceneHandler> m_sceneListener = nullptr;
	std::vector<EventHandler*> listeners;
};
