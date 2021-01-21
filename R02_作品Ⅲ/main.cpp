#include "DxLib.h"
#include "resouce.h"

#define GAME_WIDTH  960
#define GAME_HEIGHT 560

#define GAME_COLOR  32 

#define GAME_WINDOW_BAR   0

#define GAME_WINDOW_NAME  "Runner"

#define GAME_FPS    60

#define MOUSE_BUTTON_CODE    129

#define FONT_PATH_MAX		  255
#define PATH_MAX			 255

#define FONT_TANU_PATH         TEXT(".\\FONT\\TanukiMagic.ttf")
#define FONT_TANU_NAME         TEXT("たぬき油性マジック")

#define FONT_INSTALL_ERR_TITLE TEXT("フォントインストールエラー")
#define FONT_CREATE_ERR_TITLE  TEXT("フォント作成エラー")

#define IMAGE_LOAD_ERR_TITLE   TEXT("画像読み込みエラー")

#define IMAGE_BACK_PATH        TEXT(".\\IMAGE\\background.jpg")
#define IMAGE_PLAYER_PATH      TEXT(".\\IMAGE\\Dornuts.png")

#define IMAGE_ENEMY_PATH       TEXT(".\\IMAGE\\enemy.png")

//タイトルやプレイ画面の背景
#define IMAGE_TITLE_BK_PATH       TEXT(".\\IMAGE\\background.jpg")

//タイトルロゴ
#define IMAGE_TITLE_ROGO_PATH     TEXT(".\\IMAGE\\TITLE.png")
#define IMAGE_TITLE_ROGO_ROTA     0.005  
#define IMAGE_TITLE_ROGO_ROTA_MAX 1.0
#define IMAGE_TITLE_ROGO_X_SPEED  1

//点滅
#define IMAGE_TITLE_PATH		TEXT(".\\IMAGE\\title_start.png")
#define IMAGE_TITLE_CNT		1
#define IMAGE_TITLE_CNT_MAX	30

//スタート画面背景
#define IMAGE_START_PATH		TEXT(".\\IMAGE\\sousa.png")

//点滅
#define IMAGE_START_ROGO_PATH		TEXT(".\\IMAGE\\sousa_start.png")
#define IMAGE_TITLE_CNT		1
#define IMAGE_TITLE_CNT_MAX	30

//クリア画像
#define IMAGE_END_COMP_PATH       TEXT(".\\IMAGE\\COMP.jpg")
#define IMAGE_END_COMP_CNT        1
#define IMAGE_END_COMP_CNT_MAX    30

//失敗画像
#define IMAGE_END_FAIL_PATH       TEXT(".\\IMAGE\\gameover2.png")
#define IMAGE_END_FAIL_CNT        1
#define IMAGE_END_FAIL_CNT_MAX    30

#define IMAGE_BACK_REV_PATH    TEXT(".\\IMAGE\\background_2.jpg")
#define IMAGE_BACK_NUM         4

#define MUSIC_LOAD_ERR_TITLE   TEXT("音楽読み込みエラー")

#define MUSIC_PLAY_BGM_PATH         TEXT(".\\MUSIC\\トロピカル.mp3")

#define MUSIC_START_BGM_PATH        TEXT(".\\MUSIC\\pops_up_the_mind_wings.mp3")

#define MUSIC_BGM_TITLE_PATH        TEXT(".\\MUSIC\\Rising_sun.mp3")
#define MUSIC_BGM_COMP_PATH         TEXT(".\\MUSIC\\トロピカル.mp3")
#define MUSIC_BGM_FAIL_PATH         TEXT(".\\MUSIC\\トロピカル.mp3")

#define ENEMY_MAX              10

#define GAME_MAP_TATE_MAX      9
#define GAME_MAP_YOKO_MAX      45
#define GAME_MAP_KIND_MAX      3

#define GAME_MAP_PATH          TEXT(".\\MAP\\map.png")

#define MAP_DIV_WIDTH          64
#define MAP_DIV_HEIGHT         64
#define MAP_DIV_TATE           10
#define MAP_DIV_YOKO           4
#define MAP_DIV_NUM            MAP_DIV_TATE * MAP_DIV_YOKO

#define START_ERR_TITLE        TEXT("スタート位置エラー")
#define START_ERR_CAPTION      TEXT("スタート位置が決まっていません")

#define GOAL_ERR_TITLE        TEXT("ゴール位置エラー")
#define GOAL_ERR_CAPTION      TEXT("ゴール位置が決まっていません")

#define MOUSE_R_CLICK_TITLE    TEXT("ゲーム中断")
#define MOUSE_R_CLICK_CAPTION  TEXT("ゲームを中断し、タイトル画面に戻りますか？")

#define JUMP_FRAME	60
#define JUMP_POWER	8

enum GAME_MAP_KIND
{
	n = -1,
	t = 0,
	w = 1,
	k = 2,
	g = 3,
	s = 10,
	e = 5
};

enum GAME_SCENE {
	GAME_SCENE_TITLE,
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_PLAY_2,
	GAME_SCENE_PLAY_3,
	GAME_SCENE_END,
};

enum GAME_STAGE {
	GAME_STAGE1,
	GAME_STAGE2,
	GAME_STAGE3
};

enum GAME_END {
	GAME_END_COMP,
	GAME_END_FAIL
};

enum GAME_SPEED {
	CHARA_SPEED_LOW = 1,
	CHARA_SPEED_MIDI = 2,
	CHARA_SPEED_HIGH = 3
};

typedef struct STRUCT_1_POINT
{
	int x = -1;
	int y = -1;
}iPOINT;

typedef struct STRUCT_MOUSE
{
	int InputValue = 0;
	int WheelValue = 0;
	iPOINT Point;
	iPOINT OldPoint;
	int OldButton[MOUSE_BUTTON_CODE] = { 0 };
	int Button[MOUSE_BUTTON_CODE] = { 0 };
}MOUSE;

typedef struct STRUCT_FONT
{
	char path[FONT_PATH_MAX];
	char name[FONT_PATH_MAX];
	int handle;
	int size;
	int bold;
	int type;
}FONT;

typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];
	int handle;
	int x;
	int y;
	int width;
	int height;
}IMAGE;

typedef struct STRUCT_MUSIC
{
	char path[PATH_MAX];
	int handle;
	int handle2;
}MUSIC;


typedef struct STRUCT_CHARA
{
	IMAGE image;
	int speed;
	int CenterX;
	int CenterY;

	BOOL IsJump;
	int JumpPowerMax;

	int JumpTimeCnt;
	int BeforeJumpY;

	RECT coll;
	iPOINT collBeforePt;

	//移動した距離
	int mapDis;

}CHARA;

typedef struct STRUCT_ENEMY
{
	IMAGE image;
	int speed;
	int CenterX;
	int CenterY;
	int Moveadd;

	bool view = FALSE;

	RECT coll;
	iPOINT collBeforePt;

}ENEMY;

typedef struct STRUCT_IMAGE_BACK
{
	IMAGE image;
	BOOL IsDraw;

}IMAGE_BACK;

