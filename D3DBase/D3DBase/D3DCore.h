#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>														//dx9 로 써야하는데 에러남 - 깔려있던 라이브러리를 강제로 옮겨서 추가해줬음.
//#include<d3d9.h>


#define WINNAME "D3DBase"
#define WINSIZEX 800
#define WINSIZEY 600
//전역변수
//g로 시작하는건 글로벌(전역)

static LPDIRECT3D9					g_pD3D			= NULL;						//D3D 디바이스를 생성할 D3D 객체 변수
static LPDIRECT3DDEVICE9			g_pd3dDevice	= NULL;						//렌더링에 사용될 D3D 디바이스
static LPDIRECT3DVERTEXBUFFER9		g_pVB			= NULL;						//정점을 보관할 정점 버퍼 - 정점만을 계산하기위해 생성된 버퍼

//사용자정의 정점 구조체

//struct CUSTOMVERTEX {
//	FLOAT x, y, z, rhw;													//정점과 관련된 좌표들(rhw값이 있으면 변환이 완료된 정점이라는것)
//																		//ps - rhw, x, y, z 의 순서로 변수를 잡으면 FVF 선언순서에 위배된다고 함 -확인
//
//	DWORD color;														//정점 색깔
//};
////사용자 정점 구조체에 관한 정보를 나타내는 FVF 값
////구조체는 x,y,z,rhw값과 diffuse 색깔값으로 이루어져있다는 선언
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
//

//struct CUSTOMVERTEX {
//	FLOAT x, y, z;													//정점과 관련된 좌표들(rhw값이 있으면 변환이 완료된 정점이라는것)
//																	//ps - rhw, x, y, z 의 순서로 변수를 잡으면 FVF 선언순서에 위배된다고 함 -확인
//
//	DWORD color;														//정점 색깔
//};
////사용자 정점 구조체에 관한 정보를 나타내는 FVF 값
////구조체는 x,y,z값과 diffuse 색깔값으로 이루어져있다는 선언
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct CUSTOMVERTEX {
	D3DXVECTOR3 position;				//정점의 3차원 좌표
	D3DXVECTOR3 normal;					//정점의 법선벡터
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)


//D3D초기화
HRESULT InitD3D(HWND hWnd) {
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {			//디바이스를 생성하기 위한 D3D 객체 생성
		return E_FAIL;
	}
	/*~3장까지의 구조*/
	//D3DPRESENT_PARAMETERS d3dpp;										//디바이스 생성을 위한 구조체
	//ZeroMemory(&d3dpp, sizeof(d3dpp));									//zeromemory로 함수구조체 완전초기화
	//d3dpp.Windowed = TRUE;												//일단 창모드로 생성 -확인
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//가장 효율적인 SWAP 효과 -확인
	//d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;							//현재 바탕화면 모드에 맞춰서 후면 버퍼 생성

	/*Z버퍼 포함*/
	D3DPRESENT_PARAMETERS d3dpp;										//디바이스 생성을 위한 구조체
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//zeromemory로 함수구조체 완전초기화
	d3dpp.Windowed = TRUE;												//일단 창모드로 생성 -확인
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//가장 효율적인 SWAP 효과 -확인
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;							//현재 바탕화면 모드에 맞춰서 후면 버퍼 생성
	//z버퍼 관련
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	/*
	디바이스 생성 설정
	1. 디폴트 비디오카드를 사용한다 - 2개이상되면 어떻게될지? -확인
	2. HAL 디바이스를 생성한다(HW 가속장치를 사용하겠다는 의미로 사용됨)->가속안되면? -확인
	3. 정범 처리는 모든 카드에서 지원하는 SW 처리로 생성한다(HW로 생성하면 성능이 더 높아진다
	*/

	if (FAILED(g_pD3D->CreateDevice(									//-확인 의미들체크해볼것 디바이스에따라 바뀌는게 있는지.
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pd3dDevice))) {
		return E_FAIL;
	}
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//컬링 기능을 끈다. 삼각형의 앞면, 뒷면 모두 렌더링한다 -확인

	//g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//정점의 색깔값이 있으므로, 광원기능을 끈다 -확인

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


	/*주변광원(ambient light)*/
	//g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	/*점광원*/
	//D3DLIGHT9 light;
	//light.Type = D3DLIGHT_POINT;
	//light.Diffuse.r = 1.0f;
	//light.Diffuse.g = 1.0f;
	//light.Diffuse.b = 1.0f;
	//g_pd3dDevice->SetLight(0, &light);
	//g_pd3dDevice->LightEnable(0, TRUE);
	/*방향성광원*/
	//D3DLIGHT9 light;
	//light.Type = D3DLIGHT_DIRECTIONAL;
	//light.Diffuse.r = 1.0f;
	//light.Diffuse.g = 1.0f;
	//light.Diffuse.b = 1.0f;
	//g_pd3dDevice->SetLight(0, &light);
	//g_pd3dDevice->LightEnable(0, TRUE);
	/*점적광원*/
	//D3DLIGHT9 light;
	//light.Type = D3DLIGHT_SPOT;
	//light.Diffuse.r = 1.0f;
	//light.Diffuse.g = 1.0f;
	//light.Diffuse.b = 1.0f;
	//g_pd3dDevice->SetLight(0, &light);
	//g_pd3dDevice->LightEnable(0, TRUE);
	
	//해당 광원들은 고정함수 파이프라인을 사용할 때만 유효하고 정점셰이더와 픽셀셰이더를 사용하게되면 필요가 없어진다.



	return S_OK;														//디바이스 상태정보 처리 완료
}

