#include<iostream>
#include<windows.h>
#include<conio.h>
#include<mmsystem.h> 

#include "class.h"
#include "map.h"
#pragma comment(lib, "winmm.lib")

using namespace std;

void Cur(short, short);
void Title();
void StageItem(int, int&, int&);
extern void DrawMap();
void Initailize(int);
void stage(int, Goal**, Item***, Spike***);
void stage1(Goal**, Item***);
void stage2(Goal**, Item***);
void stage3(Goal**, Item***, Spike***);
void stage4(Goal**, Item***, Spike***);
void Clear(int&);
void Ending(int&);



int** map;
//int map[91][41];
HANDLE hBuffer[2];  //버퍼 핸들
int nScreenIndex;
COORD csize = { 700, 500 }; //버퍼의 크기 관련 구조채
SMALL_RECT rect; //창 크기 관련 구조체

int player_x, player_y;
Player p;

int main()
{
	int count;
	int index_stage = 1, index_item=0, index_spike=0;
	int clear = 0;
	char key;

	Goal* goal;
	Item** item;
	Spike** spike;

	system("color F0");
	system("title Running Man");
	system("mode con: cols=600 lines=600");

	rect.Left = 0;
	rect.Right = 100;//80 - 1;
	rect.Top = 0;
	rect.Bottom = 50;//25 - 1;

	map = new int*[92];
	for (count = 0; count<92; count++)
		map[count] = new int[41];

	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); //버퍼 생성
																												  //CreateConsoleScreenBuffer(...)가 버퍼를 생성하는 함수인데요. 인자가 굉장히 많지만 대게 위 처럼 쓰면 되므로 넘어가겠습니다.
	SetConsoleScreenBufferSize(hBuffer[0], csize); //콘솔 버퍼의 크기 설정
	SetConsoleWindowInfo(hBuffer[0], TRUE, &rect); //콘솔 창의 크기 설정

												   //두번째 버퍼 생성
	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); //버퍼 생성
	SetConsoleScreenBufferSize(hBuffer[1], csize); //콘솔 버퍼의 크기 설정
	SetConsoleWindowInfo(hBuffer[1], TRUE, &rect); //콘솔 창의 크기 설정


	Title();

	PlaySound(TEXT("치타맨.wav"), NULL, SND_ASYNC | SND_LOOP);

	while (1)
	{
		Initailize(index_stage);
		stage(index_stage, &goal, &item, &spike);
		StageItem(index_stage, index_item, index_spike);
		clear = 0;
		DrawMap();

		Cur(0, 92);
		while (!clear)
		{
			DrawMap();


			if (_kbhit())
			{
				rewind(stdin);
				key = _getch();
				if (key == -32)
				{
					key = _getch();
					switch (key)
					{
					case 80:
						p.Move(p.x, p.y + 1);
						break;

					case 72:
						p.Move(p.x, p.y - 1);
						break;

					case 75:
						p.Move(p.x - 1, p.y);
						break;

					case 77:
						p.Move(p.x + 1, p.y);
						break;
					}
				}

				else if (key == 'J' || key == 'j' || key == 32)
				{
					printf("%d", p.jumping);
					p.Jump();
				}
			}

			p.CollideWall();
			/*
			if (p.x == 87 || p.x == 3)
			p.Back();
			*/

			for (count = 0; count < p.speed; count++)
			{
				DrawMap();
				p.Proceed();
			}
			while (p.jumping)
			{
				DrawMap();
				p.Drop();
				p.Proceed();
				//			Sleep(10);
			}
			///		Sleep(100);
			key = 0;


			if (goal->Verification())
			{
				Clear(index_stage);
				Initailize(index_stage);
				break;
			}

			for (count = 0; count < index_item; count++)
			{
			
				if (item[count]->Verification())
					item[count]->Disappear(&p);
			}

	

			for (count = 0; count < index_spike; count++)
			{
				if (spike[count]->Collision(index_stage))
					clear = 1;
			}
		}
	}

	for (count = 0; count<92; count++)
		delete[] map[count];
	delete[] map;
	return 0;
}




