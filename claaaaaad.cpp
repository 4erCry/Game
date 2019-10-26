#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>

using namespace std;

const int rowsCount = 30;
const int columnCount = 40;
int numGift = 7;

const unsigned char symbolHero = 2;
const unsigned char symbolWall = 177;
const unsigned char symbolBox = 254;
const unsigned char symbolGift = 15;
const unsigned char symbolExit = 176;

unsigned char levelData1[rowsCount][columnCount + 1];

HANDLE consoleHandle = 0;
bool isGameActive = true;
unsigned char levelData[rowsCount][columnCount];
int heroRow = 0;
int heroColumn = 0;
int pickedGifts = 0;

// Обход графа в глубину
bool exploreMatr(int from, int to, bool* visited)
{
	bool isGoodPath = false;

	visited[from] = true;

	if (from == to)
		return true;

	int r = from / columnCount;
	int c = from % columnCount;

	if ((r + 1 > 0) && (r + 1 < rowsCount))
		if ((levelData1[r + 1][c] != '#') && (!visited[(r + 1) * columnCount + c]))
			isGoodPath |= exploreMatr((r + 1) * columnCount + c, to, visited);

	if ((r - 1 > 0) && (r - 1 < rowsCount))
		if ((levelData1[r - 1][c] != '#') && (!visited[(r - 1) * columnCount + c]))
			isGoodPath |= exploreMatr((r - 1) * columnCount + c, to, visited);

	if ((c + 1 > 0) && (c + 1 < columnCount))
		if ((levelData1[r][c + 1] != '#') && (!visited[r * columnCount + c + 1]))
			isGoodPath |= exploreMatr(r * columnCount + c + 1, to, visited);

	if ((c - 1 > 0) && (c - 1 < columnCount))
		if ((levelData1[r][c - 1] != '#') && (!visited[r * columnCount + c - 1]))
			isGoodPath |= exploreMatr(r * columnCount + c - 1, to, visited);

	return isGoodPath;
}

// Создание пустого поля
void ClearField()
{
	for (int i = 0; i < columnCount; i++)
	{
		levelData1[0][i] = '#';
		levelData1[rowsCount - 1][i] = '#';
	}
	for (int i = 0; i < rowsCount - 1; i++)
	{
		levelData1[i][0] = '#';
		levelData1[i][columnCount - 1] = '#';
	}
	for (int i = 1; i < rowsCount - 1; i++)
	{
		for (int j = 1; j < columnCount - 1; j++)
		{
			levelData1[i][j] = ' ';
		}
	}

	levelData1[rowsCount - 2][1] = '1';
	levelData1[1][columnCount - 2] = '2';
}

// Генерация случайного уровня
void Generate()
{
	int num = rowsCount * columnCount;
	bool *visited = new bool[num]; // Массив посещенных вершин для поиска в глубину

	int *rowGift = new int[numGift];
	int *columnGift = new int[numGift];

	// Раскидываем подарки по карте
	for (int i = 0; i < numGift; i++)
	{
		rowGift[i] = 1 + rand() % (rowsCount - 2);
		columnGift[i] = 1 + rand() % (columnCount - 2);
		if (levelData1[rowGift[i]][columnGift[i]] == ' ')
			levelData1[rowGift[i]][columnGift[i]] = '$';
		else
			numGift--; // Если подарок попал на стартовое поле или дверь
	}

	int from = (rowsCount - 2) * columnCount + 1;  // Стартовое поле героя
	int to = columnCount + columnCount - 2; // Дверь

	// Генерация стен
	for (int i = 0; i < num / 1; i++)
	{
		int n = rand() % (rowsCount * columnCount);
		int r = n / columnCount;
		int c = n % columnCount;

		// Если свежая стенка преграждает путь до двери или подарков - убрать стенку
		if (levelData1[r][c] == ' ')
		{
			levelData1[r][c] = '#';

			for (int i = 0; i < num; i++)
				visited[i] = false;
			if (!exploreMatr(from, to, visited))
				levelData1[r][c] = ' ';

			for (int i = 0; i < numGift; i++)
			{
				for (int j = 0; j < num; j++)
					visited[j] = false;
				if (!exploreMatr(rowGift[i] * columnCount + columnGift[i], to, visited))
					levelData1[r][c] = ' ';
			}
		}
	}

	return;
}

