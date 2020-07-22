#include "stdafx.h"
#include "GameSys.h"

//for visual studio v120 (visual studio platform low versions)
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")


CGameSys::CGameSys()
{
}

CGameSys::~CGameSys()
{
}

void CGameSys::SetSleep(BOOL bSleep) { m_bSleep = bSleep; };


HINSTANCE   _hInstance;

///////// 맵구성이미지 //////////
// 기본 맵 구성 이미지 
CBitmap		cbmpMapEle[6];
// 항아리 
CBitmap		cbmpPot;

// background
CBitmap		cbmpBkg;

// 과일 
CBitmap		cbmpFruit[4];

// 너구리 1탄
CBitmap		cbmpBackO1;
CBitmap		cbmpBackO2;

// 너구리 이미지 
CBitmap		cbmpCharL;
CBitmap		cbmpCharR;
CBitmap		cbmpCharB;
CBitmap		cbmpCharF1;
CBitmap		cbmpCharF2;
CBitmap		cbmpCharLJump;
CBitmap		cbmpCharRJump;
CBitmap		cbmpCharLJumpReady;
CBitmap		cbmpCharRJumpReady;
CBitmap		cbmpCharRDrop;
CBitmap		cbmpCharLDrop;
CBitmap		cbmpCharRDead;
CBitmap		cbmpCharLDead;
CBitmap		cbmpCharRAttack;
CBitmap		cbmpCharLAttack;
CBitmap		cbmpCharLDJump;
CBitmap		cbmpCharRDJump;

//놀람
CBitmap		cbmpSurprise;

//적 
CBitmap		cbmpEnemyR;
CBitmap		cbmpEnemyL;
CBitmap		cbmpEnemyRRed;
CBitmap		cbmpEnemyLRed;

//뱀
CBitmap		cbmpSnakeL;
CBitmap		cbmpSnakeR;

//아기 
CBitmap		cbmpBaby;

//타일
CBitmap		cbmpTile;

//숫자
CBitmap		cbmpDigit;
CBitmap		cbmpDigit_sm;

// 'LETS...'
CBitmap		cbmpLets;
// 'BONUS'
CBitmap		cbmpBonus;
// 'Next'
CBitmap		cbmpNext;
// ending
CBitmap		cbmpEnding;

//
CBitmap		cbmpBkg2;
CBitmap		cbmpBkg3;

CBitmap		cbmpObj2;
CBitmap		cbmpObj3;

CBitmap		cbmpGameIntro;
CBitmap		cbmpGameOver;
CBitmap		cbmpGameClear;

CBitmap		cbmpHearts;
CBitmap		cbmpLife;



//완성된 맵이미지 
//위의 맵 구성부품으로 맵을 만든뒤 완성된 맵을 하나의 이미지로 저장 
CBitmap		cbmpMap;

//더블 버퍼링을 위해  
CBitmap		cbmpBit;

// 구조체 
Raccoon		_Rac;
Enemy		_Ene[7];
Item		_Item[12];	//과일 8개 + 항아리 4개 = 12 , 이것은 어느 맵이나 고정 
Tog			_Tog[10];

char		itemCount;

char macmUpward[] = { 25, 19, 14, 10, 7, 4, 2 };
char macmDownward[] = { -2, -4, -7, -10, -14, -19, -25 };
char hippoUpward[] = { 60, 50, 40, 30, 25, 20, 15 };
char hippoDownward[] = { -15, -20, -25, -30, -40, -50, -60 };
char monkeyyup[] = { 30, 24, 18, 12, 8, 4 };
char monkeyydown[] = { -4, -8, -12, -18, -24, -30 };
char monkeIndex = 0;
char hippoIndex = 0;
BOOL   Ishippo[4] = { FALSE };
BOOL   IsAlli = FALSE;
BOOL   IsMonkey = FALSE;
int		Allix;
int		Alliy;
int		Hippox[4];
int		Hippoy[4];
char	currenthippo;
int		Monkeyx;
int		Monkeyy;
// 적의 갯수 
char _EnemyCount;
char deadEnemy;
//게임 상태(4가지 상태)
char		_GameState;

//맵정보 
//내부적으로 맵 처리를 하기 위해 
char		_cMap[MAP_BLOCK_Y_COUNT][MAP_BLOCK_X_COUNT + 1];

//레벨 
char		_iLevel;

//남은 너구리 수 
char		_iLive;

//먹은 과일 수
char		_iEat;


//에니메이션을 위해서
int			_iAni;

//남은 시간
int			_iTime;


//기본적으로 화면을 전부 그리는 것이 아니고 처음에만 전부 그리고
//그후에는 변하는 부분만 다시 그린다.
//문제는 원도우을 이동하거나 다른 원도우로 가린다면 화면을 전부 다시 그려야 한다.
//이를 위해서 아래의 변수가 필요하다. 
BOOL		_bIsDrawAll;

//너구리가 죽어서 떨어질때 음악 연주 상태
BOOL		_bIsDrop_Sound;

//토글 가능한 맵 토글할지 받아오는 bool값
BOOL		_bToggleMap;
int			_iToggleTimerCnt;
int			_ToggleElementCnt;
#define		TOGGLE_TIMER_COUNT 30

// 너구리 애니메이션
BOOL		frame;
//////////////////////////////////////////////////////////////////////////////


namespace Game {
	// 가져다 씀 
	static void TransBitBlt(CDC* pDC, int x, int y, int width, int height,
		CDC& memdc, int mx, int my, COLORREF clrMask)
	{
		COLORREF cColor;
		CBitmap bmAndBack, bmAndObject, bmAndMem, bmSave;
		CDC pdcBack, pdcMem, pdcObject, pdcSave;

		pdcBack.CreateCompatibleDC(pDC);
		pdcObject.CreateCompatibleDC(pDC);
		pdcMem.CreateCompatibleDC(pDC);
		pdcSave.CreateCompatibleDC(pDC);

		bmAndBack.CreateBitmap(width, height, 1, 1, NULL);
		bmAndObject.CreateBitmap(width, height, 1, 1, NULL);
		bmAndMem.CreateCompatibleBitmap(pDC, width, height);
		bmSave.CreateCompatibleBitmap(pDC, width, height);

		pdcBack.SelectObject(bmAndBack);
		pdcObject.SelectObject(bmAndObject);
		pdcMem.SelectObject(bmAndMem);
		pdcSave.SelectObject(bmSave);

		memdc.SetMapMode(pDC->GetMapMode());

		pdcSave.BitBlt(0, 0, width, height, &memdc, mx, my, SRCCOPY);

		cColor = memdc.SetBkColor(clrMask);

		pdcObject.BitBlt(0, 0, width, height, &memdc, mx, my, SRCCOPY);

		memdc.SetBkColor(cColor);

		pdcBack.BitBlt(0, 0, width, height, &pdcObject, 0, 0, NOTSRCCOPY);
		pdcMem.BitBlt(0, 0, width, height, pDC, x, y, SRCCOPY);
		pdcMem.BitBlt(0, 0, width, height, &pdcObject, 0, 0, SRCAND);
		memdc.BitBlt(mx, my, width, height, &pdcBack, 0, 0, SRCAND);
		pdcMem.BitBlt(0, 0, width, height, &memdc, mx, my, SRCPAINT);
		pDC->BitBlt(x, y, width, height, &pdcMem, 0, 0, SRCCOPY);
		memdc.BitBlt(mx, my, width, height, &pdcSave, 0, 0, SRCCOPY);
	}

