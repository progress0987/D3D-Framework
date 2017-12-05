//#include <Windows.h>
//#include <Mmsystem.h>
//#include <d3dx9.h>
//
//LPDIRECT3D9 g_pD3D = nullptr;  // D3D 
//LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;  // �������ϴ� D3D ����̽�
//LPDIRECT3DVERTEXBUFFER9 g_pVB = nullptr;    // ���� ����
//
//											// ����� ���� ���� ����ü
//struct CUSTOMVERTEX {
//	FLOAT x, y, z; // rhw�� ���� ��ǥ���� w������ �� ���� ������ ��ȯ�� �Ϸ�� ������ �ǹ��Ѵ�.
//	DWORD color;        // diffuse color, ������ ������ ��Ÿ����.
//};
//
//// ����ڰ� ������ ������ ���� ������ ��Ÿ���� FVF ��
//// x,y,z,w ���� diffuse color�� ��Ÿ���� flag ���� �����Ͽ���.
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
//
//HRESULT InitD3D(HWND hWnd);
//HRESULT InitVB();
//VOID Cleanup();
//VOID Render();
//VOID SetupMatrices();
//LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
//INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
//{
//	// ������ Ŭ���� ���� �� �ʱ�ȭ, ���
//	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "D3D Tutorial", NULL };
//	RegisterClassEx(&wc);
//
//	// ������ ����
//HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 01: CreateDevice", WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
//
//// Direct3D �ʱ�ȭ
//if (SUCCEEDED(InitD3D(hWnd))) {
//	// ���� ���� �ʱ�ȭ
//	if (SUCCEEDED(InitVB())) {
//		// ������ ���
//		ShowWindow(hWnd, SW_SHOWDEFAULT);
//		UpdateWindow(hWnd);
//
//		// �޼��� ����
//		MSG msg;
//		ZeroMemory(&msg, sizeof(msg));
//
//		while (msg.message != WM_QUIT) {
//			// GetMessage() �Լ��� �޸� ������� �ʴ´�.
//			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
//				TranslateMessage(&msg);
//				DispatchMessage(&msg);
//			}
//			else {
//				// ó���� �޼����� ������ �������Ѵ�.
//				Render();
//			}
//
//		}
//	}
//}
//
//// ����� Ŭ������ �޸𸮸� ��ȯ�ϰ�, ��Ͽ��� �����Ѵ�.
//UnregisterClass("D3D Tutorial", wc.hInstance);
//
//return 0;
//}
//
//HRESULT InitD3D(HWND hWnd)
//{
//	// D3D ��ü ����
//	if (nullptr == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
//		return E_FAIL;
//	}
//
//	// D3D ����̽��� �����ϱ� ���ؼ� �ʿ��� ����ü
//	D3DPRESENT_PARAMETERS d3dpp;
//	ZeroMemory(&d3dpp, sizeof(d3dpp));  // 0���� �ʱ�ȭ���� ������ ����� ������ ���� �� �ִ�.
//
//	d3dpp.Windowed = TRUE;  // â���
//	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
//
//	// D3DDEVTYPE_HAL�� �ϵ���� ������ �����ϵ��� �����Ѵ�. 
//	// �������� g_pd3dDevice�� D3D ����̽� �����͸� �����Ѵ�. 
//	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice))) {
//		return E_FAIL;
//	}
//
//	// �ﰢ���� �޸��� ǥ���ϱ� ���� �ڵ�
//	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	// ������ diffuse color�� ������ �ֹǷ� ���� ����.
//	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	return S_OK;
//}
//
//// ���� ���� �ʱ�ȭ
//HRESULT InitVB()
//{
//	CUSTOMVERTEX vertices[] = {
//		//���� 0xffff0000 �� ���� ǥ���� �� �ִµ� ARGB�� �����̴�.
//		{ -1.0f	,-1.0f	,0.0f, 0xffff0000, },
//		{ 1.0f	,-1.0f	,0.0f, 0xff0000ff, },
//		{ 0.0f	,1.0f	,0.0f, 0xffffffff, },
//	};
//
//	/*
//	���� ���� ����
//
//	3���� ������������� ������ �޸𸮸� �Ҵ��Ѵ�.
//	FVF�� �����Ͽ� ������ �������� ������ �����Ѵ�.
//	FVF - Flexible Vertex Format		- ���������� ���ؽ��� �����Ѵٰ� �����Ҷ�
//	*/
//	if (FAILED(g_pd3dDevice->CreateVertexBuffer(
//		3 * sizeof(CUSTOMVERTEX),										//������ ���������� ����Ʈ ���� ũ��
//		0,																//���� ������ ���� Ȥ�� ó�����(SW/HW) ����
//		D3DFVF_CUSTOMVERTEX,											//���� ���� ����ü�� ���� ����� FVF �÷��� ��
//		D3DPOOL_DEFAULT,												//���� ���۰� ����� �޸��� ��ġ(����ī������ �ý��۸޸����� ��)�� ������� ����
//		&g_pVB,															//��ȯ�� ���� ������ �������̽�
//		NULL															//����Ǿ�����(�����Ǿ�����) ����� ���� ������ NULL
//	))) {
//		return E_FAIL;
//	}
//	VOID* pVertices;
//	if (FAILED(g_pVB->Lock(												//����ϱ� ���ؼ� ��
//		0,																//Lock�� �� ������ ������, �Ʒ��� �Բ� ���ʸ�� 0�̸� ��ü ����
//		sizeof(vertices),												//Lock�� �� ������ ũ��, ���� �Բ� ���ʸ�� 0�̸� ��ü ����
//		(void**)&pVertices,												//�а� ���� �����Ϸ��� �޸� ������ ������
//		0																//Lock ����� �Բ� ���Ǵ� �÷���
//	))) return E_FAIL;
//
//	memcpy(pVertices, vertices, sizeof(vertices));											//���ؽ����� �����سִ´�
//
//	g_pVB->Unlock();																		//����� Ǯ���� ���
//
//	return S_OK;
//}
//
//// ��ü ���ҽ��� ��ȯ�ϴ� �Լ�
//VOID Cleanup()
//{
//	// ��ü�� ���ҽ��� ��ȯ�ϴ� ������ �߿��ϴ�.
//	// �� ������ �߸��Ǹ� ������ �߻���Ų��.
//	// ������ ����
//	if (g_pVB != nullptr) {
//		g_pVB->Release();
//	}
//
//
//	if (g_pd3dDevice != nullptr) {
//		g_pd3dDevice->Release();
//	}
//
//	if (g_pD3D != nullptr) {
//		g_pD3D->Release();
//	}
//}
//
//// ȭ���� �׸��� �Լ�
//VOID Render()
//{
//	if (nullptr == g_pd3dDevice) {
//		return;
//	}
//
//	// �ĸ� ���۸� �Ķ������� ä���.
//	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
//
//
//	// BeginScene() �Լ��� ����̽����� Rendering ������ �˸���.
//	if (SUCCEEDED(g_pd3dDevice->BeginScene())) {
//
//		SetupMatrices();
//
//		// ���� ���۸� ��� ��Ʈ������ �����Ѵ�.
//		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
//
//		// D3D�� ���� ������ �����Ѵ�.
//		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
//
//		// ���� ���� ��ü�� �׸���.
//		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
//
//		// Rendering�� �������� �˸���.
//		g_pd3dDevice->EndScene();
//	}
//
//	// �ĸ� ���۸� ���� �׸��� ���۷� �����Ѵ�.
//	// Double Buffering(���� ����)�� ���õ� �����̴�.
//	// ���� ���۸� ����ϸ鼭 �� �Լ��� �������� ������ ����� �׷����� �ʴ´�.
//	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
//}
//
//// ����, ��, �������� ��� ����
//VOID SetupMatrices()
//{
//	// ���� ��� ����
//	D3DXMATRIXA16 matWorld;
//
//	UINT iTime = timeGetTime() % 1000;
//	FLOAT fAngle = iTime * (2.f * D3DX_PI) / 1000.f;
//	D3DXMatrixRotationY(&matWorld, fAngle);
//	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//	// �� ��� ����
//	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -5.0f);  // ī�޶��� ��ġ
//	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);    // ī�޶� �ٶ󺸴� ����
//	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);   // ī�޶��� ���⺤��
//
//	D3DXMATRIXA16 matView;
//	// ī�޶� ��ȯ ��� ���
//	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
//	// ���� ī�޶� ��ȯ ����� ����
//	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
//
//	// �������� ��� ����
//	D3DXMATRIXA16 matProj;
//	// ���� ��ȯ ��� ���
//	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.f);
//	// ���� ���� ��ȯ ����� ����
//	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
//}
//
//// ������ ���ν��� �Լ�
//LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg) {
//	case WM_DESTROY:
//		Cleanup();
//		PostQuitMessage(0);
//		return 0;
//
//		// �޼��� ���� ����
//		/*case WM_PAINT:
//		Render();
//		ValidateRect(hWnd, NULL);
//		return 0;*/
//	}
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
