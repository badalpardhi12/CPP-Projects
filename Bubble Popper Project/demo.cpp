#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <chrono>
#include <iostream>
#include <math.h>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "fssimplewindow.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include "yspngenc.h"

// Initializing variables
const int W = 1024, H = 768;
const double pi = 3.1415926535;
const int num = 50;
const double size = 15.0f;
const double dt = 0.1;
const double tolerance = 1.0e-06;

class Bubble
{
public:
	int pops;
	int left; 
	int iter;
	int counter;
	int t;
	int luckState;
	int bubbleState[num];
	double popperX, popperY;
	double R, G, B;
	double popperVelX, popperVelY;
	double luck;
	double frameX, frameY;
	double bubbleX[num], bubbleY[num];
	double bubbleVelX[num], bubbleVelY[num];
	char* PvelX;
	char* PvelY;
	char* numBubbles;
	char* time;
	char* luckStr;
	YsSoundPlayer player;
	YsSoundPlayer::SoundData intro;
	YsSoundPlayer::SoundData pop;
	YsSoundPlayer::SoundData bg;
	YsSoundPlayer::SoundData collision;
	YsSoundPlayer::SoundData luck50;
	YsSoundPlayer::SoundData luck60;
	YsSoundPlayer::SoundData luck70;
	YsSoundPlayer::SoundData luck80;
	YsSoundPlayer::SoundData luck90;
	YsSoundPlayer::SoundData gamewon;
	YsSoundPlayer::SoundData context;
	YsRawPngDecoder png;
public:
	Bubble();
	~Bubble();
	void cleanUp();
	void setBubble(void);
	int drawFrame(void);
	void drawPopper(void);
	int collisionCheck(double x1, double y1, double x2, double y2, 
						double v1x, double v1y, double v2x, double v2y) const;
	void bounce(double x1, double y1, double x2, double y2,
				double &v1x, double &v1y, double &v2x, double &v2y);
	//void explosion(double x0, double y0);
	void movement(void);
	void displayTitle(void) const;
	void displayVel(void);
	void displayBubblesLeft(void);
	void displayTime(int t);
	void drawBubble(void);
	void displayinstructions(void) const;
	void gameEnd();
	void ending1();
	void ending2() const;
};

Bubble::Bubble()
{
	for (int i = 0; i < num; ++i)
	{
		bubbleState[i] = 0;
		bubbleX[i] = 0;
		bubbleY[i] = 0;
		bubbleVelX[i] = 0;
		bubbleVelY[i] = 0;
	}
	t = 0;
	R = 0.0f;
	G = 0.0f;
	B = 0.0f;
	frameX = W;
	frameY = H;
	popperVelX = 0.0;
	popperVelY = 0.0;
	popperX = 0.0;
	popperY = 0.0;
	luck = 0.0;
	pops = 0;
	left = 0;
	iter = 0;
	counter = 0;
	luckState = 0;
	time = 0;
	PvelX = nullptr;
	PvelY = nullptr;
	numBubbles = nullptr;
	time = nullptr;
	luckStr = nullptr;

	FsOpenWindow(16, 16, W, H, 1);
	player.Start();

	FsChangeToProgramDir();

	// Loading Audio
	char intr[10] = "intro.wav";
	char popsound[15] = "bubble_pop.wav";
	char background[15] = "background.wav";
	char collide[12] = "collide.wav";
	char L50[14] = "50percent.wav";
	char L60[14] = "60percent.wav";
	char L70[14] = "70percent.wav";
	char L80[14] = "80percent.wav";
	char L90[14] = "90percent.wav";
	char GW[13] = "game_won.wav";
	char cont[12] = "context.wav";

	if (YSOK != intro.LoadWav(intr))
	{
		printf("Error -> Cannot load intro.wav!\n");
	}
	if (YSOK != pop.LoadWav(popsound))
	{
		printf("Error -> Cannot load bubble_pop.wav!\n");
	}
	if (YSOK != bg.LoadWav(background))
	{
		printf("Error -> Cannot load background.wav!\n");
	}
	if (YSOK != collision.LoadWav(collide))
	{
		printf("Error -> Cannot load collide.wav!\n");
	}
	if (YSOK != luck50.LoadWav(L50))
	{
		printf("Error -> Cannot load 50percent.wav!\n");
	}
	if (YSOK != luck60.LoadWav(L60))
	{
		printf("Error -> Cannot load 60percent.wav!\n");
	}
	if (YSOK != luck70.LoadWav(L70))
	{
		printf("Error -> Cannot load 70percent.wav!\n");
	}
	if (YSOK != luck80.LoadWav(L80))
	{
		printf("Error -> Cannot load 80percent.wav!\n");
	}
	if (YSOK != luck90.LoadWav(L90))
	{
		printf("Error -> Cannot load 90percent.wav!\n");
	}
	if (YSOK != gamewon.LoadWav(GW))
	{
		printf("Error -> Cannot load game_won.wav!\n");
	}
	if (YSOK != context.LoadWav(cont))
	{
		printf("Error -> Cannot load context.wav!\n");
	}

	// Loading Images
	if (YSOK == png.Decode("background.png"))
	{
		png.Flip();
		printf("%d x %d \n", png.wid, png.hei);
	}
	else
	{
		printf("Could not read the image. \n");
	}
}