	static void DrawBitmap(CDC* pDC, int x, int y, CBitmap& cbmp, bool bTrans = true,
		COLORREF crTransColor = RGB(255, 0, 255))
	{
		CDC memdc;
		CBitmap* oldCbmp;
		int bx, by;
		BITMAP bit;

		memdc.CreateCompatibleDC(pDC);
		oldCbmp = (CBitmap*)memdc.SelectObject(&cbmp);

		GetObject(cbmp.m_hObject, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;

		// 특정 색깔을 투명하게 할 것인가?
		if (bTrans)
			TransBitBlt(pDC, x, y, bx, by, memdc, 0, 0, crTransColor);
		else
			pDC->BitBlt(x, y, bx, by, &memdc, 0, 0, SRCCOPY);

		memdc.SelectObject(oldCbmp);
	}

	static void DrawBitmap(CDC* pDC, int x, int y, CBitmap& cbmp, int s_x, int s_y,
		int e_x, int e_y, bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255))
	{
		CDC memdc;
		CBitmap* oldCbmp;
		int bx, by;
		BITMAP bit;

		memdc.CreateCompatibleDC(pDC);
		oldCbmp = (CBitmap*)memdc.SelectObject(&cbmp);

		GetObject(cbmp.m_hObject, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
		// 특정 색깔을 투명하게 할 것인가?
		if (bTrans) {
			// 그릴 범위를 고려하여 이미지 크기 계산 
			int DrawWidth = e_x - s_x;
			int DrawHeight = e_y - s_y;
			TransBitBlt(pDC, x, y, DrawWidth, DrawHeight, memdc, s_x, s_y, crTransColor);
		}
		else {
			int DrawWidth = e_x - s_x;
			int DrawHeight = e_y - s_y;
			pDC->BitBlt(x, y, DrawWidth, DrawHeight, &memdc, s_x, s_y, SRCCOPY);
		}

		memdc.SelectObject(oldCbmp);
	}


	static void DrawBitmapAlpha(CDC* pDC, int x, int y, CBitmap& cbmp, int alp)
	{
		CDC memdc;
		CBitmap* oldCbmp;
		int bx, by;
		BITMAP bit;
		BLENDFUNCTION bf;

		memdc.CreateCompatibleDC(pDC);
		oldCbmp = (CBitmap*)memdc.SelectObject(&cbmp);

		GetObject(cbmp.m_hObject, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = alp;
		bf.AlphaFormat = 0;

		pDC->AlphaBlend(x, y, bx, by, &memdc, 0, 0, bx, by, bf);

		memdc.SelectObject(oldCbmp);
	}

	static void DrawBitmapAlpha(CDC* pDC, int x, int y, CBitmap& cbmp, int alp, int s_x, int s_y,
		int e_x, int e_y)
	{
		CDC memdc;
		CBitmap* oldCbmp;
		int bx, by;
		BITMAP bit;
		BLENDFUNCTION bf;

		memdc.CreateCompatibleDC(pDC);
		oldCbmp = (CBitmap*)memdc.SelectObject(&cbmp);

		GetObject(cbmp.m_hObject, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = alp;
		bf.AlphaFormat = 0;

		int DrawWidth = e_x - s_x;
		int DrawHeight = e_y - s_y;
		pDC->AlphaBlend(x, y, DrawWidth, DrawHeight, &memdc, s_x, s_y, DrawWidth, DrawHeight, bf);

		memdc.SelectObject(oldCbmp);
	}

	static void DrawDigit(CDC* pDC, int x, int y, int score, CBitmap& cbmp, int cipher = 0,
		COLORREF crTransColor = RGB(0, 0, 0))
	{
		CDC memdc;
		CBitmap* oldCbmp;
		int bx, by;
		BITMAP bit;

		memdc.CreateCompatibleDC(pDC);
		oldCbmp = (CBitmap*)memdc.SelectObject(&cbmp);

		GetObject(cbmp.m_hObject, sizeof(BITMAP), &bit);
		bx = bit.bmWidth / 10;
		by = bit.bmHeight;

		int len;
		int blank = 0;
		char str[10];

		_itoa_s(score, str, 10);
		len = strlen(str);

		if (cipher)
			blank = cipher - len - 1;

		//숫자를 자리수를 고려하여 공백을 넣고 숫자 표시  
		for (int i = 0; i < len; i++) {
			TransBitBlt(pDC, x + (blank*bx) + (i*bx), y, bx, by, memdc, bx*(str[i] - '0'), 0, crTransColor);
		}

		memdc.SelectObject(oldCbmp);
	}

	static void DrawRacLStand(CDC* pDC, int frame, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame / 8) % 5;
		frame = 4 - frame;
		switch (frame) {
		case 0:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF2, 8, 0, 111, 105);
			break;
		case 1:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF2, 114, 0, 217, 105);
			break;
		case 2:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF2, 227, 0, 330, 105);
			break;
		case 3:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF2, 342, 0, 446, 105);
			break;
		case 4:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF2, 452, 0, 555, 105);
			break;
		}
	}
	static void DrawRacRStand(CDC* pDC, int frame, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame / 8) % 5;
		switch (frame) {
		case 0:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF1, 0, 0, 103, 105);
			break;
		case 1:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF1, 109, 0, 213, 105);
			break;
		case 2:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF1, 225, 0, 328, 105);
			break;
		case 3:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF1, 338, 0, 441, 105);
			break;
		case 4:
			DrawBitmap(pDC, x - 52, y - 54, cbmpCharF1, 444, 0, 547, 105);
			break;
		}
	}
	static void DrawRacLMove(CDC* pDC, int frame, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame / 4) % 5;
		frame = 4 - frame;
		switch (frame) {
		case 0:
			DrawBitmap(pDC, x - 53, y - 52, cbmpCharL, 5, 0, 110, 101);
			break;
		case 1:
			DrawBitmap(pDC, x - 55, y - 52, cbmpCharL, 129, 0, 239, 101);
			break;
		case 2:
			DrawBitmap(pDC, x - 57, y - 52, cbmpCharL, 254, 0, 368, 101);
			break;
		case 3:
			DrawBitmap(pDC, x - 54, y - 52, cbmpCharL, 369, 0, 477, 101);
			break;
		case 4:
			DrawBitmap(pDC, x - 52, y - 52, cbmpCharL, 497, 0, 600, 101);
			break;
		}
	}
	static void DrawRacRMove(CDC* pDC, int frame, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame / 4) % 5;
		switch (frame) {
		case 0:
			DrawBitmap(pDC, x - 52, y - 52, cbmpCharR, 2, 0, 105, 101);
			break;
		case 1:
			DrawBitmap(pDC, x - 54, y - 52, cbmpCharR, 125, 0, 233, 101);
			break;
		case 2:
			DrawBitmap(pDC, x - 57, y - 52, cbmpCharR, 234, 0, 348, 101);
			break;
		case 3:
			DrawBitmap(pDC, x - 55, y - 52, cbmpCharR, 363, 0, 473, 101);
			break;
		case 4:
			DrawBitmap(pDC, x - 53, y - 52, cbmpCharR, 492, 0, 597, 101);
			break;
		}
	}
	static void DrawRacUp(CDC* pDC, int frame, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame / 3) % 4;
		DrawBitmap(pDC, x - 34, y - 57, cbmpCharB, frame * 73, 0, (frame + 1) * 73, 110);
	}
	static void DrawRacLJump(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x - 54, y - 68, cbmpCharLJump, frame * 109, 0, (frame + 1) * 109, 123);
	}
	static void DrawRacRJump(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x - 54, y - 68, cbmpCharRJump, frame * 109, 0, (frame + 1) * 109, 123);
	}
	static void DrawRacLDJump(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame ) % 4;
		frame = 3 - frame;
		DrawBitmap(pDC, x - 54, y - 53, cbmpCharLDJump, frame * 108, 0, (frame + 1) * 108, 106);
	}
	static void DrawRacRDJump(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame) % 4;
		DrawBitmap(pDC, x - 54, y - 53, cbmpCharRDJump, frame * 108, 0, (frame + 1) * 108, 106);
	}
	static void DrawRacLJumpReady(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x - 52, y - 63, cbmpCharLJumpReady, frame * 104, 0, (frame + 1) * 104, 123);
	}
	static void DrawRacRJumpReady(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x - 52, y - 63, cbmpCharRJumpReady, frame * 104, 0, (frame + 1) * 104, 123);
	}
	static void DrawRacLDrop(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x - 52, y - 57, cbmpCharLDrop, frame * 104, 0, (frame + 1) * 104, 114);
	}
	static void DrawRacRDrop(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x - 52, y - 57, cbmpCharRDrop, frame * 104, 0, (frame + 1) * 104, 114);
	}
	static void DrawRacLAttack(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x - 48, y - 57, cbmpCharLDead, frame * 96, 0, (frame + 1) * 96, 113);
	}
	static void DrawRacRAttack(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x - 48, y - 57, cbmpCharRDead, frame * 96, 0, (frame + 1) * 96, 113);
	}
	static void DrawEneL(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame / 4) % 5;
		switch (frame) {
		case 0:
			DrawBitmap(pDC, x - 56, y - 250, cbmpEnemyL, 2, 2, 113, 250);
			break;
		case 1:
			DrawBitmap(pDC, x - 48, y - 250, cbmpEnemyL, 115, 2, 206, 250);
			break;
		case 2:
			DrawBitmap(pDC, x - 46, y - 250, cbmpEnemyL, 208, 2, 293, 250);
			break;
		case 3:
			DrawBitmap(pDC, x - 42, y - 250, cbmpEnemyL, 293, 2, 374, 250);
			break;
		case 4:
			DrawBitmap(pDC, x - 50, y - 250, cbmpEnemyL, 374, 2, 474, 250);
			break;
		}
	}
	static void DrawEneR(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = (frame / 4) % 5;
		frame = 4 - frame;
		switch (frame) {
		case 0:
			DrawBitmap(pDC, x - 51, y - 250, cbmpEnemyR, 2, 2, 104, 250);
			break;
		case 1:
			DrawBitmap(pDC, x - 43, y - 250, cbmpEnemyR, 104, 2, 185, 250);
			break;
		case 2:
			DrawBitmap(pDC, x - 42, y - 250, cbmpEnemyR, 185, 2, 269, 250);
			break;
		case 3:
			DrawBitmap(pDC, x - 47, y - 250, cbmpEnemyR, 269, 2, 363, 250);
			break;
		case 4:
			DrawBitmap(pDC, x - 55, y - 250, cbmpEnemyR, 364, 2, 474, 250);
			break;
		}
	}
	static void DrawHitEneL(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = frame / 4;
		switch (frame) {
		case 0:
			DrawBitmap(pDC, x - 58, y - 302, cbmpEnemyLRed, 0, 0, 116, 302);
			break;
		case 1:
			DrawBitmap(pDC, x - 90 + 30, y - 302, cbmpEnemyLRed, 141, 0, 321, 302);
			break;
		case 2:
			DrawBitmap(pDC, x - 39 + 120, y - 302, cbmpEnemyLRed, 390, 0, 467, 302);
			break;
		case 3:
			DrawBitmap(pDC, x - 22 + 120, y - 302, cbmpEnemyLRed, 550, 0, 593, 302);
			break;
		}
	}
	static void DrawHitEneR(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = frame / 4;
		frame = 4 - frame;
		switch (frame) {
		case 0:
			DrawBitmap(pDC, x - 22 + 120, y - 302, cbmpEnemyRRed, 0, 0, 43, 302);
			break;
		case 1:
			DrawBitmap(pDC, x - 39 + 120, y - 302, cbmpEnemyRRed, 126, 0, 202, 302);
			break;
		case 2:
			DrawBitmap(pDC, x - 90 + 30, y - 302, cbmpEnemyRRed, 272, 0, 452, 302);
			break;
		case 3:
			DrawBitmap(pDC, x - 58, y - 302, cbmpEnemyRRed, 476, 0, 592, 302);
			break;
		}
	}
	static void DrawAlli(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		DrawBitmap(pDC, x-110, y+40, cbmpBaby, 0, 0, 300, 50);
	}
	static void DrawHippo(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = frame % 7;
		DrawBitmap(pDC, x , y - 71, cbmpPot, frame * 290 + 1, 1, (frame+1)*290 + 1, 122);
	}
	static void DrawMonkey(CDC* pDC, int frame = 0, int x = _Rac.x, int y = _Rac.y,
		bool bTrans = true, COLORREF crTransColor = RGB(255, 0, 255)) {
		frame = frame / 3;
		switch (frame) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			DrawBitmap(pDC, x - 73, y - 178, cbmpMapEle[0], frame * 145, 0, (frame + 1) * 145, 178);
			break;
		case 5:
			DrawBitmap(pDC, x - 92, y - 178, cbmpMapEle[0], 730, 0, 914, 178);
			break;
		case 6:
			DrawBitmap(pDC, x - 94, y - 178, cbmpMapEle[0], 917, 0, 1103, 178);
			break;
		}
	}
}

using namespace Game;

#define GWL_HINSTANCE -6

