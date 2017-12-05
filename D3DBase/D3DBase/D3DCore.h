#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>														//dx9 �� ����ϴµ� ������ - ����ִ� ���̺귯���� ������ �Űܼ� �߰�������.
//#include<d3d9.h>


#define WINNAME "D3DBase"
#define WINSIZEX 800
#define WINSIZEY 600
//��������
//g�� �����ϴ°� �۷ι�(����)

static LPDIRECT3D9					g_pD3D			= NULL;						//D3D ����̽��� ������ D3D ��ü ����
static LPDIRECT3DDEVICE9			g_pd3dDevice	= NULL;						//�������� ���� D3D ����̽�
static LPDIRECT3DVERTEXBUFFER9		g_pVB			= NULL;						//������ ������ ���� ���� - �������� ����ϱ����� ������ ����

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
//

//struct CUSTOMVERTEX {
//	FLOAT x, y, z;													//������ ���õ� ��ǥ��(rhw���� ������ ��ȯ�� �Ϸ�� �����̶�°�)
//																	//ps - rhw, x, y, z �� ������ ������ ������ FVF ��������� ����ȴٰ� �� -Ȯ��
//
//	DWORD color;														//���� ����
//};
////����� ���� ����ü�� ���� ������ ��Ÿ���� FVF ��
////����ü�� x,y,z���� diffuse �������� �̷�����ִٴ� ����
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct CUSTOMVERTEX {
	D3DXVECTOR3 position;				//������ 3���� ��ǥ
	D3DXVECTOR3 normal;					//������ ��������
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)


//D3D�ʱ�ȭ
HRESULT InitD3D(HWND hWnd) {
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {			//����̽��� �����ϱ� ���� D3D ��ü ����
		return E_FAIL;
	}
	/*~3������� ����*/
	//D3DPRESENT_PARAMETERS d3dpp;										//����̽� ������ ���� ����ü
	//ZeroMemory(&d3dpp, sizeof(d3dpp));									//zeromemory�� �Լ�����ü �����ʱ�ȭ
	//d3dpp.Windowed = TRUE;												//�ϴ� â���� ���� -Ȯ��
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//���� ȿ������ SWAP ȿ�� -Ȯ��
	//d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;							//���� ����ȭ�� ��忡 ���缭 �ĸ� ���� ����

	/*Z���� ����*/
	D3DPRESENT_PARAMETERS d3dpp;										//����̽� ������ ���� ����ü
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//zeromemory�� �Լ�����ü �����ʱ�ȭ
	d3dpp.Windowed = TRUE;												//�ϴ� â���� ���� -Ȯ��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//���� ȿ������ SWAP ȿ�� -Ȯ��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;							//���� ����ȭ�� ��忡 ���缭 �ĸ� ���� ����
	//z���� ����
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

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

	//g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//������ ������ �����Ƿ�, ��������� ���� -Ȯ��

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


	/*�ֺ�����(ambient light)*/
	//g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	/*������*/
	//D3DLIGHT9 light;
	//light.Type = D3DLIGHT_POINT;
	//light.Diffuse.r = 1.0f;
	//light.Diffuse.g = 1.0f;
	//light.Diffuse.b = 1.0f;
	//g_pd3dDevice->SetLight(0, &light);
	//g_pd3dDevice->LightEnable(0, TRUE);
	/*���⼺����*/
	//D3DLIGHT9 light;
	//light.Type = D3DLIGHT_DIRECTIONAL;
	//light.Diffuse.r = 1.0f;
	//light.Diffuse.g = 1.0f;
	//light.Diffuse.b = 1.0f;
	//g_pd3dDevice->SetLight(0, &light);
	//g_pd3dDevice->LightEnable(0, TRUE);
	/*��������*/
	//D3DLIGHT9 light;
	//light.Type = D3DLIGHT_SPOT;
	//light.Diffuse.r = 1.0f;
	//light.Diffuse.g = 1.0f;
	//light.Diffuse.b = 1.0f;
	//g_pd3dDevice->SetLight(0, &light);
	//g_pd3dDevice->LightEnable(0, TRUE);
	
	//�ش� �������� �����Լ� ������������ ����� ���� ��ȿ�ϰ� �������̴��� �ȼ����̴��� ����ϰԵǸ� �ʿ䰡 ��������.



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
//		{150.f,50.f,0.5f,1.f,D3DCOLOR_RGBA(255,0,0,255)},
//		{250.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(0,255,0,255)},
//		{50.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(0,0,255,255)},
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

/*�������� �ʱ�ȭ - ȸ������ ��� (�ǽ�3��)*/
//HRESULT InitGeometry() {
//	CUSTOMVERTEX vertices[] = {
//		//���� 0xffff0000 �� ���� ǥ���� �� �ִµ� ARGB�� �����̴�.
//		{	-1.0f	,-1.0f	,0.0f, 0xffff0000, },
//		{	1.0f	,-1.0f	,0.0f, 0xff0000ff, },
//		{	0.0f	,1.0f	,0.0f, 0xffffffff, },
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

/*Z���� ���� ��������(�ǽ�4��)*/
HRESULT InitGeometry() {
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL))) return E_FAIL;

	//�˰����� ����ؼ� �Ǹ����� �����.
	CUSTOMVERTEX* pVertices;
	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0))) return E_FAIL;			//LOCK
	for (DWORD i = 0; i < 50; i++) {
		FLOAT theta = (2 * D3DX_PI*i) / (50 - 1);
		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));		//�Ǹ��� �Ʒ��� ������ ��ǥ

		pVertices[2 * i + 0].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));			//�Ǹ��� �Ʒ��� ������ ��������

		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));		//�Ǹ��� ���� ������ ��ǥ

		pVertices[2 * i + 1].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));			//�Ǹ��� ���� ������ ��������
	}
	g_pVB->Unlock();																//UNLOCK
	
	return S_OK;
}