Bubble::~Bubble()
{
	cleanUp();
}

void Bubble::cleanUp()
{
	popperVelX = 0.0;
	popperVelY = 0.0;
	popperX = 0.0;
	popperY = 0.0;
	luck = 0.0;
	pops = 0;
	left = 0;
	iter = 0;
	counter = 0;
	luckState = 0;
	time = 0;
	R = 0.0f;
	G = 0.0f;
	B = 0.0f;

	delete[] PvelX;
	delete[] PvelY;
	delete[] numBubbles;
	delete[] time;
	delete[] luckStr;
	PvelX = nullptr;
	PvelY = nullptr;
	numBubbles = nullptr;
	time = nullptr;
	luckStr = nullptr;
	player.End();
}

void Bubble::setBubble(void)
{
	for (int i = 0; i < num; ++i)
	{
		bubbleX[i] = size + (double)(rand() % (W-(int)(2*size)));
		bubbleY[i] = size + (double)(rand() % (H-(int)(2*size)));
		bubbleVelX[i] = (double)(rand() % 10 - 5.0);
		bubbleVelY[i] = (double)(rand() % 10 - 5.0);
		bubbleState[i] = 1;
	}
	
	popperX = W/2;
	popperY = H/2;
	popperVelX = 2.0 + (double)(rand() % 20 - 10.0);
	popperVelY = 2.0 + (double)(rand() % 20 - 10.0);
}

