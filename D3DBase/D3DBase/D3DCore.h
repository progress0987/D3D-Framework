#pragma once

#include <d3d9.h>

#define WINNAME "D3DBase"
#define WINSIZEX 800
#define WINSIZEY 600

//전역변수
//g로 시작하는건 글로벌(전역)

LPDIRECT3D9					g_pD3D			= NULL;						//D3D 디바이스를 생성할 D3D 객체 변수
LPDIRECT3DDEVICE9			g_pd3dDevice	= NULL;						//렌더링에 사용될 D3D 디바이스
LPDIRECT3DVERTEXBUFFER9		g_pVB			= NULL;						//정점을 보관할 정점 버퍼 - 정점만을 계산하기위해 생성된 버퍼

//사용자정의 정점 구조체
struct CUSTOMVERTEX {
	FLOAT x, y, z, rhw;													//정점과 관련된 좌표들(rhw값이 있으면 변환이 완료된 정점이라는것)
																		//ps - rhw, x, y, z 의 순서로 변수를 잡으면 FVF 선언순서에 위배된다고 함 -확인

	DWORD color;														//정점 색깔
};
//사용자 정점 구조체에 관한 정보를 나타내는 FVF 값
//구조체는 x,y,z,rhw값과 diffuse 색깔값으로 이루어져있다는 선언
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


//D3D초기화
HRESULT InitD3D(HWND hWnd) {
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {			//디바이스를 생성하기 위한 D3D 객체 생성
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;										//디바이스 생성을 위한 구조체
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//zeromemory로 함수구조체 완전초기화
	d3dpp.Windowed = TRUE;												//일단 창모드로 생성 -확인
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//가장 효율적인 SWAP 효과 -확인
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;							//현재 바탕화면 모드에 맞춰서 후면 버퍼 생성

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

	return S_OK;														//디바이스 상태정보 처리 완료
}

//정점 버퍼의 초기화
/*	정점 버퍼를 생성하고 정점 값들을 채워넣는다.

 *	정점 버퍼란 기본적으로 정점 정보를 갖고있는 메모리의 블록이다.

 *	정점 버퍼를 생성한 후에 반드시 Lock()과 Unlock()으로 포인터를 얻어내어 정점정보를 정점 버퍼에 써넣어야 한다.

 *	또한 D3D는 인덱스 버퍼도 사용이 가능하다는 것을 명심하자. -확인

 *	정점 버퍼나 인덱스 버퍼는 기본 시스템 메모리 외에 디바이스 메모리(비디오카드의 메모리) 에 생성될 수 있는데
	대부분의 비디오카드에서는 이렇게 할 경우 엄청난 속도의 향상을 얻을 수 있다.
*/
HRESULT InitVB() {
	//삼각형을 렌더링하기 위해 정점 선언 -수정 -임시
	CUSTOMVERTEX vertices[] = {
		//색은 0xffff0000 과 같이 표현할 수 있는데 ARGB의 순서이다.
		{150.f,50.f,0.5f,1.f,D3DCOLOR_RGBA(255,0,0,255)},
		{250.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(0,255,0,255)},
		{50.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(0,0,255,255)},

		//{250.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(0,255,0,255)},
		//{350.f,250.f,0.5f,1.f,D3DCOLOR_RGBA(255,0,0,255)},
		//{150.f,50.f,0.5f,1.f,D3DCOLOR_RGBA(0,0,255,255)},
	};

	/*
	정점 버퍼 생성

	3개의 사용자정점들을 보관할 메모리를 할당한다.
	FVF를 지정하여 보관할 데이터의 형식을 지정한다.
	FVF - Flexible Vertex Format		- 유동적으로 버텍스를 지정한다고 선언할때
	*/
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(
		3 * sizeof(CUSTOMVERTEX),										//생성할 정점버퍼의 바이트 단위 크기
		0,																//정점 버퍼의 종류 혹은 처리방식(SW/HW) 지정
		D3DFVF_CUSTOMVERTEX,											//정점 정보 구조체에 따라 선언된 FVF 플래그 값
		D3DPOOL_DEFAULT,												//정점 버퍼가 저장될 메모리의 위치(비디오카드인지 시스템메모리인지 등)와 관리방식 지정
		&g_pVB,															//반환될 정점 버퍼의 인터페이스
		NULL															//예약되었는지(공유되었는지) 현재는 거의 무조건 NULL
	))) {
		return E_FAIL;
	}
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(												//사용하기 위해서 락
		0,																//Lock을 할 버퍼의 시작점, 아래와 함께 양쪽모두 0이면 전체 버퍼
		sizeof(vertices),												//Lock을 할 버퍼의 크기, 위와 함께 양쪽모두 0이면 전체 버퍼
		(void**)&pVertices,												//읽고 쓰기 수행하려는 메모리 영역의 포인터
		0																//Lock 수행과 함께 사용되는 플래그
	))) return E_FAIL;	
	
	memcpy(pVertices, vertices, sizeof(vertices));											//버텍스들을 복사해넣는다

	g_pVB->Unlock();																		//사용이 풀려서 언락

	return S_OK;
}

//릴리즈
void release() {
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
	g_pd3dDevice->Clear(												//후면 버퍼를 rgb(0,0,255) 로 채워줌
		0,
		NULL,															//아마 클리어 해줄 범위 렉트인듯 -확인
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 255),
		1.0f,
		0);

	if (SUCCEEDED(g_pd3dDevice->BeginScene())) {						//렌더링 시작
																		//실제 프로세스들이 실행될 곳
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
		g_pd3dDevice->DrawPrimitive(
			//D3DPT_TRIANGLELIST,											//그리는 타입들 인듯 -확인
			D3DPT_TRIANGLESTRIP,											//그리는 타입들 인듯 이거는 면으로 만드는것 -확인
			0,															//어디서부터 그려가는지
			1															//몇개의 삼각형을 그리는지? -확인
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