CGameSys::CGameSys(CWnd* parent)
{
	// Create the game engine
	m_parent = parent;

	// Store the instance handle
	_hInstance = (HINSTANCE)GetWindowLong(m_parent->m_hWnd, GWL_HINSTANCE);

	cbmpHearts.LoadBitmapW(IDB_HEARTS);
	cbmpLife.LoadBitmapW(IDB_LIFE);

	cbmpGameIntro.LoadBitmapW(IDB_GAMEINTRO);
	cbmpGameOver.LoadBitmapW(IDB_GAMEOVER);
	cbmpGameClear.LoadBitmapW(IDB_GAMECLEAR);


	// 원숭이
	cbmpMapEle[0].LoadBitmapW(IDB_MAP_A);
	cbmpMapEle[1].LoadBitmapW(IDB_MAP_B);
	//땅
	cbmpMapEle[2].LoadBitmapW(IDB_MAP_C);
	//박스
	cbmpMapEle[3].LoadBitmapW(IDB_MAP_D);
	cbmpMapEle[4].LoadBitmapW(IDB_MAP_E);
	//기린
	cbmpMapEle[5].LoadBitmapW(IDB_MAP_F);

	// Background
	cbmpBkg.LoadBitmapW(IDB_BKG);

	// 'LETS..' 
	cbmpLets.LoadBitmapW(IDB_LETS);

	// 'GAME OVER'
	cbmpGameOver.LoadBitmapW(IDB_GAMEOVER);
	// 'BONUS'
	cbmpBonus.LoadBitmapW(IDB_BONUS);
	// "NEXT'
	cbmpNext.LoadBitmapW(IDB_NEXT);
	// ending
	cbmpEnding.LoadBitmapW(IDB_ENDING);

	// 숫자 
	cbmpDigit.LoadBitmapW(IDB_DIGIT);
	cbmpDigit_sm.LoadBitmapW(IDB_DIGIT_SM);

	// 하마
	cbmpPot.LoadBitmapW(IDB_MAP_N);

	// 과일 
	cbmpFruit[0].LoadBitmapW(IDB_MAP_Q);
	cbmpFruit[1].LoadBitmapW(IDB_MAP_R);
	cbmpFruit[2].LoadBitmapW(IDB_MAP_S);
	cbmpFruit[3].LoadBitmapW(IDB_MAP_T);

	// 너구리 1탄
	cbmpBackO1.LoadBitmapW(IDB_Back1);
	cbmpBackO2.LoadBitmapW(IDB_Back2);

	// 너구리 
	cbmpCharL.LoadBitmapW(IDB_MoveLeft);
	cbmpCharR.LoadBitmapW(IDB_MoveRight);
	cbmpCharB.LoadBitmapW(IDB_Up);
	cbmpCharF1.LoadBitmapW(IDB_StandRight);
	cbmpCharF2.LoadBitmapW(IDB_StandLeft);
	cbmpCharLJump.LoadBitmapW(IDB_JumpLeft);
	cbmpCharRJump.LoadBitmapW(IDB_JumpRight);
	cbmpCharLJumpReady.LoadBitmapW(IDB_JumpLeftR);
	cbmpCharRJumpReady.LoadBitmapW(IDB_JumpRightR);
	cbmpCharLDrop.LoadBitmapW(IDB_DropRight);
	cbmpCharRDrop.LoadBitmapW(IDB_DropLeft);
	cbmpCharLDead.LoadBitmapW(IDB_AttackLeft);
	cbmpCharRDead.LoadBitmapW(IDB_AttackRight);
	cbmpCharLAttack.LoadBitmapW(IDB_DeadLeft);
	cbmpCharRAttack.LoadBitmapW(IDB_DeadRight);
	cbmpCharLDJump.LoadBitmapW(IDB_DJumpLeft);
	cbmpCharRDJump.LoadBitmapW(IDB_DJumpRight);

	// 깜짝 놀란 무늬
	cbmpSurprise.LoadBitmapW(IDB_SURPRISE);

	// 적 
	cbmpEnemyL.LoadBitmapW(IDB_ENEMY_LEFT);
	cbmpEnemyR.LoadBitmapW(IDB_ENEMY_RIGHT);
	cbmpEnemyLRed.LoadBitmapW(IDB_ENEMY_LEFT_RED);
	cbmpEnemyRRed.LoadBitmapW(IDB_ENEMY_RIGHT_RED);

	// 뱀 
	cbmpSnakeL.LoadBitmapW(IDB_SNAKE_LEFT);
	cbmpSnakeR.LoadBitmapW(IDB_SNAKE_RIGHT);

	// 악어
	cbmpBaby.LoadBitmapW(IDB_BABY);
	cbmpTile.LoadBitmapW(IDB_TILE); // 타일

	cbmpBkg2.LoadBitmapW(IDB_STG2_BKG);
	cbmpBkg3.LoadBitmapW(IDB_STG3_BKG);

	cbmpObj2.LoadBitmapW(IDB_STG2_OBJ);
	cbmpObj3.LoadBitmapW(IDB_STG3_OBJ);
}
void CGameSys::GameActivate()
{
	// 화면을 전부 다시 그리기 위해 
	_bIsDrawAll = TRUE;
}
void CGameSys::GameDeactivate()
{
	// 화면을 전부 다시 그리기 위해 
	_bIsDrawAll = TRUE;
}

void CGameSys::GamePaint(CDC* pDC)
{
	CDC memdc;
	CBitmap* oldBit;
	CRect crect;
	char i;

	m_parent->GetClientRect(&crect);
	memdc.CreateCompatibleDC(pDC);
	oldBit = (CBitmap*)memdc.SelectObject(&cbmpBit);

	switch (_GameState) {
	case gameIntro:
		// 171은 너구리가 자리를 다 잡고 춤추기 시작하는 시점 
		if (_bIsDrawAll == TRUE || _iAni == 1 || _iAni == 171)
			pDC->BitBlt(0, 0, crect.right, crect.bottom, &memdc, 0, 0, SRCCOPY);
		else
			pDC->BitBlt(120, 430, 780, 70, &memdc, 120, 430, SRCCOPY);
		break;

	case gamePlay:
		// SS: 일단 전체 영역을 다시 다 그리게 바꿔 둠.
		// SS: 영역 그리는 것과 관련한 코드가 뒤에 하나 더 있음. (#다시 그리는 부분 영역) 참고

		// 처음에는 전부 다 그리고 그 이후는 이전과 비교해서 변화는 부분만 다시 그린다.
		// 속도 향상을 위해 매번 전부 다시 그리지 않음 
		// 처음에 매번 화면 전체를 다시 그리게 만들었는데(구현 하기 쉬움)
		// 펜티엄200에서 테스트 결과 속도가 엄청나게 느림 
		// 그래서 변하는 부분만 다시 그리게 수정 했음 
		// 그래서 게임에서 화면을 그리는 부분이 조금 복잡해짐 
		//if (_bIsDrawAll == TRUE || _iAni == 1)

		if (_Rac.x > CAMERA_OFFSET_X && _Rac.x < WHOLE_GSW - VISUAL_GSW + CAMERA_OFFSET_X)
			pDC->BitBlt(
				0,
				0,
				VISUAL_GSW,
				crect.bottom,
				&memdc,
				_Rac.x - CAMERA_OFFSET_X,
				0,
				SRCCOPY);
		else if (_Rac.x <= CAMERA_OFFSET_X)
			pDC->BitBlt(
				0,
				0,
				VISUAL_GSW,
				crect.bottom,
				&memdc,
				0,
				0,
				SRCCOPY);
		else
			pDC->BitBlt(
				0,
				0,
				VISUAL_GSW,
				crect.bottom,
				&memdc,
				WHOLE_GSW - VISUAL_GSW,
				0,
				SRCCOPY);
		//else {
		//	//시간 
		//	pDC->BitBlt(TIME_BAR_OFFSET_L, TIME_BAR_OFFSET_T,
		//		TIME_LIMIT, TIME_BAR_HEIGHT, &memdc, TIME_BAR_OFFSET_L, TIME_BAR_OFFSET_T, SRCCOPY);

		//	//먹은과일 
		//	pDC->BitBlt(750, 110, 50, 440, &memdc, 750, 110, SRCCOPY);

		//	//너구리 주위
		//	pDC->BitBlt(
		//		_Rac.x - (RAC_MOVE_X_OFFSET * 2 + UNIT_BLOCK_X + 50),
		//		_Rac.y - (MACM_FALL_MAX_OFFSET * 2 + UNIT_BLOCK_Y + 50),
		//		(RAC_MOVE_X_OFFSET * 4) + RAC_W + 50 * 2,
		//		(MACM_FALL_MAX_OFFSET * 4) + RAC_H + 50 * 2,
		//		&memdc,
		//		_Rac.x - (RAC_MOVE_X_OFFSET * 2 + UNIT_BLOCK_X + 50),
		//		_Rac.y - (MACM_FALL_MAX_OFFSET * 2 + UNIT_BLOCK_Y + 50), SRCCOPY);

		//	//적 주위 
		//	for (i = 0; i <_EnemyCount; i++)
		//		pDC->BitBlt(_Ene[i].x - 60, _Ene[i].y - 260, 120, 260, &memdc,
		//		_Ene[i].x - 60, _Ene[i].y - 260, SRCCOPY);

		//	//토글 맵 주위
		//	for (i = 0; i <_ToggleElementCnt; i++)
		//		pDC->BitBlt(_Tog[i].x, _Tog[i].y, 50, 50, &memdc,
		//		_Tog[i].x, _Tog[i].y, SRCCOPY);

		//	//아이템 주위 
		//	for (i = 0; i < 12; i++)
		//		pDC->BitBlt(_Item[i].x, _Item[i].y, 50, 50, &memdc,
		//		_Item[i].x, _Item[i].y, SRCCOPY);

		//}
		break;
	case gameNextMap:
		if (_bIsDrawAll == TRUE || _iAni == 1)
			pDC->BitBlt(0, 0, crect.right, crect.bottom, &memdc, 0, 0, SRCCOPY);
		else
			pDC->BitBlt(500, 400, 550, 450, &memdc, 500, 400, SRCCOPY);
		break;

	case gameClear:
		if (_bIsDrawAll == TRUE || _iAni == 1)
			pDC->BitBlt(0, 0, crect.right, crect.bottom, &memdc, 0, 0, SRCCOPY);
		else
			pDC->BitBlt(500, 400, 550, 450, &memdc, 500, 400, SRCCOPY);

		break;

	case gameOver:
		if (_bIsDrawAll == TRUE || _iAni == 1)
			pDC->BitBlt(0, 0, crect.right, crect.bottom, &memdc, 0, 0, SRCCOPY);
		else {
			//'GAME OVER'
			pDC->BitBlt(0, 200, 900, 75, &memdc, 0, 200, SRCCOPY);
			//떨어지는 너구리 
			pDC->BitBlt(430, 0, 50, 590, &memdc, 430, 0, SRCCOPY);
		}
		break;
	}
	pDC->SelectObject(oldBit);
}
void CGameSys::GameCycle(CDC* pDC)
{
	switch (_GameState) {
	case gameIntro:
		_bIsDrawAll = FALSE;
		GameIntro(pDC);
		break;

	case gamePlay:
		_bIsDrawAll = FALSE;
		GamePlay(pDC);
		break;

	case gameNextMap: // SS: 다음 맵으로 넘어감. 클리어는 아님.
		_bIsDrawAll = FALSE;
		GameNextMap(pDC);
		break;

	case gameClear:
		_bIsDrawAll = FALSE;
		GameClear(pDC);
		break;

	case gameOver:
		_bIsDrawAll = FALSE;
		GameOver(pDC);
	}
}