typedef struct STRUCT_IMAGE_ROTA
{
	IMAGE image;
	double angle;
	double angleMAX;
	double rate;
	double rateMAX;
}IMAGE_ROTA;

typedef struct STRUCT_IMAGE_BLINK
{
	IMAGE image;
	int Cnt;
	int CntMAX;
	BOOL IsDraw;
}IMAGE_BLINK;

typedef struct STRUCT_MAP_IMAGE
{
	char path[PATH_MAX];
	int handle[MAP_DIV_NUM];
	int kind[MAP_DIV_NUM];
	int width;
	int height;
}MAPCHIP;

typedef struct STRUCT_MAP
{
	GAME_MAP_KIND kind;
	int x;
	int y;
	int width;
	int height;
}MAP;

int StartTimeFps;
int CountFps;
float CalcFps;
int SampleNumFps = GAME_FPS;

int AllKeyState[256] = { '\0' };
int OldAllKeyState[256] = { '\0' };

MOUSE mouse;

FONT FontTanu32;
FONT MS;

int GameScene;

int GameEndkind;
RECT GoalRect = { -1,-1,-1,-1 };

float yadd = 0.0f;

//IMAGE ImageBack;

IMAGE_BACK ImageBack[IMAGE_BACK_NUM];

IMAGE ImageTitleBk;
IMAGE ImageStart;
IMAGE_ROTA ImageTitleROGO;
IMAGE_BLINK ImageTitleSTART;
IMAGE_BLINK ImageStartSTART;

IMAGE_BLINK ImageEndCOMP;
IMAGE_BLINK ImageEndFAIL;

CHARA player;

ENEMY enemy[ENEMY_MAX];
iPOINT enemyPt[ENEMY_MAX] = { {-1,-1} };
int enemyCnt = 0;

MUSIC START_BGM;
MUSIC PLAY_BGM;
MUSIC BGM_TITLE;
MUSIC BGM_COMP;
MUSIC BGM_FAIL;

//GAME_SCENE GameScene;
GAME_STAGE GameStage;


GAME_MAP_KIND mapData[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,k,k,k,k,
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,k,k,k,k,
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,k,k,k,k,
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,k,k,k,k,t,k,k,k,k,k,k,
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,k,t,t,k,k,k,k,k,t,t,t,t,t,t,t,k,t,t,t,t,k,t,k,k,k,k,k,k,
	k,t,t,t,t,t,t,t,t,t,t,t,t,t,t,k,t,t,t,t,t,t,t,t,k,k,t,t,t,t,t,k,t,t,t,t,t,k,t,k,k,k,k,k,k,
	k,t,t,t,t,t,t,t,t,t,t,t,t,k,t,t,t,t,k,k,t,t,t,t,k,t,k,t,t,t,t,t,t,t,k,t,k,k,t,k,k,k,k,k,k,
	k,t,t,t,t,t,t,s,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,k,t,t,k,t,t,t,t,k,k,t,t,t,k,g,k,k,k,k,k,k,
	k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,t,t,k,k,k,k,k,t,k,k,k,k,k,k,t,k,k,k,k,t,t,t,k,k,k,k,k,k,k,k
};

//GAME_MAP_KIND mapData_2[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
//	w,w,w,w,w,w,w,w,w,w,w,s,w,w,w,
//	w,t,t,t,t,t,t,t,t,t,t,t,t,t,w,
//	w,t,t,t,w,w,w,t,t,e,w,w,w,t,w,
//	t,t,e,t,t,t,w,t,t,w,t,t,w,t,w,
//	w,w,w,t,t,t,w,t,w,w,w,t,t,t,w,
//	w,t,t,t,e,t,w,t,w,t,w,t,t,t,w,
//	w,t,t,e,w,w,t,t,t,t,t,t,e,t,w,
//	w,g,w,w,t,t,t,w,t,t,t,t,t,w,w,
//	w,w,t,t,t,t,t,t,w,w,w,w,w,w,w
//};

//GAME_MAP_KIND mapData_3[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
//	k,k,k,k,k,k,k,k,k,k,k,g,k,k,k,
//	k,t,t,t,t,t,t,t,t,t,t,t,t,t,k,
//	k,t,t,t,k,k,k,t,t,e,k,k,k,t,k,
//	k,t,e,t,t,t,k,t,t,k,t,t,k,t,k,
//	k,k,k,t,t,t,k,t,k,t,k,t,t,t,k,
//	k,t,t,t,e,t,k,t,k,t,k,t,t,t,k,
//	k,t,t,e,k,k,t,t,t,t,t,t,e,t,k,
//	k,s,k,k,t,t,t,k,t,t,t,t,t,k,k,
//	k,k,t,t,t,t,t,t,k,k,k,k,k,k,k
//};

