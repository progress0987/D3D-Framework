/***********************************
 *	D3D �⺻ �����ӿ�ũ
 *	20171201 David Kim
 *	�ذ�å ���� ���� ����
 ***********************************/


#define WINNAME "D3DBase"
#define WINSIZEX 800
#define WINSIZEY 600
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>														//dx9 �� ����ϴµ� ������ - ����ִ� ���̺귯���� ������ �Űܼ� �߰�������.
 //#include<d3d9.h>


 //��������
 //g�� �����ϴ°� �۷ι�(����)

LPDIRECT3D9					g_pD3D = NULL;						//D3D ����̽��� ������ D3D ��ü ����
LPDIRECT3DDEVICE9			g_pd3dDevice = NULL;						//�������� ���� D3D ����̽�
LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;						//������ ������ ���� ���� - �������� ����ϱ����� ������ ����

																		//��������� ���� ����ü
//struct CUSTOMVERTEX {
//	FLOAT x, y, z, rhw;													//������ ���õ� ��ǥ��(rhw���� ������ ��ȯ�� �Ϸ�� �����̶�°�)
//																		//ps - rhw, x, y, z �� ������ ������ ������ FVF ��������� ����ȴٰ� �� -Ȯ��
//
//	DWORD color;														//���� ����
//};
////����� ���� ����ü�� ���� ������ ��Ÿ���� FVF ��
////����ü�� x,y,z,rhw���� diffuse �������� �̷�����ִٴ� ����
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
//////////////////////////////2������� rhw�ִ°��� �����.

struct CUSTOMVERTEX {
	FLOAT x, y, z;													//������ ���õ� ��ǥ��(rhw���� ������ ��ȯ�� �Ϸ�� �����̶�°�)
																		//ps - rhw, x, y, z �� ������ ������ ������ FVF ��������� ����ȴٰ� �� -Ȯ��

	DWORD color;														//���� ����
};
//����� ���� ����ü�� ���� ������ ��Ÿ���� FVF ��
//����ü�� x,y,z,rhw���� diffuse �������� �̷�����ִٴ� ����
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)