void CGameSys::CheckFrame() {
	if (_Rac.state == frame) {
		_Rac.step++;
		if (_Rac.step == 20)
			_Rac.step = 0;
	}
	else
		_Rac.step = 0;
}


void CGameSys::HandleKeys(CDC* pDC)
{
	frame = _Rac.state;
	switch (_GameState) {

		//인트로 화면 
	case gameIntro:
		if (GetAsyncKeyState(JUMP) < 0) {
			//인트로 음악 종료 
			PlaySound(NULL, _hInstance, 0);
			// 초기화 
			_iLevel = 0;
			_iLive = LIVE;
			Init();
			LoadMap(pDC);
			_GameState = gamePlay;
			//스페이스를 조금 길게 누르면 게임이 시작하자 마자 너구리가 점프하므로 
			//이를 방지 하기 위해 
			Sleep(100);
		}
		break;

		//게임중
		//키를 동시에 두개 누른것을 처리하기 위해서는 API만으로는 불가 
		//다이렉트엑스를 쓰던지 키 버퍼를 만들어야 함 
		//여기서는 키 버퍼 비슷하게 구현 
		//왼쪽 입력이 들어온 상태에서 점프입력이 들어오면 또는 
		//오른쪽 입력이 들어온 상태에서 점프입력이 들어오면 
		//다시 말해 현재 입력이 점프이고 그 이전 입력이 오른쪽이나 왼쪽이면
		//길게 점프 함 
		//별도의 버퍼 메모리 없이 키 버퍼 구현 

	case gamePlay:
		char upOrDown;
		int x, y;
		switch (_Rac.state) {
			// SS: 일단 키 스테이트상 서있을 때와 움직일 때의 차이는 없기 때문에 같이 둠.
		case standing:
		case moving:
			if (GetAsyncKeyState(JUMP) < 0) {
				_Rac.state = jumpReady;
			}
			else if (GetAsyncKeyState(LEFT) < 0)
				if (_Rac.isToRight)      KeyLWhenR();
				else {
					KeyLWhenL();
				}
			else if (GetAsyncKeyState(RIGHT) < 0)
				if (_Rac.isToRight) {
					KeyRWhenR();
				}
				else               KeyRWhenL();
			else if (GetAsyncKeyState(UP) < 0) {
				if (_cMap[(_Rac.y - RAC_MOVE_Y_OFFSET) / UNIT_BLOCK_Y - 1][_Rac.x / UNIT_BLOCK_X] == 'F') {
					_Rac.state = goingUpDown;
				}
			}
			else if (GetAsyncKeyState(DOWN) < 0) {
				if (_cMap[(_Rac.y + RAC_MOVE_Y_OFFSET) / UNIT_BLOCK_Y + 1][_Rac.x / UNIT_BLOCK_X] == 'F') {
					_Rac.state = goingUpDown;
				}
			}
			else
			{
				_Rac.state = standing;
			}
			break;

		case goingUpDown: // SS: 사다리 탈 때만 해당 (기린)
			if (GetAsyncKeyState(UP) < 0) {
				// if (!CollisionCheck(toUp)) _Rac.y -= RAC_MOVE_Y_OFFSET; // for debug use
				if (_cMap[(_Rac.y - RAC_MOVE_Y_OFFSET) / UNIT_BLOCK_Y - 1][_Rac.x / UNIT_BLOCK_X] == 'F') {
					_Rac.y -= RAC_MOVE_Y_OFFSET;
				}
				else { _Rac.state = standing; }
				CheckFrame();
			}
			else if (GetAsyncKeyState(DOWN) < 0) {
				// if (!CollisionCheck(toUp)) _Rac.y += RAC_MOVE_Y_OFFSET; // for debug use
				if (_cMap[(_Rac.y + RAC_MOVE_Y_OFFSET) / UNIT_BLOCK_Y + 1][_Rac.x / UNIT_BLOCK_X] == 'F') {
					_Rac.y += RAC_MOVE_Y_OFFSET;
				}
				else { _Rac.state = standing; }
				CheckFrame();
			}
			else if (GetAsyncKeyState(LEFT) < 0)
				if (_Rac.isToRight)      KeyLWhenR();
				else {
					KeyLWhenL();
				}
			else if (GetAsyncKeyState(RIGHT) < 0)
				if (_Rac.isToRight) {
					KeyRWhenR();
				}
				else               KeyRWhenL();
				break;
		case jumpReady:  // SS: 점프 전 대기단계 1프레임
			_Rac.state = jumping;
			_Rac.jumpIdx = 0;
			break;
		case jumping:   // SS: 점프 상승단계 MACM_LEN 프레임
			x = _Rac.x;
			y = _Rac.y - macmUpward[_Rac.macmIdx];


			if (multiJump()) {
				
			}
			else if (GetAsyncKeyState(LEFT) < 0) {
				if (multiJump()) { }
				else jumpingDirection(toLeft, toUp, y);
			}
			else if (GetAsyncKeyState(RIGHT) < 0) {
				if (multiJump()) { }
				else jumpingDirection(toRight, toUp, y);
			}
			else {
				jumpingDirection(toNone, toUp, y);
			}


			if (_Rac.isInAirUpward == TRUE && _Rac.macmIdx >= MACM_LEN - 1) {
				_Rac.isInAirUpward = FALSE;
				_Rac.state = falling;
				_Rac.macmIdx = 0;
				_Rac.isDouble = FALSE;
			}

			CheckFrame();
			break;
		case falling: // SS: 점프/혹은 아무데서나 움직여서 떨어질 때 하강단계. 
			hippoIndex = 0;
			x = _Rac.x;
			if (Ishippo[currenthippo] == TRUE) {
				if (_Rac.macmIdx >= MACM_LEN)  // 처음 하강 MACM_LEN 프레임 이후 이동 최대 오프셋을
					y = _Rac.y + 49; // 지정
				else
					y = _Rac.y - hippoDownward[_Rac.macmIdx];
			}
			else {
				if (_Rac.macmIdx >= MACM_LEN)  // 처음 하강 MACM_LEN 프레임 이후 이동 최대 오프셋을
					y = _Rac.y + MACM_FALL_MAX_OFFSET; // MACM_FALL_MAX_OFFSET 으로 고정.
				else
					y = _Rac.y - macmDownward[_Rac.macmIdx];
			}


			_Rac.y = y;
			checkYBelowGround(y);
			_Rac.macmIdx++;

			if (fallJump()) {
			}
			else if (GetAsyncKeyState(LEFT) < 0) {
				if (fallJump()) {}
				else jumpingDirection(toLeft, toDown, y);
			}
			else if (GetAsyncKeyState(RIGHT) < 0) {
				if (fallJump()) {}
				else jumpingDirection(toRight, toDown, y);
			}
			else {
				jumpingDirection(toNone, toDown, y);
			}

			break;
		case landing: // SS: 착지 1프레임
			hippoIndex = 0;
			Ishippo[currenthippo] = FALSE;
			if (GetAsyncKeyState(JUMP) < 0) {
				_Rac.state = jumpReady;
			}
			else if (GetAsyncKeyState(LEFT) < 0)
				if (_Rac.isToRight)      KeyLWhenR();
				else               KeyLWhenL();
			else if (GetAsyncKeyState(RIGHT) < 0)
				if (_Rac.isToRight)      KeyRWhenR();
				else               KeyRWhenL();
			else
				_Rac.state = standing;
			break;
			//case hitStepBack: // 
			//   _Rac.state = dead;
			//   break;
		case onAlli:
			_Rac.animIdx++;
			ChangeRacX(15);
			if (_Rac.animIdx == 50) {
				Allix = _Rac.x;
				Alliy = _Rac.y;
				_Rac.state = landing;
			}
			break;
		case onMonkey:
			if (monkeIndex == 21) {
				IsMonkey = FALSE;
				monkeIndex = 0;
				_Rac.y -= 10;
				checkYBelowGround(_Rac.y);
				ChangeRacX(110);
			}
			if (!IsMonkey) {
				ChangeRacX(40);
				if (monkeIndex < 6) {
					_Rac.y -= monkeyyup[monkeIndex];
				}
				else {
					_Rac.y -= monkeyydown[monkeIndex - 6];
				}
				if (monkeIndex == 12) {
					monkeIndex = 0;
					_Rac.state = landing;
				}
				checkYBelowGround(_Rac.y);
			}
			monkeIndex++;
			break;
		case onHippo:
			if (Ishippo[currenthippo]) {
				if (monkeIndex < 7) {
					_Rac.y -= hippoUpward[hippoIndex];
				}
				else
					Ishippo[currenthippo] = FALSE;
			}
			else {
				_Rac.y -= hippoDownward[hippoIndex - 7];
			}

			checkYBelowGround(_Rac.y);

			if (GetAsyncKeyState(LEFT) < 0)
			if (_Rac.isToRight)      KeyLWhenR();
			else {
				_Rac.x -= 20;
			}
			else if (GetAsyncKeyState(RIGHT) < 0)
			if (_Rac.isToRight) {
				_Rac.x += 20;
			}
			hippoIndex++;
			if (hippoIndex == 14) {
				hippoIndex = 0;
				_Rac.state = falling;
			}
			break;
		case dead:
			// TODO:
			break;
   
    	   	}


		break;

	case gameNextMap:
		break;

	case gameClear: //레벨 클리어 
		break;

	case gameOver: //GAME OVER
		if (GetAsyncKeyState(JUMP) < 0) {
			PlaySound(NULL, _hInstance, 0);
			_iAni = 0;
			_GameState = gameIntro;
			Sleep(100);
		}

	}

	if (_Rac.isRolling) {
		if (_Rac.isToRight)
			ChangeRacX(10);
		else
			ChangeRacX(-10);
	}

	if (_Rac.state == frame) {
		if (_Rac.state == standing) {
			_Rac.step++;
			if (_Rac.step == 20)
				_Rac.step = 0;
		}
	}
	else
		_Rac.step = 0;

}

