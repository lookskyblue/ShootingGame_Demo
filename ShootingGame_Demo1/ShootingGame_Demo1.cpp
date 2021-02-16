// ShootingGame_Demo1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#pragma once
#include <random>
#include "framework.h"
#include "ShootingGame_Demo1.h"
#include "Obj.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

enum player_size { PLAYER_WIDTH = 40, PLAYER_HEIGHT = 40 };
enum monster_size { MONS_WIDTH = 50, MONS_HEIGHT = 50 };

LARGE_INTEGER g_tTime;
LARGE_INTEGER g_tSecond;
float g_fDeltaTime;

HBITMAP g_myBitMap;
HWND g_hWnd;
HDC g_hDC;
RECT g_rcWindow;
bool bGameLoop = true;

std::random_device g_rd;
std::mt19937 g_gen(g_rd());

Player* g_player;
std::vector<Monster> mons_vec;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void GameInit();
void Update();
void Render();
void LateUpdate();
void RenderPlayer(HDC*);
void GetUserKey(float);
void MoveBullet(float);
void RenderMonster(HDC*);
void AddMonster();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SHOOTINGGAMEDEMO1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHOOTINGGAMEDEMO1));
    MSG msg;

    GameInit();

    QueryPerformanceCounter(&g_tTime);
    QueryPerformanceFrequency(&g_tSecond);

    while (bGameLoop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {
            Update();
            Render();
            LateUpdate();
        }
    }

    ReleaseDC(g_hWnd, g_hDC);

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHOOTINGGAMEDEMO1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_SHOOTINGGAMEDEMO1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   g_hWnd = hWnd;
   g_hDC = GetDC(hWnd);
   
   RECT rc = { 0, 0, 700, 900 };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 
       SWP_NOMOVE|SWP_NOZORDER);
  
   GetClientRect(hWnd, &g_rcWindow);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            HDC hMemDC = CreateCompatibleDC(g_hDC);
            HBITMAP OldBitMap = (HBITMAP)SelectObject(hMemDC, g_myBitMap);
            BitBlt(g_hDC, 0, 0, g_rcWindow.right, g_rcWindow.bottom, hMemDC, 0, 0, SRCCOPY);

            SelectObject(hMemDC, OldBitMap);
            DeleteObject(OldBitMap);
            DeleteDC(hMemDC);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        bGameLoop = false;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void GameInit() {
    g_player = new Player(g_rcWindow.right / 2 - PLAYER_WIDTH, g_rcWindow.bottom - PLAYER_HEIGHT, g_rcWindow.right / 2, g_rcWindow.bottom);
    mons_vec.reserve(30);
}

void Render() {
    HDC hMemDC = CreateCompatibleDC(g_hDC);
    HBITMAP OldBitMap;

    if (g_myBitMap == NULL)
        g_myBitMap = CreateCompatibleBitmap(g_hDC, g_rcWindow.right, g_rcWindow.bottom);

    OldBitMap = (HBITMAP)SelectObject(hMemDC, g_myBitMap);
    FillRect(hMemDC, &g_rcWindow, GetSysColorBrush(COLOR_WINDOW + 2));
    
    RenderPlayer(&hMemDC);
    RenderMonster(&hMemDC);

    SelectObject(hMemDC, OldBitMap);
    DeleteObject(OldBitMap);
    DeleteDC(hMemDC);

    InvalidateRect(g_hWnd, nullptr, false);
}

void Update() {

    LARGE_INTEGER tNowTime;
    QueryPerformanceCounter(&tNowTime);

    g_fDeltaTime = (tNowTime.QuadPart - g_tTime.QuadPart)
        / (float)(g_tSecond.QuadPart);

    g_tTime = tNowTime;

    static float fTimeScale = 1.f;

    float fDeltaTime = g_fDeltaTime * fTimeScale;

    GetUserKey(fDeltaTime);
    MoveBullet(fDeltaTime);

    static float mons_regen_time;
    mons_regen_time += g_fDeltaTime;

    if (mons_regen_time >= 1.f)
    {
        AddMonster();

        mons_regen_time = 0;
    }
}