//정점 버퍼의 초기화 (실습2장)
/*	정점 버퍼를 생성하고 정점 값들을 채워넣는다.

 *	정점 버퍼란 기본적으로 정점 정보를 갖고있는 메모리의 블록이다.

 *	정점 버퍼를 생성한 후에 반드시 Lock()과 Unlock()으로 포인터를 얻어내어 정점정보를 정점 버퍼에 써넣어야 한다.

 *	또한 D3D는 인덱스 버퍼도 사용이 가능하다는 것을 명심하자. -확인

 *	정점 버퍼나 인덱스 버퍼는 기본 시스템 메모리 외에 디바이스 메모리(비디오카드의 메모리) 에 생성될 수 있는데
	대부분의 비디오카드에서는 이렇게 할 경우 엄청난 속도의 향상을 얻을 수 있다.
*/
//HRESULT InitVB() {
//	//삼각형을 렌더링하기 위해 정점 선언 -수정 -임시
//	CUSTOMVERTEX vertices[] = {
//		//색은 0xffff0000 과 같이 표현할 수 있는데 ARGB의 순서이다.
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
//	정점 버퍼 생성
//
//	3개의 사용자정점들을 보관할 메모리를 할당한다.
//	FVF를 지정하여 보관할 데이터의 형식을 지정한다.
//	FVF - Flexible Vertex Format		- 유동적으로 버텍스를 지정한다고 선언할때
//	*/
//	if (FAILED(g_pd3dDevice->CreateVertexBuffer(
//		3 * sizeof(CUSTOMVERTEX),										//생성할 정점버퍼의 바이트 단위 크기
//		0,																//정점 버퍼의 종류 혹은 처리방식(SW/HW) 지정
//		D3DFVF_CUSTOMVERTEX,											//정점 정보 구조체에 따라 선언된 FVF 플래그 값
//		D3DPOOL_DEFAULT,												//정점 버퍼가 저장될 메모리의 위치(비디오카드인지 시스템메모리인지 등)와 관리방식 지정
//		&g_pVB,															//반환될 정점 버퍼의 인터페이스
//		NULL															//예약되었는지(공유되었는지) 현재는 거의 무조건 NULL
//	))) {
//		return E_FAIL;
//	}
//	VOID* pVertices;
//	if (FAILED(g_pVB->Lock(												//사용하기 위해서 락
//		0,																//Lock을 할 버퍼의 시작점, 아래와 함께 양쪽모두 0이면 전체 버퍼
//		sizeof(vertices),												//Lock을 할 버퍼의 크기, 위와 함께 양쪽모두 0이면 전체 버퍼
//		(void**)&pVertices,												//읽고 쓰기 수행하려는 메모리 영역의 포인터
//		0																//Lock 수행과 함께 사용되는 플래그
//	))) return E_FAIL;	
//	
//	memcpy(pVertices, vertices, sizeof(vertices));											//버텍스들을 복사해넣는다
//
//	g_pVB->Unlock();																		//사용이 풀려서 언락
//
//	return S_OK;
//}

