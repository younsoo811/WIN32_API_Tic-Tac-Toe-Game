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
		700,
		400,
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
#define  BSIZE 40	//반지름

BOOL InRectangle(int mx, int my)
{
	if (mx<BSIZE * 16 && my<BSIZE * 8) return TRUE;	//사각형 안을 클릭한 경우
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hBrush;
	static BOOL Selection;
	static BOOL E_Selection;
	static int ex, ey;
	static int sx, sy;
	static int dx, dy;
	static int	x, y;
	static int savei, savej;
	static int savex[8][4] = { 0 };
	static int savey[8][4] = { 0 };

	switch (iMsg)
	{
	case WM_CREATE:
		sx = 0; sy = 0;

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		//사각틀 생성
		for (int j = 0; j < 4; j++){
			sx = 0;

			for (int i = 0; i < 8; i++){
				Rectangle(hdc, sx, sy, sx + BSIZE * 2, sy + BSIZE * 2);
				if (savex[i][j] != 0 && savey[i][j] != 0){
					Ellipse(hdc, savex[i][j] - 1, savey[i][j] - 1, (savex[i][j] - 1) + BSIZE * 2, (savey[i][j] - 1) + BSIZE * 2);
				}

				sx += BSIZE * 2;
			}
			sy += BSIZE * 2;
		}
		sx, sy = 0;	//초기화

		//클릭하면 원 그리기
		if (Selection){
			//(HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
			for (int b = 0; b < 4; b++){
				sx = 0;
				for (int a = 0; a < 8; a++){
					if (dx > sx && dx < sx + BSIZE * 2 && dy > sy && dy < sy + BSIZE * 2){	//마우스 커서 위치가 어느 작은 사각형 위치에 있는지 찾기
						ex = sx; ey = sy;
						savei = a;
						savej = b;
						E_Selection = TRUE;
						break;
					}
					sx += BSIZE * 2;
				}
				sy += BSIZE * 2;
			}


		}
		if (E_Selection){
			Ellipse(hdc, ex, ey, ex + BSIZE * 2, ey + BSIZE * 2);
			savex[savei][savej] = ex + 1;
			savey[savei][savej] = ey + 1;
		}

		sx, sy = 0;	//초기화

		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		dx = LOWORD(lParam);	//마우스 좌표
		dy = HIWORD(lParam);

		if (InRectangle(dx, dy)) {	//큰 사각형 내부에 클릭 했는지 확인
			Selection = TRUE;

		}

		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}