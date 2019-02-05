#ifndef HEADER
#define HEADER

#include<iostream>
#include<windows.h>
#include<conio.h>
#include<mmsystem.h> 

extern void DrawMap();

class Item;
class Goal;
class Character;
class Player;
class Enemy;
class Stage;
class Object;
class Floor;
class Wall;
class Spike;


class Item
{
public:
	int x;
	int y;
	int HP;
	int speed;
	int jump;
	int exist;
	int **map;

	Item();
	Item(int** map, int x, int y, int HP, int speed, int jump);
	int Verification();
	void Disappear(Player* p);
};

class Goal : public Item
{


public:
	Goal();
	Goal(int** map, int x, int y, int HP, int speed, int jump);
	void Clear();
};


class Character
{


public:
	int HP;
	int atk;
	int speed;
	int direction;
	int leg;
	int jump;
	int jumping;
	int x;
	int y;
	int** map;

	void Attack();
	void Hit();
	void Back();
	void die();
	void Move(int a, int b);
	void Proceed();
	void Drop();
	void Initailize(int** map, int);
	void Collide();
};

class Player : public Character
{
public:
	void die();
	void GetItem(Item* item);
	void Jump();
	void CollideWall();
};

class Enemy : public Character
{
	void die();
	void recognize();
};

class Object
{
public:
	int exist;
	int destroy;
	int** map;

public:
	void Destroy();
	void Init(int** map, int destroy);

};

class Floor : public Object
{


public:
	Floor(int** map, int x, int y, int width, int height, int destroy);
};

class Wall : public Object
{

public:
	Wall(int** map, int x, int y, int width, int height, int destroy);

};

class Spike : public Object
{
	int x;
	int y;

public:
	Spike(int** map, int x, int y, int width, int height, int destroy);
	int Collision(int);

};


//Item

Item::Item()
{
}

Item::Item(int** map, int x, int y, int HP, int speed, int jump)
{
	int count;

	exist = 1;

	this->map = map;

	this->x = x;
	this->y = y;

	if (HP)
	{
		this->HP = HP;
		map[x + 3][y + 2] = 'H';
		map[x + 4][y + 2] = 'P';
	}

	else if (speed)
	{
		this->speed = speed;
		map[x + 1][y + 2] = 'S';
		map[x + 2][y + 2] = 'p';
		map[x + 3][y + 2] = 'e';
		map[x + 4][y + 2] = 'e';
		map[x + 5][y + 2] = 'd';
	}

	else if (jump)
	{
		this->jump = jump;
		map[x + 2][y + 2] = 'J';
		map[x + 3][y + 2] = 'u';
		map[x + 4][y + 2] = 'm';
		map[x + 5][y + 2] = 'p';
	}

	for (count = x + 1; count <= x + 6; count++)
	{
		map[count][y] = 35;
		map[count][y + 3] = '_';
	}

	for (count = y + 1; count <= y + 3; count++)
	{
		map[x][count] = 35;
		map[x + 7][count] = 35;
	}


}

int Item::Verification()
{
	int count;
	for (count = x + 1; count <= x + 6; count++)
		if (map[count][y] != 35)
			return 1;

	for (count = x + 1; count <= x + 6; count++)
		if (map[count][y + 3] != '_')
			return 1;

	for (count = y + 1; count <= y + 3; count++)
		if (map[x][count] != 35 || map[x + 7][count] != 35)
			return 1;

	return 0;
}

void Item::Disappear(Player* p)
{
	int count1, count2;
	if (exist)
	{
		p->GetItem(this);
		for (count1 = x; count1 <= x + 7; count1++)
			for (count2 = y; count2 <= y + 3; count2++)
				map[count1][count2] = 0;
		exist = 0;
	}
	else
		return;
}

//Goal
Goal::Goal()
{
}

Goal::Goal(int** map, int x, int y, int HP, int speed, int jump)
{

	int count;
	exist = 1;

	this->map = map;

	this->x = x;
	this->y = y;


	for (count = x + 1; count <= x + 6; count++)
	{
		map[count][y] = 35;
		map[count][y + 3] = '_';
	}

	for (count = y + 1; count <= y + 3; count++)
	{
		map[x][count] = 35;
		map[x + 7][count] = 35;
	}


	map[x + 2][y + 2] = 'G';
	map[x + 3][y + 2] = 'O';
	map[x + 4][y + 2] = 'A';
	map[x + 5][y + 2] = 'L';
}

void Goal::Clear()
{

}

//Character

void Character::Back()
{
	direction *= -1;
}

