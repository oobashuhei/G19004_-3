#include "DxLib.h"

#define GAME_WIDTH		1200
#define GAME_HEIGHT		600
#define GAME_COLOR		32

#define GAME_WINDOW_BAR	0
#define GAME_WINDOW_NAME	"Runner"

#define GAME_FPS			60

#define PATH_MAX			255

#define IMAGE_LOAD_ERR_TITLE	TEXT("画像読込エラー")

//画像
#define IMAGE_BACK_PATH		TEXT(".\\IMAGE\\background.jpg")
#define IMAGE_PLAYER_PATH		TEXT("GN.png")

#define MUSIC_LOAD_ERR_TITLE	TEXT("音楽読込エラー")

#define MUSIC_PLAY_BGM_PATH		TEXT(".\\MUSIC\\トロピカル.mp3")
#define MUSIC_TITLE_BGM_PATH		TEXT(".\\MUSIC\\トロピカル.mp3")
#define MUSIC_START_BGM_PATH		TEXT(".\\MUSIC\\トロピカル.mp3")
#define MUSIC_END_BGM_PATH		TEXT(".\\MUSIC\\トロピカル.mp3")

enum GAME_SCENE {
	GAME_SCENE_TITLE,
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END
};

enum CHARA_SPEED {
	CHARA_SPEED_LOW = 1,
	CHARA_SPEED_MIDI = 2,
	CHARA_SPEED_HIGH = 3
};

typedef struct STRUCT_I_POINT
{
	int x = -1;
	int y = -1;
}iPOINT;

typedef struct STRUCT_IMAGE {
	char path[PATH_MAX];
	int handle;
	int x;
	int y;
	int width;
	int height;
}IMAGE;

typedef struct STRUCT_CHARA {
	IMAGE image;
	int speed;
	int CenterX;
	int CenterY;
}CHARA;

typedef struct STRUCT_MUSIC {
	char path[PATH_MAX];
	int handle;
}MUSIC;

int StartTimeFps;
int CountFps;
float CalcFps;
int SampleNumFps = GAME_FPS;

char AllKeyState[256] = { '\0' };
char OldAllKeyState[256] = { '\0' };

int GameScene;

//背景
IMAGE ImageBack;
//プレイヤー
CHARA player;
//音楽
MUSIC PLAY_BGM;
MUSIC TITLE_BGM;
MUSIC START_BGM;
MUSIC END_BGM;

VOID MY_FPS_UPDATE(VOID);
VOID MY_FPS_DRAW(VOID);
VOID MY_FPS_WAIT(VOID);

VOID MY_ALL_KEYDOWN_UPDATE(VOID);
BOOL MY_KEY_DOWN(int);
BOOL MY_KEY_UP(int);
BOOL MY_KEYDOWN_KEEP(int, int);

VOID MY_TITLE(VOID);
VOID MY_TITLE_PROC(VOID);
VOID MY_TITLE_DRAW(VOID);

VOID MY_START(VOID);
VOID MY_START_PROC(VOID);
VOID MY_START_DRAW(VOID);

VOID MY_PLAY(VOID);
VOID MY_PLAY_PROC(VOID);
VOID MY_PLAY_DRAW(VOID);

VOID MY_END(VOID);
VOID MY_END_PROC(VOID);
VOID MY_END_DRAW(VOID);

BOOL MY_LOAD_IMAGE(VOID);
VOID MY_DELETE_IMAGE(VOID);

BOOL MY_LOAD_MUSIC(VOID);
VOID MY_DELETE_MUSIC(VOID);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInsstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	SetWindowStyleMode(GAME_WINDOW_BAR);
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));
	SetAlwaysRunFlag(TRUE);
	//SetWindowIconID(IDI_ICON1);

	if (DxLib_Init() == -1) { return -1; }
	//初期化

	//画像読込
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	//音楽読込
	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	int DrawX = 0;
	int DrawY = 0;

	iPOINT InputFirst = { 0,0 };
	iPOINT InputEnd = { 0,0 };

	//ゲームシーンタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	SetDrawScreen(DX_SCREEN_BACK);

	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		MY_ALL_KEYDOWN_UPDATE();

		MY_FPS_UPDATE();

		DrawString(DrawX, DrawY, "糞土方", GetColor(255, 255, 255));

		if (MY_KEY_DOWN(KEY_INPUT_UP) == TRUE) { DrawY--; }
		if (MY_KEY_DOWN(KEY_INPUT_DOWN) == TRUE) { DrawY++; }
		if (MY_KEY_DOWN(KEY_INPUT_LEFT) == TRUE) { DrawX--; }
		if (MY_KEY_DOWN(KEY_INPUT_RIGHT) == TRUE) { DrawX++; }

		DrawFormatString(0, 20, GetColor(255, 255, 255), "DrawX:%d", DrawX);
		DrawFormatString(0, 35, GetColor(255, 255, 255), "DrawY:%d", DrawY);

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

		case GAME_SCENE_END:
			MY_END();
			break;
		}

		MY_FPS_DRAW();

		ScreenFlip();

		MY_FPS_WAIT();

	}

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
	//キーコードのキーを押している時
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//キーを押している
	}
	else
	{
		return FALSE;	//キーを押していない
	}
}

BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//直前は押していて
		&& AllKeyState[KEY_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//キーを押し上げている
	}
	else
	{
		return FALSE;	//キーを押し上げていない
	}
}

BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;
	}
}

//タイトル画面
VOID MY_TITLE(VOID)
{
	MY_TITLE_PROC();	//スタート画面の処理
	MY_TITLE_DRAW();	//スタート画面の描画

	return;
}

//タイトル画面の処理
VOID MY_TITLE_PROC(VOID)
{
	if (CheckSoundMem(TITLE_BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, TITLE_BGM. handle);
		PlaySoundMem(TITLE_BGM. handle, DX_PLAYTYPE_LOOP);
	}
	//エンターキーを押したら、操作画面シーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		if (CheckSoundMem(TITLE_BGM.handle) != 0)
		{
			StopSoundMem(TITLE_BGM.handle);
		}
		GameScene = GAME_SCENE_START;
	}

	return;
}

//タイトル画面の描画
VOID MY_TITLE_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(255, 0, 0), TRUE);

	DrawGraph(ImageBack.x, ImageBack.y, ImageBack.handle, TRUE);

	DrawString(0, 0, "タイトル画面(エンターキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

VOID MY_START(VOID)
{
	MY_START_PROC();	//スタート画面の処理
	MY_START_DRAW();	//スタート画面の描画

	return;
}

VOID MY_START_PROC(VOID)
{
	if (CheckSoundMem(START_BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, START_BGM. handle);
		PlaySoundMem(START_BGM. handle, DX_PLAYTYPE_LOOP);
	}

	//Sキーを押したら、プレイシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_LSHIFT) == TRUE)
	{
		if (CheckSoundMem(START_BGM.handle) != 0)
		{
			StopSoundMem(START_BGM.handle);
		}
		GameScene = GAME_SCENE_PLAY;
	}

	return;
}

VOID MY_START_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(255, 0, 0), TRUE);

	DrawGraph(ImageBack.x, ImageBack.y, ImageBack.handle, TRUE);

	DrawString(0, 0, "操作説明画面(Lシフトキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//プレイ画面
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//プレイ画面の処理
	MY_PLAY_DRAW();	//プレイ画面の描画

	return;
}

//プレイ画面の処理
VOID MY_PLAY_PROC(VOID)
{
	if (CheckSoundMem(PLAY_BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, PLAY_BGM.handle);
		PlaySoundMem(PLAY_BGM.handle, DX_PLAYTYPE_LOOP);
	}

	if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
		if (CheckSoundMem(PLAY_BGM.handle) != 0)
		{
			StopSoundMem(PLAY_BGM.handle);
		}
		GameScene = GAME_SCENE_END;
	}

	player.image.x = player.CenterX - player.image.width / 2;
	player.image.y = player.CenterY - player.image.height / 2;

	//画面外にプレイヤーが行かないようにする
	if (player.image.x < 0) { player.image.x = 0; }
	if (player.image.x + player.image.width > GAME_WIDTH) { player.image.x = GAME_WIDTH - player.image.width; }
	if (player.image.y < 0) { player.image.y = 0; }
	if (player.image.y + player.image.height > GAME_HEIGHT) { player.image.y = GAME_HEIGHT - player.image.height; }
	return;
}

//プレイ画面の描画
VOID MY_PLAY_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(0, 255, 0), TRUE);

	DrawString(0, 0, "プレイ画面(スペースキーを押して下さい)", GetColor(255, 255, 255));

	//プレイヤー描画
	DrawExtendGraph(
		player.image.x, player.image.y,
		player.image.x + player.image.width * 2, player.image.y + player.image.height * 2,
		player.image.handle, TRUE);
	return;
}