int Bubble::drawFrame(void)
{

	if (iter <= W)
	{
		player.PlayOneShot(intro);
		for (iter = 0; iter < W; iter+=7)
		{
			FsPollDevice();
			auto key = FsInkey();
			if (FSKEY_ESC == key)
			{
				return 1;
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glRasterPos2i(W / 2 - iter / 2, 767);
			glDrawPixels(iter, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);

			// Title Display
			glColor3f(0.0, 0.0, 0.0);
			glRasterPos2i(220, H / 2);
			YsGlDrawFontBitmap28x44("Welcome to Bubble Pop!");

			glColor3f(1.0, 0.0, 0.0);
			glRasterPos2i(330, H / 2 + 50);
			YsGlDrawFontBitmap28x44("Test Your Luck");

			FsSleep(1);
			FsSwapBuffers();
		}
		return 0;
	}
	else
	{
		glRasterPos2i(0, 767);
		glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
		return 0;
	}
}

void Bubble::drawPopper(void) 
{
	glLineWidth(5);
	glBegin(GL_POLYGON);
	counter++;
	if (counter % 1000 == 0)
	{
		R = (double)rand() / (double)RAND_MAX;
		G = (double)rand() / (double)RAND_MAX;
		B = (double)rand() / (double)RAND_MAX;
	}
	glColor3f(R, G, B);
	if (popperVelX > 0 && popperVelY > 0)
	{
		glVertex2f(popperX, popperY + 10);
		glVertex2f(popperX + 20, popperY + 20);
		glVertex2f(popperX + 10, popperY);
		glVertex2f(popperX + 10, popperY - 10);
		glVertex2f(popperX - 10, popperY - 10);
		glVertex2f(popperX - 10, popperY + 10);
		glVertex2f(popperX, popperY + 10);
	}
	else if (popperVelX > 0 && popperVelY < 0)
	{
		glVertex2f(popperX + 10, popperY);
		glVertex2f(popperX + 20, popperY - 20);
		glVertex2f(popperX, popperY - 10);
		glVertex2f(popperX - 10, popperY - 10);
		glVertex2f(popperX - 10, popperY + 10);
		glVertex2f(popperX + 10, popperY + 10);
		glVertex2f(popperX + 10, popperY);
	}
	else if (popperVelX < 0 && popperVelY > 0)
	{
		glVertex2f(popperX - 10, popperY);
		glVertex2f(popperX - 20, popperY + 20);
		glVertex2f(popperX, popperY + 10);
		glVertex2f(popperX + 10, popperY + 10);
		glVertex2f(popperX + 10, popperY - 10);
		glVertex2f(popperX - 10, popperY - 10);
		glVertex2f(popperX - 10, popperY);
	}
	else if (popperVelX < 0 && popperVelY < 0)
	{
		glVertex2f(popperX - 10, popperY);
		glVertex2f(popperX - 20, popperY - 20);
		glVertex2f(popperX, popperY - 10);
		glVertex2f(popperX + 10, popperY - 10);
		glVertex2f(popperX + 10, popperY + 10);
		glVertex2f(popperX - 10, popperY + 10);
		glVertex2f(popperX - 10, popperY);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 64; ++i)
	{
		double a = (double)i * pi / 32.0;
		double c = cos(a);
		double s = sin(a);
		glVertex2d(popperX + c * (size - 3.0), popperY + s * (size - 3.0));
	}
	glEnd();
}


int Bubble::collisionCheck(double x1, double y1, double x2, double y2,
							double v1x, double v1y, double v2x, double v2y) const
{
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dist = sqrt(dx * dx + dy * dy);
	if (dist < (20.0f))
	{
		double rvx = v2x - v1x;
		double rvy = v2y - v1y;
		if (dx * rvx + dy * rvy < 0.0)
		{
			return 1;
		}
	}
	return 0;
}

void Bubble::bounce(double x1, double y1, double x2, double y2,
					double &v1x, double &v1y, double &v2x, double &v2y)
{
	// Momentum conservation principle

	double nx, ny;
	nx = x2 - x1;
	ny = y2 - y1;

	double n = sqrt(nx * nx + ny * ny);
	if (n < tolerance)
	{
		return;
	}
	nx /= n;
	ny /= n;

	double k1 = v1x * nx + v1y * ny;
	double k2 = v2x * nx + v2y * ny;

	v1x = v1x + nx * (k2 - k1);
	v1y = v1y + ny * (k2 - k1);

	v2x = v2x + nx * (k1 - k2);
	v2y = v2y + ny * (k1 - k2);
}

//void Bubble::explosion(double x0, double y0)
//{
//	int pVx[40];
//	int pVy[40];
//	int px[40];
//	int py[40];
//	int count = 0;
//	for (int i = 0; i < 40; ++i)
//	{
//		pVx[i] = (rand() % 40) - 20;
//		pVy[i] = (rand() % 40) - 20;
//		px[i] = (int)x0;
//		py[i] = (int)y0;
//		if (0 == pVx[i])
//		{
//			++pVx[i];
//		}
//		if (0 == pVy[i])
//		{
//			++pVy[i];
//		}
//	}
//	if (count < 40)
//	{
//		for (int i = 0; i < 40; ++i)
//		{
//			px[i] += pVx[i];
//			py[i] += pVy[i];
//			glPointSize(3);
//			glBegin(GL_POINTS);
//			glColor3f(0.0, 0.0, 0.0);
//			glVertex2i(px[i], py[i]);
//			glEnd();
//		}
//		++count;
//	}
//}

void Bubble::movement(void)
{
	pops = 0;
	for (int i = 0; i < num; ++i)
	{
		if (bubbleState[i] == 0)
		{
			pops += 1;
		}
	}
	for (int i = 0; i < num; ++i)
	{
		// edge bounce bubble
		if (bubbleX[i] > W / 2 + frameX / 2 - size || bubbleX[i] < W / 2 - frameX / 2 + size)
		{
			bubbleVelX[i] *= -1;
		}
		if (bubbleY[i] > H / 2 + frameY / 2 - size || bubbleY[i] < H / 2 - frameY / 2 + size)
		{
			bubbleVelY[i] *= -1;
		}
	}

	// edge bounce popper
	if (popperX > W / 2 + frameX / 2 - size || popperX < W / 2 - frameX / 2 + size)
	{
		player.PlayOneShot(collision);
		popperVelX *= -1;
	}
	if (popperY > H / 2 + frameY / 2 - size || popperY < H / 2 - frameY / 2 + size)
	{
		player.PlayOneShot(collision);
		popperVelY *= -1;
	}

	popperX += popperVelX;
	popperY += popperVelY;

	if (popperVelX == 0)
	{
		popperVelX = 1.0f;
	}
	if (popperVelY == 0)
	{
		popperVelY = 1.0f;
	}

	for (int i = 0; i < num; ++i)
	{
		bubbleX[i] += bubbleVelX[i];
		bubbleY[i] += bubbleVelY[i];

		if (bubbleVelX[i] == 0)
		{
			bubbleVelX[i] = 1.0f;
		}
		if (bubbleVelY[i] == 0)
		{
			bubbleVelY[i] = 1.0f;
		}
		
	}
	// collision
	for (int i = 0; i < num; ++i)
	{
		for (int j = i + 1; j < num; ++j)
		{
			if (collisionCheck(bubbleX[i], bubbleY[i], bubbleX[j], bubbleY[j],
				bubbleVelX[i], bubbleVelY[i], bubbleVelX[j], bubbleVelY[j]) == 1 && bubbleState[i] == 1 && bubbleState[j] == 1)
			{
				bounce(bubbleX[i], bubbleY[i], bubbleX[j], bubbleY[j], 
					bubbleVelX[i], bubbleVelY[i], bubbleVelX[j], bubbleVelY[j]);
			}
		}
		if (collisionCheck(bubbleX[i], bubbleY[i], popperX, popperY,
			bubbleVelX[i], bubbleVelY[i], popperVelX, popperVelY) == 1 && bubbleState [i] == 1)
		{
			bounce(bubbleX[i], bubbleY[i], popperX, popperY,
				bubbleVelX[i], bubbleVelY[i], popperVelX, popperVelY);
			if (popperVelX < 50.0 && popperVelY < 50)
			{
				popperVelX *= 1.2;
				popperVelY *= 1.2;
			}
			player.PlayOneShot(collision);

		}
		if (popperVelX > 0 && popperVelY > 0 && 
			collisionCheck(bubbleX[i], bubbleY[i], popperX + 20, popperY + 20,
			bubbleVelX[i], bubbleVelY[i], popperVelX, popperVelY) == 1 && bubbleState[i] == 1)
		{
			bubbleState[i] = 0;
			//explosion(bubbleX[i], bubbleY[i]);player.Start();
			player.PlayOneShot(pop);
		}
		else if (popperVelX > 0 && popperVelY < 0 &&
				collisionCheck(bubbleX[i], bubbleY[i], popperX + 20, popperY - 20,
				bubbleVelX[i], bubbleVelY[i], popperVelX, popperVelY) == 1 && bubbleState[i] == 1)
		{
			bubbleState[i] = 0;
			//explosion(bubbleX[i], bubbleY[i]);
			player.PlayOneShot(pop);
		}
		else if (popperVelX < 0 && popperVelY < 0 &&
				collisionCheck(bubbleX[i], bubbleY[i], popperX - 20, popperY - 20,
				bubbleVelX[i], bubbleVelY[i], popperVelX, popperVelY) == 1 && bubbleState[i] == 1)
		{
			bubbleState[i] = 0;
			//explosion(bubbleX[i], bubbleY[i]);
			player.PlayOneShot(pop);
		}
		else if (popperVelX < 0 && popperVelY > 0 &&
				collisionCheck(bubbleX[i], bubbleY[i], popperX - 20, popperY + 20,
				bubbleVelX[i], bubbleVelY[i], popperVelX, popperVelY) == 1 && bubbleState[i] == 1)
		{
			bubbleState[i] = 0;
			//explosion(bubbleX[i], bubbleY[i]);
			player.PlayOneShot(pop);
		}
	}
	
}

void Bubble::displayTitle(void) const
{
	// Title Display
	glColor3ub(0, 0, 0);
	glRasterPos2i(420, 20);
	YsGlDrawFontBitmap16x20("Bubble Pop");
}

void Bubble::displayVel()
{
	// Velocity Display
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.8, 0.9);
	glVertex2f(335, 710);
	glVertex2f(335, 750);
	glVertex2f(648, 750);
	glVertex2f(648, 710);
	glEnd();

	std::string strVx = std::to_string(popperVelX);
	std::string strVy = std::to_string(popperVelY);
	PvelX = new char[strVx.size() + 1];
	std::copy(strVx.begin(), strVx.end(), PvelX);
	PvelX[strVx.size() - 3] = '\0';
	PvelY = new char[strVy.size() + 1];
	std::copy(strVy.begin(), strVy.end(), PvelY);
	PvelY[strVy.size() - 3] = '\0';

	glColor3ub(0, 0, 0);
	glRasterPos2i(350, 727);
	YsGlDrawFontBitmap10x14("Popper Velocity in X: ");
	glRasterPos2i(580, 727);
	YsGlDrawFontBitmap10x14(PvelX);
	glRasterPos2i(350, 748);
	YsGlDrawFontBitmap10x14("Popper Velocity in Y: ");
	glRasterPos2i(580, 748);
	YsGlDrawFontBitmap10x14(PvelY);
}