GAME_MAP_KIND mapDataInit[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

MAPCHIP mapChip;

MAP map[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

iPOINT startPt{ -1,-1 };

RECT mapColl[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

VOID MY_FPS_UPDATE(VOID);
VOID MY_FPS_DRAW(VOID);
VOID MY_FPS_WAIT(VOID);

VOID MY_ALL_KEYDOWN_UPDATE(VOID);
BOOL MY_KEY_DOWN(int);
BOOL MY_KEY_UP(int);
BOOL MY_KEYDOWN_KEEP(int, int);

VOID MY_MOUSE_UPDATE(VOID);
BOOL MY_MOUSE_DOWN(int);
BOOL MY_MOUSE_UP(int);
BOOL MY_MOUSE_KEEP(int, int);

BOOL MY_FONT_INSTALL_ONCE(VOID);
VOID MY_FONT_UNINSTALL_ONCE(VOID);
BOOL MY_FONT_CREATE(VOID);
VOID MY_FONT_DELETE(VOID);

VOID MY_TITLE(VOID);
VOID MY_TITLE_PROC(VOID);
VOID MY_TITLE_DRAW(VOID);

VOID MY_START(VOID);
VOID MY_START_PROC(VOID);
VOID MY_START_DRAW(VOID);

VOID MY_PLAY(VOID);
VOID MY_PLAY_PROC(VOID);
VOID MY_PLAY_DRAW(VOID);

//VOID MY_PLAY_2(VOID);
//VOID MY_PLAY_PROC_2(VOID);
//VOID MY_PLAY_DRAW_2(VOID);
//
//VOID MY_PLAY_3(VOID);
//VOID MY_PLAY_PROC_3(VOID);
//VOID MY_PLAY_DRAW_3(VOID);

VOID MY_END(VOID);
VOID MY_END_PROC(VOID);
VOID MY_END_DRAW(VOID);

BOOL MY_LOAD_IMAGE(VOID);
VOID MY_DELETE_IMAGE(VOID);

BOOL MY_LOAD_MUSIC(VOID);
VOID MY_DELETE_MUSIC(VOID);

BOOL MY_CHECK_MAP1_PLAYER_COLL(RECT);
BOOL MY_CHECK_RECT_COLL(RECT, RECT);

//ゴール判定
BOOL MY_CHECK_MAP1_GOAL(RECT player);

VOID MY_PLEY_MOVE_JUMP(VOID);	//ジャンプ処理

int spHandle;
int spX;
int spY;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpCmdLine, int nCmdShow)

{
	ChangeWindowMode(TRUE);

	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);

	SetWindowStyleMode(GAME_WINDOW_BAR);

	SetMainWindowText(TEXT(GAME_WINDOW_NAME));

	SetAlwaysRunFlag(TRUE);

	SetWindowIconID(IDI_ICON1);

	if (DxLib_Init() == -1) { return -1; }

	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	if (MY_FONT_INSTALL_ONCE() == FALSE) { return -1; }

	if (MY_FONT_CREATE() == FALSE) { return -1; }

	SetMouseDispFlag(TRUE);

	//タイトルからスタート
	GameScene = GAME_SCENE_TITLE;

	//ステージは1から
	//GameStage = GAME_STAGE1;

	ChangeFontType(DX_FONTTYPE_ANTIALIASING_8X8);

	SetDrawScreen(DX_SCREEN_BACK);


	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			if (mapData[tate][yoko] == s)
			{
				startPt.x = mapChip.width * yoko + mapChip.width / 2;
				startPt.y = mapChip.height * tate + mapChip.height / 2;
			}

			if (mapData[tate][yoko] == g)
			{
				GoalRect.left = mapChip.width * yoko;
				GoalRect.top = mapChip.height * tate;
				GoalRect.right = mapChip.width * (yoko + 1);
				GoalRect.bottom = mapChip.height * (tate + 1);
			}

			if (mapData[tate][yoko] == e && enemyCnt < ENEMY_MAX)
			{
				enemyPt[enemyCnt].x = mapChip.width * yoko + mapChip.width / 2;
				enemyPt[enemyCnt].y = mapChip.height * tate + mapChip.height / 2;
				enemy[enemyCnt].view = TRUE;
				enemyCnt++;
				map[tate][yoko].kind = t;
			}
		}
	}

	if (startPt.x == -1 && startPt.y == -1)
	{
		MessageBox(GetMainWindowHandle(), START_ERR_CAPTION, START_ERR_TITLE, MB_OK);
		return -1;
	}

	if (GoalRect.left == -1)
	{
		MessageBox(GetMainWindowHandle(), GOAL_ERR_CAPTION, GOAL_ERR_TITLE, MB_OK);
		return -1;
	}
	while (TRUE)
	{

		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		MY_ALL_KEYDOWN_UPDATE();

		MY_MOUSE_UPDATE();

		MY_FPS_UPDATE();


		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			MY_TITLE();
			break;

		case GAME_SCENE_START:
			MY_START();
			break;

		case GAME_SCENE_PLAY:
			MY_PLAY();
			break;

			//case GAME_SCENE_PLAY_2:
			//	MY_PLAY_2();
			//	break;

			//case GAME_SCENE_PLAY_3:
			//	MY_PLAY_3();
			//	break;

		case GAME_SCENE_END:
			MY_END();
			break;
		}

		MY_FPS_DRAW();

		ScreenFlip();

		MY_FPS_WAIT();
	}

	//SetClassLong(GetMainWindowHandle(), GCL_HCURSOR, OldWindowClass);

	MY_FONT_DELETE();

	MY_FONT_UNINSTALL_ONCE();

	MY_DELETE_IMAGE();

	MY_DELETE_MUSIC();

	DxLib_End();

	return 0;
}

VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0)
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps)
	{
		int now = GetNowCount();

		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

VOID MY_FPS_DRAW(VOID)
{
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;

	if (waitTime > 0)
	{
		WaitTimer(waitTime);
	}
	return;
}

VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];

	for (int i = 0; i < 256; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey);

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)
		{
			AllKeyState[i]++;
		}
		else
		{
			AllKeyState[i] = 0;
		}
	}
	return;
}
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1
		&& AllKeyState[KEY_INPUT_] == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID MY_MOUSE_UPDATE(VOID)
{
	mouse.OldPoint = mouse.Point;

	for (int i = 0; i < MOUSE_BUTTON_CODE; i++)
	{
		mouse.OldButton[i] = mouse.Button[i];
	}

	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	mouse.InputValue = GetMouseInput();

	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }

	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }

	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }

	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

