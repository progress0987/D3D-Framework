/***********************************
 *	D3D �⺻ �����ӿ�ũ
 *	20171201 David Kim
 *	�ذ�å ���� ���� ����
 ***********************************/

#include "D3DCore.h"

//������ ���ν���
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
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L,0L,	//������ Ŭ���� ��� -Ȯ��
					GetModuleHandle(NULL),NULL,NULL,NULL,NULL,"d3d",NULL };
	RegisterClassEx(&wc);

	HWND	hWnd = CreateWindow("d3d",								//������ ����
		WINNAME,														//������ �̸�
		WS_OVERLAPPEDWINDOW,
		100,															//������ ������ X
		100,															//������ ������ Y
		WINSIZEX,														//������ ��
		WINSIZEY,														//������ ����
		//GetDesktopWindow(),												//�θ� ������ �ڵ� -Ȯ��
		NULL,
		NULL,															//�޴� ������� -Ȯ��
		wc.hInstance,													//������Ŭ������ �ν��Ͻ�
		NULL);															//LPARAM

	//D3D �ʱ�ȭ
	if (SUCCEEDED(InitD3D(hWnd))) {
		if (SUCCEEDED(InitVB())) {
			ShowWindow(hWnd, SW_SHOWDEFAULT);								//������ ���
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT) {							//�޼��� ����
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
	UnregisterClass("d3d", wc.hInstance);								//��ϵ� Ŭ���� �Ұ�
	return 0;
}