//��� ����
//����� �� ���� �ִµ�, ���� ����, ��, �������� ����̴�.
VOID SetupMatrices()
{
	/* ���� ��� ����*/
	/*�ǽ� 3��*/
	//D3DXMATRIXA16 matWorld;
	//UINT iTime = timeGetTime() % 1000;
	//FLOAT fAngle = iTime * (2.f * D3DX_PI) / 1000.f;
	//D3DXMatrixRotationY(&matWorld, fAngle);
	//g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	/*�ǽ� 4��*/
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);								//��������� ������ķ� ������. -Ȯ��
	D3DXMatrixRotationX(&matWorld, timeGetTime() / 500.f);		//X���� �߽����� ȸ����� ����
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// �� ��� ����
	D3DXMATRIXA16 matView;

	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);  // ī�޶��� ��ġ
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);    // ī�޶� �ٶ󺸴� ����
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);   // ī�޶��� ���⺤��

	// ī�޶� ��ȯ ��� ���
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	// ���� ī�޶� ��ȯ ����� ����
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// �������� ��� ����
	D3DXMATRIXA16 matProj;
	// ���� ��ȯ ��� ���
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.f);
	// ���� ���� ��ȯ ����� ����
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

//���� ����

VOID setupLights() {
	//���� ����.
	//������ ����̽��� �� �ϳ��� ������ �� �ִ�.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pd3dDevice->SetMaterial(&mtrl);

	//���� ����
	D3DXVECTOR3 vecDir;											//���⼺ ����(Directional light)�� ���� ���� ����
	D3DLIGHT9 light;											//���� ����ü
	ZeroMemory(&light, sizeof(D3DLIGHT9));						//����ü�� 0���� �ʱ�ȭ
	light.Type = D3DLIGHT_DIRECTIONAL;							//������ ����(��, ���⼺, ����Ʈ����Ʈ) -Ȯ��
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 1.0f;	//���� ��� ����
	vecDir = D3DXVECTOR3(
		cosf(timeGetTime() / 350.f),							//������ ����
		1.0f,
		sinf(timeGetTime()/350.f)
		);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);	//��ֶ����� -Ȯ��

	//������ ������ �������ͷ� �����
	light.Range = 1000.f;										//���� �ִ� ���ްŸ�
	g_pd3dDevice->SetLight(0, &light);							//�ش� ������ 0���ε����� ����
	g_pd3dDevice->LightEnable(0, TRUE);							//0�� ������ Ȱ��ȭ�Ѵ�
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			//���������� Ȱ��ȭ�Ѵ�.

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x002020);		//ȯ�汤��(ambient light)�� �� ����

}

//������
void release() {
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
void render() {
	if (g_pd3dDevice == NULL) {
		return;
	}
	/*2,3��*/
	//g_pd3dDevice->Clear(												//�ĸ� ���۸� rgb(0,0,255) �� ä����
	//	0,
	//	NULL,															//�Ƹ� Ŭ���� ���� ���� ��Ʈ�ε� -Ȯ��
	//	D3DCLEAR_TARGET,
	//	D3DCOLOR_XRGB(0, 128, 255),
	//	1.0f,
	//	0);
	/*4��*/
	g_pd3dDevice->Clear(												//�ĸ� ���۸� rgb(0,0,255) �� ä����
		0,
		NULL,															//�Ƹ� Ŭ���� ���� ���� ��Ʈ�ε� -Ȯ��
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,								//Z���۱��� �����ش�.
		D3DCOLOR_XRGB(0, 128, 255),
		1.0f,
		0);

	//������ ���� ����
	setupLights();

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
		/*2,3��*/
		//g_pd3dDevice->DrawPrimitive(
		//	//D3DPT_TRIANGLELIST,										//�׸��� Ÿ�Ե� �ε� -Ȯ��
		//	D3DPT_TRIANGLESTRIP,										//�׸��� Ÿ�Ե� �ε� �̰Ŵ� ������ ����°� -Ȯ��
		//	0,															//��𼭺��� �׷�������
		//	1															//��� �ﰢ���� �׸�����? -Ȯ��
		//);
		/*4��*/
		g_pd3dDevice->DrawPrimitive(
			//D3DPT_TRIANGLELIST,										//�׸��� Ÿ�Ե� �ε� -Ȯ��
			D3DPT_TRIANGLESTRIP,										//�׸��� Ÿ�Ե� �ε� �̰Ŵ� ������ ����°� -Ȯ��
			0,															//��𼭺��� �׷�������
			2*50-2															//��� �ﰢ���� �׸�����? -Ȯ��
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
