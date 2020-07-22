#pragma once
class CDrawBmp
{
public:
	CDrawBmp();
	~CDrawBmp();
	CDrawBmp(CWnd* parent);

	// 비트맵 이미지 전체를 그린다.
	// 5번째 인자는 6번째인자가 지정한 색깔을 투명으로 그릴 것인가?
	void DrawBitmap(CDC* pDC, int x, int y, CBitmap& cbmp, bool bTrans,
		COLORREF crTransColor = RGB(0, 0, 0));

	// 비트맵 이미지의 부분만을 그린다.
	// s_x : start_x
	// e_x : end_x
	// 보통 사용하는 함수와 다릅니다.
	// 이 함수는 이미지의 특정 범위를 정해서 그 부분을 그립니다.
	// 보통 책에 있는 함수와 조금 다릅니다. 
	void DrawBitmap(CDC* pDC, int x, int y, CBitmap& cbmp, bool bTrans, int s_x, int s_y,
		int e_x, int e_y, COLORREF crTransColor = RGB(0, 0, 0));

	// 비트맵 이미지 전체에 알파 효과를 줘서 그린다.
	void DrawBitmapAlpha(CDC* pDC, int x, int y, CBitmap& cbmp, int alp);

	// 비트맵 이미지 부분을 알파 효과를 줘서 그린다.
	void DrawBitmapAlpha(CDC* pDC, int x, int y, CBitmap& cbmp, int alp, int s_x, int s_y,
		int e_x, int e_y);

	// 숫자 표시 함수 
	// 최대 10자리 숫자까지만 표시됨 
	// cipher은 자리수, 자리수를 지정하면 부족한 만큼 앞에 공백이 생김, 즉 우측 정렬 
	void DrawDigit(CDC* pDC, int x, int y, int score, CBitmap& cbmp, int cipher = 0,
		COLORREF crTransColor = RGB(0, 0, 0));

	void TransBitBlt(CDC* pDC, int x, int y, int width, int height,
		CDC& memdc, int mx, int my, COLORREF clrMask);

	CWnd* m_parent;
};

