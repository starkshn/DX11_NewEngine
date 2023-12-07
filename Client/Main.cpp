#include "pch.h"
#include "Main.h"
#include "Game.h"
#include "01.TriangleDemo.h"
#include "02.QuadDemo.h"
#include "03.ConstantBufferDemo.h"

// ���α׷� ���� �κ�
int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	GameDesc desc;
	desc.appName = L"GameCoding";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);

	// ���߿� ��������� �ȴ�.
	desc.app = make_shared<ConstantBufferDemo>(); // ���� ������ ���� �־��ִ� �κ�

	GAME->Run(desc);

	return 0;
}