void Initailize(int index_stage)
{
	int count1, count2;

	for (count1 = 0; count1<91; count1++)
	{
		for (count2 = 0; count2<41; count2++)
			map[count1][count2] = 0;
	}

	for (count1 = 0; count1 <= 90; count1++)
	{
		map[count1][0] = '=';
		map[count1][40] = '=';
	}

	for (count1 = 1; count1 <= 39; count1++)
	{
		map[0][count1] = '|';
		map[90][count1] = '|';
	}

	p.Initailize(map, index_stage);
}

void DrawMap()
{
	int count_x, count_y;
	char str[5];

	DWORD dw;   //좌표를 저장하기 위한 구조채를 선언.
	COORD CursorPosition;    //좌표 설정


	for (count_x = 0; count_x <= 90; count_x++)
	{
		for (count_y = 0; count_y <= 40; count_y++)
		{
			if (map[count_x][count_y])
			{
				str[0] = map[count_x][count_y];
				CursorPosition.X = count_x;
				CursorPosition.Y = count_y;
				SetConsoleCursorPosition(hBuffer[nScreenIndex], CursorPosition);    //좌표 이동
				WriteFile(hBuffer[nScreenIndex], str, 1, &dw, NULL);    //버퍼에 씀
			}
		}
	}

	SetConsoleActiveScreenBuffer(hBuffer[nScreenIndex]);    //버퍼를 활성하 함으로써 화면에 버퍼의 내용을 한번에 출력
	nScreenIndex = !nScreenIndex;   //다음 버퍼 선택

	COORD Coor = { 0, 0 }; //세번째 줄의 함수를 위한 구조체
	for (count_y = 0; count_y <= 50; count_y++)
	{
		FillConsoleOutputCharacter(hBuffer[nScreenIndex], 0, 91, Coor, &dw);
		Coor.Y++;
	}
}