BOOL MY_MOUSE_DOWN(int MOUSE_INPUT_)
{
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_MOUSE_UP(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1
		&& mouse.Button[MOUSE_INPUT_] == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_MOUSEDOWN_KEEP(int MOUSE_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_FONT_INSTALL_ONCE(VOID)
{
	if (AddFontResourceEx(FONT_TANU_PATH, FR_PRIVATE, NULL) == 0)
	{
		MessageBox(GetMainWindowHandle(), FONT_TANU_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}
	return TRUE;
}

VOID MY_FONT_UNINSTALL_ONCE(VOID)
{
	RemoveFontResourceEx(FONT_TANU_PATH, FR_PRIVATE, NULL);

	return;
}

BOOL MY_FONT_CREATE(VOID)
{
	strcpy_s(MS.path, sizeof(MS.path), FONT_TANU_PATH);
	strcpy_s(MS.name, sizeof(MS.name), FONT_TANU_NAME);
	MS.handle = -1;
	MS.size = 40;
	MS.bold = 1;
	MS.type = DX_FONTTYPE_EDGE;

	MS.handle = CreateFontToHandle(MS.name, MS.size, MS.bold, MS.type);
	if (MS.handle == -1) { MessageBox(GetMainWindowHandle(), FONT_TANU_NAME, FONT_CREATE_ERR_TITLE, MB_OK); return FALSE; }

	return TRUE;
}

VOID MY_FONT_DELETE(VOID)
{
	DeleteFontToHandle(MS.handle);

	return;
}

//タイトル画面
VOID MY_TITLE(VOID)
{
	MY_TITLE_PROC();
	MY_TITLE_DRAW();
}

VOID MY_TITLE_PROC(VOID)
{
	if (CheckSoundMem(BGM_TITLE.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 20 / 100, BGM_TITLE.handle);

		//DX_PLAYTYPE_NORMAL;
		//DX_PLAYTYPE_BACK;
		//DX_PLAYTYPE_LOOP;
		PlaySoundMem(BGM_TITLE.handle, DX_PLAYTYPE_LOOP);
	}

	//エンターキーで操作画面
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		//音が鳴ってなかったらBGMを流す
		if (CheckSoundMem(BGM_TITLE.handle) != 0)
		{
			StopSoundMem(BGM_TITLE.handle);
		}
		GameScene = GAME_SCENE_START;
	}

	//Y軸に背景画像をスライドさせていく
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		ImageBack[num].image.x++;

		if (ImageBack[num].IsDraw == FALSE)
		{
			if (ImageBack[num].image.x + ImageBack[num].image.width > 0)
			{
				ImageBack[num].IsDraw = TRUE;
			}
		}

		if (ImageBack[num].image.x > GAME_WIDTH)
		{
			ImageBack[num].image.x = 0 - ImageBack[0].image.width * 3;
			ImageBack[num].IsDraw = FALSE;
		}
	}

	//タイトルロゴを拡大
	if (ImageTitleROGO.rate < ImageTitleROGO.rateMAX)
	{
		ImageTitleROGO.rate += IMAGE_TITLE_ROGO_ROTA;
	}

	//タイトルロゴを移動
	if (ImageTitleROGO.image.x < GAME_WIDTH / 2)
	{
		ImageTitleROGO.image.x += IMAGE_TITLE_ROGO_X_SPEED;
	}
	else
	{
		//スタートを点滅
		if (ImageTitleSTART.Cnt < ImageTitleSTART.CntMAX)
		{
			ImageTitleSTART.Cnt += IMAGE_TITLE_CNT;
		}
		else
		{
			//描画する/しないを決める
			if (ImageTitleSTART.IsDraw == FALSE)
			{
				ImageTitleSTART.IsDraw = TRUE;
			}
			else if (ImageTitleSTART.IsDraw == TRUE)
			{
				ImageTitleSTART.IsDraw = FALSE;
			}
			ImageTitleSTART.Cnt = 0;
		}
	}

	return;
}

VOID MY_TITLE_DRAW(VOID)
{
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		if (ImageBack[num].IsDraw == TRUE)
		{
			DrawGraph(ImageBack[num].image.x, ImageBack[num].image.y, ImageBack[num].image.handle, TRUE);
		}
	}

	//タイトルロゴを回転しながら描画
	DrawRotaGraph(
		ImageTitleROGO.image.x, ImageTitleROGO.image.y,	//画像の座標
		ImageTitleROGO.rate,							//画像の拡大率
		ImageTitleROGO.angle,							//画像の回転率
		ImageTitleROGO.image.handle, TRUE);

	//点滅
	if (ImageTitleSTART.IsDraw == TRUE)
	{
		//タイトルスタートの描画
		DrawGraph(ImageTitleSTART.image.x, ImageTitleSTART.image.y, ImageTitleSTART.image.handle, TRUE);
	}

	DrawString(0, 0, "スタート画面(エンターキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//スタート画面
VOID MY_START(VOID)
{
	MY_START_PROC();
	MY_START_DRAW();
}

VOID MY_START_PROC(VOID)
{
	if (CheckSoundMem(START_BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 20 / 100, START_BGM.handle);

		//DX_PLAYTYPE_NORMAL;
		//DX_PLAYTYPE_BACK;
		//DX_PLAYTYPE_LOOP;
		PlaySoundMem(START_BGM.handle, DX_PLAYTYPE_LOOP);
	}

	if (MY_KEY_DOWN(KEY_INPUT_S) == TRUE)
	{

		if (CheckSoundMem(START_BGM.handle) != 0)
		{
			StopSoundMem(START_BGM.handle);
		}

		SetMouseDispFlag(FALSE);

		//初期化
		player.CenterX = startPt.x;
		player.CenterY = startPt.y;

		player.image.x = player.CenterX;
		player.image.y = player.CenterY;

		player.collBeforePt.x = player.CenterX;
		player.collBeforePt.y = player.CenterY;

		//マップの移動した距離初期化
		player.mapDis = 0;

		SetMousePoint(player.image.x, player.image.y);

		//敵の場所初期化
		for (int i = 0; i < enemyCnt; i++)
		{
			enemy[i].Moveadd = 1;

			enemy[i].view = TRUE;

			enemy[i].CenterX = enemyPt[i].x;
			enemy[i].CenterY = enemyPt[i].y;

			enemy[i].image.x = enemy[i].CenterX;
			enemy[i].image.y = enemy[i].CenterY;
		}

		GameEndkind = GAME_END_FAIL;

		GameScene = GAME_SCENE_PLAY;

	}

	//スタートを点滅
	if (ImageStartSTART.Cnt < ImageStartSTART.CntMAX)
	{
		ImageStartSTART.Cnt += IMAGE_TITLE_CNT;
	}
	else
	{
		//描画する/しないを決める
		if (ImageStartSTART.IsDraw == FALSE)
		{
			ImageStartSTART.IsDraw = TRUE;
		}
		else if (ImageStartSTART.IsDraw == TRUE)
		{
			ImageStartSTART.IsDraw = FALSE;
		}
		ImageStartSTART.Cnt = 0;
	}


	//if (ImageTitleROGO.rate < ImageTitleROGO.rateMAX)
	//{
	//	ImageTitleROGO.rate += IMAGE_TITLE_ROGO_ROTA;
	//}

	//if (ImageTitleROGO.image.x < GAME_WIDTH / 2)
	//{
	//	ImageTitleROGO.image.x += IMAGE_TITLE_ROGO_X_SPEED;
	//}

	return;
}

VOID MY_START_DRAW(VOID)
{
	DrawGraph(ImageStart.x, ImageStart.y, ImageStart.handle, TRUE);

	//DrawRotaGraph(
	//	ImageTitleROGO.image.x, ImageTitleROGO.image.y,
	//	ImageTitleROGO.rate,
	//	ImageTitleROGO.angle,
	//	ImageTitleROGO.image.handle, TRUE);

	if (ImageStartSTART.IsDraw == TRUE)
	{
		DrawGraph(ImageStartSTART.image.x, ImageStartSTART.image.y, ImageStartSTART.image.handle, TRUE);
	}

	DrawString(0, 0, "スタート画面(Sキーを押してください)", GetColor(255, 255, 255));

	return;
}

//プレイ画面
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();
	MY_PLAY_DRAW();
}

VOID MY_PLAY_PROC(VOID)
{
	if (CheckSoundMem(PLAY_BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 30 / 100, PLAY_BGM.handle);

		//DX_PLAYTYPE_NORMAL;
		//DX_PLAYTYPE_BACK;
		//DX_PLAYTYPE_LOOP;
		PlaySoundMem(PLAY_BGM.handle, DX_PLAYTYPE_LOOP);
	}

	if (mouse.Button[MOUSE_INPUT_RIGHT] == TRUE)
	{
		iPOINT R_ClickPt = mouse.Point;

		SetMouseDispFlag(TRUE);

		int Ret = MessageBox(GetMainWindowHandle(), MOUSE_R_CLICK_CAPTION, MOUSE_R_CLICK_TITLE, MB_YESNO);

		if (Ret == IDYES)
		{
			GameScene = GAME_SCENE_TITLE;

			if (CheckSoundMem(PLAY_BGM.handle) != 0)
			{
				StopSoundMem(PLAY_BGM.handle);
			}
			return;
		}
		else if (Ret == IDNO)
		{
			SetMousePoint(R_ClickPt.x, R_ClickPt.y);

			SetMouseDispFlag(FALSE);
		}
	}

	//player移動処理
	if (MY_KEY_DOWN(KEY_INPUT_UP) || MY_KEY_DOWN(KEY_INPUT_W))
	{
		player.CenterY -= CHARA_SPEED_MIDI;
		player.coll.left = player.CenterX - mapChip.width / 2 + 5;
		player.coll.top = player.CenterY - mapChip.height / 2 + 5;
		player.coll.right = player.CenterX + mapChip.width / 2 - 5;
		player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;
		if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
		{
			player.CenterY += CHARA_SPEED_MIDI;
		}
	}
	if (MY_KEY_DOWN(KEY_INPUT_DOWN) || MY_KEY_DOWN(KEY_INPUT_S))
	{
		player.CenterY += CHARA_SPEED_MIDI;
		player.coll.left = player.CenterX - mapChip.width / 2 + 5;
		player.coll.top = player.CenterY - mapChip.height / 2 + 5;
		player.coll.right = player.CenterX + mapChip.width / 2 - 5;
		player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;
		if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
		{
			player.CenterY -= CHARA_SPEED_MIDI;
		}
	}
	if (MY_KEY_DOWN(KEY_INPUT_RIGHT) || MY_KEY_DOWN(KEY_INPUT_D))
	{
		//player.CenterX += CHARA_SPEED_MIDI;

		//マップが動いた数＝プレイヤーが動いた数
		player.mapDis += player.speed;

		//マップの当たり判定プレイヤーが動いた数だけ動く
		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				//プレイヤーが右に動いた分マップを左に動かす
				mapColl[tate][yoko].left -= player.speed;
				mapColl[tate][yoko].right -= player.speed;
			}
		}
		player.coll.left = player.CenterX - mapChip.width / 2 + 5;
		player.coll.top = player.CenterY - mapChip.height / 2 + 5;
		player.coll.right = player.CenterX + mapChip.width / 2 - 5;
		player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;

		//マップ壁の当たり判定
		if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
		{
			//player.CenterX -= CHARA_SPEED_MIDI;
			//プレイヤーが戻ったとき
			player.mapDis -= player.speed;

			//マップの判定を右に動かす
			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					mapColl[tate][yoko].left += player.speed;
					mapColl[tate][yoko].right += player.speed;
				}
			}
		}
		//マップゴールの当たり判定処理
		if (MY_CHECK_MAP1_GOAL(player.coll) == TRUE)
		{
			if (CheckSoundMem(PLAY_BGM.handle) != 0)
			{
				StopSoundMem(PLAY_BGM.handle);
			}

			GameEndkind = GAME_END_COMP;

			GameScene = GAME_SCENE_END;
		}
	}
	if (MY_KEY_DOWN(KEY_INPUT_LEFT) || MY_KEY_DOWN(KEY_INPUT_A))
	{
		//player.CenterX -= CHARA_SPEED_MIDI;
		//マップが動いた数＝プレイヤーが動いた数
		player.mapDis -= player.speed;

		//マップの当たり判定プレイヤーが動いた数だけ動く
		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				//プレイヤーが左に動いた分マップを右に動かす
				mapColl[tate][yoko].left += player.speed;
				mapColl[tate][yoko].right += player.speed;
			}
		}
		player.coll.left = player.CenterX - mapChip.width / 2 + 5;
		player.coll.top = player.CenterY - mapChip.height / 2 + 5;
		player.coll.right = player.CenterX + mapChip.width / 2 - 5;
		player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;
		if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
		{
			//player.CenterX += CHARA_SPEED_MIDI;
			player.mapDis += player.speed;
			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					//マップ判定を左に動かす
					mapColl[tate][yoko].left -= player.speed;
					mapColl[tate][yoko].right -= player.speed;
				}
			}
		}
		//マップゴールの当たり判定処理
		if (MY_CHECK_MAP1_GOAL(player.coll) == TRUE)
		{
			if (CheckSoundMem(PLAY_BGM.handle) != 0)
			{
				StopSoundMem(PLAY_BGM.handle);
			}

			GameEndkind = GAME_END_COMP;

			GameScene = GAME_SCENE_END;
		}
	}

	player.coll.left = player.CenterX - mapChip.width / 2 + 5;
	player.coll.top = player.CenterY - mapChip.height / 2 + 5;
	player.coll.right = player.CenterX + mapChip.width / 2 - 5;
	player.coll.bottom = player.CenterY + mapChip.height / 2 - 5;

	BOOL IsMove = TRUE;

	if (MY_CHECK_MAP1_PLAYER_COLL(player.coll) == TRUE)
	{
		SetMousePoint(player.collBeforePt.x, player.collBeforePt.y);
		IsMove = FALSE;
	}

	if (IsMove == TRUE)
	{
		if (mouse.Point.x >= 0 && mouse.Point.x <= GAME_WIDTH
			&& mouse.Point.y >= 0 && mouse.Point.y <= GAME_HEIGHT)
		{
			player.image.x = player.CenterX - player.image.width / 2;
			player.image.y = player.CenterY - player.image.height / 2;

			player.collBeforePt.x = player.CenterX;
			player.collBeforePt.y = player.CenterY;
		}
	}

	//当たり判定
	RECT PlayerRect;
	int CollRange = 5;
	PlayerRect.left = player.image.x + player.image.width / 2 - CollRange;
	PlayerRect.top = player.image.y + player.image.height / 2 - CollRange;
	PlayerRect.right = player.image.x + player.image.width / 2 + CollRange;
	PlayerRect.bottom = player.image.y + player.image.height / 2 + CollRange;

	//ゴール
	if (MY_CHECK_RECT_COLL(PlayerRect, GoalRect) == TRUE)
	{
		if (CheckSoundMem(PLAY_BGM.handle) != 0)
		{
			StopSoundMem(PLAY_BGM.handle);
		}

		SetMouseDispFlag(TRUE);

		GameEndkind = GAME_END_COMP;

		GameScene = GAME_SCENE_END;

		return;
	}

	for (int i = 0; i < enemyCnt; i++)
	{
		if (enemy[i].view == TRUE)
		{
			//敵が動いて壁に当たったとき
			enemy[i].CenterX += enemy[i].Moveadd;
			if (MY_CHECK_MAP1_PLAYER_COLL(enemy[i].coll) == TRUE)
			{
				enemy[i].CenterX -= enemy[i].Moveadd * 2;
				enemy[i].Moveadd *= -1;
			}
			//敵の当たり判定
			enemy[i].coll.left = enemy[i].CenterX - mapChip.width / 2 + 5;
			enemy[i].coll.top = enemy[i].CenterY - mapChip.height / 2 + 5;
			enemy[i].coll.right = enemy[i].CenterX + mapChip.width / 2 - 5;
			enemy[i].coll.bottom = enemy[i].CenterY + mapChip.height / 2 - 5;

			//敵の移動
			if (enemy[i].image.x >= 0 && enemy[i].image.x < GAME_WIDTH)
			{
				enemy[i].image.x = enemy[i].CenterX - enemy[i].image.width / 2;
				enemy[i].image.y = enemy[i].CenterY - enemy[i].image.height / 2;

				enemy[i].collBeforePt.x = enemy[i].CenterX;
				enemy[i].collBeforePt.y = enemy[i].CenterY;
			}

			if (MY_CHECK_RECT_COLL(PlayerRect, enemy[i].coll) == TRUE)
			{
				if (CheckSoundMem(PLAY_BGM.handle) != 0)
				{
					StopSoundMem(PLAY_BGM.handle);
				}

				SetMouseDispFlag(TRUE);

				GameEndkind = GAME_END_FAIL;

				GameScene = GAME_SCENE_END;

				return;
			}
		}
	}

	if (player.image.x > GAME_WIDTH || player.image.y > GAME_HEIGHT
		|| player.image.x + player.image.width < 0 || player.image.y + player.image.height < 0)
	{
		if (CheckSoundMem(PLAY_BGM.handle) != 0)
		{
			StopSoundMem(PLAY_BGM.handle);
		}

		SetMouseDispFlag(TRUE);

		GameEndkind = GAME_END_FAIL;

		GameScene = GAME_SCENE_END;

		return;
	}

	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		ImageBack[num].image.x++;

		if (ImageBack[num].IsDraw == FALSE)
		{
			if (ImageBack[num].image.x + ImageBack[num].image.width > 0)
			{
				ImageBack[num].IsDraw = TRUE;
			}
		}

		if (ImageBack[num].image.x > GAME_WIDTH)
		{
			ImageBack[num].image.x = 0 - ImageBack[0].image.width * 3;
			ImageBack[num].IsDraw = FALSE;
		}
	}
	return;
}

