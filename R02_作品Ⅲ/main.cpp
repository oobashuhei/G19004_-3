#include "DxLib.h"

#define GAME_WIDTH		1200
#define GAME_HEIGHT		600
#define GAME_COLOR		32

#define GAME_WINDOW_BAR	0
#define GAME_WINDOW_NAME	"Runner"

#define GAME_FPS			60

#define PATH_MAX			255

#define IMAGE_LOAD_ERR_TITLE	TEXT("�摜�Ǎ��G���[")

//�摜
#define IMAGE_BACK_PATH		TEXT(".\\IMAGE\\background.jpg")
#define IMAGE_TITLE_PATH		TEXT(".\\IMAGE\\TITLE.png")
#define IMAGE_START_PATH		TEXT(".\\IMAGE\\sousa.jpg")

//�v���C���[�摜
#define IMAGE_PLAYER_PATH		TEXT("GN.png")

#define IMAGE_BACK_REV_PATH	TEXT(".\\IMAGE\\background_2.jpg")
#define IMAGE_BACK_NUM		4

#define MUSIC_LOAD_ERR_TITLE	TEXT("���y�Ǎ��G���[")

#define MUSIC_PLAY_BGM_PATH		TEXT(".\\MUSIC\\�g���s�J��.mp3")
#define MUSIC_TITLE_BGM_PATH		TEXT(".\\MUSIC\\GNOP.mp3")
#define MUSIC_START_BGM_PATH		TEXT(".\\MUSIC\\�g���s�J��.mp3")
#define MUSIC_END_BGM_PATH		TEXT(".\\MUSIC\\�g���s�J��.mp3")

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

typedef struct STRUCT_IMAGE_BACK
{
	IMAGE image;
	BOOL IsDraw;
}IMAGE_BACK;

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

int Scroll;

//�w�i
IMAGE_BACK ImageBack[IMAGE_BACK_NUM];
IMAGE TITLE;
IMAGE ImageSousa;
//�v���C���[
CHARA player;
//���y
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
	//������

	//�摜�Ǎ�
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	//���y�Ǎ�
	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	int DrawX = 0;
	int DrawY = 0;

	iPOINT InputFirst = { 0,0 };
	iPOINT InputEnd = { 0,0 };

	//�Q�[���V�[���^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	SetDrawScreen(DX_SCREEN_BACK);

	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		MY_ALL_KEYDOWN_UPDATE();

		MY_FPS_UPDATE();

		DrawString(DrawX, DrawY, "���y��", GetColor(255, 255, 255));

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
	//�L�[�R�[�h�̃L�[�������Ă��鎞
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//�L�[�������Ă���
	}
	else
	{
		return FALSE;	//�L�[�������Ă��Ȃ�
	}
}

BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//���O�͉����Ă���
		&& AllKeyState[KEY_INPUT_] == 0)	//���͉����Ă��Ȃ��Ƃ�
	{
		return TRUE;	//�L�[�������グ�Ă���
	}
	else
	{
		return FALSE;	//�L�[�������グ�Ă��Ȃ�
	}
}

BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;
	}
}

//�^�C�g�����
VOID MY_TITLE(VOID)
{
	MY_TITLE_PROC();	//�X�^�[�g��ʂ̏���
	MY_TITLE_DRAW();	//�X�^�[�g��ʂ̕`��

	return;
}

//�^�C�g����ʂ̏���
VOID MY_TITLE_PROC(VOID)
{
	if (CheckSoundMem(TITLE_BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, TITLE_BGM. handle);
		PlaySoundMem(TITLE_BGM. handle, DX_PLAYTYPE_LOOP);
	}
	//�G���^�[�L�[����������A�����ʃV�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		if (CheckSoundMem(TITLE_BGM.handle) != 0)
		{
			StopSoundMem(TITLE_BGM.handle);
		}
		GameScene = GAME_SCENE_START;
	}

	//�w�i�ړ�
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

