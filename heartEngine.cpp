#include <iostream>
#include <windows.h>

#include "heartengineALPHA.h"

using namespace std;
using namespace shapes;

int	nScreenWidth = 120;
int	nScreenHeight = 40;

class Player : public Entity
{
	public:
		
	Player()
	{
		this->size.width = 5;
		this->size.height = 5;
		
		texture = new char[this->size.width * this->size.height];
		
		string playerTexture;
		
		playerTexture += "##O##";
		playerTexture += "#FBI#";
		playerTexture += "#| |#";
		playerTexture += "#|||#";
		playerTexture += "#[#]#";
		
		for(x = 0; x < this->size.width; x++)
		{
			for(y = 0; y < this->size.height; y++)
			texture[y * this->size.width + x] = playerTexture[y * this->size.width + x];
		}
	}
};

class LevelManager
{
	private:
		HANDLE hConsole;
		DWORD dwBytesWritten;
		COORD center;
		World world;
		Camera camera;
		Renderer renderer;
			
	public:
		int level;
		
	LevelManager() : level(0)
	{
		hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		dwBytesWritten = 0;
		
		center.X = 0;
		center.Y = 0;
		
		camera.world = &world;
		renderer.camera = &camera;
		
		world.setWorldSpaceSize(500, 500);
		world.clearWorldSpace();
		
		renderer.setScreenDimension(nScreenWidth, nScreenHeight);
		renderer.clearMatrix();
	}
	
	void levelTest()
	{
		SetConsoleActiveScreenBuffer(hConsole);
		Player p1;
		Map battleMap;
		
		string frames[5];
		frames[0] = "1";
		frames[1] = "2";
		frames[2] = "3";
		frames[3] = "4";
		frames[4] = "5";
		
		camera.setUpCamera(120, 25);		
		
		p1.world = &world;
		p1.setPosition(20, 10);
		
		camera.follow(p1);
		
		battleMap.generateDefaultTexture(30, 10);
		battleMap.setPosition(5, 5);
		battleMap.moveToWorldSpace(world);
		
		Sprite s;
		
		s.getFrames(1, 1, frames, 5);
		
		while(true)
		{
			Sleep(50);
			
			s.moveToWorldSpace(world);
			
			if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			p1.moveLeft();
			
			if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			p1.moveRight();
			
			if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
			p1.moveUp();
			
			if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
			p1.moveDown();
			
			renderer.update();
			WriteConsoleOutputCharacterA(hConsole, renderer.matrix, nScreenWidth * nScreenHeight, center, &dwBytesWritten);
		}
	}
};

main()
{
	LevelManager test;
	test.levelTest();
}