void LateUpdate() {

    //Check Player Bullet with Monster
    auto itr_bullet_begin = g_player->p_bullet.GetBegin();
    auto itr_bullet_end = g_player->p_bullet.GetEnd();

    auto mons_itr_begin = mons_vec.begin();
    auto mons_itr_end = mons_vec.end();

    for (auto bullet_itr = itr_bullet_begin; bullet_itr != itr_bullet_end; ++bullet_itr)
    {
        for(; mons_itr_begin != mons_itr_end; ++mons_itr_begin)
        {
            if (bullet_itr->l <= mons_itr_begin->GetR() &&
                bullet_itr->r >= mons_itr_begin->GetL() &&
                bullet_itr->t <= mons_itr_begin->GetB() &&
                bullet_itr->b >= mons_itr_begin->GetT())
            {
                bullet_itr = g_player->p_bullet.EraseBullet(bullet_itr);
                itr_bullet_end = g_player->p_bullet.GetEnd();

                mons_itr_begin->Attacked(-g_player->p_bullet.GetBulletDemage());

                if (mons_itr_begin->GetHP() <= 0)
                {
                    mons_itr_begin = mons_vec.erase(mons_itr_begin);
                    mons_itr_end = mons_vec.end();

                    Beep(1500, 20);
                }

                if (bullet_itr == itr_bullet_end ||
                    mons_itr_begin == mons_itr_end)
                    goto OUT_CHECK;
            }
        }
    }
    OUT_CHECK:

    // Check Player Bullet Ouf Of Boundary
    itr_bullet_begin = g_player->p_bullet.GetBegin();
    itr_bullet_end = g_player->p_bullet.GetEnd();

    for (auto itr = itr_bullet_begin; itr != itr_bullet_end;)
    {
        if (itr->b <= g_rcWindow.top)
        {
            itr = g_player->p_bullet.EraseBullet(itr);
            itr_bullet_end = g_player->p_bullet.GetEnd();
        }

        else
            ++itr;
    }

    //Check Player Boundary

    if (g_player->GetL() <= g_rcWindow.left)
    {
        g_player->SetL(g_rcWindow.left);
        g_player->SetR(g_rcWindow.left + PLAYER_WIDTH);
    }

    if (g_player->GetT() <= g_rcWindow.top)
    {
        g_player->SetT(g_rcWindow.top);
        g_player->SetB(g_rcWindow.top + PLAYER_HEIGHT);
    }

    if (g_player->GetR() >= g_rcWindow.right)
    {
        g_player->SetR(g_rcWindow.right);
        g_player->SetL(g_rcWindow.right - PLAYER_WIDTH);
    }

    if (g_player->GetB() >= g_rcWindow.bottom)
    {
        g_player->SetB(g_rcWindow.bottom);
        g_player->SetT(g_rcWindow.bottom - PLAYER_HEIGHT);

    }

}

void RenderPlayer(HDC* hMemDC)
{
    // BODY
    Rectangle(*hMemDC, g_player->GetL(), g_player->GetT(), g_player->GetR(), g_player->GetB());

        // BULLET
    std::vector<RECTANGLE>::iterator begin = g_player->p_bullet.GetBegin();
    std::vector<RECTANGLE>::iterator end = g_player->p_bullet.GetEnd();

    for (auto itr = begin; itr != end; ++itr)
        Rectangle(*hMemDC, itr->l, itr->t, itr->r, itr->b);
}

void RenderMonster(HDC* hMemDC)
{
    for (auto& const elem : mons_vec)
        Rectangle(*hMemDC, elem.GetL(), elem.GetT(), elem.GetR(), elem.GetB());
}

void GetUserKey(float fDeltaTime)
{
    float fspeed = g_player->GetSpeed() * fDeltaTime;

    if (GetAsyncKeyState(VK_LEFT))
        g_player->MoveX(-fspeed);

    if (GetAsyncKeyState(VK_UP))
        g_player->MoveY(-fspeed);

    if (GetAsyncKeyState(VK_RIGHT))
        g_player->MoveX(fspeed);

    if (GetAsyncKeyState(VK_DOWN))
        g_player->MoveY(fspeed);

    static float timeGap;
    timeGap += fDeltaTime;

    if (GetAsyncKeyState(VK_SPACE) && timeGap >= 0.3f)
    {
        timeGap = 0;

        float bullet_l = (g_player->GetR() + g_player->GetL()) / 2.f;
        float bullet_t = g_player->GetT();

        float bullet_speed = 1000;
        g_player->p_bullet.FireBullet(bullet_l - 10.f, bullet_t - 20.f, bullet_l + 10.f, bullet_t, bullet_speed);

        Beep(500, 10);
    }
}

void MoveBullet(float fDeltaTime)
{
    float bullet_speed = g_player->p_bullet.GetBulletSpeed() * fDeltaTime;

    g_player->p_bullet.MoveY(-bullet_speed);
}

void AddMonster()
{
    std::uniform_int_distribution<int> rand_x(g_rcWindow.left, g_rcWindow.right - MONS_WIDTH);
    std::uniform_int_distribution<int> rand_y(g_rcWindow.top, g_rcWindow.bottom / 2);

    int startX = rand_x(g_gen);
    int startY = rand_y(g_gen);

    mons_vec.push_back(Monster(startX, startY, startX + MONS_WIDTH, startY + MONS_HEIGHT, 500));
}