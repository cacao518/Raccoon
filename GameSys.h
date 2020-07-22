#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <mmsystem.h>
#include "Resource.h"
#include "DrawBmp.h"

// ���ӿ��� ����ϴ� Ű ���� 
#define LEFT      VK_LEFT
#define RIGHT      VK_RIGHT
#define UP         VK_UP
#define DOWN      VK_DOWN
#define   JUMP      VK_SPACE

// ó���� �־����� �ʱ��� �� 
#define   LIVE      4

// ������ ���� 
#define LAST_LEVEL   3


// unit block size & map's x,y block count
#define UNIT_BLOCK_X 50
#define UNIT_BLOCK_Y 50
#define UNIT_BLOCK_HX 25
#define UNIT_BLOCK_HY 25

#define MAP_BLOCK_X_COUNT 130
#define MAP_BLOCK_Y_COUNT 10
#define VISUAL_MAP_BLOCK_X_COUNT 20

#define TIME_LIMIT 500
#define TIME_BAR_OFFSET_L 60
#define TIME_BAR_OFFSET_T (UNIT_BLOCK_Y * MAP_BLOCK_Y_COUNT)
#define TIME_BAR_HEIGHT 25

#define CAMERA_OFFSET_X 300
// ���̴� Game Screen Width
#define VISUAL_GSW (UNIT_BLOCK_X * VISUAL_MAP_BLOCK_X_COUNT)
// ��ü Game Screen Width
#define WHOLE_GSW (UNIT_BLOCK_X * MAP_BLOCK_X_COUNT)

//#define WND_SIZE_W (VISUAL_GSW + 60)
#define WND_SIZE_W VISUAL_GSW
#define WND_SIZE_H TIME_BAR_OFFSET_T

// delay frame cycle
#define RAC_READY_DELAY_FRAME 1
#define RAC_LAND_DELAY_FRAME 1
#define RAC_HIT_DELAY_FRAME 5

// movement offset (on each frame)
#define RAC_HIT_STEP_BACK_OFFSET 15
#define RAC_MOVE_X_OFFSET 10
#define RAC_MOVE_Y_OFFSET 10 // on a rope/giraffe
#define GUARD_FAT_X_OFFSET 8
#define GUARD_SLIM_X_OFFSET 12
#define MOVING_BLOCK_X_OFFSET 10
#define ALLIGATOR_X_OFFSET 10

// Character pixel & bounding size
#define RAC_W (UNIT_BLOCK_X * 2)
#define RAC_H (UNIT_BLOCK_Y * 2)
#define RAC_HW UNIT_BLOCK_X
#define RAC_HH UNIT_BLOCK_Y
#define HELPFUL_ITEM_W UNIT_BLOCK_X
#define HELPFUL_ITEM_H UNIT_BLOCK_Y
#define GUARD_FAT_W (UNIT_BLOCK_X * 3)
#define GUARD_FAT_H (UNIT_BLOCK_Y * 2)
#define GUARD_SLIM_W UNIT_BLOCK_X
#define GUARD_SLIM_H (UNIT_BLOCK_Y * 5)

// etc
#define HIPPO_TRAMPOLINE_BLOCK_HEIGHT 7

#define RAC_MAX_JUMP_COUNT 2

#define MACM_LEN 7 // Mid-Air Curve Motion
#define MACM_FALL_MAX_OFFSET 20
#define MULTI_JUMP_MIN_MACM_IDX 4

enum Stage { tutorial, savana, amazon };
enum GameState { gameIntro, gamePlay, gameNextMap, gameClear, gameOver };
enum RacState { standing, moving, jumping, jumpReady, landing, falling, hitStepBack, goingUpDown, onAlli, onMonkey, onHippo, dead};
enum Direction { toLeft, toRight, toUp, toDown, toNone };
enum DirectionMode { horizontal, vertical, inBothDirection };


//�ʱ���
struct Raccoon {
	int  step;         // �����϶� ���ϸ��̼��� ���� 
	char speedx;      // �ӵ� 
	char speedy;      // �ӵ� 
	BOOL isDouble;	  // ��������
	BOOL isRolling;	  // ������ ������
	CRect bmpRect;	  // �ٽ� �׸� ����

	int x;            // x��ǥ
	int y;            // y��ǥ 
	BOOL isInAirUpward;   // FALSE when Falling
	char state;         //
	BOOL isToRight;      // FALSE when Left
	char animIdx;      // �����϶� ���ϸ��̼��� ����
	char macmIdx;      // mid-air curve motion Index
	char jumpIdx;      // single jump or more?
};

//�� (���� ���� ���� ó��, ���̴� ���� ���İ��� ������� �ʴ´�, �츸 ���)
struct Enemy {
	int x;            // x��ǥ
	int y;            // y��ǥ 
	int alpha;         // ���� ����ϴٰ� �����ϰ� ��Ÿ����. �̰͸� ����  
	char speed;         // �ӵ� 
	BOOL type;         // TRUE�� �Ϲ����� ��,  FALSE�� ��
	BOOL isRight;      // �������� ���� �ֳ�?
	int sxBlockIndex;   // Start Block Index;
	int exBlockIndex;   // End Block Index;
	char animIdx;		// �����϶� ���ϸ��̼�
	BOOL hit;			// �¾�����
	CRect bmpRect;	  // �ٽ� �׸� ����
};