//�^�C�g����ʂ̕`��
VOID MY_TITLE_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(255, 0, 0), TRUE);

	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		//�`��ł���Ƃ��́E�E�E
		if (ImageBack[num].IsDraw == TRUE)
		{
			//�w�i��`��
			DrawGraph(ImageBack[num].image.x, ImageBack[num].image.y, ImageBack[num].image.handle, TRUE);

			//�w�i�摜�̐������e�X�g�I�ɕ\��
			DrawFormatString(
				ImageBack[num].image.x,
				ImageBack[num].image.y,
				GetColor(255, 255, 255), "�w�i�摜�F%d", num + 1);
		}
	}

	DrawGraph(TITLE.x, TITLE.y, TITLE.handle, TRUE);

	DrawString(0, 0, "�^�C�g�����(�G���^�[�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

VOID MY_START(VOID)
{
	MY_START_PROC();	//�X�^�[�g��ʂ̏���
	MY_START_DRAW();	//�X�^�[�g��ʂ̕`��

	return;
}

VOID MY_START_PROC(VOID)
{
	if (CheckSoundMem(START_BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, START_BGM. handle);
		PlaySoundMem(START_BGM. handle, DX_PLAYTYPE_LOOP);
	}

	//S�L�[����������A�v���C�V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_S) == TRUE)
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

	DrawGraph(ImageSousa.x, ImageSousa.y, ImageSousa.handle, TRUE);

	DrawString(0, 0, "����������(S�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

//�v���C���
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//�v���C��ʂ̏���
	MY_PLAY_DRAW();	//�v���C��ʂ̕`��

	return;
}

//�v���C��ʂ̏���
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

	//��ʊO�Ƀv���C���[���s���Ȃ��悤�ɂ���
	if (player.image.x < 0) { player.image.x = 0; }
	if (player.image.x + player.image.width > GAME_WIDTH) { player.image.x = GAME_WIDTH - player.image.width; }
	if (player.image.y < 0) { player.image.y = 0; }
	if (player.image.y + player.image.height > GAME_HEIGHT) { player.image.y = GAME_HEIGHT - player.image.height; }
	return;
}

//�v���C��ʂ̕`��
VOID MY_PLAY_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(0, 255, 0), TRUE);

	DrawString(0, 0, "�v���C���(�X�y�[�X�L�[�������ĉ�����)", GetColor(255, 255, 255));

	//�v���C���[�`��
	DrawExtendGraph(
		player.image.x, player.image.y,
		player.image.x + player.image.width * 2, player.image.y + player.image.height * 2,
		player.image.handle, TRUE);

	/*	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{

				if (player.CenterX > (GAME_WIDTH / 2) && player.CenterX <= (GAME_MAP_YOKO_MAX * mapChip.width) - (GAME_WIDTH / 2))
				{
					if (player.CenterX - (GAME_WIDTH / 2) - mapChip.width <= map[player][tate][yoko].x &&
						map[player][tate][yoko].x <= player.CenterX + (GAME_WIDTH / 2) &&
						0 <= map[player][tate][yoko].y &&
						map[player][tate][yoko].y < GAME_HEIGHT + mapChip.height)
					{
						DrawGraph(
							map[player][tate][yoko].x - (player.CenterX - (GAME_WIDTH / 2)),
							map[player][tate][yoko].y,
							mapChip.handle[map[player][tate][yoko].kind],
							TRUE);
						DrawRotaGraph(player.image.x - (player.CenterX - (GAME_WIDTH / 2)) + player.image.width / 2, player.image.y + player.image.height / 2, 1.0, player.Muki / 18 * M_PI, player.image.handle, TRUE);
					}
				}
				else if (player.CenterX > (GAME_MAP_YOKO_MAX * mapChip.width) - (GAME_WIDTH / 2))
				{
					if ((GAME_MAP_YOKO_MAX)*mapChip.width - GAME_WIDTH - mapChip.width <= map[player][tate][yoko].x &&
						map[player][tate][yoko].x <= (GAME_MAP_YOKO_MAX + 1) * mapChip.width &&
						0 <= map[player][tate][yoko].y &&
						map[player][tate][yoko].y < GAME_HEIGHT + mapChip.height)
					{
						DrawGraph(
							map[player][tate][yoko].x - (GAME_MAP_YOKO_MAX * mapChip.width - GAME_WIDTH),
							map[player][tate][yoko].y,
							mapChip.handle[map[player][tate][yoko].kind],
							TRUE);
						DrawRotaGraph(player.image.x - (GAME_MAP_YOKO_MAX * mapChip.width - GAME_WIDTH) + player.image.width / 2, player.image.y + player.image.height / 2, 1.0, player.Muki / 18 * M_PI, player.image.handle, TRUE);
					}
				}
				else
				{
					DrawGraph(
						map[player][tate][yoko].x,
						map[player][tate][yoko].y,
						mapChip.handle[map[player][tate][yoko].kind],
						TRUE);
					DrawRotaGraph(player.image.x + player.image.width / 2, player.image.y + player.image.height / 2, 1.0, player.Muki / 18 * M_PI, player.image.handle, TRUE);
				}
			}
		}*/
	return;
}

//�G���h���
VOID MY_END(VOID)
{
	MY_END_PROC();	//�G���h��ʂ̏���
	MY_END_DRAW();	//�G���h��ʂ̕`��
	return;
}

//�G���h��ʂ̏���
VOID MY_END_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		//if (CheckSoundMem(BGM_FAIL.handle) != 0)
		//{
		//	StopSoundMem(BGM_FAIL.handle);	//BGM���~�߂�
		//}

		if (CheckSoundMem(END_BGM.handle) != 0)
		{
			StopSoundMem(END_BGM.handle);	//BGM���~�߂�
		}

		GameScene = GAME_SCENE_TITLE;
	}

	return;
}

