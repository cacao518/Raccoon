#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <mmsystem.h>
#include "Resource.h"
#include "DrawBmp.h"

// 게임에서 사용하는 키 지정 
#define LEFT      VK_LEFT
#define RIGHT      VK_RIGHT
#define UP         VK_UP
#define DOWN      VK_DOWN
#define   JUMP      VK_SPACE

// 처음에 주어지는 너구리 수 
#define   LIVE      4

// 마지막 레벨 
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
// 보이는 Game Screen Width
#define VISUAL_GSW (UNIT_BLOCK_X * VISUAL_MAP_BLOCK_X_COUNT)
// 전체 Game Screen Width
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


//너구리
struct Raccoon {
	int  step;         // 움직일때 에니메이션을 위해 
	char speedx;      // 속도 
	char speedy;      // 속도 
	BOOL isDouble;	  // 더블점프
	BOOL isRolling;	  // 아이템 구르기
	CRect bmpRect;	  // 다시 그릴 영역

	int x;            // x좌표
	int y;            // y좌표 
	BOOL isInAirUpward;   // FALSE when Falling
	char state;         //
	BOOL isToRight;      // FALSE when Left
	char animIdx;      // 움직일때 에니메이션을 위해
	char macmIdx;      // mid-air curve motion Index
	char jumpIdx;      // single jump or more?
};

//적 (적과 뱀을 같이 처리, 차이는 적은 알파값을 사용하지 않는다, 뱀만 사용)
struct Enemy {
	int x;            // x좌표
	int y;            // y좌표 
	int alpha;         // 뱀은 희미하다가 선명하게 나타난다. 이것를 위해  
	char speed;         // 속도 
	BOOL type;         // TRUE는 일반적인 적,  FALSE는 뱀
	BOOL isRight;      // 오른쪽을 보고 있나?
	int sxBlockIndex;   // Start Block Index;
	int exBlockIndex;   // End Block Index;
	char animIdx;		// 움직일때 에니메이션
	BOOL hit;			// 맞았을때
	CRect bmpRect;	  // 다시 그릴 영역
};

//과일, 항아리 
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

//사라졌다 나타났다 하는 맵 요소
struct Tog {
	int x;            // x좌표
	int y;            // y좌표 
	int i;
	int j;
	int alpha;         // 
	BOOL incAlpha;		// increment alpha
	int delay;			// 얼마나 오래 보여줄지
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

	// 초기화 
	void Init();

	// 리소스에서 맵정보를 읽어옴  
	void LoadMap(CDC* pDC);

	// 충돌 감지
	// 압정이나 낭떠러지 같은 고정된 것과의 충돌 검사 
	void CheckCollision();

	// 충돌 감지 
	// 적이나 뱀같이 움직이는 것과의 충돌 검사 
	// 이 같이 따로 나누어서 처리 하는 이유는 효율을 위해 
	// 부연하면 압정이나 낭떠러지는 너구리가 움직이지 않으면 부딪칠수 없음 
	// 따라서 너구리가 움직일때 검사하면 됨  
	// 그러나 적이나 뱀은 너구리가 움직이지 않아도 부딪친다.
	// 따라서 너구리의 움직임과 관계없이 주기적으로 충돌 검사를 해야한다.
	// 결론은 압정이나 낭떠러지 충돌검사를 주기적으로 하지 않으므로 
	// 효율울 향상 
	void CheckCollision_Enemy();

	// 게임시작 화면 
	void GameIntro(CDC* pDC);

	// 게임중
	void GamePlay(CDC* pDC);

	// 해당 레벨의 과일을 다 먹음   
	void GameClear(CDC* pDC);

	// 게임 끝 
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