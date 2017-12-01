#pragma once

#include <d3d9.h>

#define WINNAME "D3DBase"
#define WINSIZEX 800
#define WINSIZEY 600

//��������
//g�� �����ϴ°� �۷ι�(����)

LPDIRECT3D9					g_pD3D			= NULL;						//D3D ����̽��� ������ D3D ��ü ����
LPDIRECT3DDEVICE9			g_pd3dDevice	= NULL;						//�������� ���� D3D ����̽�
LPDIRECT3DVERTEXBUFFER9		g_pVB			= NULL;						//������ ������ ���� ����

//��������� ���� ����ü
struct CUSTOMVERTEX {
	FLOAT x, y, z, rhw;													//������ ���õ� ��ǥ��(rhw���� ������ ��ȯ�� �Ϸ�� �����̶�°�)
																		//ps - rhw, x, y, z �� ������ ������ ������ FVF ��������� ����ȴٰ� �� -Ȯ��

	DWORD color;														//���� ����
};
//����� ���� ����ü�� ���� ������ ��Ÿ���� FVF ��
//����ü�� x,y,z,rhw���� diffuse �������� �̷�����ִٴ� ����
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


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

	return S_OK;														//����̽� �������� ó�� �Ϸ�
}

//���� ������ �ʱ�ȭ
/*	���� ���۸� �����ϰ� ���� ������ ä���ִ´�.

 *	���� ���۶� �⺻������ ���� ������ �����ִ� �޸��� ����̴�.

 *	���� ���۸� ������ �Ŀ� �ݵ�� Lock()�� Unlock()���� �����͸� ���� ���������� ���� ���ۿ� ��־�� �Ѵ�.

 *	���� D3D�� �ε��� ���۵� ����� �����ϴٴ� ���� �������. -Ȯ��

 *	���� ���۳� �ε��� ���۴� �⺻ �ý��� �޸� �ܿ� ����̽� �޸�(����ī���� �޸�) �� ������ �� �ִµ�
	��κ��� ����ī�忡���� �̷��� �� ��� ��û�� �ӵ��� ����� ���� �� �ִ�.
*/
HRESULT InitVB() {
	//�ﰢ���� �������ϱ� ���� ���� ���� -���� -�ӽ�
	CUSTOMVERTEX vertices[] = {
		{150.f,50.f,0.5f,1.f,0xffff0000},
		{250.f,250.f,0.5f,1.f,0xff00ff00},
		{50.f,250.f,0.5f,1.f,0xff00ffff},
	};

	/*
	���� ���� ����

	3���� ������������� ������ �޸𸮸� �Ҵ��Ѵ�.
	FVF�� �����Ͽ� ������ �������� ������ �����Ѵ�.
	*/
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),	//���� ũ��
		0,																//��뷮? -Ȯ��
		D3DFVF_CUSTOMVERTEX,											//FVF ����(Ŀ������ �տ��� ������) FVF�� ���� -Ȯ��
		D3DPOOL_DEFAULT,												//D3D POOL? -Ȯ��
		&g_pVB,															//���ؽ� �����ϴ� ����
		NULL
	))) {
		return E_FAIL;
	}
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0))) return E_FAIL;		//����ϱ� ���ؼ� ��
	
	memcpy(pVertices, vertices, sizeof(vertices));											//���ؽ����� �����سִ´�

	g_pVB->Unlock();																		//����� Ǯ���� ���

	return S_OK;
}

//������
void release() {
	if (g_pd3dDevice != NULL) {
		g_pd3dDevice->Release();
	}
	if (g_pD3D != NULL) {
		g_pD3D->Release();
	}
}

//����
void render() {
	if (g_pd3dDevice == NULL) {
		return;
	}
	g_pd3dDevice->Clear(												//�ĸ� ���۸� rgb(0,0,255) �� ä����
		0,
		NULL,															//�Ƹ� Ŭ���� ���� ���� ��Ʈ�ε� -Ȯ��
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 255),
		1.0f,
		0);

	if (SUCCEEDED(g_pd3dDevice->BeginScene())) {						//������ ����
																		//���� ���μ������� ����� ��
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
			D3DPT_TRIANGLELIST,											//�׸��� Ÿ�Ե� �ε� -Ȯ��
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