//����, �׾Ƹ� 
struct Item {
	char i;
	char j;
	int x;
	int y;
	char ch;
};

#define TOG_RESET_DELAY 60
#define TOG_ALPHA_OFFSET 51
#define TOG_COUNT 16

//������� ��Ÿ���� �ϴ� �� ���
struct Tog {
	int x;            // x��ǥ
	int y;            // y��ǥ 
	int i;
	int j;
	int alpha;         // 
	BOOL incAlpha;		// increment alpha
	int delay;			// �󸶳� ���� ��������
};

class CGameSys
{
protected:
	CWnd* m_parent;

	HINSTANCE           m_hInstance;
	HWND                m_hWindow;
	TCHAR               m_szWindowClass[32];
	TCHAR               m_szTitle[32];
	WORD                m_wIcon, m_wSmallIcon;
	int                 m_iWidth, m_iHeight;
	int                 m_iFrameDelay;
	BOOL                m_bSleep;
	HCURSOR            m_hcur;

public:
	CGameSys();
	CGameSys(CWnd* parent);
	~CGameSys();

	void EngineInit(CWnd* parent, LPTSTR szWindowClass,
		LPTSTR szTitle, WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight);
	void GameEnd();
	void GameActivate();
	void GameDeactivate();
	void GamePaint(CDC* pDC);
	void GameCycle(CDC* pDC);
	void HandleKeys(CDC* pDC);

	// �ʱ�ȭ 
	void Init();

	// ���ҽ����� �������� �о��  
	void LoadMap(CDC* pDC);

	// �浹 ����
	// �����̳� �������� ���� ������ �Ͱ��� �浹 �˻� 
	void CheckCollision();

	// �浹 ���� 
	// ���̳� �찰�� �����̴� �Ͱ��� �浹 �˻� 
	// �� ���� ���� ����� ó�� �ϴ� ������ ȿ���� ���� 
	// �ο��ϸ� �����̳� ���������� �ʱ����� �������� ������ �ε�ĥ�� ���� 
	// ���� �ʱ����� �����϶� �˻��ϸ� ��  
	// �׷��� ���̳� ���� �ʱ����� �������� �ʾƵ� �ε�ģ��.
	// ���� �ʱ����� �����Ӱ� ������� �ֱ������� �浹 �˻縦 �ؾ��Ѵ�.
	// ����� �����̳� �������� �浹�˻縦 �ֱ������� ���� �����Ƿ� 
	// ȿ���� ��� 
	void CheckCollision_Enemy();

	// ���ӽ��� ȭ�� 
	void GameIntro(CDC* pDC);

	// ������
	void GamePlay(CDC* pDC);

	// �ش� ������ ������ �� ����   
	void GameClear(CDC* pDC);

	// ���� �� 
	void GameOver(CDC* pDC);


	void KeyLWhenR();
	void KeyRWhenR();
	void KeyLWhenL();
	void KeyRWhenL();
	BOOL OnMapElementCollide(char ci, char cj, char updateMode, char iOrJ);
	BOOL CollisionCheck(char direction);
	BOOL PartialCollisionCheckL(char i, char j);
	BOOL PartialCollisionCheckR(char i, char j);
	BOOL PartialCollisionCheckU(char i, char j);
	BOOL PartialCollisionCheckD(char i, char j);
	void jumpingDirection(char horizontalDirection, char verticalDirection, int y);
	void fallingDirection(char horizontalDirection);
	BOOL multiJump();
	// General Methods
	//static GameEngine*  GetEngine() { return m_pGameEngine; };
	//BOOL                Initialize(int iCmdShow);
	//LRESULT             HandleEvent(HWND hWindow, UINT msg, WPARAM wParam,
	//   LPARAM lParam);
	//void                ErrorQuit(LPTSTR szErrorMsg);

	//// Accessor Methods
	//HINSTANCE GetInstance() { return m_hInstance; };
	//HWND      GetWindow() { return m_hWindow; };
	//void      SetWindow(HWND hWindow) { m_hWindow = hWindow; };
	//LPTSTR    GetTitle() { return m_szTitle; };
	//WORD      GetIcon() { return m_wIcon; };
	//WORD      GetSmallIcon() { return m_wSmallIcon; };
	//int       GetWidth() { return m_iWidth; };
	//int       GetHeight() { return m_iHeight; };
	//int       GetFrameDelay() { return m_iFrameDelay; };
	//void      SetFrameRate(int iFrameRate) {
	//   m_iFrameDelay = 1000 /
	//      iFrameRate;
	//};
	//BOOL      GetSleep() { return m_bSleep; };
	void      SetSleep(BOOL bSleep);
	void DrawEnemies(CDC* memdc);
	void DrawItems(CDC* memdc);
	void DrawRaccoon(CDC* memdc);
	void DrawToggledMapElements(CDC* memdc);
	void GameNextMap(CDC* pDC);
	BOOL fallJump();
	void CheckFrame();
	void DynamicBoxCheck();
	void ChangeRacX(int x);
	void SetRacX(int x);
	BOOL checkYBelowGround(int y);
};