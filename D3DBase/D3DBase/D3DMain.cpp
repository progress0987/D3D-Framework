/***********************************
 *	D3D 기본 프레임워크
 *	20171201 David Kim
 *	해골책 보고 따라 만듬
 ***********************************/

#include "D3DCore.h"

//윈도우 프로시저
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		release();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//WINAPI

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT) {
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L,0L,	//윈도우 클래스 등록 -확인
					GetModuleHandle(NULL),NULL,NULL,NULL,NULL,"d3d",NULL };
	RegisterClassEx(&wc);

	HWND	hWnd = CreateWindow("d3d",								//윈도우 생성
		WINNAME,														//윈도우 이름
		WS_OVERLAPPEDWINDOW,
		100,															//윈도우 시작점 X
		100,															//윈도우 시작점 Y
		WINSIZEX,														//윈도우 폭
		WINSIZEY,														//윈도우 높이
		//GetDesktopWindow(),												//부모 윈도우 핸들 -확인
		NULL,
		NULL,															//메뉴 만들건지 -확인
		wc.hInstance,													//윈도우클래스의 인스턴스
		NULL);															//LPARAM

	//D3D 초기화
	if (SUCCEEDED(InitD3D(hWnd))) {
		if (SUCCEEDED(InitVB())) {
			ShowWindow(hWnd, SW_SHOWDEFAULT);								//윈도우 출력
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT) {							//메세지 루프
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else {
					render();
				}
			}
		}
	}
	UnregisterClass("d3d", wc.hInstance);								//등록된 클래스 소거
	return 0;
}