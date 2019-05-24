#include <iostream>
#include <windows.h>
#include <sstream>

using namespace std;

struct Rect
{
	public:
		int width, height, area;
	
	Rect() : width(0), height(0), area(0) {}
	
	int computeArea()//Might want to change this to a return only and delete var area
	{
		this->area = this->width * this->height;
		return this->area; 
	}
};

class World
{
	private:
		int x, y;
		
	public:
		char *worldSpace;
		char *entityLayer;
		char *uiLayer;
		Rect size;
		
	World() : worldSpace(NULL), entityLayer(NULL), uiLayer(NULL){}
	
	~World()
	{ delete worldSpace; }
	
	void setWorldSpaceSize(int width, int height)
	{
		this->worldSpace = new char[width * height];
		this->entityLayer = new char[width * height];
		this->uiLayer = new char[width * height];
		
		this->size.height = height;
		this->size.width = width;
	}
	
	void clearWorldSpace()
	{
		for(x = 0; x < this->size.width; x++)
		{
			for(y = 0; y < this->size.height; y++)
			{
				this->worldSpace[y * this->size.width + x] = '+'; 
				this->entityLayer[y * this->size.width + x] = '#';
				this->uiLayer[y * this->size.width + x] = '#';
			}
		}
	}
};

class Component
{
	public:
		int x, y, z;
		COORD offSet;
		COORD pos;
		Rect size;
	
	Component()
	{
		this->offSet.X = 0;
		this->offSet.Y = 0;
		this->pos.X = 0;
		this->pos.Y = 0;
		this->size.width = 0;
		this->size.height = 0;
	}
	
	void setOffSet(int xCoord, int yCoord)
	{
		this->offSet.X = xCoord;
		this->offSet.Y = yCoord;
	}
	
	void setPosition(int xCoord, int yCoord)
	{
		this->pos.X = xCoord;
		this->pos.Y = yCoord;
	}
	
	void setSize(int width, int height)
	{
		this->size.width = width;
		this->size.height = height;
	}
};

class UI : public Component
{
	private:
		char *uiTexture;
		
	public:
		bool isVisible;
		
	UI() : isVisible(false) {}
	
	void generateDefaultTexture(int width, int height)
	{
		bool odd = false;
		this->uiTexture = new char[width * height];
		
		this->size.width = width;
		this->size.height = height;
		
		for(y = 0; y < height; y++)
		{
			for(x = 0; x < width; x++)
			this->uiTexture[y * width + x] = (x + odd) % 2 == 0 ? 'X' : 'O';
			
			odd = !odd;
		}
	}
	
	
	/*
	void setUITexture(string UI, int width, int height)
	{
		int x, y;
		
		uiLayer = new char[width * height];
		
		for(x = 0; x < width; x++)
		{
			for(y = 0; y < height; y++)
			uiLayer[y * width + x] = UI[y * width + x];//I think I can just copy the UI to uiLayer
		}
		---OR---
		//uiLayer = string sometHing maybe
	}
	
	void getData(//to update something)
	{
		
	}
	
	*/
};

struct AnimationFrame
{
	private:
		Rect size;
		
	public:
		string *frame;
		AnimationFrame *nextFrame;
		
	~AnimationFrame()
	{
		delete frame;	
		delete nextFrame; 
	}
	
	void setFrame(int width, int height, string *frame)
	{
		this->frame = frame;
		this->size.width = width;
		this->size.height = height;
	}
};

class Sprite : public Component
{
	public:
		AnimationFrame *start;
		string *frames;
		bool runFrames;
		
	Sprite() : start(NULL), frames(NULL), runFrames(false) {}
	
	void getFrames(int width, int height, string *frames, int numFrames)
	{
		this->size.width = width;
		this->size.height = height;
		
		AnimationFrame *temp1, *temp2;
		
		int i = 0;
		
		for(i; i < numFrames; i++)
		{
			temp1 = new AnimationFrame;
			
			temp1->setFrame(this->size.width, this->size.height, frames++);
			temp1->nextFrame = NULL;
			
			if(start == NULL)
			{
				start = temp1;
				temp2 = temp1;
			}
			
			else
			{
				temp2->nextFrame = temp1;
				temp2 = temp1;
			}
		}
	}
	
	void moveToWorldSpace(World &w)
	{ 
		if(this->start != NULL && w.worldSpace != NULL)
		{
			for(x = 0; x < this->size.width; x++)//Width
			{
				for(y = 0; y < this->size.height; y++)//Height
				w.worldSpace[(y + this->pos.Y) * w.size.width + (x + this->pos.X)] = start->frame[y * this->size.width + x][0];//Counter height(to increase y location) * Screen width + counter width(to increase x location)
			}
			
			start = start->nextFrame;
		}
	}
};

class Entity : public Component
{
	public:
		char *texture;
		int ammo;
		int health;
		World *world;
	
	void clearTexture()
	{
		for(x = 0; x < this->size.width; x++)
		{
			for(y = 0; y < this->size.height; y++)
			world->entityLayer[(y + this->pos.Y) * world->size.width + (x + this->pos.X)] = '#';
		}
	}
	
	void updateTexture()
	{
		for(x = 0; x < this->size.width; x++)
		{
			for(y = 0; y < this->size.height; y++)
			world->entityLayer[(y + this->pos.Y) * world->size.width + (x + this->pos.X)] = texture[y * this->size.width + x];
		}
	}
	
	void moveUp()
	{
		if(this->pos.Y > 0)
		{
			clearTexture();
			this->pos.Y -= 1;
			updateTexture();
		}
	}
	