/*기하정보 초기화 - 회전에서 사용 (실습3장)*/
//HRESULT InitGeometry() {
//	CUSTOMVERTEX vertices[] = {
//		//색은 0xffff0000 과 같이 표현할 수 있는데 ARGB의 순서이다.
//		{	-1.0f	,-1.0f	,0.0f, 0xffff0000, },
//		{	1.0f	,-1.0f	,0.0f, 0xff0000ff, },
//		{	0.0f	,1.0f	,0.0f, 0xffffffff, },
//	};
//
//	/*
//	정점 버퍼 생성
//
//	3개의 사용자정점들을 보관할 메모리를 할당한다.
//	FVF를 지정하여 보관할 데이터의 형식을 지정한다.
//	FVF - Flexible Vertex Format		- 유동적으로 버텍스를 지정한다고 선언할때
//	*/
//	if (FAILED(g_pd3dDevice->CreateVertexBuffer(
//		3 * sizeof(CUSTOMVERTEX),										//생성할 정점버퍼의 바이트 단위 크기
//		0,																//정점 버퍼의 종류 혹은 처리방식(SW/HW) 지정
//		D3DFVF_CUSTOMVERTEX,											//정점 정보 구조체에 따라 선언된 FVF 플래그 값
//		D3DPOOL_DEFAULT,												//정점 버퍼가 저장될 메모리의 위치(비디오카드인지 시스템메모리인지 등)와 관리방식 지정
//		&g_pVB,															//반환될 정점 버퍼의 인터페이스
//		NULL															//예약되었는지(공유되었는지) 현재는 거의 무조건 NULL
//	))) {
//		return E_FAIL;
//	}
//	VOID* pVertices;
//	if (FAILED(g_pVB->Lock(												//사용하기 위해서 락
//		0,																//Lock을 할 버퍼의 시작점, 아래와 함께 양쪽모두 0이면 전체 버퍼
//		sizeof(vertices),												//Lock을 할 버퍼의 크기, 위와 함께 양쪽모두 0이면 전체 버퍼
//		(void**)&pVertices,												//읽고 쓰기 수행하려는 메모리 영역의 포인터
//		0																//Lock 수행과 함께 사용되는 플래그
//	))) return E_FAIL;
//
//	memcpy(pVertices, vertices, sizeof(vertices));											//버텍스들을 복사해넣는다
//
//	g_pVB->Unlock();																		//사용이 풀려서 언락
//
//	return S_OK;
//}

/*Z깊이 포함 정점버퍼(실습4장)*/
HRESULT InitGeometry() {
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL))) return E_FAIL;

	//알고리즘을 사용해서 실린더를 만든다.
	CUSTOMVERTEX* pVertices;
	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0))) return E_FAIL;			//LOCK
	for (DWORD i = 0; i < 50; i++) {
		FLOAT theta = (2 * D3DX_PI*i) / (50 - 1);
		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));		//실린더 아래쪽 원통의 좌표

		pVertices[2 * i + 0].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));			//실린더 아래쪽 원통의 법선벡터

		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));		//실린더 위쪽 원통의 좌표

		pVertices[2 * i + 1].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));			//실린더 위쪽 원통의 법선벡터
	}
	g_pVB->Unlock();																//UNLOCK
	
	return S_OK;
}

//행렬 설정
//행렬은 세 개가 있는데, 각각 월드, 뷰, 프로젝션 행렬이다.
VOID SetupMatrices()
{
	/* 월드 행렬 설정*/
	/*실습 3장*/
	//D3DXMATRIXA16 matWorld;
	//UINT iTime = timeGetTime() % 1000;
	//FLOAT fAngle = iTime * (2.f * D3DX_PI) / 1000.f;
	//D3DXMatrixRotationY(&matWorld, fAngle);
	//g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	/*실습 4장*/
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);								//월드행렬을 단위행렬로 설정함. -확인
	D3DXMatrixRotationX(&matWorld, timeGetTime() / 500.f);		//X축을 중심으로 회전행렬 생성
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// 뷰 행렬 설정
	D3DXMATRIXA16 matView;

	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);  // 카메라의 위치
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);    // 카메라가 바라보는 지점
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);   // 카메라의 상향벡터

	// 카메라 변환 행렬 계산
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	// 계산된 카메라 변환 행렬을 적용
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// 프로젝션 행렬 설정
	D3DXMATRIXA16 matProj;
	// 투영 변환 행렬 계산
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.f);
	// 계산된 투영 변환 행렬을 적용
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

//광원 설정

