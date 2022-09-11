#include <stdio.h>
#include <graphics.h>
#include <string>
//Music
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996);

#define Max_length 547
#define Max_width 800
#define Max_quantity 16

DWORD T1, T2, T3, T4;

struct Image {
	IMAGE BackGround;
	IMAGE SendBigStars[2];
	IMAGE Cute[2];
	IMAGE Enemy[4];
}image;

enum type {
	SMALL,
    BIG,
};

struct plane {
	int x;
	int y;
	int hp;
	bool exists;
	union {
		int type;
		int score;
	}un;
}SendBigStars, Enemy[Max_quantity], Cute[Max_quantity];

void EnemyHp(int i) {

	//定义敌人血量
	if (rand() % 10 == 0) {
		Enemy[i].un.type = BIG;
		Enemy[i].hp = 3;
	}
	else {
		Enemy[i].un.type = SMALL;
		Enemy[i].hp = 1;
	}
}


void ProduceEnemy() {

	//产生敌人
	for (int i = 0; i < Max_quantity; i++) {
		if (Enemy[i].exists == false) {
			Enemy[i].exists = true;
			Enemy[i].x = rand() % (Max_length - 120);
			Enemy[i].y = 0;
			EnemyHp(i);
			break;
		}
	}
}

void EnemyMove(int speed) {

	//敌机移动
	for (int i = 0; i < Max_quantity; i++) {
		if (Enemy[i].exists == true) {
			Enemy[i].y += speed;
			if (Enemy[i].y >= Max_width) {
				Enemy[i].exists = false;
			}
		}
	}
}

void ProduceCute() {
	
	//产生Cute
	for (int i = 0; i < Max_quantity; i++) {
		if (Cute[i].exists == false) {
			Cute[i].exists = true;
			Cute[i].x = SendBigStars.x + 43;
			Cute[i].y = SendBigStars.y;
			break;
		}
	}
}

void CuteMove(int speed) {
	
	//派派移动
	for (int i = 0; i < Max_quantity; i++) {
		if (Cute[i].exists == true) {
			Cute[i].y -= speed;
			if (Cute[i].y <= 0) {
				Cute[i].exists = false;
			}
		}
	}
}

void CheckScore() {
	for (int i = 0; i < Max_quantity; i++) {
		char tempscore[30] = "";
		sprintf(tempscore, "分数：%d", Enemy[i].un.score);
		outtextxy(10, 10, tempscore);
	}
}

void Play() {

	for (int i = 0; i < Max_quantity; i++) {
		if (Enemy[i].exists == false) continue;
		for (int j = 0; j < Max_quantity; j++) {
			if (Cute[j].exists == false) continue;
			if (Cute[j].x > Enemy[i].x && Cute[j].x<Enemy[i].x + 120 &&
				Cute[j].y>Enemy[i].y && Cute[j].y < Enemy[i].y + 117) {
				Cute[j].exists = false;
				Enemy[i].hp--;
				Enemy[i].un.score++;
			}
		}
		if (Enemy[i].hp <= 0) {
			Enemy[i].exists = false;
		}
		CheckScore();
	}
}

void GameInit() {

	srand(GetTickCount());
	T1 = T2 = T3 = T4 = GetTickCount();
	//播放背景音乐
	mciSendString("open ./res/game_music.mp3 alias bgm", 0, 0, 0);
	mciSendString("play bgm repeat", 0, 0, 0);
	//LoadBackGround
	loadimage(&image.BackGround, "./res/background.jpg");
	//LoadSendBigStars
	loadimage(&image.SendBigStars[0], "./res/planeNormal_1.jpg");
	loadimage(&image.SendBigStars[1], "./res/planeNormal_2.jpg");
	//LoadCute
	loadimage(&image.Cute[0], "./res/bullet1.jpg");
	loadimage(&image.Cute[1], "./res/bullet2.jpg");
	//LoadEnemy
    loadimage(&image.Enemy[0], "./res/enemy_1.jpg");
	loadimage(&image.Enemy[1], "./res/enemy_2.jpg");
	loadimage(&image.Enemy[2], "./res/enemyPlane1.jpg");
	loadimage(&image.Enemy[3], "./res/enemyPlane2.jpg");;
	//初始化SendBigStars
	SendBigStars.x = Max_length / 2 - 20;
	SendBigStars.y = Max_width - 120;
	SendBigStars.hp = 999;
	SendBigStars.un.score = 0;
	SendBigStars.exists = true;
	//初始化Cute
	for (int i = 0; i < Max_quantity; i++) {
		Cute[i].exists = false;
	}
	//初始化Enemy
	for (int i = 0; i < Max_quantity; i++) {
		Enemy[i].exists = false;
		EnemyHp(i);
	}
}

void GameDraw() {

	//DrawBackGround
	putimage(0, 0, &image.BackGround);
	//DrawSendBigStars
	putimage(SendBigStars.x, SendBigStars.y, &image.SendBigStars[0], NOTSRCERASE);
	putimage(SendBigStars.x, SendBigStars.y, &image.SendBigStars[1], SRCINVERT);
	//DrawCute
	for (int i = 0; i < Max_quantity; i++) {
		if (Cute[i].exists == true) {
			putimage(Cute[i].x, Cute[i].y, &image.Cute[0], NOTSRCERASE);
			putimage(Cute[i].x, Cute[i].y, &image.Cute[1], SRCINVERT);
		}
	}
	//DrawEnemy
	for (int i = 0; i < Max_quantity; i++) {
		if (Enemy[i].exists == true) {
			if (Enemy[i].un.type == type::SMALL) {
                putimage(Enemy[i].x, Enemy[i].y, &image.Enemy[0], NOTSRCERASE);
	    	    putimage(Enemy[i].x, Enemy[i].y, &image.Enemy[1], SRCINVERT);
			}
			else {
				putimage(Enemy[i].x, Enemy[i].y, &image.Enemy[2], NOTSRCERASE);
		        putimage(Enemy[i].x, Enemy[i].y, &image.Enemy[3], SRCINVERT);
			}
		}
	}
}

void GameMove(int speed) {
	
	//SendBigStars移动
	if (GetAsyncKeyState(VK_UP) && SendBigStars.y >= 0) {
		SendBigStars.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) && SendBigStars.y <= Max_width - 120) {
		SendBigStars.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) && SendBigStars.x >= -50) {
		SendBigStars.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && SendBigStars.x <= Max_length - 55) {
		SendBigStars.x += speed;
	}
	if (GetAsyncKeyState(VK_SPACE)&& T2 - T1 > 200) {

		//FirCute
		ProduceCute();
		//Music
		mciSendString("close cute", 0, 0, 0);
		mciSendString("open ./res/f_gun.mp3 alias cute", 0, 0, 0);
		mciSendString("play cute", 0, 0, 0);
		T1 = T2;
	}
	T2 = GetTickCount();
	if (T4 - T3 > rand() % 500 + 500) {
		ProduceEnemy();
		T3 = T4; 
	}
	T4 = GetTickCount();
	EnemyMove(1);
	CuteMove(2);
}

int main() {
	
	initgraph(Max_length,Max_width);
	GameInit();

	BeginBatchDraw();
	while (1) {
		
		
		GameDraw();
		FlushBatchDraw();
		GameMove(1);
		Play();
	}

	EndBatchDraw();
	getchar();
	return 0;
}