void Bubble::displayBubblesLeft()
{
	// Bubbles Left Display
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.8, 0.9);
	glVertex2f(5, 5);
	glVertex2f(180, 5);
	glVertex2f(180, 22);
	glVertex2f(5, 22);
	glEnd();

	glColor3ub(0, 0, 0);
	glRasterPos2i(10, 20);
	YsGlDrawFontBitmap10x14("Bubbles Left: ");
	std::string numB = std::to_string(num - pops);
	numBubbles = new char[numB.size() + 1];
	std::copy(numB.begin(), numB.end(), numBubbles);
	numBubbles[numB.size()] = '\0';
	glRasterPos2i(160, 20);
	YsGlDrawFontBitmap10x14(numBubbles);
}

void Bubble::displayTime(int t2)
{
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.8, 0.9);
	glVertex2f(W-152, 5);
	glVertex2f(W-18, 5);
	glVertex2f(W-18, 22);
	glVertex2f(W-152, 22);
	glEnd();

	glColor3ub(0, 0, 0);
	glRasterPos2i(W-150, 20);
	YsGlDrawFontBitmap10x14("Time Left: ");
	std::string T = std::to_string(104 - t2);
	time = new char[T.size() + 1];
	std::copy(T.begin(), T.end(), time);
	time[T.size()] = '\0';
	glRasterPos2i(W-40, 20);
	YsGlDrawFontBitmap10x14(time);
}

