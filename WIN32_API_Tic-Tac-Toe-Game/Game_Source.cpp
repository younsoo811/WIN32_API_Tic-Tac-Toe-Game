#include <windows.h>
#include <TCHAR.H>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	 hwnd;
	MSG 	 msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Window Title Name"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

#include <math.h>
#define  BSIZE 100	//지름

BOOL InRectangle(int mx, int my)
{
	if (mx<BSIZE * 5 && my<BSIZE * 5) return TRUE;	//사각형 안을 클릭한 경우
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;
	/*RECT rect;*/
	HPEN hPen;
	
	
	static int dx, dy;

	
	static int savex[5][5] = { 0 };
	static int savey[5][5] = { 0 };
	static int startX, startY, endX, endY;
	static int	cx, cy;
	static BOOL isC;
	static int	count;
	static int	iColor;

	switch (iMsg)
	{
	case WM_CREATE:
		startX = 0; startY = 0;
		endX = 0;  endY = 0;
		count = 0;

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		//사각틀 생성
		for (int i = 0; i < 5; i++){
			MoveToEx(hdc, BSIZE*(i + 1), 0, NULL);
			LineTo(hdc, BSIZE*(i + 1), BSIZE * 5);
		}

		for (int i = 0; i < 5; i++){
			MoveToEx(hdc, 0, BSIZE*(i + 1), NULL);
			LineTo(hdc, BSIZE * 5, BSIZE*(i + 1));
		}

		//저장된 원 그리기
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				if (savex[i][j] != 0 && savey[i][j] != 0){

					Ellipse(hdc, savex[i][j] - 50, savey[i][j] - 50, savex[i][j] + 50, savey[i][j] + 50);
				}
			}
		}

		//원그리기
		if (isC){
			if ((iColor % 2) == 0){
				hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				(HPEN)SelectObject(hdc, hPen);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
				(HPEN)SelectObject(hdc, hPen);
			}
			iColor++;
			Ellipse(hdc, cx - 50, cy - 50, cx + 50, cy + 50);
			isC = false;
		}



		/*DeleteObject(hPen);*/
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		dx = LOWORD(lParam);	//마우스 좌표
		dy = HIWORD(lParam);

		cx = (dx / BSIZE)*BSIZE + BSIZE / 2;		//원의 중심 좌표 구하기
		cy = (dy / BSIZE)*BSIZE + BSIZE / 2;

		savex[dx / BSIZE][dy / BSIZE] = cx;
		savey[dx / BSIZE][dy / BSIZE] = cy;

		if (InRectangle(dx, dy)) {	//큰 사각형 내부에 클릭 했는지 확인

			isC = TRUE;
		}

		InvalidateRgn(hwnd, NULL, TRUE);
		break;
		/*case WM_LBUTTONUP:
		Selection = FALSE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}