//エンド画面
VOID MY_END(VOID)
{
	MY_END_PROC();	//エンド画面の処理
	MY_END_DRAW();	//エンド画面の描画
	return;
}

//エンド画面の処理
VOID MY_END_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		//if (CheckSoundMem(BGM_FAIL.handle) != 0)
		//{
		//	StopSoundMem(BGM_FAIL.handle);	//BGMを止める
		//}

		if (CheckSoundMem(END_BGM.handle) != 0)
		{
			StopSoundMem(END_BGM.handle);	//BGMを止める
		}

		GameScene = GAME_SCENE_TITLE;
	}

	return;
}

//エンド画面の描画
VOID MY_END_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(0, 0, 255), TRUE);

	DrawString(0, 0, "エンド画面(エスケープキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

BOOL MY_LOAD_IMAGE(VOID)
{
	//背景画像
	strcpy_s(ImageBack.path, IMAGE_BACK_PATH);		//パスの設定
	ImageBack.handle = LoadGraph(ImageBack.path);	//読み込み
	if (ImageBack.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageBack.handle, &ImageBack.width, &ImageBack.height);	//画像の幅と高さを取得
	ImageBack.x = GAME_WIDTH / 2 - ImageBack.width / 2;		//左右中央揃え
	ImageBack.y = GAME_HEIGHT / 2 - ImageBack.height / 2;	//上下中央揃え

	//プレイヤーの画像
	strcpy_s(player.image.path, IMAGE_PLAYER_PATH);		//パスの設定
	player.image.handle = LoadGraph(player.image.path);	//読み込み
	if (player.image.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_PLAYER_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(player.image.handle, &player.image.width, &player.image.height);	//画像の幅と高さを取得
	player.image.x = GAME_WIDTH / 2 - player.image.width / 2;		//左右中央揃え
	player.image.y = GAME_HEIGHT / 2 - player.image.height / 2;		//上下中央揃え
	player.CenterX = player.image.x + player.image.width / 2;		//画像の横の中心を探す
	player.CenterY = player.image.y + player.image.height / 2;		//画像の縦の中心を探す
	player.speed = CHARA_SPEED_LOW;									//スピードを設定

	return TRUE;
}

//画像をまとめて削除する関数
VOID MY_DELETE_IMAGE(VOID)
{
	DeleteGraph(ImageBack.handle);
	DeleteGraph(player.image.handle);

	return;
}

BOOL MY_LOAD_MUSIC(VOID)
{
	//プレイ
	strcpy_s(PLAY_BGM.path, MUSIC_PLAY_BGM_PATH);		
	PLAY_BGM.handle = LoadSoundMem(PLAY_BGM.path);	
	if (PLAY_BGM.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_PLAY_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//タイトル背景音楽
	strcpy_s(TITLE_BGM.path, MUSIC_TITLE_BGM_PATH);			//パスの設定
	TITLE_BGM.handle = LoadSoundMem(TITLE_BGM.path);		//読み込み
	if (TITLE_BGM.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_TITLE_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//操作画面背景音楽
	strcpy_s(START_BGM.path, MUSIC_TITLE_BGM_PATH);			//パスの設定
	START_BGM.handle = LoadSoundMem(START_BGM.path);		//読み込み
	if (START_BGM.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_TITLE_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//エンド背景音楽
	strcpy_s(END_BGM.path, MUSIC_END_BGM_PATH);		
	END_BGM.handle = LoadSoundMem(END_BGM.path);	
	if (END_BGM.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_END_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	////フォールトBGM
	//strcpy_s(BGM_FAIL.path, MUSIC_BGM_FAIL_PATH);		
	//BGM_FAIL.handle = LoadSoundMem(BGM_FAIL.path);		
	//if (BGM_FAIL.handle == -1)
	//{
	//	//エラーメッセージ表示
	//	MessageBox(GetMainWindowHandle(), MUSIC_BGM_FAIL_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
	//	return FALSE;
	//}

	return TRUE;
}

VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(TITLE_BGM.handle);
	DeleteSoundMem(START_BGM.handle);	//操作説明BGM削除
	DeleteSoundMem(PLAY_BGM.handle);	//プレイBGM削除
	DeleteSoundMem(END_BGM.handle);	//ENDBGM削除
}