VOID setupLights() {
	//재질 설정.
	//재질은 디바이스에 단 하나만 설정될 수 있다.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pd3dDevice->SetMaterial(&mtrl);

	//광원 설정
	D3DXVECTOR3 vecDir;											//방향성 광원(Directional light)이 향할 빛의 방향
	D3DLIGHT9 light;											//광원 구조체
	ZeroMemory(&light, sizeof(D3DLIGHT9));						//구조체를 0으로 초기화
	light.Type = D3DLIGHT_DIRECTIONAL;							//광원의 종류(점, 방향성, 스포트라이트) -확인
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 1.0f;	//색과 밝기 설정
	vecDir = D3DXVECTOR3(
		cosf(timeGetTime() / 350.f),							//광원의 방향
		1.0f,
		sinf(timeGetTime()/350.f)
		);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);	//노멀라이즈 -확인

	//광원의 방향을 단위벡터로 만든다
	light.Range = 1000.f;										//광원 최대 도달거리
	g_pd3dDevice->SetLight(0, &light);							//해당 광원을 0번인덱스에 설정
	g_pd3dDevice->LightEnable(0, TRUE);							//0번 광원을 활성화한다
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			//광원설정을 활성화한다.

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x002020);		//환경광원(ambient light)의 값 설정

}

//릴리즈
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

//렌더
void render() {
	if (g_pd3dDevice == NULL) {
		return;
	}
	/*2,3장*/
	//g_pd3dDevice->Clear(												//후면 버퍼를 rgb(0,0,255) 로 채워줌
	//	0,
	//	NULL,															//아마 클리어 해줄 범위 렉트인듯 -확인
	//	D3DCLEAR_TARGET,
	//	D3DCOLOR_XRGB(0, 128, 255),
	//	1.0f,
	//	0);
	/*4장*/
	g_pd3dDevice->Clear(												//후면 버퍼를 rgb(0,0,255) 로 채워줌
		0,
		NULL,															//아마 클리어 해줄 범위 렉트인듯 -확인
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,								//Z버퍼까지 지워준다.
		D3DCOLOR_XRGB(0, 128, 255),
		1.0f,
		0);

	//광원과 재질 설정
	setupLights();

	//월드,뷰,프로젝션 행렬을 설정
	SetupMatrices();

	if (SUCCEEDED(g_pd3dDevice->BeginScene())) {						//렌더링 시작 (렌더링시작(beginscene) 과 렌더링 끝(endscene)
																		//사이에는 최대한 간결하게해야 코드 속도가 높아진다.

																		//이하는 실제 프로세스들이 실행될 곳
		////////////////////////테스트 부분


		//정점버퍼의 삼각형을 그리기 시작
		//1. 정점 정보가 담겨있는 정점 버퍼를 출력 스트림으로 할당한다
		g_pd3dDevice->SetStreamSource(
			0,															//스트림의 아이디같은거인듯
			g_pVB,														//버텍스들의 버퍼
			0,															//어디서부터 읽어올지 오프셋
			sizeof(CUSTOMVERTEX)										//한번에 읽어오는 크기(한번에 읽어오는 정보의 크기인듯)
		);
		//2. D3D에 정점 셰이더 정보를 지정한다. 대부분의 경우에는 FVF만 지정한다.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		//3. 기하 정보를 출력하기 위한 DrawPrimitive() 함수 호출
		/*2,3장*/
		//g_pd3dDevice->DrawPrimitive(
		//	//D3DPT_TRIANGLELIST,										//그리는 타입들 인듯 -확인
		//	D3DPT_TRIANGLESTRIP,										//그리는 타입들 인듯 이거는 면으로 만드는것 -확인
		//	0,															//어디서부터 그려가는지
		//	1															//몇개의 삼각형을 그리는지? -확인
		//);
		/*4장*/
		g_pd3dDevice->DrawPrimitive(
			//D3DPT_TRIANGLELIST,										//그리는 타입들 인듯 -확인
			D3DPT_TRIANGLESTRIP,										//그리는 타입들 인듯 이거는 면으로 만드는것 -확인
			0,															//어디서부터 그려가는지
			2*50-2															//몇개의 삼각형을 그리는지? -확인
		);

		g_pd3dDevice->EndScene();										//그리기 종료
	}

	g_pd3dDevice->Present(												//후면버퍼를 화면으로 출력 -확인
		NULL,															//소스 렉트
		NULL,															//대상 렉트
		NULL,
		NULL
	);
}