VOID MY_PLAY_DRAW(VOID)
{

	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		if (ImageBack[num].IsDraw == TRUE)
		{
			DrawGraph(ImageBack[num].image.x, ImageBack[num].image.y, ImageBack[num].image.handle, TRUE);
		}
	}

	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			//マップ当たり判定
			DrawGraph(
				map[tate][yoko].x - player.mapDis,
				map[tate][yoko].y,
				mapChip.handle[map[tate][yoko].kind],
				TRUE);

			//デバッグモード
			if (map[tate][yoko].kind == k)
			{
				DrawBox(
					mapColl[tate][yoko].left,
					mapColl[tate][yoko].top,
					mapColl[tate][yoko].right, 
					mapColl[tate][yoko].bottom, 
					GetColor(255, 255, 255),
					TRUE
				);
			}
		}

	}

	DrawGraph(player.image.x, player.image.y, player.image.handle, TRUE);

	for (int i = 0; i < enemyCnt; i++)
	{
		if (enemy[i].view == TRUE) {
			DrawGraph(enemy[i].image.x, enemy[i].image.y, enemy[i].image.handle, TRUE);
		}
	}
	return;
}

VOID MY_END(VOID)
{
	MY_END_PROC();
	MY_END_DRAW();
}

VOID MY_END_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		if (CheckSoundMem(BGM_COMP.handle) != 0)
		{
			StopSoundMem(BGM_COMP.handle);
		}

		if (CheckSoundMem(BGM_FAIL.handle) != 0)
		{
			StopSoundMem(BGM_FAIL.handle);
		}

		SetMouseDispFlag(TRUE);

		GameScene = GAME_SCENE_TITLE;

		return;
	}

	if (MY_KEY_DOWN(KEY_INPUT_1) == TRUE)
	{
		if (CheckSoundMem(BGM_COMP.handle) != 0)
		{
			StopSoundMem(BGM_COMP.handle);
		}

		if (CheckSoundMem(BGM_FAIL.handle) != 0)
		{
			StopSoundMem(BGM_FAIL.handle);
		}

		SetMouseDispFlag(TRUE);

		GameScene = GAME_SCENE_START;

		return;
	}

	switch (GameEndkind)
	{
	case GAME_END_COMP:

		if (CheckSoundMem(BGM_COMP.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 30 / 100, BGM_COMP.handle);

			//DX_PLAYTYPE_NORMAL;
			//DX_PLAYTYPE_BACK;
			//DX_PLAYTYPE_LOOP;
			PlaySoundMem(BGM_COMP.handle, DX_PLAYTYPE_LOOP);
		}

		if (ImageEndCOMP.Cnt < ImageEndCOMP.CntMAX)
		{
			ImageEndCOMP.Cnt += IMAGE_END_COMP_CNT;
		}
		else
		{
			if (ImageEndCOMP.IsDraw == FALSE)
			{
				ImageEndCOMP.IsDraw = TRUE;
			}
			else if (ImageEndCOMP.IsDraw == TRUE)
			{
				ImageEndCOMP.IsDraw = FALSE;
			}
			ImageEndCOMP.Cnt = 0;
		}
		break;

	case GAME_END_FAIL:

		if (CheckSoundMem(BGM_FAIL.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 20 / 100, BGM_FAIL.handle);

			//DX_PLAYTYPE_NORMAL;
			//DX_PLAYTYPE_BACK;
			//DX_PLAYTYPE_LOOP;
			PlaySoundMem(BGM_FAIL.handle, DX_PLAYTYPE_LOOP);
		}

		if (ImageEndFAIL.Cnt < ImageEndFAIL.CntMAX)
		{
			ImageEndFAIL.Cnt += IMAGE_END_FAIL_CNT;
		}
		else
		{
			if (ImageEndFAIL.IsDraw == FALSE)
			{
				ImageEndFAIL.IsDraw = TRUE;
			}
			else if (ImageEndFAIL.IsDraw == TRUE)
			{
				ImageEndFAIL.IsDraw = FALSE;
			}
			ImageEndFAIL.Cnt = 0;
		}
		break;
	}
	return;
}