void Bubble::drawBubble(void)
{
	left = num - pops;
	luck = (1 - ((double)(left) / num)) * 100.0;

	if (counter % 10 == 0)
	{
		R = (double)rand() / (double)RAND_MAX + 0.3;
		G = (double)rand() / (double)RAND_MAX + 0.3;
		B = (double)rand() / (double)RAND_MAX + 0.3;
	}

	player.PlayBackground(bg);

	for (int j = 0; j < num; ++j)
	{
		if (bubbleState[j] == 1)
		{
			glLineWidth(1);
			glBegin(GL_LINE_LOOP);
			glColor3f(R, G, B); 
			for (int k = 0; k < 3; ++k)
			{
				for (int i = 0; i < 64; ++i)
				{
					double a = (double)i * pi / 32.0;
					double c = cos(a);
					double s = sin(a);
					glVertex2d(bubbleX[j] + c * (size + (double)(k)), bubbleY[j] + s * (size + (double)(k)));
				}
			}
			
			glColor3f(0.6, 0.8, 0.9);
			for (int k = 1; k < 3; ++k)
			{
				for (int i = 0; i < 64; ++i)
				{
					double a = (double)i * pi / 32.0;
					double c = cos(a);
					double s = sin(a);
					glVertex2d(bubbleX[j] + c * (size - (double)(k)), bubbleY[j] + s * (size - (double)(k)));
				}
			}
			glEnd();
		}
	}

	if (luck < 55 && luck > 45 && t > 100)
	{
		player.PlayOneShot(luck50);
	}
	else if (luck < 65 && luck > 56 && t > 100)
	{
		player.PlayOneShot(luck60);
	}
	else if (luck < 75 && luck > 66 && t > 100)
	{
		player.PlayOneShot(luck70);
	}
	else if (luck < 85 && luck > 76 && t > 100)
	{
		player.PlayOneShot(luck80);
	}
	else if (luck < 100 && luck > 86 && t > 102)
	{
		player.PlayOneShot(luck90);
	}
	else if (luck == 100 )
	{
		player.PlayOneShot(gamewon);
	}
	if (t > 30 && t < 40)
	{
		player.PlayOneShot(context);
		displayinstructions();
	}

	player.KeepPlaying();

}

