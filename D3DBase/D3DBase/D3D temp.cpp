//#include <Windows.h>
//#include <Mmsystem.h>
//#include <d3dx9.h>
//
//LPDIRECT3D9 g_pD3D = nullptr;  // D3D 
//LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;  // 렌더링하는 D3D 디바이스
//LPDIRECT3DVERTEXBUFFER9 g_pVB = nullptr;    // 정점 버퍼
//
//											// 사용자 정의 정점 구조체
//struct CUSTOMVERTEX {
//	FLOAT x, y, z; // rhw는 동차 좌표계의 w값으로 이 값이 있으면 변환이 완료된 정점을 의미한다.
//	DWORD color;        // diffuse color, 정점의 색깔을 나타낸다.
//};
//
//// 사용자가 설정한 정점에 대한 정보를 나타내는 FVF 값
//// x,y,z,w 값과 diffuse color를 나타내는 flag 값을 설정하였다.
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
//	// 윈도우 클래스 선언 및 초기화, 등록
//	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "D3D Tutorial", NULL };
//	RegisterClassEx(&wc);
//
//	// 윈도우 생성
//HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 01: CreateDevice", WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
//
//// Direct3D 초기화
//if (SUCCEEDED(InitD3D(hWnd))) {
//	// 정점 버퍼 초기화
//	if (SUCCEEDED(InitVB())) {
//		// 윈도우 출력
//		ShowWindow(hWnd, SW_SHOWDEFAULT);
//		UpdateWindow(hWnd);
//
//		// 메세지 루프
//		MSG msg;
//		ZeroMemory(&msg, sizeof(msg));
//
//		while (msg.message != WM_QUIT) {
//			// GetMessage() 함수의 달리 대기하지 않는다.
//			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
//				TranslateMessage(&msg);
//				DispatchMessage(&msg);
//			}
//			else {
//				// 처리할 메세지가 없으면 렌더링한다.
//				Render();
//			}
//
//		}
//	}
//}
//
//// 등록한 클래스의 메모리를 반환하고, 등록에서 제외한다.
//UnregisterClass("D3D Tutorial", wc.hInstance);
//
//return 0;
//}
//
//HRESULT InitD3D(HWND hWnd)
//{
//	// D3D 객체 생성
//	if (nullptr == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
//		return E_FAIL;
//	}
//
//	// D3D 디바이스를 생성하기 위해서 필요한 구조체
//	D3DPRESENT_PARAMETERS d3dpp;
//	ZeroMemory(&d3dpp, sizeof(d3dpp));  // 0으로 초기화하지 않으면 결과가 나오지 않을 수 있다.
//
//	d3dpp.Windowed = TRUE;  // 창모드
//	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
//
//	// D3DDEVTYPE_HAL은 하드웨어 가속을 지원하도록 설정한다. 
//	// 전역변수 g_pd3dDevice에 D3D 디바이스 포인터를 저장한다. 
//	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice))) {
//		return E_FAIL;
//	}
//
//	// 삼각형의 뒷면을 표시하기 위한 코드
//	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	// 정점이 diffuse color를 가지고 있므로 빛을 껐다.
//	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	return S_OK;
//}
//
//// 정점 버퍼 초기화
//HRESULT InitVB()
//{
//	CUSTOMVERTEX vertices[] = {
//		{ -1.f, -1.f, 0.0f, 0xffff0000, },
//		{ 1.f, -1.f, 0.0f, 0xff0000ff, },
//		{ 0.f, 1.0f, 0.0f, 0xffffffff, },
//	};
//
//	// 정점 버퍼 생성
//	if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(vertices), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL))) {
//		return E_FAIL;
//	}
//
//	// 생성한 정점 버퍼에 값을 채운다.
//	// Lock(), Unlock() 함수를 사용해야 한다.
//	VOID* pVertices;
//	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0))) {
//		return E_FAIL;
//	}
//	memcpy(pVertices, vertices, sizeof(vertices));
//	g_pVB->Unlock();
//
//	return S_OK;
//}
//
//// 객체 리소스를 반환하는 함수
//VOID Cleanup()
//{
//	// 객체의 리소스를 반환하는 순서가 중요하다.
//	// 이 순서가 잘못되면 에러를 발생시킨다.
//	// 생성의 역순
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
//// 화면을 그리는 함수
//VOID Render()
//{
//	if (nullptr == g_pd3dDevice) {
//		return;
//	}
//
//	// 후면 버퍼를 파란색으로 채운다.
//	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
//
//
//	// BeginScene() 함수로 디바이스에게 Rendering 시작을 알린다.
//	if (SUCCEEDED(g_pd3dDevice->BeginScene())) {
//
//		SetupMatrices();
//
//		// 정점 버퍼를 출력 스트림으로 설정한다.
//		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
//
//		// D3D에 정점 정보를 제공한다.
//		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
//
//		// 실제 기하 물체를 그린다.
//		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
//
//		// Rendering이 끝났음을 알린다.
//		g_pd3dDevice->EndScene();
//	}
//
//	// 후면 버퍼를 현재 그리는 버퍼로 설정한다.
//	// Double Buffering(이중 버퍼)와 관련된 내용이다.
//	// 이중 버퍼를 사용하면서 이 함수를 실행하지 않으면 제대로 그려지지 않는다.
//	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
//}
//
//// 월드, 뷰, 프로젝션 행렬 설정
//VOID SetupMatrices()
//{
//	// 월드 행렬 설정
//	D3DXMATRIXA16 matWorld;
//
//	UINT iTime = timeGetTime() % 1000;
//	FLOAT fAngle = iTime * (2.f * D3DX_PI) / 1000.f;
//	D3DXMatrixRotationY(&matWorld, fAngle);
//	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
//
//	// 뷰 행렬 설정
//	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -5.0f);  // 카메라의 위치
//	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);    // 카메라가 바라보는 지점
//	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);   // 카메라의 상향벡터
//
//	D3DXMATRIXA16 matView;
//	// 카메라 변환 행렬 계산
//	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
//	// 계산된 카메라 변환 행렬을 적용
//	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
//
//	// 프로젝션 행렬 설정
//	D3DXMATRIXA16 matProj;
//	// 투영 변환 행렬 계산
//	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.f);
//	// 계산된 투영 변환 행렬을 적용
//	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
//}
//
//// 윈도우 프로시저 함수
//LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg) {
//	case WM_DESTROY:
//		Cleanup();
//		PostQuitMessage(0);
//		return 0;
//
//		// 메세지 루프 변경
//		/*case WM_PAINT:
//		Render();
//		ValidateRect(hWnd, NULL);
//		return 0;*/
//	}
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