VOID MY_END_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(0, 0, 255), TRUE);

	MY_PLAY_DRAW();

	switch (GameEndkind)
	{
	case GAME_END_COMP:

		DrawGraph(ImageEndCOMP.image.x, ImageEndCOMP.image.y, ImageEndCOMP.image.handle, TRUE);

		//if (ImageEndCOMP.IsDraw == TRUE)
		//{
		//	DrawGraph(ImageEndCOMP.image.x, ImageEndCOMP.image.y, ImageEndCOMP.image.handle, TRUE);
		//}
		break;

	case GAME_END_FAIL:

		DrawGraph(ImageEndFAIL.image.x, ImageEndFAIL.image.y, ImageEndFAIL.image.handle, TRUE);

		//if (ImageEndFAIL.IsDraw == TRUE)
		//{
		//	DrawGraph(ImageEndFAIL.image.x, ImageEndFAIL.image.y, ImageEndFAIL.image.handle, TRUE);
		//}
		break;

	}

	DrawString(0, 0, "タイトル画面(エスケープキーを押してください)", GetColor(255, 255, 255));
	DrawString(0, 20, "操作画面(1キーを押してください)", GetColor(255, 255, 255));
	return;
}

BOOL MY_LOAD_IMAGE(VOID)
{
	//タイトル画面背景画像
	strcpy_s(ImageTitleBk.path, IMAGE_TITLE_BK_PATH);
	ImageTitleBk.handle = LoadGraph(ImageTitleBk.path);
	if (ImageTitleBk.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_BK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleBk.handle, &ImageTitleBk.width, &ImageTitleBk.height);
	ImageTitleBk.x = GAME_WIDTH / 2 - ImageTitleBk.width / 2;
	ImageTitleBk.y = GAME_HEIGHT / 2 - ImageTitleBk.height / 2;

	//タイトルロゴ画面画像
	strcpy_s(ImageTitleROGO.image.path, IMAGE_TITLE_ROGO_PATH);
	ImageTitleROGO.image.handle = LoadGraph(ImageTitleROGO.image.path);
	if (ImageTitleROGO.image.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleROGO.image.handle, &ImageTitleROGO.image.width, &ImageTitleROGO.image.height);
	ImageTitleROGO.image.x = 230;
	ImageTitleROGO.image.y = GAME_HEIGHT / 2;
	ImageTitleROGO.angle = DX_PI * 2;
	ImageTitleROGO.angleMAX = DX_PI * 2;
	ImageTitleROGO.rate = 0.0;
	ImageTitleROGO.rateMAX = IMAGE_TITLE_ROGO_ROTA_MAX;

	strcpy_s(ImageTitleSTART.image.path, IMAGE_TITLE_PATH);					//パスの設定
	ImageTitleSTART.image.handle = LoadGraph(ImageTitleSTART.image.path);			//読み込み
	if (ImageTitleSTART.image.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleSTART.image.handle, &ImageTitleSTART.image.width, &ImageTitleSTART.image.height);	//画像の幅と高さを取得
	ImageTitleSTART.image.x = GAME_WIDTH / 2 - ImageTitleSTART.image.width / 2;					//左右中央揃え
	ImageTitleSTART.image.y = ImageTitleROGO.image.y + ImageTitleROGO.image.height / 2 + 10;	//ロゴの下に表示
	ImageTitleSTART.Cnt = 0.0;								//カウンタ
	ImageTitleSTART.CntMAX = IMAGE_TITLE_CNT_MAX;		//カウンタMAX
	ImageTitleSTART.IsDraw = FALSE;							//描画させない

	//スタート画面画像
	strcpy_s(ImageStart.path, IMAGE_START_PATH);
	ImageStart.handle = LoadGraph(ImageStart.path);
	if (ImageStart.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_START_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageStart.handle, &ImageStart.width, &ImageStart.height);
	ImageStart.x = GAME_WIDTH / 2 - ImageStart.width / 2;
	ImageStart.y = GAME_HEIGHT / 2 - ImageStart.height / 2;

	strcpy_s(ImageStartSTART.image.path, IMAGE_START_ROGO_PATH);					//パスの設定
	ImageStartSTART.image.handle = LoadGraph(ImageStartSTART.image.path);		//読み込み
	if (ImageStartSTART.image.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_START_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageStartSTART.image.handle, &ImageStartSTART.image.width, &ImageStartSTART.image.height);	//画像の幅と高さを取得
	ImageStartSTART.image.x = GAME_WIDTH / 2 - ImageStartSTART.image.width / 2;							//左右中央揃え
	ImageStartSTART.image.y = ImageTitleROGO.image.y + ImageTitleROGO.image.height / 2 + 10;				//ロゴの下に表示
	ImageStartSTART.Cnt = 0.0;								//カウンタ
	ImageStartSTART.CntMAX = IMAGE_TITLE_CNT_MAX;			//カウンタMAX
	ImageStartSTART.IsDraw = FALSE;						//描画させない

	//成功画像
	strcpy_s(ImageEndCOMP.image.path, IMAGE_END_COMP_PATH);
	ImageEndCOMP.image.handle = LoadGraph(ImageEndCOMP.image.path);
	if (ImageEndCOMP.image.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_END_COMP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEndCOMP.image.handle, &ImageEndCOMP.image.width, &ImageEndCOMP.image.height);
	ImageEndCOMP.image.x = GAME_WIDTH / 2 - ImageEndCOMP.image.width / 2;
	ImageEndCOMP.image.y = GAME_HEIGHT / 2 - ImageEndCOMP.image.height / 2;
	ImageEndCOMP.Cnt = 0.0;
	ImageEndCOMP.CntMAX = IMAGE_END_COMP_CNT_MAX;
	ImageEndCOMP.IsDraw = FALSE;

	//失敗画像
	strcpy_s(ImageEndFAIL.image.path, IMAGE_END_FAIL_PATH);
	ImageEndFAIL.image.handle = LoadGraph(ImageEndFAIL.image.path);
	if (ImageEndFAIL.image.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_END_FAIL_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEndFAIL.image.handle, &ImageEndFAIL.image.width, &ImageEndFAIL.image.height);
	ImageEndFAIL.image.x = GAME_WIDTH / 2 - ImageEndFAIL.image.width / 2;
	ImageEndFAIL.image.y = GAME_HEIGHT / 2 - ImageEndFAIL.image.height / 2;
	ImageEndFAIL.Cnt = 0.0;
	ImageEndFAIL.CntMAX = IMAGE_END_COMP_CNT_MAX;
	ImageEndFAIL.IsDraw = FALSE;



	strcpy_s(ImageBack[0].image.path, IMAGE_BACK_PATH);
	strcpy_s(ImageBack[1].image.path, IMAGE_BACK_REV_PATH);
	strcpy_s(ImageBack[2].image.path, IMAGE_BACK_PATH);
	strcpy_s(ImageBack[3].image.path, IMAGE_BACK_REV_PATH);

	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		ImageBack[num].image.handle = LoadGraph(ImageBack[num].image.path);
		if (ImageBack[num].image.handle == -1)
		{
			MessageBox(GetMainWindowHandle(), IMAGE_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
		GetGraphSize(ImageBack[num].image.handle, &ImageBack[num].image.width, &ImageBack[num].image.height);
	}

	ImageBack[0].image.x = 0 - ImageBack[0].image.width * 0;
	ImageBack[0].image.y = GAME_HEIGHT / 2 - ImageBack[0].image.height / 2;
	ImageBack[0].IsDraw = FALSE;

	ImageBack[1].image.x = 0 - ImageBack[0].image.width * 1;
	ImageBack[1].image.y = GAME_HEIGHT / 2 - ImageBack[1].image.height / 2;
	ImageBack[1].IsDraw = FALSE;

	ImageBack[2].image.x = 0 - ImageBack[0].image.width * 2;
	ImageBack[2].image.y = GAME_HEIGHT / 2 - ImageBack[0].image.height / 2;
	ImageBack[2].IsDraw = FALSE;

	ImageBack[3].image.x = 0 - ImageBack[0].image.width * 3;
	ImageBack[3].image.y = GAME_HEIGHT / 2 - ImageBack[0].image.height / 2;
	ImageBack[3].IsDraw = FALSE;

	strcpy_s(player.image.path, IMAGE_PLAYER_PATH);
	player.image.handle = LoadGraph(player.image.path);

	if (player.image.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), IMAGE_PLAYER_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(player.image.handle, &player.image.width, &player.image.height);
	player.image.x = GAME_WIDTH / 2 - player.image.width / 2;
	player.image.y = GAME_HEIGHT / 2 - player.image.height / 2;
	player.CenterX = player.image.x + player.image.width / 2;
	player.CenterY = player.image.y + player.image.height / 2;
	player.speed = CHARA_SPEED_HIGH;

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		strcpy_s(enemy[i].image.path, IMAGE_ENEMY_PATH);
		enemy[i].image.handle = LoadGraph(enemy[i].image.path);
		if (enemy[i].image.handle == -1)
		{
			MessageBox(GetMainWindowHandle(), IMAGE_ENEMY_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
			return(FALSE);
		}
		GetGraphSize(enemy[i].image.handle, &enemy[i].image.width, &enemy[i].image.height);
		enemy[i].image.x = GAME_WIDTH / 2 - enemy[i].image.width / 2;
		enemy[i].image.y = GAME_HEIGHT / 2 - enemy[i].image.height / 2;
		enemy[i].CenterX = enemy[i].image.x + enemy[i].image.width / 2;
		enemy[i].CenterY = enemy[i].image.y + enemy[i].image.height / 2;
		enemy[i].speed = CHARA_SPEED_LOW;
	}

	int mapRes = LoadDivGraph(
		GAME_MAP_PATH,
		MAP_DIV_NUM, MAP_DIV_TATE, MAP_DIV_YOKO,
		MAP_DIV_WIDTH, MAP_DIV_HEIGHT,
		&mapChip.handle[0]);

	if (mapRes == -1)
	{
		MessageBox(GetMainWindowHandle(), GAME_MAP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	GetGraphSize(mapChip.handle[0], &mapChip.width, &mapChip.height);

	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			mapDataInit[tate][yoko] = mapData[tate][yoko];

			map[tate][yoko].kind = mapData[tate][yoko];

			map[tate][yoko].width = mapChip.width;
			map[tate][yoko].height = mapChip.height;

			map[tate][yoko].x = yoko * map[tate][yoko].width;
			map[tate][yoko].y = tate * map[tate][yoko].height;

		}

	}

	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			mapColl[tate][yoko].left = (yoko + 0) * mapChip.width + 1;
			mapColl[tate][yoko].top = (tate + 0) * mapChip.height + 1;
			mapColl[tate][yoko].right = (yoko + 1) * mapChip.width - 1;
			mapColl[tate][yoko].bottom = (tate + 1) * mapChip.height - 1;
		}

	}

	if (mapRes == -1)
	{
		MessageBox(GetMainWindowHandle(), GAME_MAP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	GetGraphSize(mapChip.handle[0], &mapChip.width, &mapChip.height);

	//for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	//{
	//	for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
	//	{
	//		mapDataInit[tate][yoko] = mapData_2[tate][yoko];

	//		map[tate][yoko].kind = mapData_2[tate][yoko];

	//		map[tate][yoko].width = mapChip.width;
	//		map[tate][yoko].height = mapChip.height;

	//		map[tate][yoko].x = yoko * map[tate][yoko].width;
	//		map[tate][yoko].y = tate * map[tate][yoko].height;

	//	}

	//}

	//for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	//{
	//	for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
	//	{
	//		mapColl[tate][yoko].left = (yoko + 0) * mapChip.width + 1;
	//		mapColl[tate][yoko].top = (tate + 0) * mapChip.height + 1;
	//		mapColl[tate][yoko].right = (yoko + 1) * mapChip.width - 1;
	//		mapColl[tate][yoko].bottom = (tate + 1) * mapChip.height - 1;
	//	}

	//}

	//if (mapRes == -1)
	//{
	//	MessageBox(GetMainWindowHandle(), GAME_MAP_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
	//	return FALSE;
	//}

	//GetGraphSize(mapChip.handle[0], &mapChip.width, &mapChip.height);

	//for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	//{
	//	for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
	//	{
	//		mapDataInit[tate][yoko] = mapData_3[tate][yoko];

	//		map[tate][yoko].kind = mapData_3[tate][yoko];

	//		map[tate][yoko].width = mapChip.width;
	//		map[tate][yoko].height = mapChip.height;

	//		map[tate][yoko].x = yoko * map[tate][yoko].width;
	//		map[tate][yoko].y = tate * map[tate][yoko].height;

	//	}

	//}

	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			mapColl[tate][yoko].left = (yoko + 0) * mapChip.width + 1;
			mapColl[tate][yoko].top = (tate + 0) * mapChip.height + 1;
			mapColl[tate][yoko].right = (yoko + 1) * mapChip.width - 1;
			mapColl[tate][yoko].bottom = (tate + 1) * mapChip.height - 1;
		}

	}

	return TRUE;
}

