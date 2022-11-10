#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>

bool youLost;	//deklarowanie zmiennej decydujacej o tym czy mozemy kontynuowac gre

//deklarowanie wielkosci planszy do gry
const int WIDTH = 20;	 
const int HEIGHT = 20;

int x, y, foodX, foodY, score = 0, bodyX[100], bodyY[100], length;

enum eDirection {STOP = 0, LEFT, UP, RIGHT, DOWN}dir;	//deklarowanie zbioru komend dla czesci logicznej (to co waz moze robic)

void Base()	
{
	youLost = false;

	dir = STOP;

	x = WIDTH / 2;
	y = HEIGHT / 2;

	//dodawanie losowosci przy losowaniu pozycji jedzonka dla weza (zeby nie byla identyczna za kazdym podejsciem)
	srand(time(NULL));	

	//losowanie pozycji pojawiajacego sie jedzonka
	foodX = rand() % WIDTH;
	foodY = rand() % HEIGHT;

	score = 0;
}

void Draw()		//funckja rysujaca plansze gry, weza oraz spawnujaca jedzonko
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });

	for (int i = 0; i < WIDTH + 2; i++)
	{
		std::cout << "#";
	}
	std::cout << std::endl;

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (j == 0)
			{
				std::cout << "#";
			}
			if(i == y && j == x)
				std::cout << "O";
			else if (i == foodY && j == foodX)
			{
				std::cout << "F";
			}else {
				bool draw = false;
				for (int k = 0; k < length; k++)
				{
					if (bodyX[k] == j && bodyY[k] == i)
					{
						std::cout << "o";
						draw = true;
					}
				}
				if (!draw)
				{
					std::cout << " ";
				}
			}
			if (j == WIDTH - 1)
			{
				std::cout << "#";
			}
		}
		std::cout << std::endl;
	}

	for(int i = 0; i < WIDTH + 2; i++)
	{
		std::cout << "#";
	}
	std::cout << std::endl;

	std::cout << "Wynik: " << score << std::endl;
}

void Input()	//funkcja zczytuj¹ca klawisze klikniete przez gracza
{
	if (_kbhit())
	{
		//switch zmieniajacy wartosc naszej zmiennej do ruchu w zaleznosci od wcisnietego przycisku
		switch (_getch())	
		{
			case 'a':
				dir = LEFT;
			break;
			case 'w':
				dir = UP;
			break;
			case 'd':
				dir = RIGHT;
			break;
			case 's':
				dir = DOWN;
			break;
		};
	}
}

void Logic()	//funkcja odpowiadajaca za poruszanie sie weza i sprawdzanie czy uzytkownik przegral
{
	int pastX = bodyX[0];
	int pastY = bodyY[0];
	int past2X, past2Y;

	//deklarowanie poczatkowej pozycji za ktora ma podazac cialo weza, bez tego program wyswietla nam cialo w lewym gornym rogu przestrzeni gry (nie podaza za glowa weza)
	bodyX[0] = x;
	bodyY[0] = y;

	for (int i = 1; i < length; i++)
	{
		past2X = bodyX[i];
		past2Y = bodyY[i];

		bodyX[i] = pastX;
		bodyY[i] = pastY;

		pastX = past2X;
		pastY = past2Y;
	}

	//switch sterujacy naszym wezem zaleznie od wartosci zmiennej dir
	switch (dir)	
	{
		case LEFT:
			x--;
		break;
		case UP:
			y--;
		break;
		case RIGHT:
			x++;
		break;
		case DOWN:
			y++;
		break;
		default:
		break;
	};
	/*
	if (x > WIDTH || x < 0 || y > HEIGHT || y < 0) youLost = true;	//przegrana w momencie walniecia w sciane
	*/
	for (int i = 0; i < length; i++)	
	{
		//przegrywamy gdy koordynaty glowy weza "walna" w cialo
		if (bodyX[i] == x && bodyY[i] == y)	
		{
			youLost = true;
		}
	}
	//sprawdzenie czy nie natrafilismy na sciane, jezeli tak to waz wychodzi z naprzeciwleglej strony
	//dla koordynatu x:
	if (x >= WIDTH)
	{
		x = 0;
	}
	else if (x < 0) {
		x = WIDTH - 1;
	}

	//to samo dla koordynatu y:
	if (y >= HEIGHT)
	{
		y = 0;
	}
	else if (y < 0) {
		y = HEIGHT - 1;
	}

	//warunek sprawdzajacy czy uzytkownik dotarl do miejsca gdzie jest jedzonko
	if (x == foodX && y == foodY)	
	{
		score++;
		length++;

		//po zebraniu jedzonka spawnujemy kolejne
		foodX = rand() % WIDTH;
		foodY = rand() % HEIGHT;
	}
}

int main()
{

	Base();
	while (!youLost)
	{
		Draw();
		Input();
		Logic();
		Sleep(100);
	}
	std::cout <<std::endl<< "Game Over!";
}