#define SAFE_DELETE(a) if (a) { delete a; a=NULL; }
#define SAFE_FREE(a) if (a) { free(a); a=NULL; }

void CGameSys::LoadMap(CDC* pDC)
{
	CDC memdc;
	CRect crect;
	CBitmap* oldBit;
	CBrush myBrush;
	CBrush* oldBrush;

	HRSRC hRSrc;
	HGLOBAL hMem;
	PVOID ptr;

	DWORD size;
	char* str = NULL;
	int m_index = 0;
	char ch;

	// 리소스에서 맵 읽어옴 
	hRSrc = FindResource(_hInstance, MAKEINTRESOURCE(IDR_MAP), _T("TEXT"));
	size = SizeofResource(_hInstance, hRSrc);
	hMem = LoadResource(_hInstance, hRSrc);
	ptr = LockResource(hMem);
	str = (char*)malloc(sizeof(char)*(size + 1));
	memcpy(str, ptr, size);
	str[size] = 0;

	// 레벨을 고려해서 맵 파일에서 offset계산 
	// +3은 #을 skip하기 위해 
	m_index += ((MAP_BLOCK_Y_COUNT*(MAP_BLOCK_X_COUNT + 2)) + 3) * _iLevel + 3;

	static int i, j;

	for (i = 0; i < MAP_BLOCK_Y_COUNT; i++) {
		for (j = 0; j < MAP_BLOCK_X_COUNT + 1; j++) {
			ch = str[m_index++];
			_cMap[i][j] = ch;
		}
		ch = str[m_index++];
	}

	//할당 받은 메모리 반환
	SAFE_FREE(str);

	// 맵을 메모리에 그려서 하나의 비트맵으로 만듬 
	// 이처럼 하는 이유는 
	// 만약에 맵을 다시 그려야할 경우가 생길때 
	// 맵을 전체 이미지 하나로 따로 저장해두지 않으면 
	// 리소스의 맵 파일로 부터 매번 읽어와야 한다.
	// 이렇게 되면 I/O가 많아 지므로 비효율적이다.
	// 따라서 효율을 높이기위해 이렇게 한다. 
	// 그리고 맵을 일부분만 다시 그릴때 위와 같이 만들어 두는 것이 더 편리하다.

	m_parent->GetClientRect(&crect);

	if (cbmpMap.m_hObject == NULL)
		cbmpMap.CreateCompatibleBitmap(pDC, WHOLE_GSW, crect.bottom);

	memdc.CreateCompatibleDC(pDC);
	oldBit = (CBitmap*)memdc.SelectObject(cbmpMap);

	myBrush.CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (CBrush*)memdc.SelectObject(myBrush);

	memdc.FillRect(&crect, &myBrush);

	char index = 0;
	itemCount = 0;

	//////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	switch (_iLevel) {
	case tutorial:
		// 배경 처리...
		DrawBitmap(&memdc, 0, 0, cbmpBackO2, true, RGB(255, 0, 255));
		DrawBitmap(&memdc, 0, 0, cbmpBackO1, true, RGB(255, 0, 255));
		break;
	case savana:
		DrawBitmap(&memdc, 0, 0, cbmpBkg2, false);
		DrawBitmap(&memdc, 0, -10, cbmpObj2, true, RGB(255, 0, 255));
		break;
	case amazon:
		DrawBitmap(&memdc, 0, 0, cbmpBkg3, false);
		DrawBitmap(&memdc, 0, 50, cbmpObj3, 0, 0, 6216, 512);
		break;
	}

	for (i = 0; i < MAP_BLOCK_Y_COUNT; i++) {
		for (j = 0; j < MAP_BLOCK_X_COUNT; j++) {
			switch (_cMap[i][j]) {
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
				//DrawBitmap(&memdc, j * UNIT_BLOCK_X, i * UNIT_BLOCK_Y, cbmpBkg, TRUE);
				//DrawBitmap(&memdc, j * UNIT_BLOCK_X, i * UNIT_BLOCK_Y, cbmpMapEle[_cMap[i][j] - 65], TRUE);
				break;
			case 'G':
				// 적이라면
				// *-- 적규칙 --
				// G부터 시작 
				// G는 오른쪽 봄 , H는 왼쪽 봄 , 둘은 속도 동일 2 + 0
				// I는 오른쪽 봄 , J는 왼쪽 봄 , 둘은 속도 동일 2 + 1 
				// K는 오른쪽 봄 , L는 왼쪽 봄 , 둘은 속도 동일 2 + 2 
				_Ene[_EnemyCount].x = j * UNIT_BLOCK_X + UNIT_BLOCK_HX;
				_Ene[_EnemyCount].y = i * UNIT_BLOCK_Y + UNIT_BLOCK_Y;
				_Ene[_EnemyCount].isRight = TRUE;
				_Ene[_EnemyCount].speed = 5;
				//_Ene[_EnemyCount].type = TRUE;
				//_Ene[_EnemyCount].alpha = 255;	//나중에 뱀만 사용한다. 
				_Ene[_EnemyCount].sxBlockIndex = j;
				break;
			case 'H':
				_Ene[_EnemyCount].exBlockIndex = j;
				_EnemyCount++;
				break;
			case 'I':
				Allix = j * UNIT_BLOCK_X;
				Alliy = i * UNIT_BLOCK_Y - 40;
				break;
			case 'R':
				_Tog[_ToggleElementCnt].x = j * UNIT_BLOCK_X;
				_Tog[_ToggleElementCnt].y = i * UNIT_BLOCK_Y;
				_Tog[_ToggleElementCnt].i = j;
				_Tog[_ToggleElementCnt].j = i;
				_Tog[_ToggleElementCnt].alpha = 255;
				_Tog[_ToggleElementCnt].incAlpha = FALSE;
				_Tog[_ToggleElementCnt].delay = TOG_RESET_DELAY;
				_ToggleElementCnt++;
				break;
			case 'M':
				_Tog[_ToggleElementCnt].x = j * UNIT_BLOCK_X;
				_Tog[_ToggleElementCnt].y = i * UNIT_BLOCK_Y;
				_Tog[_ToggleElementCnt].i = j;
				_Tog[_ToggleElementCnt].j = i;
				_Tog[_ToggleElementCnt].alpha = 0;
				_Tog[_ToggleElementCnt].incAlpha = TRUE;
				_Tog[_ToggleElementCnt].delay = TOG_RESET_DELAY;
				_ToggleElementCnt++;
				break;
			case 'N':
				Monkeyx = j * UNIT_BLOCK_X;
				Monkeyy = i * UNIT_BLOCK_Y;
				break;
			case 'O':
				Hippox[currenthippo] = j * UNIT_BLOCK_X;
				Hippoy[currenthippo] = i * UNIT_BLOCK_Y;
				currenthippo++;
				break;
			case 'P':
				_Item[itemCount].i = j;
				_Item[itemCount].j = i;
				_Item[itemCount].x = j*UNIT_BLOCK_X;
				_Item[itemCount].y = i*UNIT_BLOCK_Y;
				_Item[itemCount].ch = 'P';
				itemCount++;
				break;
			case 'Q':
				//아이템 (과일, 항아리)
				_Item[index].x = j * UNIT_BLOCK_X;
				_Item[index].y = i * UNIT_BLOCK_Y;
				_Item[index].ch = _cMap[i][j];
				index++;
				break;
			}
		}
	}
	// 과일 (게임 상단에 현재 레벨을 알수 있는 과일들)
	// 당근 , 앵두 ... 
	/*for (i = 0; i < _iLevel+1; i++)
	DrawBitmap(&memdc, (670 - _iLevel * 55) + i * 55, 70, cbmpFruit[i], TRUE);*/
}


void CGameSys::GameIntro(CDC* pDC)
{
	CRect crect;
	CDC memdc;
	CBitmap* oldBit;
	CBrush backBrush;
	CBrush* oldBrush;

	m_parent->GetClientRect(&crect);

	if (cbmpBit.m_hObject == NULL)
		cbmpBit.CreateCompatibleBitmap(pDC, WHOLE_GSW, crect.bottom);

	memdc.CreateCompatibleDC(pDC);
	oldBit = (CBitmap*)memdc.SelectObject(cbmpBit);

	backBrush.CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (CBrush*)memdc.SelectObject(backBrush);

	//시간
	if (_iAni == 1000)
		_iAni = 0;
	else
		_iAni++;

	DrawBitmap(&memdc, 0, 0, cbmpGameIntro, FALSE);
	PlaySound(MAKEINTRESOURCE(IDR_INTRO), _hInstance, SND_RESOURCE | SND_ASYNC);

	m_parent->InvalidateRect(NULL, FALSE);
}