void SetupSystem()
{
	time_t currentTime;
	time(&currentTime);
	srand(currentTime);

	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

// Инициализация уровня
void Initialise()
{
	ClearField();
	Generate();

	pickedGifts = 0;

	for (int r = 0; r < rowsCount; r++)
		for (int c = 0; c < columnCount; c++)
		{
			unsigned char symbol = levelData1[r][c];

			switch (symbol)
			{
			case '#':
				levelData[r][c] = symbolWall;
				break;
			case '1':
				levelData[r][c] = symbolHero;

				heroRow = r;
				heroColumn = c;

				break;
			case '2':
				levelData[r][c] = symbolExit;
				break;
			case 'X':
				levelData[r][c] = symbolBox;
				break;
			case '$':
				levelData[r][c] = symbolGift;
				break;

			default:
				levelData[r][c] = symbol;
				break;
			}
		}
}

// Первоначальный вывод всей картинки
void Render()
{
	COORD cursorCoord;
	cursorCoord.X = 0;
	cursorCoord.Y = 0;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);

	printf("\n\n\t");
	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < columnCount; c++)
		{
			unsigned char symbol = levelData[r][c];

			switch (symbol)
			{
			case symbolWall:
				SetConsoleTextAttribute(consoleHandle, 15);
				break;
			case symbolHero:
				SetConsoleTextAttribute(consoleHandle, 10);
				break;
			case symbolBox:
				SetConsoleTextAttribute(consoleHandle, 14);
				break;
			case symbolGift:
				SetConsoleTextAttribute(consoleHandle, 14);
				break;
			case symbolExit:
				SetConsoleTextAttribute(consoleHandle, 12);
				break;

			}

			printf("%c", symbol);
		}
		printf("\n\t");
	}
	SetConsoleTextAttribute(consoleHandle, 7);
	printf("\n\n\tUse WASD to move ");
	SetConsoleTextAttribute(consoleHandle, 10);
	printf("Hero");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(". Pick all ");
	SetConsoleTextAttribute(consoleHandle, 14);
	printf("gifts");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(" to open the ");
	SetConsoleTextAttribute(consoleHandle, 12);
	printf("door");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(". R - Restart level. E - exit.");
}

// Обновление конкретного пикселя
void RenderUpdate(int r, int c)
{
	COORD cursorCoord;
	cursorCoord.X = c + 8;
	cursorCoord.Y = r + 2;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);

	unsigned char symbol = levelData[r][c];

	switch (symbol)
	{
	case symbolWall:
		SetConsoleTextAttribute(consoleHandle, 15);
		break;
	case symbolHero:
		SetConsoleTextAttribute(consoleHandle, 10);
		break;
	case symbolBox:
		SetConsoleTextAttribute(consoleHandle, 14);
		break;
	case symbolGift:
		SetConsoleTextAttribute(consoleHandle, 14);
		break;
	case symbolExit:
		SetConsoleTextAttribute(consoleHandle, 12);
		break;
	}

	printf("%c", symbol);
}

// Движение героя
void MoveHeroTo(int row, int column)
{
	unsigned char destinationCell = levelData[row][column];
	bool canMoveToCell = false;

	switch (destinationCell)
	{
	case ' ':
		canMoveToCell = true;
		break;
	case symbolExit:
		if (pickedGifts == numGift)
		{
			Initialise();
			Render();
		}
		break;
	case symbolGift:
		canMoveToCell = true;
		pickedGifts++;
		break;
	case symbolBox:
		int heroDirectionR = row - heroRow;
		int heroDirectionC = column - heroColumn;

		if (levelData[row + heroDirectionR][column + heroDirectionC] == ' ')
		{
			canMoveToCell = true;

			levelData[row][column] = ' ';

			levelData[row + heroDirectionR][column + heroDirectionC] = symbolBox;
			RenderUpdate(row + heroDirectionR, column + heroDirectionC);
		}

		break;
	}

	if (canMoveToCell)
	{
		levelData[heroRow][heroColumn] = ' ';
		RenderUpdate(heroRow, heroColumn);

		heroRow = row;
		heroColumn = column;

		levelData[heroRow][heroColumn] = symbolHero;
		RenderUpdate(heroRow, heroColumn);
	}
}

// Чтение символов управления
void Update()
{
	unsigned char inputChar = _getch();
	inputChar = tolower(inputChar);

	switch (inputChar)
	{
	case 'w':
		MoveHeroTo(heroRow - 1, heroColumn);
		break;
	case 's':
		MoveHeroTo(heroRow + 1, heroColumn);
		break;
	case 'a':
		MoveHeroTo(heroRow, heroColumn - 1);
		break;
	case 'd':
		MoveHeroTo(heroRow, heroColumn + 1);
		break;
	case 'r':
		Initialise();
		Render();
		break;
	case 'e':
		isGameActive = false;
		break;
	}
}

// Завершение игры
void Shutdown()
{
	system("cls");
	printf("\n\tPress any key to continue...");
	_getch();
}

int main()
{
	SetupSystem();
	Initialise();
	Render();

	do
	{
		Update();
	} while (isGameActive);

	Shutdown();

	return 0;
}