void Cur(short x, short y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void Title()
{
	char key;
	while (1)
	{

		cout << "\n\n\n\n\n\n\n\n\n\n";
		cout << ".@@@@@#.                                  *@                              =@#        =@*" << endl;
		cout << ".@    +@.                                  @                              =@@+      .@%#" << endl;
		cout << ".@.    @=  **    +%   @:#%@@:   #++%%@*   =@   @:*%%@=    +@%%%=@         =@ @:     @:%#   *%%%@=   %++%%@*" << endl;
		cout << ".@*++*@+   %#    *@   @%   :@.  @@.   @*  +@   @@   .@=  @@    %@         =@ :@    @# %#   :   .@.  @@.   @*" << endl;
		cout << ".@+=+@+    %#    *@   @.    @=  @*    #@  +@   @:    @* .@     .@         =@  *@  #@  %#   :#%%#@=  @*    #%" << endl;
		cout << ".@.   @%   %#    *@   @.    @=  @*    #%  +@   @:    @* .@     =@         =@   @#:@   %#  #@    @=  @*    #%" << endl;
		cout << ":@.    @@  =@= .+@@   @.    @=  @#    #@  +@   @:    @*  #@= .+@@         =@    @@.   @#  %@. :%@=  @#    %%" << endl;
		cout << " +      +=  .*#*..=   +     +.  =:    :=  .+   +.    +:   .*#*..@         .+    .:    =:   =*#+ +.  =:    :=" << endl;
		cout << "                                                         .     @*" << endl;
		cout << "                                                         +@%%@%:" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		cout << "                    ::::.                                           = .: ::      .:  =. +" << endl;
		cout << "                   :@***#@*                                        *# @. .@#    *@.  @ +@" << endl;
		cout << "                   .@     @+  .  :.   :::      :::     ::.         @ +@    @#  #@   #+ @" << endl;
		cout << "                   .@     @= .@*#*= *@+=*@=  #@++*=  @%=+#.                 @##@" << endl;
		cout << "                   .@***%@=  .@=   *@    :@  @*     :@:                      @@" << endl;
		cout << "                   .@:::     .@    %@######.  +@@*   .*@@+                 :@.=@." << endl;
		cout << "                   .@        .@    +@            #@      @#               +@.  =@:" << endl;
		cout << "                   :@        .@.    +@%##@+ .@###@: +@##%@.              %@     :@*" << endl;

		key = _getch();
		if (key == 'X' || key == 'x')
		{
			break;
		}

	}
}

void StageItem(int index_stage, int& index_item, int& index_spike)
{
	switch (index_stage)
	{
		case 1:
			index_item = 0;
			index_spike = 0;
			break;

		case 2:
			index_item = 1;
			index_spike = 0;
			break;

		case 3:
			index_item = 0;
			index_spike = 9;
			break;

		case 4:
			index_item = 2;
			index_spike = 2;
			break;
	}

}

void stage(int index_stage, Goal** g, Item*** i, Spike*** s)
{
	switch (index_stage)
	{
	case 1:
		stage1(g, i);
		break;

	case 2:
		stage2(g, i);
		break;

	case 3:
		stage3(g, i, s);
		break;

	case 4:
		stage4(g, i, s);
		break;

	default:
		Ending(index_stage);
		break;

	}
}

void stage1(Goal** g, Item*** i)
{
	Floor* F[20];
	F[0] = new Floor(map, 30, 37, 20, 1, 0);
	F[1] = new Floor(map, 30, 31, 20, 1, 0);
	F[2] = new Floor(map, 30, 25, 20, 1, 0);
	F[3] = new Floor(map, 30, 19, 20, 1, 0);
	F[4] = new Floor(map, 30, 13, 20, 1, 0);
//	F[5] = new Floor(map, 27, 7, 23, 1, 0);
//	F[12] = new Floor(map, 30, 1, 20, 1, 0);
	//	F[13]=new Floor(map, 30, 2, 20, 1, 0);


	F[6] = new Floor(map, 1, 34, 20, 1, 0);
	F[7] = new Floor(map, 1, 28, 20, 1, 0);
	F[8] = new Floor(map, 1, 22, 20, 1, 0);
	F[9] = new Floor(map, 1, 16, 20, 1, 0);
//	F[10] = new Floor(map, 1, 10, 20, 1, 0);
//	F[11] = new Floor(map, 1, 4, 20, 1, 0);

	Wall w1(map, 50, 14, 1, 26, 0);
	*g = new Goal(map, 80, 36, 0, 0, 0);
}

void stage2(Goal** g, Item*** i)
{
	Floor* F[20];
	Wall* W[10];
	Item** I = new Item*[5];
//	Item* I[5];
	*i = I;

	F[0] = new Floor(map, 10, 36, 4, 2, 0);
	F[1] = new Floor(map, 16, 34, 4, 2, 0);
	F[2] = new Floor(map, 22, 32, 4, 2, 0);
	F[3] = new Floor(map, 28, 30, 4, 2, 0);
	F[4] = new Floor(map, 34, 28, 4, 2, 0);
	F[5] = new Floor(map, 40, 26, 4, 2, 0);
	F[6] = new Floor(map, 46, 24, 18, 2, 0);

	F[7] = new Floor(map, 40, 19, 4, 2, 0);
	F[8] = new Floor(map, 34, 15, 4, 2, 0);
	F[9] = new Floor(map, 22, 11, 10, 2, 0);
	F[10] = new Floor(map, 1, 7, 21, 2, 0);

	W[0] = new Wall(map, 64, 18, 1, 22, 0);
	W[1] = new Wall(map, 21, 7, 1, 4, 0);


	I[0] = new Item(map, 1, 3, 0, 0, 2);
	
	*g=new Goal(map, 80, 36, 0, 0, 0);
}

void stage3(Goal** g, Item*** i, Spike*** s)
{
	new Floor(map, 1, 8, 15, 1, 0);
	new Floor(map, 22, 8, 60, 1, 0);

	new Floor(map, 31, 15, 60, 1, 0);

	new Floor(map, 23, 21, 17, 1, 0);


	new Floor(map, 46, 21, 19, 1, 0);
	new Floor(map, 71, 21, 14, 1, 0);


	new Wall(map, 15, 9, 1, 30, 0);
	new Wall(map, 22, 9, 1, 30, 0);

	new Wall(map, 39, 22, 1, 18, 0);
	new Wall(map, 45, 22, 1, 18, 0);

	new Wall(map, 64, 22, 1, 18, 0);
	new Wall(map, 71, 22, 1, 18, 0);


	Spike** S = new Spike*[10];

	S[0]=new Spike(map, 16, 39, 0, 0, 0);
	S[1]=new Spike(map, 18, 39, 0, 0, 0);
	S[2]=new Spike(map, 20, 39, 0, 0, 0);

	S[3] = new Spike(map, 40, 39, 0, 0, 0);
	S[4] = new Spike(map, 42, 39, 0, 0, 0);
	S[5] = new Spike(map, 44, 39, 0, 0, 0);

	S[6] = new Spike(map, 65, 39, 0, 0, 0);
	S[7] = new Spike(map, 67, 39, 0, 0, 0);
	S[8] = new Spike(map, 69, 39, 0, 0, 0);
	*s = S;


	*g = new Goal(map, 80, 36, 0, 0, 0);
}


void stage4(Goal** g, Item*** i, Spike*** s)
{
	Spike** S;
	Item** I;

	new Floor(map, 20, 35, 20, 1, 0);
	new Floor(map, 40, 29, 20, 1, 0);
	new Floor(map, 60, 25, 20, 1, 0);
	new Floor(map, 70, 20, 20, 1, 0);
	new Floor(map, 55, 15, 20, 1, 0);
	new Floor(map, 64, 10, 20, 1, 0);
	new Floor(map, 73, 6, 17, 1, 0);

	new Floor(map, 45, 15, 10, 1, 0);

	new Floor(map, 1, 19, 30, 1, 0);

	new Wall(map, 20, 36, 1, 4, 0);
	new Wall(map, 54, 10, 1, 5, 0);
	new Wall(map, 31, 16, 1, 3, 0);

	S = new Spike*[10];
	S[0] = new Spike(map, 19, 18, 0, 0, 0);
	S[1] = new Spike(map, 88, 39, 0, 0, 0);

	I = new Item*[2];
	I[0] = new Item(map, 24, 36, 0, 0, 1);
	I[1] = new Item(map, 82, 2, 0, 0, 1);

	*i = I;
	*s = S;
	*g = new Goal(map, 1, 15, 0, 0, 0);

}

void Clear(int& index_stage)
{
	if (index_stage == 4)
	{
		Ending(index_stage);
		return;
	}

	index_stage++;
	char key;


	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	system("cls");
	system("mode con: cols=600 lines=600");
	while (1)
	{

		cout << "                                       :+***+=   =@:                                   ==" << endl;
		cout << "                                     #@%+=:==#+  +@:                                   @@" << endl;
		cout << "                                   :@#           +@:      .:.         .:.          .   @@" << endl;
		cout << "                                   @%            +@:    %@#*#@%    *@%**@@+   .@*%%##  @@" << endl;
		cout << "                                  +@:            +@:  .@*     %@         *@.  .@@      %@" << endl;
		cout << "                                  =@+            +@:  #@@@@@@@@@:  .*%@@%@@.  .@+      %@" << endl;
		cout << "                                   @@.           +@:  *@          *@=    +@.  .@+      ::" << endl;
		cout << "                                    #@%:     =:  +@:   @@:    ==  #@:   +@@:  .@+      ==" << endl;
		cout << "                                      =#@@@@%*.  :%.    =#@@@%*.   +%@@%=:%.  .%=      %@" << endl;
		cout << "\n\n\n";
		cout << "                   *@@@@@@#:                                                   #@ :@  :@%       @@   @@ +@" << endl;
		cout << "                   #@     +@%                                                 :@. @*   .@@    .@%   :@  @=" << endl;
		cout << "                   #@      %@   #= *%#   =#%%#=     =#%%%*    +%%%%=          ++ :*      @@. +@+    +: :*" << endl;
		cout << "                   #@     :@%   @@%: . :@#.  .@@   @@   .=  :@*   .:                      *@#@." << endl;
		cout << "                   #@%%%@@@=    @@     @%.:::::@#  @@=.     .@%:                          :@@@" << endl;
		cout << "                   #@.          @#    :@%******+=   :*@@%=    =#@@#.                     #@: %@." << endl;
		cout << "                   #@           @#     @@               *@:       @@                    @@    *@+" << endl;
		cout << "                   #@           @#      %@#++*%@  .@#++*@%  *@*++#@+                  +@%      =@@" << endl;
		cout << "                    .           .         ::=:      :=:.     .:=:.                    ..         .." << endl;
		cout << "" << endl;
		cout << "\n\n\n";
		cout << "                                                                               #%." << endl;
		cout << "      =%                                                                +#     :+" << endl;
		cout << "      %@         ...                  ...        ...           ..       @@               ..                       .." << endl;
		cout << "    *%@@%%%   *@%###@%.            =@@#*#@=   #@%*#%@%.   +@+%%*#@@.  %%@@%%#  %@   .@##%*#@@=   +@:     @@    :@@#*%@*" << endl;
		cout << "      #@     @@      #@:          #@:        @%      %@.  =@%     @@    @@     #@   .@@     #@.  =@:     %@   +@:     @#" << endl;
		cout << "      %@    +@.       @%         .@*        *@        @#  =@:     #@    @@     #@   .@*     =@:  =@:     %@   @@%@@@@@@@" << endl;
		cout << "      %@    =@=       @*          @#        +@:      .@+  =@:     #@    @@     #@   .@*     =@:  =@:     %@   @@" << endl;
		cout << "      #@.    #@+    :@@           =@#.   ..  %@=    :@%   =@=     %@    @@     #@   .@*     +@:   @@    *@@   :@%.    +." << endl;
		cout << "       *@@%   .*@@@@#:              +%@@@%.   :#@@@@#.    :%:     *%     #@@#  +%    %+     =%.    *@@@%.+%     +%@@@%*" << endl;

		key = _getch();
		if (key == 'x' || key == 'X')
		{
			break;
		}
	}

}

void Ending(int& index_stage)
{
	char key;
	index_stage = 1;


	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	system("cls");
	
	cout << "													  ...    +																	" << endl;
	cout << "						  +@@      @+      #@              *@%***%= .@.                           @=							" << endl;
	cout << "						 .@ @*     @=      *@             @*        .@     =+=     .=+=    .: ==  @=							" << endl;
	cout << "						 @:  @:    @=      *@            @#         .@   #@:.:@#  =*:.:@+  *@#==  @:							" << endl;
	cout << "						@@.:.*@    @=      *@            @*         .@  +@++*+*@.  :+**@%  +@     @:							" << endl;
	cout << "					   *@=+++=#@   @=      *@            =@.        .@  =@        @+   #%  +@     =								" << endl;
	cout << "					  =@       @@  @%+***: #@+***+        .@@*==+#= .@.  *@+==*+  @#==#@@  *@     %=							" << endl;
	cout << "					  .         .  ......   ......           .::.    .     :::     .::            .								" << endl;
	cout << "																																" << endl;
	cout << "																																" << endl;
	cout << "																																" << endl;
	cout << "   :*###*.                                                                                     +@                       .*	" << endl;
	cout << " =@*.   :.                                                =@                              @.                            =@	" << endl;
	cout << "+@          .####%=   *++##@+    +%###+%   @:#%= +%##%=  %%@#%  @.    @.  #=*%# .%##%*  +#@%#= =#   .####%:   #++##@+   :@	" << endl;
	cout << "@*         =@.    @#  %@    @*  @#    %@   @#         @:  =@    @:    @:  @@         @%   @.   *@  =@.    @*  @@    @+  :@	" << endl;
	cout << "%@         @*     =@  %#    %# .@     =@   @.    *%##+@=  +@    @.    @:  @*    =%##*%%   @.   +@  @*     =@  @*    %*  .@	" << endl;
	cout << " @@.       =@.    @*  %#    %#  @*    @@   @.   #@   :@=  +@    @+   =@:  @*   :@    @%   @:   *@  +@     @*  @*    @*		" << endl;
	cout << "  :#%%%%%:  .#%###:   *+    *+   *%##*=@   @.    #%##:%:   #%#.  %%##:@.  #+    +@##*+*   +@#= =#   .#%###:   #+    #+  =@	" << endl;
	cout << "									  @*																						" << endl;
	cout << "								 +%####:																						" << endl;
	cout << "											restart : X																			" << endl;
	while (1)
	{
		key = _getch();
		if (key == 'x' || key == 'X')
			return;
	}
}