void Character::Move(int a, int b)
{
	int count1, count2;
	if (a <= 2 || a >= 88 || b <= 2 || b >= 38)
		return;

	for (count1 = a - 1; count1 <= a + 1; count1++)
		for (count2 = b; count2 <= b + 2; count2++)
			if (map[count1][count2] == '|')
			{
				Back();
				return;
			}


	for (count1 = a - 1; count1 <= a + 1; count1++)
		for (count2 = b; count2 <= b + 2; count2++)
			if (map[count1][count2] == '=')
				return;



	if (x != 0 && y != 0)
	{
		map[x][y] = 0;
		map[x - 1][y + 1] = map[x + 1][y + 1] = 0;
		map[x][y + 1] = 0;
		map[x - 1][y + 2] = map[x][y + 2] = map[x + 1][y + 2] = 0;
	}

	x = a;
	y = b;

	map[x][y] = '0';
	map[x - 1][y + 1] = map[x + 1][y + 1] = '/';
	map[x][y + 1] = '1';
	if (1)
	{
		map[x + 1][y + 2] = '/';
		if (leg == 1)
		{
			map[x - 1][y + 2] = '/';
			leg++;
		}
		else if (leg == 2)
		{
			map[x][y + 2] = '/';
			leg++;
		}
		else
		{
			map[x][y + 2] = '-';
			leg = 1;
		}
	}

	if (map[x - 1 - direction][y + 3] == 0 && map[x - direction][y + 3] == 0 && map[x + 1 - direction][y + 3] == 0)
		jumping = 1;

	else if (map[x - 1 - direction][y + 3] == '_' || map[x - direction][y + 3] == '_' || map[x + 1 - direction][y + 3] == '_')
		jumping = 1;

	else
		jumping = 0;
}

void Character::Proceed()
{
	Move(x + direction, y);
}

void Character::Drop()
{
	Move(x, y + 1);
	while (_kbhit())
		_getch();
}

void Character::Initailize(int** map, int index_stage)
{
	this->map = map;
	HP = 100;
	atk = 1;
	speed = 2;
	direction = 1;
	leg = 1;
	jump = 5;
	jumping = 0;

	switch (index_stage)
	{

		case 3:
			Move(4, 5);
			break;

		default:
			Move(5, 36);
			break;
	}

}

void Character::Collide()
{

}

//Player

void Player::GetItem(Item* item)
{
	if (item->HP)
		HP += item->HP;

	else if (item->speed)
		speed += item->speed;

	else if (item->jump)
		jump += item->jump;
}

void Player::Jump()
{
	int count;

	if (jumping)
		return;

	for (count = 1; count <= jump; count++)
	{

		Move(x, y - 1);
		//		CollideWall();
		Proceed();
		//		CollideWall();
		DrawMap();
		if (count == jump)
			Sleep(20);
		Sleep(10);
	}


	while (_kbhit())
		_getch();
}

void Player::CollideWall()
{
	if (map[x - 1][y] == '|' || map[x + 1][y] == '|' || map[x - 2][y] == '|' || map[x + 2][y] == '|' || map[x - 3][y] == '|' || map[x + 3][y] == '|' || map[x - 4][y] == '|')
		Back();

}

//Object

void Object::Init(int** map, int destroy)
{
	this->map = map;
	exist = 1;
	this->destroy = destroy;
}

//Floor



Floor::Floor(int** map, int x, int y, int width, int height, int destroy)
{
	int count1, count2;

	Init(map, destroy);
	for (count1 = x; count1<x + width; count1++)
	{
		for (count2 = y; count2<y + height; count2++)
			map[count1][count2] = '=';
	}
}

//Wall

Wall::Wall(int** map, int x, int y, int width, int height, int destroy)
{
	int count1, count2;

	Init(map, destroy);
	for (count1 = x; count1<x + width; count1++)
	{
		for (count2 = y; count2<y + height; count2++)
			map[count1][count2] = '|';
	}
}

//Spike
Spike::Spike(int** map, int x, int y, int width, int height, int destroy)
{
	this->x = x;
	this->y = y;
	this->map = map;

	map[x][y] = '/';
	map[x+1][y] = '|';
}

int Spike::Collision(int index_stage)
{
	if (map[x][y - 1] == '/' || map[x][y - 1] == '天' || map[x + 1][y - 1] == '/' || map[x + 1][y - 1] == '天' || map[x - 1][y] == '/')
		return 1;

	if ((map[x - 1][y] == '天' || map[x + 2][y] == '/' || map[x+2][y]=='天' || map[x + 3][y] == '/' || map[x + 3][y] == '天')&& index_stage==4)
		return 1;
	return 0;
}

#endif