HRESULT InitD3D(HWND hWnd);
HRESULT InitVB();
HRESULT InitGeometry();
VOID release();
VOID render();
VOID SetupMatrices();
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//WINAPI

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT) {
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L,0L,	//������ Ŭ���� ��� -Ȯ��
		GetModuleHandle(NULL),NULL,NULL,NULL,NULL,WINNAME,NULL };
	RegisterClassEx(&wc);

	HWND	hWnd = CreateWindow(WINNAME,								//������ ����
		WINNAME,														//������ �̸�
		WS_OVERLAPPEDWINDOW,
		100,															//������ ������ X
		100,															//������ ������ Y
		WINSIZEX,														//������ ��
		WINSIZEY,														//������ ����
		GetDesktopWindow(),												//�θ� ������ �ڵ� -Ȯ��
																		//NULL,
		NULL,															//�޴� ������� -Ȯ��
		wc.hInstance,													//������Ŭ������ �ν��Ͻ�
		NULL);															//LPARAM

																		//D3D �ʱ�ȭ
	if (SUCCEEDED(InitD3D(hWnd))) {
		//if (SUCCEEDED(InitVB())) {									//VB�� �Ǹ� ����ϴ°�(�ǽ� 2��)
		if (SUCCEEDED(InitGeometry())) {								//geometry�� �Ǹ� ����ϴ°�(�ǽ� 3��)
			ShowWindow(hWnd, SW_SHOWDEFAULT);							//������ ���
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
	UnregisterClass(WINNAME, wc.hInstance);								//��ϵ� Ŭ���� �Ұ�
	return 0;
}
//D3D�ʱ�ȭ
HRESULT InitD3D(HWND hWnd) {
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {			//����̽��� �����ϱ� ���� D3D ��ü ����
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;										//����̽� ������ ���� ����ü
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//zeromemory�� �Լ�����ü �����ʱ�ȭ
	d3dpp.Windowed = TRUE;												//�ϴ� â���� ���� -Ȯ��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//���� ȿ������ SWAP ȿ�� -Ȯ��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;							//���� ����ȭ�� ��忡 ���缭 �ĸ� ���� ����

																		/*
																		����̽� ���� ����
																		1. ����Ʈ ����ī�带 ����Ѵ� - 2���̻�Ǹ� ��Ե���? -Ȯ��
																		2. HAL ����̽��� �����Ѵ�(HW ������ġ�� ����ϰڴٴ� �ǹ̷� ����)->���ӾȵǸ�? -Ȯ��
																		3. ���� ó���� ��� ī�忡�� �����ϴ� SW ó���� �����Ѵ�(HW�� �����ϸ� ������ �� ��������
																		*/

	if (FAILED(g_pD3D->CreateDevice(									//-Ȯ�� �ǹ̵�üũ�غ��� ����̽������� �ٲ�°� �ִ���.
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pd3dDevice))) {
		return E_FAIL;
	}
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//�ø� ����� ����. �ﰢ���� �ո�, �޸� ��� �������Ѵ� -Ȯ��

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//������ ������ �����Ƿ�, ��������� ���� -Ȯ��

	return S_OK;														//����̽� �������� ó�� �Ϸ�
}

//���� ������ �ʱ�ȭ (�ǽ�2��)
/*	���� ���۸� �����ϰ� ���� ������ ä���ִ´�.

*	���� ���۶� �⺻������ ���� ������ �����ִ� �޸��� ����̴�.

*	���� ���۸� ������ �Ŀ� �ݵ�� Lock()�� Unlock()���� �����͸� ���� ���������� ���� ���ۿ� ��־�� �Ѵ�.

*	���� D3D�� �ε��� ���۵� ����� �����ϴٴ� ���� �������. -Ȯ��

*	���� ���۳� �ε��� ���۴� �⺻ �ý��� �޸� �ܿ� ����̽� �޸�(����ī���� �޸�) �� ������ �� �ִµ�
��κ��� ����ī�忡���� �̷��� �� ��� ��û�� �ӵ��� ����� ���� �� �ִ�.
*/
//HRESULT InitVB() {
//	//�ﰢ���� �������ϱ� ���� ���� ���� -���� -�ӽ�
//	CUSTOMVERTEX vertices[] = {
//		//���� 0xffff0000 �� ���� ǥ���� �� �ִµ� ARGB�� �����̴�.
//		{ 150.f,50.f,0.5f,1.f,D3DCOLOR_RGBA(255,0,0,255) },
//		{ 250.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(0,255,0,255) },
//		{ 50.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(0,0,255,255) },
//
//		//{250.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(0,255,0,255)},
//		//{350.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(255,0,0,255)},
//		//{150.f,50.f,0.5f,1.f,D3DCOLOR_RGBA(0,0,255,255)},
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

//�������� �ʱ�ȭ - ȸ������ ��� (�ǽ�3��)
HRESULT InitGeometry() {
	CUSTOMVERTEX vertices[] = {
		//���� 0xffff0000 �� ���� ǥ���� �� �ִµ� ARGB�� �����̴�.
		{ -1.0f	,-1.0f	,0.0f, 0xffff0000, },
		{ 1.0f	,-1.0f	,0.0f, 0xff0000ff, },
		{ 0.0f	,1.0f	,0.0f, 0xffffffff, },
	};

	/*
	���� ���� ����

	3���� ������������� ������ �޸𸮸� �Ҵ��Ѵ�.
	FVF�� �����Ͽ� ������ �������� ������ �����Ѵ�.
	FVF - Flexible Vertex Format		- ���������� ���ؽ��� �����Ѵٰ� �����Ҷ�
	*/
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(
		3 * sizeof(CUSTOMVERTEX),										//������ ���������� ����Ʈ ���� ũ��
		0,																//���� ������ ���� Ȥ�� ó�����(SW/HW) ����
		D3DFVF_CUSTOMVERTEX,											//���� ���� ����ü�� ���� ����� FVF �÷��� ��
		D3DPOOL_DEFAULT,												//���� ���۰� ����� �޸��� ��ġ(����ī������ �ý��۸޸����� ��)�� ������� ����
		&g_pVB,															//��ȯ�� ���� ������ �������̽�
		NULL															//����Ǿ�����(�����Ǿ�����) ����� ���� ������ NULL
	))) {
		return E_FAIL;
	}
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(												//����ϱ� ���ؼ� ��
		0,																//Lock�� �� ������ ������, �Ʒ��� �Բ� ���ʸ�� 0�̸� ��ü ����
		sizeof(vertices),												//Lock�� �� ������ ũ��, ���� �Բ� ���ʸ�� 0�̸� ��ü ����
		(void**)&pVertices,												//�а� ���� �����Ϸ��� �޸� ������ ������
		0																//Lock ����� �Բ� ���Ǵ� �÷���
	))) return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));											//���ؽ����� �����سִ´�

	g_pVB->Unlock();																		//����� Ǯ���� ���

	return S_OK;
}