VOID MY_DELETE_IMAGE(VOID)
{
	//DeleteGraph(ImageBack.handle);

	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		DeleteGraph(ImageBack[0].image.handle);
	}
	DeleteGraph(player.image.handle);

	for (int i = 0; i < ENEMY_MAX; i++)DeleteGraph(enemy[i].image.handle);

	DeleteGraph(ImageTitleBk.handle);
	DeleteGraph(ImageTitleROGO.image.handle);
	DeleteGraph(ImageTitleSTART.image.handle);

	DeleteGraph(ImageEndCOMP.image.handle);
	DeleteGraph(ImageEndFAIL.image.handle);


	for (int i_num = 0; i_num < MAP_DIV_NUM; i_num++) { DeleteGraph(mapChip.handle[i_num]); }

	return;
}

BOOL MY_LOAD_MUSIC(VOID)
{
	strcpy_s(START_BGM.path, MUSIC_START_BGM_PATH);
	START_BGM.handle = LoadSoundMem(START_BGM.path);
	if (START_BGM.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_START_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	strcpy_s(PLAY_BGM.path, MUSIC_PLAY_BGM_PATH);
	PLAY_BGM.handle = LoadSoundMem(PLAY_BGM.path);
	if (PLAY_BGM.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_PLAY_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	strcpy_s(BGM_TITLE.path, MUSIC_BGM_TITLE_PATH);
	BGM_TITLE.handle = LoadSoundMem(BGM_TITLE.path);
	if (BGM_TITLE.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM_TITLE_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	strcpy_s(BGM_COMP.path, MUSIC_BGM_COMP_PATH);
	BGM_COMP.handle = LoadSoundMem(BGM_COMP.path);
	if (BGM_COMP.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM_COMP_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	strcpy_s(BGM_FAIL.path, MUSIC_BGM_FAIL_PATH);
	BGM_FAIL.handle = LoadSoundMem(BGM_FAIL.path);
	if (BGM_FAIL.handle == -1)
	{
		MessageBox(GetMainWindowHandle(), MUSIC_BGM_FAIL_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(START_BGM.handle);
	DeleteSoundMem(PLAY_BGM.handle);

	DeleteSoundMem(BGM_TITLE.handle);
	DeleteSoundMem(BGM_COMP.handle);
	DeleteSoundMem(BGM_FAIL.handle);

	return;
}

BOOL MY_CHECK_MAP1_PLAYER_COLL(RECT player)
{
	for (int tate = 0; tate <= GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko <= GAME_MAP_YOKO_MAX; yoko++)
		{
			if (MY_CHECK_RECT_COLL(player, mapColl[tate][yoko]) == TRUE)
			{
				if (map[tate][yoko].kind == k) { return TRUE; }
			}
		}
	}

	return FALSE;
}

//ゴールに当たった時
BOOL MY_CHECK_MAP1_GOAL(RECT player)
{
	for (int tate = 0; tate <= GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko <= GAME_MAP_YOKO_MAX; yoko++)
		{
			if (MY_CHECK_RECT_COLL(player, mapColl[tate][yoko]) == TRUE)
			{
				if (map[tate][yoko].kind == g) { return TRUE; }
			}
		}
	}

	return FALSE;
}

BOOL MY_CHECK_RECT_COLL(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;
	}
	return FALSE;
}