void Bubble::displayinstructions(void) const
{
	if (t > 30 && t < 50)
	{
		glColor3ub(255, 255, 0);
		glRasterPos2i(240, H / 2);
		YsGlDrawFontBitmap16x20("Based on the number of Bubbles left");
		glRasterPos2i(240, H / 2 + 30);
		YsGlDrawFontBitmap16x20("your goodluck % will be calculated.");
	}
}

void Bubble::gameEnd()
{
	counter++;
	if(counter%50 == 0)
	{
		R = (double)rand() / (double)RAND_MAX + 0.3;
		G = (double)rand() / (double)RAND_MAX + 0.3;
		B = (double)rand() / (double)RAND_MAX + 0.3;
	}

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(R, G, B);
	for (int i = 0; i < 64; ++i)
	{
		double a = (double)i * pi / 32.0;
		double c = cos(a);
		double s = sin(a);
		glVertex2d(W/2 + c * (double)(iter), H/2 + s * (double)(iter));
	}
	glEnd(); 
	
}

void Bubble::ending1()
{
	glColor3ub(0, 0, 0);
	glRasterPos2i(280, H / 2 - 30);
	YsGlDrawFontBitmap16x20("Time's Up! Total balls left: ");

	glRasterPos2i(760, H / 2 - 30);
	YsGlDrawFontBitmap16x20(numBubbles);

	glColor3ub(0, 0, 0);
	glRasterPos2i(250, H / 2 + 30);
	YsGlDrawFontBitmap16x20("You had ");

	glColor3ub(255, 0, 0);
	std::string L = std::to_string(luck);
	luckStr = new char[L.size() + 1];
	std::copy(L.begin(), L.end(), luckStr);
	luckStr[L.size() - 5] = '\0';
	glRasterPos2i(390, H / 2 + 30);
	YsGlDrawFontBitmap16x20(luckStr);

	glColor3ub(0, 0, 0);
	glRasterPos2i(450, H / 2 + 30);
	YsGlDrawFontBitmap16x20(" % of good luck today.");

}

void Bubble::ending2() const
{
	glColor3ub(0, 0, 0);
	glRasterPos2i(300, H / 2);
	YsGlDrawFontBitmap16x20("Congrats! You are in luck today.");
}

int main(void)
{
	srand(time(nullptr));

	Bubble B;
	int counter = 0; 
	int exit;
	time_t t1;
	time_t t2;
	t1 = (unsigned int)time(NULL);

	B.setBubble();

	for (;;)
	{
		auto key = FsInkey();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		FsPollDevice();
		if (FSKEY_ESC == key)
		{
			break;
		}

		exit = B.drawFrame();
		if (exit == 1)
		{
			return 0;
		}
		B.drawPopper();
		B.movement();
		B.drawBubble();
		B.displayTitle();
		B.displayVel();
		B.displayBubblesLeft();

		// Time Left
		t2 = int(difftime((unsigned int)time(NULL), t1));
		B.t = t2;
		B.displayTime(t2);

		if (t2 > 104 || B.pops == num)
		{
			for (B.iter = 0; B.iter < W+100; B.iter += 3)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				FsPollDevice();
				auto key = FsInkey();
				if (FSKEY_ESC == key)
				{
					break;
				}
				B.gameEnd();
				if (t2 > 104)
				{
					B.ending1();
				}
				else
				{
					B.ending2();
				}
				FsSleep(1);
				FsSwapBuffers();
			}
			break;
		}
		FsSleep(1);
		FsSwapBuffers();
	}

	return 0;
}