	void moveDown()
	{
		if(this->pos.Y < world->size.height - this->size.height)
		{
			clearTexture();
			this->pos.Y += 1;
			updateTexture();
		}
	}
	
	void moveLeft()
	{
		if(this->pos.X > 0)
		{
			clearTexture();
			this->pos.X -= 1;
			updateTexture();
		}
	}
	
	void moveRight()
	{
		if(this->pos.X < world->size.width - this->size.width)
		{
			clearTexture();
			this->pos.X += 1;
			updateTexture();
		}
	}
};

class Map : public Component
{
	private:
		char *map;
		
	public:
	
	~Map()
	{ delete this->map; }
	
	void getMap(int width, int height, char *map)
	{
		this->map = map;
		this->size.width = width;
		this->size.height = height;
	}
	
	void generateDefaultTexture(int width, int height)
	{
		bool odd = false;
		this->map = new char[width * height];
		
		this->size.width = width;
		this->size.height = height;
		
		for(y = 0; y < height; y++)
		{
			for(x = 0; x < width; x++)
			this->map[y * width + x] = (x + odd) % 2 == 0 ? 'X' : 'O';
			
			odd = !odd;
		}
	}
	
	void moveToWorldSpace(World &w)
	{ 
		if(this->map != NULL && w.worldSpace != NULL)
		{
			for(x = 0; x < this->size.width; x++)//Width
			{
				for(y = 0; y < this->size.height; y++)//Height
				w.worldSpace[(y + this->pos.Y) * w.size.width + (x + this->pos.X)] = this->map[y * this->size.width + x];//Counter height(to increase y location) * Screen width + counter width(to increase x location)
			}
		}
	}
};

class Camera : public Component
{
	private:
		Entity *target;
		
	public:
		World *world;
		char *cmrLayer;

	Camera(World *wrld) : world(wrld), cmrLayer(NULL), target(NULL){}
	
	Camera() : world(NULL), cmrLayer(NULL), target(NULL){}
	
	~Camera()
	{
		delete this->world;
		delete this->target;
		delete this->cmrLayer;
	}
	
	void setUpCamera(int width, int height)
	{
		this->size.width = width;
		this->size.height = height;
		
		this->cmrLayer = new char[width * height];
	}
	
	void follow(Entity &e)
	{ target = &e; }
	
	void getCameraFeed()
	{
		if(this->cmrLayer != NULL)
		{
			if(target != NULL)
			{
				if(((this->pos.X + this->size.width / 2) + target->pos.X) / 2 > this->size.width / 2 && ((this->pos.X + this->size.width / 2) + target->pos.X) / 2 < world->size.width - this->size.width / 2 - 1)
				this->pos.X = target->pos.X - this->size.width / 2;
				
				if(((this->pos.Y + this->size.height / 2) + target->pos.Y) / 2 > this->size.height / 2 - 1 && ((this->pos.Y + this->size.height / 2) + target->pos.Y) / 2 < world->size.height - this->size.height / 2 - 1)
				this->pos.Y = target->pos.Y - this->size.height / 2;
			}
			
			for(x = 0; x < this->size.width; x++)
			{
				for(y = 0; y < this->size.height; y++)
				{
					if(world->entityLayer[(y + this->pos.Y) * world->size.width + (x + this->pos.X)] == '#')
					this->cmrLayer[y * this->size.width + x] = world->worldSpace[(y + this->pos.Y) * world->size.width + (x + this->pos.X)];
					
					else
					this->cmrLayer[y * this->size.width + x] = world->entityLayer[(y + this->pos.Y) * world->size.width + (x + this->pos.X)];
				}
			}
		}
	}

	void moveUp()
	{
		if(this->pos.Y > 0)
		this->pos.Y -= 1;
	}
	
	void moveDown()
	{
		if(this->pos.Y < world->size.height - this->size.height)
		this->pos.Y += 1;
	}
	
	void moveLeft()
	{
		if(this->pos.X > 0)
		this->pos.X -= 1;
	}
	
	void moveRight()
	{
		if(this->pos.X < world->size.width - this->size.width)
		this->pos.X += 1;
	}
};

class Renderer
{
	private:
		int x, y;
		Rect size;
		
	public:
		char *matrix;
		Camera *camera;
		UI *ui;
		
	Renderer(Camera *cam) : camera(cam)
	{
		this->matrix = NULL;
		this->ui = NULL;
		
		this->size.width = 0;
		this->size.height = 0;
	}
	
	Renderer()
	{
		this->camera = NULL;
		this->matrix = NULL;
		this->ui = NULL;
		
		this->size.width = 0;
		this->size.height = 0;
	}
	
	~Renderer()
	{ 
		delete this->matrix;
		delete this->camera;
		delete this->ui;
	}
	
	void clearMatrix()
	{
		for(x = 0; x < this->size.width; x++)
		{
			for(y = 0; y < this->size.height; y++)
			this->matrix[y * this->size.width + x] = ' ';
		}
	}
	
	void setScreenDimension(int width, int height)
	{
		this->size.width = width;
		this->size.height = height;
		
		this->matrix = new char[width * height];
	}
	
	void loadCamera()
	{
		for(x = 0; x < camera->size.width; x++)
		{
			for(y = 0; y < camera->size.height; y++)
			this->matrix[(y + camera->offSet.Y) * this->size.width + (x + camera->offSet.X)] = camera->cmrLayer[y * camera->size.width + x];
		}
	}
	
	void loadUI()
	{
		
	}
	
	void update()
	{
		camera->getCameraFeed();
		loadCamera();
		
		/*
		if(ui->isVisible != false && ui != NULL)
		{
			
		}*/
	}
};

string to_string(int num)
{
	stringstream ss;
	ss << num;
	
	return (ss.str());
}