void CGameSys::GamePlay(CDC* pDC)
{
	CRect crect;
	CDC memdc;
	CBitmap* oldBit;
	CPen myPen;
	CPen* oldPen;
	CBrush myBrush;
	CBrush* oldBrush;

	char i;

	m_parent->GetClientRect(&crect);

	if (cbmpBit.m_hObject == NULL)
		cbmpBit.CreateCompatibleBitmap(pDC, WHOLE_GSW, crect.bottom);

	memdc.CreateCompatibleDC(pDC);
	oldBit = (CBitmap*)memdc.SelectObject(cbmpBit);

	// 시간 바를 표시할 브러시와 펜 
	myPen.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	oldPen = (CPen*)memdc.SelectObject(myPen);

	myBrush.CreateSolidBrush(RGB(25, 184, 5));
	oldBrush = (CBrush*)memdc.SelectObject(myBrush);

	//에니메이션 구현을 위해 	
	//너구리가 죽으면 움직임을 멈추는 것들은 _iTime을 기준으로 에니메이션
	//너구리가 죽으면서 하는 움직임은 _iAni을 이용 	
	_iAni++;

	// Play종료 조건 

	// 너구리 죽음 	
	if (_Rac.state == dead) {
		PlaySound(NULL, _hInstance, 0);
		PlaySound(MAKEINTRESOURCE(IDR_RAC_DIE), _hInstance, SND_RESOURCE | SND_ASYNC);
		Sleep(1500);
		if (_iLive == 0) { // 마지막 너구리가 죽으면 
			Init();
			_GameState = gameOver;	//gameover
		}
		else {
			_iLive--;
			Init();
			LoadMap(pDC);
		}
	}



	// 남은 시간	
	// 너구리가 죽으면 시간은 멈춘다.
	if (_iAni % 5 == 0 && _Rac.state != dead) {
		_iTime--;
		if (_iTime == 0)
			_Rac.state = dead;	//너구리 죽음 		
	}

	// SS: #다시 그리는 부분 영역
	CRect r = _Rac.bmpRect;
	// 맵(처음 시작 할때 맵 전체를 한번 그린다.)
	if (_iAni == 1) {
		DrawBitmap(&memdc, 0, 0, cbmpMap);
		if (_iLevel == savana) {
			DrawMonkey(&memdc, 0, Monkeyx, Monkeyy);
		}
		else if (_iLevel == amazon) {
			DrawAlli(&memdc, 0, Allix, Alliy);
			for (int i = 0; i < 4; i++) {
				DrawHippo(&memdc, 0, Hippox[i], Hippoy[i]);
			}
		}
	}
	else {

		//너구리 주위
		DrawBitmap(&memdc,
			r.left,
			r.top,
			cbmpMap,
			r.left,
			r.top,
			r.right,
			r.bottom,
			false
			);
		
		/*DrawBitmap(&memdc,
			_Rac.x - (RAC_MOVE_X_OFFSET * 2 + UNIT_BLOCK_X + 50),
			_Rac.y - (MACM_FALL_MAX_OFFSET * 2 + UNIT_BLOCK_Y + 50),
			cbmpMap,
			_Rac.x - (RAC_MOVE_X_OFFSET * 2 + UNIT_BLOCK_X + 50),
			_Rac.y - (MACM_FALL_MAX_OFFSET * 2 + UNIT_BLOCK_Y + 50),
			_Rac.x + (RAC_MOVE_X_OFFSET * 2 + UNIT_BLOCK_X + 50),
			_Rac.y + (MACM_FALL_MAX_OFFSET * 2 + UNIT_BLOCK_Y + 50),
			false
			);*/

		//적 주위 
		for (i = 0; i < _EnemyCount; i++) {
			CRect r = _Ene[i].bmpRect;
			DrawBitmap(&memdc,
				r.left,
				r.top,
				cbmpMap,
				r.left,
				r.top,
				r.right,
				r.bottom);
		}

		for (i = 0; i <_ToggleElementCnt; i++)
			DrawBitmap(&memdc, _Tog[i].x, _Tog[i].y, cbmpMap, _Tog[i].x, _Tog[i].y, _Tog[i].x + 50, _Tog[i].y + 50, FALSE);

		if (_iLevel == amazon) {
			if (IsAlli == TRUE) {

				DrawAlli(&memdc, 0, Allix, Alliy);

				DrawBitmap(&memdc, Allix - 1000, Alliy + 40, cbmpMap, Allix - 1000, Alliy + 40, Allix + 700, Alliy + 90);

				if (_Rac.animIdx == 50)

					DrawAlli(&memdc, 0, Allix, Alliy);

			}

			else if (IsAlli == FALSE) {

				DrawAlli(&memdc, 0, Allix, Alliy);
			}

			for (int i = 0; i < 4; i++) {
				if (Ishippo[i] == TRUE) {
					DrawBitmap(&memdc, Hippox[i], Hippoy[i] - 150, cbmpMap, Hippox[i], Hippoy[i] - 150, Hippox[i] + 300, Hippoy[i] + 150);
					DrawHippo(&memdc, hippoIndex, Hippox[i], Hippoy[i]);
					
				}
				else
					DrawHippo(&memdc, 0, Hippox[i], Hippoy[i]);
			}
		}
		else if (_iLevel == savana) {
			if (IsMonkey == TRUE) {
				DrawBitmap(&memdc, Monkeyx - 100, Monkeyy - 180, cbmpMap, Monkeyx - 100, Monkeyy - 180, Monkeyx + 100, Monkeyy + 180);
				DrawMonkey(&memdc, monkeIndex, Monkeyx, Monkeyy);
			} 
    	     else
				DrawMonkey(&memdc, 0, Monkeyx, Monkeyy);
		}
	}

	// 시간 바 표시 
	if (_iAni % 50 == 0 || _iAni == 1) {
		memdc.FillSolidRect(TIME_BAR_OFFSET_L, TIME_BAR_OFFSET_T,
			TIME_LIMIT - _iTime, TIME_BAR_HEIGHT, RGB(3, 40, 120));
		memdc.Rectangle(TIME_LIMIT + TIME_BAR_OFFSET_L - _iTime, TIME_BAR_OFFSET_T,
			TIME_LIMIT + TIME_BAR_OFFSET_L, TIME_BAR_OFFSET_T + TIME_BAR_HEIGHT);
	}

	//먹은 과일 수 (이전과 변화가 있을 때만 그린다.)
	static char Eat = -1;
	if (_iEat != Eat) {
		for (i = 0; i < _iEat; i++)
			DrawBitmap(&memdc, 750, 490 - (i * 55), cbmpFruit[_iLevel], FALSE);
		Eat = _iEat;
	}

	DrawToggledMapElements(&memdc);
	DrawItems(&memdc);
	DrawEnemies(&memdc);
	DrawRaccoon(&memdc);

	if (_Rac.state == standing)
		DynamicBoxCheck();
	//적 충돌 감지 
	CheckCollision_Enemy();
	if (_Rac.state == dead) {
		CRect r2 = _Ene[deadEnemy].bmpRect;
		DrawBitmap(&memdc,r.left,r.top,cbmpMap,r.left,r.top,r.right,r.bottom,false);
		DrawBitmap(&memdc, r2.left, r2.top, cbmpMap, r2.left, r2.top, r2.right, r2.bottom, false);
		if (_Ene[deadEnemy].isRight) {
			DrawEneR(&memdc, _Ene[deadEnemy].animIdx, _Ene[deadEnemy].x, _Ene[deadEnemy].y);
		}
		else
			DrawEneL(&memdc, _Ene[deadEnemy].animIdx, _Ene[deadEnemy].x, _Ene[deadEnemy].y);

		DrawRaccoon(&memdc);
	}

	m_parent->InvalidateRect(NULL, FALSE);
}


void CGameSys::GameClear(CDC* pDC)
{
	CRect crect;
	CDC memdc;
	CBitmap* oldBit;
	CBrush backBrush;
	CBrush* oldBrush;

	static int bonus;

	m_parent->GetClientRect(&crect);

	if (cbmpBit.m_hObject == NULL)
		cbmpBit.CreateCompatibleBitmap(pDC, WHOLE_GSW, crect.bottom);

	memdc.CreateCompatibleDC(pDC);
	oldBit = (CBitmap*)memdc.SelectObject(cbmpBit);

	backBrush.CreateSolidBrush(RGB(25, 184, 5));
	oldBrush = (CBrush*)memdc.SelectObject(backBrush);

	// 화면 지우기
	_iAni++;


	// 마지막 레벨 성공여부	
	if (_iLevel != LAST_LEVEL) {

		if (_iAni == 130) {
			_iLevel++;
			Init();
			LoadMap(pDC);
			_GameState = gamePlay;
		}

		DrawBitmap(&memdc, 0, 0, cbmpGameClear, FALSE);
		memdc.FillSolidRect(430, 435, 580, 480, RGB(255,255,255));

	}
	else {
		//마지막 레벨 성공 	
		if (_iAni == 400) {
			Init();
			_GameState = gameIntro;
		}
		// ending 메세지
		DrawBitmap(&memdc, 0, 0, cbmpGameClear, FALSE);
	}

	m_parent->InvalidateRect(NULL, FALSE);
}

void CGameSys::GameOver(CDC* pDC)
{
	CRect crect;
	CDC memdc;
	CBitmap* oldBit;
	CPen myPen;
	CPen* oldPen;
	CBrush myBrush, backBrush;
	CBrush* oldBrush;

	m_parent->GetClientRect(&crect);

	if (cbmpBit.m_hObject == NULL)
		cbmpBit.CreateCompatibleBitmap(&memdc, crect.right, crect.bottom);

	memdc.CreateCompatibleDC(pDC);
	oldBit = (CBitmap*)memdc.SelectObject(cbmpBit);

	myPen.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	oldPen = (CPen*)memdc.SelectObject(myPen);

	myBrush.CreateSolidBrush(RGB(165, 113, 66));
	backBrush.CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (CBrush*)memdc.SelectObject(myBrush);

	if (_iAni != 350)
		_iAni++;
	else {
		Init();
		_GameState = gameIntro;
	}

	DrawBitmap(&memdc, 0, 0, cbmpGameOver, FALSE);

	if (_iAni == 1) {
		PlaySound(MAKEINTRESOURCE(IDR_GAMEOVER), _hInstance, SND_RESOURCE | SND_ASYNC);
	}

	if (_iAni == 110) {
		PlaySound(MAKEINTRESOURCE(IDR_RAC_DIE), _hInstance, SND_RESOURCE | SND_ASYNC);
	}

	m_parent->InvalidateRect(NULL, FALSE);
}

void CGameSys::Init()
{
	//초기화 
	_iTime = TIME_LIMIT;	//게임 제한 시간 
	_iEat = 0;				//먹은 과일수 
	_iAni = 0;				//애니메이션 효과를 위해 
	_bIsDrop_Sound = FALSE;	//너구리가 떨어질때 나는 소리 상태 

	_EnemyCount = 0;		//적의 숫자 
	_ToggleElementCnt = 0;
	_iToggleTimerCnt = 0;
	_bToggleMap = FALSE;

	_Rac.state = standing;
	_Rac.isToRight = toRight;
	_Rac.x = RAC_HW;
	_Rac.y = UNIT_BLOCK_Y * (MAP_BLOCK_Y_COUNT - 2);
	_Rac.animIdx = 0;
	_Rac.macmIdx = 0;
	_Rac.isInAirUpward = TRUE;
	_Rac.jumpIdx = 0;
	_Rac.isDouble = FALSE;
	_Rac.isRolling = FALSE;

	for (int i = 0; i < _EnemyCount; i++) {
		_Ene[i].hit = FALSE;
		_Ene[i].animIdx = 0;
	}
}



