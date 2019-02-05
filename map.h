#pragma once
#ifndef HEADER
#define HEADER

#include<iostream>
#include<windows.h>
#include<conio.h>
#include<mmsystem.h> 

#include "class.h"

void stage1(int** map)
{
	Floor* f = new Floor[12] = { Floor(map, 30, 33, 20, 1, 0), Floor(map, 30, 27, 20, 1, 0), Floor(map, 30, 22, 20, 1, 0), Floor(map, 30, 17, 20, 1, 0)
		, Floor(map, 30, 12, 20, 1, 0), Floor(map, 30, 7, 20, 1, 0), Floor(map, 1, 35, 20, 1, 0), Floor(map, 1, 30, 20, 1, 0), Floor(map, 1, 25, 20, 1, 0), Floor(map, 1, 20, 20, 1, 0)
		, Floor(map, 1, 15, 20, 1, 0), Floor fw(map, 1, 10, 20, 1, 0) };

	/*	Floor f2(map, 30, 33, 20, 1, 0);
	Floor f3(map, 30, 27, 20, 1, 0);
	Floor f4(map, 30, 22, 20, 1, 0);
	Floor f5(map, 30, 17, 20, 1, 0);
	Floor f6(map, 30, 12, 20, 1, 0);
	Floor f7(map, 30, 7, 20, 1, 0);


	Floor f8(map, 1, 35, 20, 1, 0);
	Floor f9(map, 1, 30, 20, 1, 0);
	Floor fq(map, 1, 25, 20, 1, 0);
	Floor fw(map, 1, 20, 20, 1, 0);
	Floor fe(map, 1, 15, 20, 1, 0);
	Floor fr(map, 1, 10, 20, 1, 0);*/

	Wall w1(map, 50, 8, 1, 32, 0);
}


#endif