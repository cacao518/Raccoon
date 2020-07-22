#pragma once
class CDrawBmp
{
public:
	CDrawBmp();
	~CDrawBmp();
	CDrawBmp(CWnd* parent);

	// ��Ʈ�� �̹��� ��ü�� �׸���.
	// 5��° ���ڴ� 6��°���ڰ� ������ ������ �������� �׸� ���ΰ�?
	void DrawBitmap(CDC* pDC, int x, int y, CBitmap& cbmp, bool bTrans,
		COLORREF crTransColor = RGB(0, 0, 0));

	// ��Ʈ�� �̹����� �κи��� �׸���.
	// s_x : start_x
	// e_x : end_x
	// ���� ����ϴ� �Լ��� �ٸ��ϴ�.
	// �� �Լ��� �̹����� Ư�� ������ ���ؼ� �� �κ��� �׸��ϴ�.
	// ���� å�� �ִ� �Լ��� ���� �ٸ��ϴ�. 
	void DrawBitmap(CDC* pDC, int x, int y, CBitmap& cbmp, bool bTrans, int s_x, int s_y,
		int e_x, int e_y, COLORREF crTransColor = RGB(0, 0, 0));

	// ��Ʈ�� �̹��� ��ü�� ���� ȿ���� �༭ �׸���.
	void DrawBitmapAlpha(CDC* pDC, int x, int y, CBitmap& cbmp, int alp);

	// ��Ʈ�� �̹��� �κ��� ���� ȿ���� �༭ �׸���.
	void DrawBitmapAlpha(CDC* pDC, int x, int y, CBitmap& cbmp, int alp, int s_x, int s_y,
		int e_x, int e_y);

	// ���� ǥ�� �Լ� 
	// �ִ� 10�ڸ� ���ڱ����� ǥ�õ� 
	// cipher�� �ڸ���, �ڸ����� �����ϸ� ������ ��ŭ �տ� ������ ����, �� ���� ���� 
	void DrawDigit(CDC* pDC, int x, int y, int score, CBitmap& cbmp, int cipher = 0,
		COLORREF crTransColor = RGB(0, 0, 0));

	void TransBitBlt(CDC* pDC, int x, int y, int width, int height,
		CDC& memdc, int mx, int my, COLORREF clrMask);

	CWnd* m_parent;
};