void CGameSys::KeyLWhenR()
{
	_Rac.isToRight = toLeft;
}
void CGameSys::KeyRWhenL()
{
	_Rac.isToRight = toRight;
}
void CGameSys::KeyRWhenR()
{
	BOOL checkToDown = CollisionCheck(toDown);
	BOOL checkToRight = CollisionCheck(toRight);

	if (GetAsyncKeyState(JUMP) < 0)
		_Rac.state = jumpReady;
	else if (!checkToDown) {
		_Rac.state = falling;
		_Rac.macmIdx = 0;
	}
	else if (checkToRight) {
	}
	else if (checkToDown) {
		ChangeRacX(10);
		_Rac.state = moving;
	}

	if (_Rac.state == frame) {
		_Rac.step++;
		if (_Rac.step == 20)
			_Rac.step = 0;
	}
	else
		_Rac.step = 0;
}
void CGameSys::KeyLWhenL()
{
	BOOL checkToDown = CollisionCheck(toDown);
	BOOL checkToLeft = CollisionCheck(toLeft);

	if (GetAsyncKeyState(JUMP) < 0)
		_Rac.state = jumpReady;
	else if (!checkToDown) {
		_Rac.state = falling;
		_Rac.macmIdx = 0;
	}
	else if (checkToLeft) {
	}
	else if (checkToDown) {
		ChangeRacX(-10);
		_Rac.state = moving;
	}

	if (_Rac.state == frame) {
		_Rac.step++;
		if (_Rac.step == 20)
			_Rac.step = 0;
	}
	else
		_Rac.step = 0;
}

BOOL CGameSys::CollisionCheck(char direction)
{
	int x, y;
	char i, j, ci1, cj1, ci2, cj2;
	x = _Rac.x;
	y = _Rac.y;
	i = x / UNIT_BLOCK_X;
	j = y / UNIT_BLOCK_Y;
	char modX, modY;
	modX = x % UNIT_BLOCK_X;
	modY = y % UNIT_BLOCK_Y;

	BOOL result = FALSE;

	switch (direction) {
	case toLeft:
		if (modX) i++;
		if (PartialCollisionCheckL(i, j)) result = TRUE;
		if (modY) j++;
		if (PartialCollisionCheckL(i, j)) result = TRUE;
		break;
	case toRight:
		if (PartialCollisionCheckR(i, j)) result = TRUE;
		if (modY) j++;
		if (PartialCollisionCheckR(i, j)) result = TRUE;
		break;
	case toUp:
		if (modY) j++;
		if (PartialCollisionCheckU(i, j)) result = TRUE;
		if (modX) i++;
		if (PartialCollisionCheckU(i, j)) result = TRUE;
		break;
	case toDown:
		//if (modY == 0)break; // uncommenting it ignores collision
		if (PartialCollisionCheckD(i, j)) result = TRUE;
		if (modX) i++;
		if (PartialCollisionCheckD(i, j)) result = TRUE;
		break;
	}

	return result;
}
void CGameSys::DynamicBoxCheck()
{
	int x, y;
	char i, j;
	x = _Rac.x;
	y = _Rac.y;
	i = x / UNIT_BLOCK_X;
	j = y / UNIT_BLOCK_Y;
	char modX, modY;
	modX = x % UNIT_BLOCK_X;
	modY = y % UNIT_BLOCK_Y;

	BOOL result = FALSE;

	//if (modY == 0)break; // uncommenting it ignores collision
	if (PartialCollisionCheckD(i, j)) result = TRUE;
	if (modX) i++;
	if (PartialCollisionCheckD(i, j)) result = TRUE;

	if (!result) {
		_Rac.state = falling;
		_Rac.macmIdx = 0;
		_Rac.isInAirUpward = FALSE;
	}
}
BOOL CGameSys::OnMapElementCollide(char ci, char cj, char updateMode, char iOrJ)
{
	BOOL result = FALSE;
	char a;

	switch (_cMap[cj][ci]) {
	case 'E':
		_Rac.state = hitStepBack;
		break;
	case 'N':
		IsMonkey = TRUE;
		_Rac.state = onMonkey;
		_Rac.isInAirUpward = TRUE;
		_Rac.macmIdx = 0;
		break;
	case 'R':
	case 'M':
		for (a = 0; a < _ToggleElementCnt; a++) {
			if (_Tog[a].i == ci && _Tog[a].j == cj) {
				if (_Tog[a].alpha == 255) {
					switch (updateMode) {
					case horizontal:
						SetRacX(iOrJ * UNIT_BLOCK_X);
						break;
					case vertical:
						_Rac.y = iOrJ * UNIT_BLOCK_Y;
						checkYBelowGround(_Rac.y);
						break;
					}
					result = TRUE;
				}
				break;
			}
		}
		break;
	case 'D':
	case 'C':
		switch (updateMode) {
		case horizontal:
			SetRacX(iOrJ * UNIT_BLOCK_X);
			break;
		case vertical:
			_Rac.y = iOrJ * UNIT_BLOCK_Y;
			checkYBelowGround(_Rac.y);
			break;
		}
		result = TRUE;
		break;
	case 'I':
		_Rac.state = onAlli;
		_Rac.isInAirUpward = TRUE;
		_Rac.macmIdx = 0;
		IsAlli = TRUE;
		break;
	case 'J':
		_Rac.y = iOrJ * UNIT_BLOCK_Y;
		_Rac.state = onHippo;
		_Rac.isInAirUpward = TRUE;
		_Rac.macmIdx = 0;
		for (int i = 0; i < 4; i++) {
			if ((ci * UNIT_BLOCK_X - Hippox[i]) <= 150 && (ci * UNIT_BLOCK_X - Hippox[i]) >= 0) {
				Ishippo[i] = TRUE;
				currenthippo = i;
			}
		}
		
		break;
	case 'P':
		for (a = 0; a < itemCount; a++) {
			Item item = _Item[a];
			if (item.i == ci && item.j == cj) {
				_Rac.isRolling = TRUE;
				_Item[a].ch = '#';
			}

		}
		break;
	case 'Y':
		// 다음 맵으로..
		switch (updateMode) {
		case horizontal:
		case vertical:
			_GameState = gameNextMap;
		}
		break;
	case 'Z':
		// 다음 스테이지로...
		switch (updateMode) {
		case horizontal:
		case vertical:
			Sleep(500);
			_iAni = 0;
			_GameState = gameClear;
		}
		break;
	}
	return result;
}

BOOL CGameSys::PartialCollisionCheckL(char i, char j)
{
	BOOL result = FALSE;
	char ci1, ci2, cj1, cj2;
	ci1 = ci2 = i - 2;
	cj1 = j - 1;
	cj2 = j;
	if (ci1 >= 0) {
		if (cj1 >= 0) {
			if (OnMapElementCollide(ci1, cj1, horizontal, i)) result = TRUE;
		}
		if (OnMapElementCollide(ci2, cj2, horizontal, i)) result = TRUE;
	}
	return result;
}
BOOL CGameSys::PartialCollisionCheckR(char i, char j)
{
	BOOL result = FALSE;
	char ci1, ci2, cj1, cj2;
	ci1 = ci2 = i + 1;
	cj1 = j - 1;
	cj2 = j;

	if (ci1 < MAP_BLOCK_X_COUNT) {
		if (cj1 >= 0) {
			if (OnMapElementCollide(ci1, cj1, horizontal, i)) result = TRUE;
		}
		if (OnMapElementCollide(ci2, cj2, horizontal, i)) result = TRUE;
	}
	return result;
}
BOOL CGameSys::PartialCollisionCheckU(char i, char j)
{
	BOOL result = FALSE;
	char ci1, ci2, cj1, cj2;
	ci1 = i - 1;
	ci2 = i;
	cj1 = cj2 = j - 2;

	if (cj1 >= 0) {
		if (ci1 >= 0) {
			if (OnMapElementCollide(ci1, cj1, vertical, j)) result = TRUE;
		}
		if (OnMapElementCollide(ci2, cj2, vertical, j)) result = TRUE;
	}
	return result;
}
BOOL CGameSys::PartialCollisionCheckD(char i, char j)
{
	BOOL result = FALSE;
	char ci1, ci2, cj1, cj2;
	ci1 = i - 1;
	ci2 = i;
	cj1 = cj2 = j + 1;

	if (cj1 < MAP_BLOCK_Y_COUNT) {
		if (ci1 >= 0) {
			if (OnMapElementCollide(ci1, cj1, vertical, j)) result = TRUE;
		}
		if (OnMapElementCollide(ci2, cj2, vertical, j)) result = TRUE;
	}
	return result;
}

void CGameSys::jumpingDirection(char horizontalDirection, char verticalDirection, int y)
{
	char horiDir = horizontalDirection;
	switch (horiDir) {
	case toRight:
		_Rac.isToRight = toRight;
		break;
	case toLeft:
		_Rac.isToRight = toLeft;
	}

	switch (verticalDirection) {
	case toUp:
		if (CollisionCheck(toUp)) {
			_Rac.state = falling;
			_Rac.macmIdx = 0;
		}
		else {
			_Rac.y = y;
			checkYBelowGround(y);
			_Rac.macmIdx++;
			if (horiDir != toNone)
				if (CollisionCheck(horiDir)) {
				}
				else {
					if (horiDir == toRight)
						ChangeRacX(10);
					else
						ChangeRacX(-10);
				}
		}
		break;
	case toDown:
		if (CollisionCheck(toDown)) {
			_Rac.state = landing;
			_Rac.macmIdx = 0;
			_Rac.isInAirUpward = TRUE;
		}
		else {
			if (horiDir != toNone) {
				if (CollisionCheck(horiDir)) {
				}
				else {
					if (horiDir == toRight)
						ChangeRacX(10);
					else
						ChangeRacX(-10);
				}
			}
		}
		break;
	}
}
void CGameSys::fallingDirection(char horizontalDirection) {

	char horiDir = horizontalDirection;
	switch (horiDir) {
	case toRight:
		_Rac.isToRight = toRight;
		break;
	case toLeft:
		_Rac.isToRight = toLeft;
	}

	if (CollisionCheck(toDown)) {
		_Rac.state = landing;
		_Rac.isInAirUpward = TRUE;
		_Rac.macmIdx = 0;
	}
	else {
		if (horiDir != toNone)
			if (CollisionCheck(_Rac.isToRight)) {

			}
			else {
				if (horiDir == toRight)
					ChangeRacX(10);
				else
					ChangeRacX(-10);
			}
	}
}
BOOL CGameSys::multiJump()
{
	BOOL result = GetAsyncKeyState(JUMP) < 0
		&& _Rac.macmIdx > MULTI_JUMP_MIN_MACM_IDX
		&& (_Rac.jumpIdx < RAC_MAX_JUMP_COUNT - 1);
	if (result) {
		_Rac.jumpIdx++;
		_Rac.macmIdx = 0;
		_Rac.isInAirUpward = TRUE;
		_Rac.isDouble = TRUE;
	}
	return result;
}