//�G���h��ʂ̕`��
VOID MY_END_DRAW(VOID)
{
	//DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(0, 0, 255), TRUE);

	DrawString(0, 0, "�G���h���(�G�X�P�[�v�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

BOOL MY_LOAD_IMAGE(VOID)
{
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

	//�^�C�g�����S
	strcpy_s(TITLE.path, IMAGE_TITLE_PATH);		//�p�X�̐ݒ�
	TITLE.handle = LoadGraph(TITLE.path);	//�ǂݍ���
	if (TITLE.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(TITLE.handle, &TITLE.width, &TITLE.height);	//�摜�̕��ƍ������擾
	TITLE.x = GAME_WIDTH / 2 - TITLE.width / 2;		//���E��������
	TITLE.y = GAME_HEIGHT / 2 - TITLE.height / 2;	//�㉺��������

	//�����ʉ摜
	strcpy_s(ImageSousa.path, IMAGE_START_PATH);		//�p�X�̐ݒ�
	ImageSousa.handle = LoadGraph(ImageSousa.path);	//�ǂݍ���
	if (ImageSousa.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_START_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageSousa.handle, &ImageSousa.width, &ImageSousa.height);	//�摜�̕��ƍ������擾
	ImageSousa.x = GAME_WIDTH / 2 - ImageSousa.width / 2;		//���E��������
	ImageSousa.y = GAME_HEIGHT / 2 - ImageSousa.height / 2;	//�㉺��������

	//�v���C���[�̉摜
	strcpy_s(player.image.path, IMAGE_PLAYER_PATH);		//�p�X�̐ݒ�
	player.image.handle = LoadGraph(player.image.path);	//�ǂݍ���
	if (player.image.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_PLAYER_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(player.image.handle, &player.image.width, &player.image.height);	//�摜�̕��ƍ������擾
	player.image.x = GAME_WIDTH / 2 - player.image.width / 2;		//���E��������
	player.image.y = GAME_HEIGHT / 2 - player.image.height / 2;		//�㉺��������
	player.CenterX = player.image.x + player.image.width / 2;		//�摜�̉��̒��S��T��
	player.CenterY = player.image.y + player.image.height / 2;		//�摜�̏c�̒��S��T��
	player.speed = CHARA_SPEED_LOW;									//�X�s�[�h��ݒ�

	return TRUE;
}

//�摜���܂Ƃ߂č폜����֐�
VOID MY_DELETE_IMAGE(VOID)
{
	for (int num = 0; num < IMAGE_BACK_NUM; num++)
	{
		DeleteGraph(ImageBack[0].image.handle);
	}

	DeleteGraph(TITLE.handle);
	DeleteGraph(player.image.handle);

	return;
}

BOOL MY_LOAD_MUSIC(VOID)
{
	//�v���C
	strcpy_s(PLAY_BGM.path, MUSIC_PLAY_BGM_PATH);		
	PLAY_BGM.handle = LoadSoundMem(PLAY_BGM.path);	
	if (PLAY_BGM.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), MUSIC_PLAY_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�^�C�g���w�i���y
	strcpy_s(TITLE_BGM.path, MUSIC_TITLE_BGM_PATH);			//�p�X�̐ݒ�
	TITLE_BGM.handle = LoadSoundMem(TITLE_BGM.path);		//�ǂݍ���
	if (TITLE_BGM.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), MUSIC_TITLE_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�����ʔw�i���y
	strcpy_s(START_BGM.path, MUSIC_START_BGM_PATH);			//�p�X�̐ݒ�
	START_BGM.handle = LoadSoundMem(START_BGM.path);		//�ǂݍ���
	if (START_BGM.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), MUSIC_START_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�G���h�w�i���y
	strcpy_s(END_BGM.path, MUSIC_END_BGM_PATH);		
	END_BGM.handle = LoadSoundMem(END_BGM.path);	
	if (END_BGM.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), MUSIC_END_BGM_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	////�t�H�[���gBGM
	//strcpy_s(BGM_FAIL.path, MUSIC_BGM_FAIL_PATH);		
	//BGM_FAIL.handle = LoadSoundMem(BGM_FAIL.path);		
	//if (BGM_FAIL.handle == -1)
	//{
	//	//�G���[���b�Z�[�W�\��
	//	MessageBox(GetMainWindowHandle(), MUSIC_BGM_FAIL_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
	//	return FALSE;
	//}

	return TRUE;
}

VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(TITLE_BGM.handle);
	DeleteSoundMem(START_BGM.handle);	//�������BGM�폜
	DeleteSoundMem(PLAY_BGM.handle);	//�v���CBGM�폜
	DeleteSoundMem(END_BGM.handle);	//ENDBGM�폜
}