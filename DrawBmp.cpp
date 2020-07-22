#include <string.h>
#include "stdafx.h"
#include "DrawBmp.h"


CDrawBmp::CDrawBmp()
{
}


CDrawBmp::~CDrawBmp()
{
}

CDrawBmp::CDrawBmp(CWnd* parent)
	:m_parent(parent)
{
}

void CDrawBmp::DrawBitmap(CDC* pDC, int x, int y, CBitmap& cbmp, bool bTrans,
	COLORREF crTransColor)
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

void CDrawBmp::DrawBitmap(CDC* pDC, int x, int y, CBitmap& cbmp, bool bTrans, int s_x, int s_y,
	int e_x, int e_y, COLORREF crTransColor)
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


void CDrawBmp::DrawBitmapAlpha(CDC* pDC, int x, int y, CBitmap& cbmp, int alp)
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

void CDrawBmp::DrawBitmapAlpha(CDC* pDC, int x, int y, CBitmap& cbmp, int alp, int s_x, int s_y,
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

void CDrawBmp::DrawDigit(CDC* pDC, int x, int y, int score, CBitmap& cbmp, int cipher,
	COLORREF crTransColor)
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

// 가져다 씀 
void CDrawBmp::TransBitBlt(CDC* pDC, int x, int y, int width, int height,
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

