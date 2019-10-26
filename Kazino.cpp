#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>


//Функции для построения игрового поля
void verhstroka() //Верхняя строка
{
	printf("\t%c", 201);
	for (int i = 0; i <= 10; i++)
		printf("%c", 205);
	printf("%c\n", 187);
}

void nizhstroka() //Нижняя строка
{
	printf("\t%c", 200);
	for (int i = 0; i <= 10; i++)
		printf("%c", 205);
	printf("%c\n", 188);
}

void puststroka() //Пустая строка(без панелей и надписей)
{
	printf("\t%c", 186);
	for (int i = 0; i <= 10; i++)
		printf("%c", 177);
	printf("%c\n", 186);
}


int main()
{
	//Игровые константы
	const int startDengi = 800;
	const int stavka = 50;
	const int predel = 5000;
	const int bonus2x = 60;
	const int bonus3x = 100;

	//Игровые переменные
	int dengi = startDengi;
	int bonus = 0;
	char panel1 = 'X';
	char panel2 = 'X';
	char panel3 = 'X';

	time_t vremya = time(0); //Получение времени
	srand(vremya); //Случайное число на основании времени

	//Цикл игры
	do
	{
		system("cls");

		//Заполнение игрового поля
		verhstroka();
		puststroka();
		puststroka();
		printf("\t%c%c%cBANDITO%c%c%c\n",
			186, 177, 177, 177, 177, 186);
		puststroka();
		puststroka();
		printf("\t%c%c%c %c %c %c %c%c%c\n",
			186, 177, 177, panel1, panel2, panel3, 177, 177, 186);
		puststroka();
		puststroka();
		puststroka();
		nizhstroka();

		//Вывод состояния игрока
		printf("\n\t Money:%i$\n", dengi);
		if (bonus > 0)
			printf("\n\t Bonus:+%i$\n", bonus);
		printf("\n\t press enter to start\n");

		_getch();

		dengi -= stavka; //Ставка игрока

		//Получение случайное комбинации для "Бандита"
		panel1 = 3 + (rand() % 4);
		panel2 = 3 + (rand() % 4);
		panel3 = 3 + (rand() % 4);

		//Проверка бонуса
		if ((panel1 == panel2) && (panel1 == panel3)) //Совпадение трех панелей
			bonus = bonus3x;
		else
			if ((panel1 == panel2) || (panel1 == panel3)) //Совпадение двух панелей
				bonus = bonus2x;
			else
				bonus = 0;

		dengi += bonus; //Зачисление бонуса



	}
	while ((dengi >= stavka) || (dengi >= predel));

	//Результат игры
	system("cls");
	if (dengi <= stavka)
		printf("\tVi vse proigrali :-(\n");
	else
		printf("\tVam uzhe hvatit :-n");

	_getch();
	return 0;
}