//������
VOID release() {
	if (g_pVB != NULL) {
		g_pVB->Release();
	}
	if (g_pd3dDevice != NULL) {
		g_pd3dDevice->Release();
	}
	if (g_pD3D != NULL) {
		g_pD3D->Release();
	}
}

//����
VOID render() {
	if (g_pd3dDevice == NULL) {
		return;
	}
	g_pd3dDevice->Clear(												//�ĸ� ���۸� rgb(0,0,255) �� ä����
		0,
		NULL,															//�Ƹ� Ŭ���� ���� ���� ��Ʈ�ε� -Ȯ��
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 255),
		1.0f,
		0);
	//����,��,�������� ����� ����
	SetupMatrices();

	if (SUCCEEDED(g_pd3dDevice->BeginScene())) {						//������ ���� (����������(beginscene) �� ������ ��(endscene)
																		//���̿��� �ִ��� �����ϰ��ؾ� �ڵ� �ӵ��� ��������.

																		//���ϴ� ���� ���μ������� ����� ��
																		////////////////////////�׽�Ʈ �κ�


																		//���������� �ﰢ���� �׸��� ����
																		//1. ���� ������ ����ִ� ���� ���۸� ��� ��Ʈ������ �Ҵ��Ѵ�
		g_pd3dDevice->SetStreamSource(
			0,															//��Ʈ���� ���̵������ε�
			g_pVB,														//���ؽ����� ����
			0,															//��𼭺��� �о���� ������
			sizeof(CUSTOMVERTEX)										//�ѹ��� �о���� ũ��(�ѹ��� �о���� ������ ũ���ε�)
		);
		//2. D3D�� ���� ���̴� ������ �����Ѵ�. ��κ��� ��쿡�� FVF�� �����Ѵ�.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		//3. ���� ������ ����ϱ� ���� DrawPrimitive() �Լ� ȣ��
		g_pd3dDevice->DrawPrimitive(
			//D3DPT_TRIANGLELIST,										//�׸��� Ÿ�Ե� �ε� -Ȯ��
			D3DPT_TRIANGLESTRIP,										//�׸��� Ÿ�Ե� �ε� �̰Ŵ� ������ ����°� -Ȯ��
			0,															//��𼭺��� �׷�������
			1															//��� �ﰢ���� �׸�����? -Ȯ��
		);

		g_pd3dDevice->EndScene();										//�׸��� ����
	}

	g_pd3dDevice->Present(												//�ĸ���۸� ȭ������ ��� -Ȯ��
		NULL,															//�ҽ� ��Ʈ
		NULL,															//��� ��Ʈ
		NULL,
		NULL
	);
}
//��� ����
//����� �� ���� �ִµ�, ���� ����, ��, �������� ����̴�.
VOID SetupMatrices() {
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� ���
	D3DXMATRIXA16 matWorld;

	UINT iTime = timeGetTime() % 1000;									//float ������ ���е��� ���ؼ� 1000���� ������ ����(1��)
	FLOAT fAngle = iTime*(2.0f*D3DX_PI) / 1000.f;						//1�ʸ��� �ѹ�����(2*pi) ȸ��
	D3DXMatrixRotationY(&matWorld, fAngle);								//Y��������� ȸ������� ����

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);					//������ ȸ������� ������ķ� ����̽��� ����

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�� ���

	//�� ����� �����ϱ� ���ؼ��� ������ ���� �ʿ�.
	D3DXMATRIXA16 matView;

	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);									//1. ���� ��ġ (0,3,-5)
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);								//2. ���� �ٶ󺸴� ��ġ(0,0,0)
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);									//3. õ�������� ��Ÿ���� ��溤��(0,1,0) - �Ƹ��� �ϴúκ��� �������ִºκ��ε�? -Ȯ��
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);				//1,2,3�� ����� �� ��� ���� - �¼���ǥ��(LH)

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);					//������ �� ����� ����̽��� ����

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�������� ���

	//�������� ����� �����ϱ� ���ؼ��� �þ߰�(FOV - Field of View)�� ��Ⱦ��(aspect ratio), Ŭ���� ����� ���� �ʿ��ϴ�.
	D3DXMATRIXA16 matProj;
	/*
	matProj			���� ������ ���
	D3DX_PI/4		FOV(45��)
	1.0f			��Ⱦ��
	1.0f			���� Ŭ���� ���(near clipping plane)
	100.f			���Ÿ� Ŭ���� ���(far clipping plane)
	*/
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.f, 1.f, 100.f);	//�¼���ǥ��(LH)

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}


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

/////////////////////////////////////////////////////////////////////////////////////////////
