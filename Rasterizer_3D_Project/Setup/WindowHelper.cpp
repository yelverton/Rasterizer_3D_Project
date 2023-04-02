#include "WindowHelper.h"
#include <iostream>

#define SCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	// Handle any messages the switch statement didn't by using default methodology
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SetupWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow, HWND& window)
{
	const wchar_t CLASS_NAME[] = L"Eget Projekt";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	int CenterX = (SCREEN_WIDTH / 2) - (width / 2);
	int CenterY = (SCREEN_HEIGHT / 2) - (height / 2);

	RECT wr;
	wr.left = CenterX;
	wr.top = CenterY;
	wr.right = wr.left + width;
	wr.bottom = wr.top + height;

	window = CreateWindowEx(NULL,
		CLASS_NAME,
		L"Eget Projekt",
		WS_OVERLAPPEDWINDOW,
		wr.left,
		wr.top,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		instance,
		NULL);

	if (window != nullptr)
		ShowWindow(window, nCmdShow);
	else
		ErrorLog::Log("Failed to create Window, window = nullptr");

	return true;
}