BOOL CGameSys::fallJump()
{
	BOOL result = GetAsyncKeyState(JUMP) < 0
		&& (_Rac.jumpIdx < RAC_MAX_JUMP_COUNT - 1);
	if (result) {
		_Rac.jumpIdx++;
		_Rac.macmIdx = 0;
		_Rac.isInAirUpward = TRUE;
		_Rac.state = jumping;
		_Rac.isDouble = TRUE;
	}
	return result;
}


void CGameSys::DrawEnemies(CDC* memdc)
{
	char i;

	for (i = 0; i <_EnemyCount; i++) {

		if (_Rac.state != dead) {
			if (_Ene[i].hit) {
				if (_Ene[i].animIdx < 13) {
					if (_Ene[i].isRight) { // to the Right
						DrawHitEneR(memdc, _Ene[i].animIdx, _Ene[i].x, _Ene[i].y);
					}
					else { // to the Left
						DrawHitEneL(memdc, _Ene[i].animIdx, _Ene[i].x, _Ene[i].y);
					}
					_Ene[i].animIdx++;
				}
			}
			else {
				if (_Ene[i].isRight) { // to the Right
					if (_Ene[i].x - UNIT_BLOCK_HX == _Ene[i].exBlockIndex * UNIT_BLOCK_X) {
						_Ene[i].speed = -5;
						_Ene[i].isRight = toLeft;
					}
					else {
						_Ene[i].x += _Ene[i].speed;
					}
					DrawEneR(memdc, _iAni, _Ene[i].x, _Ene[i].y);
				}
				else { // to the Left
					if (_Ene[i].x - UNIT_BLOCK_HX == _Ene[i].sxBlockIndex * UNIT_BLOCK_X) {
						_Ene[i].speed = 5;
						_Ene[i].isRight = toRight;
					}
					else {
						_Ene[i].x += _Ene[i].speed;
					}
					DrawEneL(memdc, _iAni, _Ene[i].x, _Ene[i].y);
				}
			}
		}
		Enemy en = _Ene[i];
		_Ene[i].bmpRect = CRect(en.x - 60, en.y - 310, en.x + 180, en.y);
	}
}


void CGameSys::DrawToggledMapElements(CDC* memdc)
{
	//SS: 사라졌다 나타났다 하는 블럭. 개발 우선순위 나중
	char i;


	//바뀌는 맵 요소 그리기
	for (i = 0; i < _ToggleElementCnt; i++) {
		Tog tog = _Tog[i];

		if (tog.incAlpha) {
			if (tog.alpha < 255) {
				_Tog[i].alpha += TOG_ALPHA_OFFSET;	//선명하게  
				DrawBitmapAlpha(memdc, tog.x, tog.y, cbmpTile, tog.alpha);
			}
			else if (tog.delay) {
				_Tog[i].delay--;
				DrawBitmap(memdc, tog.x, tog.y, cbmpTile);
			}
			else {
				_Tog[i].delay = TOG_RESET_DELAY;
				_Tog[i].incAlpha = FALSE;
				DrawBitmap(memdc, tog.x, tog.y, cbmpTile);
			}
		}
		else {
			if (tog.alpha > 0) {
				_Tog[i].alpha -= TOG_ALPHA_OFFSET;	//투명하게
				DrawBitmapAlpha(memdc, tog.x, tog.y, cbmpTile, tog.alpha);
			}
			else if (tog.delay) {
				_Tog[i].delay--;
			}
			else {
				_Tog[i].delay = TOG_RESET_DELAY;
				_Tog[i].incAlpha = TRUE;
			}
		}
	}
}



void CGameSys::DrawItems(CDC* memdc)
{
	char i;

	for (i = 0; i < itemCount; i++) {
		if (_Item[i].ch == 'P')
			DrawBitmap(memdc, _Item[i].x, _Item[i].y, cbmpBonus);
		else if (_Item[i].ch == '#') {
			DrawBitmap(memdc, _Item[i].x, _Item[i].y, cbmpMap, FALSE, 200, 0, 250, 50);
			_Item[i].ch = '.';
		}
	}
}

void CGameSys::DrawRaccoon(CDC* memdc)
{
	// SS: 순수하게 너구리 그리는 부분을 담당
	BOOL isRight = _Rac.isToRight;

	_Rac.bmpRect = CRect(_Rac.x - RAC_HW - 10, _Rac.y - RAC_HH - 10, _Rac.x + RAC_HW + 10, _Rac.y + RAC_HH + 10);

	//너구리 
	if (_Rac.isRolling) {
		if (isRight)
			DrawRacRDJump(memdc, _Rac.step);
		else
			DrawRacLDJump(memdc, _Rac.step);
	}
	else {
		switch (_Rac.state) {
		case standing:
			if (isRight)
				DrawRacRStand(memdc, _Rac.step);
			else
				DrawRacLStand(memdc, _Rac.step);
			break;
		case moving:
			if (isRight)
				DrawRacRMove(memdc, _Rac.step);
			else
				DrawRacLMove(memdc, _Rac.step);
			break;
		case jumpReady:
			if (isRight)
				DrawRacRJumpReady(memdc);
			else
				DrawRacLJumpReady(memdc);
			break;
		case jumping:
			if (_Rac.isDouble) {
				if (isRight)
					DrawRacRDJump(memdc, _Rac.step);
				else
					DrawRacLDJump(memdc, _Rac.step);
			}
			else {
				if (isRight)
					DrawRacRJump(memdc);
				else
					DrawRacLJump(memdc);
			}
			break;
		case falling:
			//if(_Rac.isInAirUpward)
			if (isRight)
				DrawRacRDrop(memdc);
			else
				DrawRacLDrop(memdc);
			break;
		case landing:
			if (isRight)
				DrawRacRStand(memdc, _Rac.step);
			else
				DrawRacLStand(memdc, _Rac.step);
			break;
		case goingUpDown:
			DrawRacUp(memdc, _Rac.step);
			break;
		case onAlli:
			DrawAlli(memdc, _Rac.animIdx);
			if (isRight)
				DrawRacRStand(memdc, _Rac.step);
			else
				DrawRacLStand(memdc, _Rac.step);
			break;
		case onMonkey:
			if (!IsMonkey) {
				DrawRacRDJump(memdc, monkeIndex);
			}
			break;
		case onHippo :
			if (isRight)
				DrawRacRDJump(memdc, hippoIndex);
			else
				DrawRacLDJump(memdc, hippoIndex);
			break;
			break;
		case dead:
			if (isRight)
				DrawRacRAttack(memdc);
			else
				DrawRacLAttack(memdc);
			break;
		}
	}


}


void CGameSys::GameNextMap(CDC* pDC)
{
	//SS: 게임 클리어를 하지 않고 다음 맵으로 보내는 함수.
	//초기화 
	_iEat = 0;				//먹은 과일수 
	_iAni = 0;				//애니메이션 효과를 위해 

	_EnemyCount = 0;		//적의 숫자 
	_ToggleElementCnt = 0;
	_iToggleTimerCnt = 0;
	_bToggleMap = FALSE;

	_Rac.state = standing;
	_Rac.isToRight = toRight;
	_Rac.x = RAC_HW;

	_iLevel++;

	/*_Rac.macmIdx = 0;
	_Rac.isInAirUpward = TRUE;
	_Rac.jumpIdx = 0;*/

	LoadMap(pDC);
	_GameState = gamePlay;
}

void CGameSys::CheckCollision_Enemy()
{
	static int x1, y1, x2, y2;
	static int xx1, yy1, xx2, yy2;

	x1 = _Rac.x - RAC_HW;
	y1 = _Rac.y - RAC_HH;
	x2 = _Rac.x + RAC_HW;
	y2 = _Rac.y + RAC_HH;

	char i;
	for (i = 0; i < _EnemyCount; i++) {
		xx1 = _Ene[i].x - UNIT_BLOCK_HX;
		yy1 = _Ene[i].y - (UNIT_BLOCK_X * 5) + 20;
		xx2 = _Ene[i].x + UNIT_BLOCK_HX;
		yy2 = _Ene[i].y;

		// 충돌 감지 		
		if ((x1 >= xx1 && x1 <= xx2 && y1 > yy1 && y1 < yy2) ||
			(x1 >= xx1 && x1 <= xx2 && y2 > yy1 && y2 <= yy2) ||
			(x2 >= xx1 && x2 <= xx2 && y1 > yy1 && y1 < yy2) ||
			(x2 >= xx1 && x2 <= xx2 && y2 > yy1 && y2 <= yy2))
			if (_Rac.isRolling) {
				_Ene[i].hit = TRUE;
			}
			else {
				//너구리 죽는 상태로 
				deadEnemy = i;
				_Rac.state = dead;
			}
	}
}



void CGameSys::ChangeRacX(int x)
{
	if ((_Rac.x > RAC_HW && x < 0)
		|| (_Rac.x < WHOLE_GSW - RAC_HW && x > 0)) {
		_Rac.x += x;
	}
}

void CGameSys::SetRacX(int x)
{
	if (_Rac.x > 0 && _Rac.x < WHOLE_GSW)
		_Rac.x = x;
}


BOOL CGameSys::checkYBelowGround(int y)
{
	if (y > WND_SIZE_H - 30) {
		_Rac.state = dead;
		_Rac.y = WND_SIZE_H - 30;
	}
	return 0;
}
