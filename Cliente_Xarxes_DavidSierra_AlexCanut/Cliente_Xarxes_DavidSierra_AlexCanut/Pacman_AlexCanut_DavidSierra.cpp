#include <windows.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <string>
#include <stdlib.h>
//#include <stdlib.h>


#define ARRIBA     72      // CONSTANTS AMB LES FLETXES DEL TECLAT
#define IZQUIERDA  75
#define DERECHA    77
#define ABAJO      80
#define ESC        27
#define ENTER      13

using namespace std;

//Asuntos ajenos al Pacman
bool userRegistered = false;
bool mustExit = false;
bool welcomed = false;
bool gameOver = false;
string userName;

struct jugadorT {
	string name;
	int score;
};



int rand1 = 0;
int rand2 = 0;
int rand3 = 0;
int rand4 = 0;
int rand5 = 0;

int backcolor = 0;
int dir = 0;
int x = 39, y = 22;
int anteriorpx, anteriorpy;
int puntCont;

long int punts = -5;
int vides = 3;

mutex mutxy;
mutex mutMov;
mutex mutPint;
mutex mutRand;

int sceneState = 0;

void setCColor(int color)
{
	
	static HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color | (backcolor * 0x10 + 0x100));

}

int color[7] = {
	0x009,
	0x00E,
	0x00C,
	0x002,
	0x00B,
	0x005,
	0x00F //Blanco
};

struct fantasma {
	int fdir; //direcció del fantasma que pot prendre valors del 0-3 i pot ser inicialitzat rand() % 4
	int _x, _y; // posicíó del fantasma
	int col; //color del fantasma
};



void gotoxy(int x, int y)  // funcio que posiciona el cursor a la coordenada (x,y)
{
	mutxy.lock();
	HANDLE hCon;
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon, dwPos);
	mutxy.unlock();
}

char mapa[50][100] = {
	"                                                      ",
	"                  AXXXXXXXXXXXXXXXXXXXB AXXXXXXXXXXXXXXXXXXXB",
	"                  Y___________________Y Y___________________Y",
	"                  Y_AXXXXXB_AXXXXXXXB_Y Y_AXXXXXXXB_AXXXXXB_Y",
	"                  Y_Y     Y_Y       Y_Y Y_Y       Y_Y     Y_Y",
	"                  Y_DXXXXXC_DXXXXXXXC_DXC_DXXXXXXXC_DXXXXXC_Y",
	"                  Y________|_________|___|_________|________Y",
	"                  Y_AXXXXXB_AXB_AXXXXXXXXXXXXXB_AXB_AXXXXXB_Y",
	"                  Y_DXXXXXC_Y Y_DXXXXB   AXXXXC_Y Y_DXXXXXC_Y",
	"                  Y_________Y Y______Y   Y______Y Y_________Y",
	"                  DXXXXXXXB_Y DXXXXB_Y   Y_AXXXXC Y_AXXXXXXXC",
	"                          Y_Y AXXXXC_DXXXC_DXXXXB Y_Y        ",
	"                          Y_Y Y_________________Y Y_Y        ",
	"                  XXXXXXXXC_DXC_AXXXXXX XXXXXXB_DXC_DXXXXXXXX",
	"                  _________|____Y      *      Y____|_________",
	"                  XXXXXXXXB_AXB_DXXXXXXXXXXXXXC_AXB_AXXXXXXXX",
	"                          Y_Y Y_________________Y Y_Y        ",
	"                          Y_Y Y_AXXXXXXXXXXXXXB_Y Y_Y        ",
	"                  AXXXXXXXC_DXC_DXXXXB   AXXXXC_DXC_DXXXXXXXB",
	"                  Y________|_________Y   Y_________|________Y",
	"                  Y_AXXXXXB_AXXXXXXB_Y   Y_AXXXXXXB_AXXXXXB_Y",
	"                  Y_DXXXB Y_DXXXXXXC_DXXXC_DXXXXXXC_Y AXXXC_Y",
	"                  Y_____Y Y_________|_____|_________Y Y_____Y",
	"                  DXXXB_Y Y_AXB_AXXXXXXXXXXXXXB_AXB_Y Y_AXXXC",
	"                  AXXXC_DXC_Y Y_DXXXXB   AXXXXC_Y Y_DXC_DXXXB",
	"                  Y_________Y Y______Y   Y______Y Y_________Y",
	"                  Y_AXXXXXXXC DXXXXB_Y   Y_AXXXXC DXXXXXXXB_Y",
	"                  Y_DXXXXXXXXXXXXXXC_DXXXC_DXXXXXXXXXXXXXXC_Y",
	"                  Y_________________|_____|_________________Y",
	"                  DXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXC",
};

bool CheckMap() {
	bool atLeastOne = false;
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 100; j++) {
			if (mapa[i][j] == '_') {
				atLeastOne = true;
			}
		}
	}
	return atLeastOne;
}

void GameOverCheck() {
	if (vides <= 0 || CheckMap() == false) {
		gameOver = true;
	}
}

void pintar_mapa()// Funcio que imprimeix el mapa basant-se en el mapa codificat
{
	for (int i = 0; i < 78; i++) {
		for (int j = 0; j < 30; j++) {
			
			gotoxy(i, j);

			if (mapa[j][i] == 'X') {
				setCColor(color[0]);
				printf("%c", 205);
			}
			if (mapa[j][i] == '_') {
				setCColor(color[1]);
				printf("%c", 250);
			}
			else if (mapa[j][i] == 'Y') {
				setCColor(color[0]);
				printf("%c", 186);
			}
			else if (mapa[j][i] == 'A') {
				setCColor(color[0]);
				printf("%c", 201);
			}
			else if (mapa[j][i] == 'B') {
				setCColor(color[0]);
				printf("%c", 187);
			}
			else if (mapa[j][i] == 'C') {
				setCColor(color[0]);
				printf("%c", 188);
			}
			else if (mapa[j][i] == 'D') {
				setCColor(color[0]);
				printf("%c", 200);
			}


		}
	}
}

void tecleig() {
	if (_kbhit()) {
		char tecla = _getch();
		switch (tecla) {
		case ARRIBA:
			dir = 0;
			break;

		case ABAJO:
			dir = 1;
			break;

		case DERECHA:
			dir = 2;
			break;

		case IZQUIERDA:
			dir = 3;
			break;
		}
	}

}

void pintarPacman(int x, int y) {

	//TODO seleccionar color (funció)
	setCColor(color[1]);

	//TODO anar a la posicio indicada (funció)
	gotoxy(x, y);

	//TODO imprimir el caracter amb codi ASCII 6
	printf("%c", 6);

}

void borrarPacman(int x, int y) {
	//TODO anar a la posicio indicada (funció)
	gotoxy(x, y);
	printf(" ");
	//TODO imprimir el caracter d'espai
	if (mapa[y][x] == '_')
	{
		mapa[y][x] = ' '; 
		punts += 5; 
		puntCont += 5;
		if (puntCont == 25&&vides<3&&vides>0) {
			vides++;
			puntCont = 0;
		}
	}
}


void mourePacman()
{
	//TODO guardar la posició anterior del pacman
	anteriorpx = x;
	anteriorpy = y;

	//TODO detectar cap a on moure pacman
	tecleig();
	//TODO comprovar si es pot moure en aquesta direcció

	if (dir == 0 && mapa[y - 1][x] != 'X' && mapa[y - 1][x] != 'A' && mapa[y - 1][x] != 'Y' && mapa[y - 1][x] != 'B' && mapa[y - 1][x] != 'C' && mapa[y - 1][x] != 'D')    
		y--;


	else if (dir == 1 && mapa[y + 1][x] != 'X' && mapa[y + 1][x] != 'A' && mapa[y + 1][x] != 'Y' && mapa[y + 1][x] != 'B' && mapa[y + 1][x] != 'C' && mapa[y + 1][x] != 'D')       
		y++;

	if (dir == 2 && mapa[y][x + 1] != 'X' && mapa[y][x + 1] != 'A' && mapa[y][x + 1] != 'Y' && mapa[y][x + 1] != 'B' && mapa[y][x + 1] != 'C' && mapa[y][x + 1] != 'D')    
		x++;

	else if (dir == 3 && mapa[y][x - 1] != 'X' && mapa[y][x - 1] != 'A' && mapa[y][x - 1] != 'Y' && mapa[y][x - 1] != 'B' && mapa[y][x - 1] != 'C' && mapa[y][x - 1] != 'D')        
		x--;
	//TODO esborrar el pacman de l'anterior posició i imprimir-lo en la nova
	if (x == 17 && y == 14&&dir==3) {
		x = 60;
	}
	else if (x == 61 && y == 14 && dir == 2) {
		x = 18;
	}
	borrarPacman(anteriorpx, anteriorpy);
	pintarPacman(x, y);			
	
}
		


	fantasma inicialitzarFantasma(int x, int y, int color) {
	//TODO inicialitzar el fantasma amb les dades donades i la direcció rand() % 4

	fantasma aFantasma;
	aFantasma._x = x;
	aFantasma._y = y;
	aFantasma.col = color;

	aFantasma.fdir = rand() % 4;

	return aFantasma;
}

void pintarFantasma(fantasma* ghost) {
	mutPint.lock();
	//TODO seleccionar color (funció)
	setCColor(color[ghost->col]);

	//TODO anar a la posicio indicada (funció)
	gotoxy(ghost->_x, ghost->_y);

	//TODO imprimir el caracter amb codi ASCII 6

	printf("%c", 6);
	mutPint.unlock();
}
void borrarFantasma(fantasma* ghost) {

	//TODO anar a la posicio indicada (funció)
	gotoxy(ghost->_x, ghost->_y);

	//TODO imprimir el caracter d'espai
	printf(" ");
}

void xocPacman(fantasma* ghost) {
	//TODO detectar si el fantasma colisiona amb l'actual o l'anterior posició del Pacman, de ser aixi posicionar al jugador a x=39, y=22, dir = 4 i disminuir en una les vides
	if ((ghost->_x == x && ghost->_y == y)||(ghost->_x==anteriorpx&&ghost->_y==anteriorpy))
	{
		x = 39;
		y = 22;
		dir = 4;
		vides--;
		puntCont = 0;
		if (punts >= 25) {
			punts -= 25;
		}
	}

}



void moureFantasma(fantasma* ghost) {

	mutMov.lock();
	borrarFantasma(ghost);
	int prevX = ghost->_x;
	int	prevY = ghost->_y;

	if (mapa[ghost->_y][ghost->_x] == '|') {
		//ghost->fdir = rand() % 4;
		ghost->fdir = rand5;
	}
	if (mapa[ghost->_y][ghost->_x] == '*') {
		ghost->fdir = 0;
	}

	if (ghost->fdir == 2) {
		if (mapa[ghost->_y][ghost->_x + 1] != 'X' && mapa[ghost->_y][ghost->_x + 1] != 'A' && mapa[ghost->_y][ghost->_x + 1] != 'Y' && mapa[ghost->_y][ghost->_x + 1] != 'B' && mapa[ghost->_y][ghost->_x + 1] != 'C' && mapa[ghost->_y][ghost->_x + 1] != 'D')
		{
			ghost->_x++;
		}
		else 
		{

			ghost->fdir = rand1;
		}
	}
	if (ghost->fdir == 3) {
		if (mapa[ghost->_y][ghost->_x - 1] != 'X' && mapa[ghost->_y][ghost->_x - 1] != 'A' && mapa[ghost->_y][ghost->_x - 1] != 'Y' && mapa[ghost->_y][ghost->_x - 1] != 'B' && mapa[ghost->_y][ghost->_x - 1] != 'C' && mapa[ghost->_y][ghost->_x - 1] != 'D')
		{
			ghost->_x--;
		}
		else
		{

			ghost->fdir = rand2;
		}
	}
	if (ghost->fdir == 0) {
		if (mapa[ghost->_y - 1][ghost->_x] != 'X' && mapa[ghost->_y - 1][ghost->_x] != 'A' && mapa[ghost->_y - 1][ghost->_x] != 'Y' && mapa[ghost->_y - 1][ghost->_x] != 'B' && mapa[ghost->_y - 1][ghost->_x] != 'C' && mapa[ghost->_y - 1][ghost->_x] != 'D')
		{
		
			ghost->_y--;
		}
		else 
		{ 

			ghost->fdir = rand3;
			
		}
	}
	if (ghost->fdir == 1) {
		if (mapa[ghost->_y + 1][ghost->_x] != 'X' && mapa[ghost->_y + 1][ghost->_x] != 'A' && mapa[ghost->_y + 1][ghost->_x] != 'Y' && mapa[ghost->_y + 1][ghost->_x] != 'B' && mapa[ghost->_y + 1][ghost->_x] != 'C' && mapa[ghost->_y + 1][ghost->_x] != 'D')
		{
			ghost->_y++;
		}
		else
		{
	
			ghost->fdir = rand4;
		}
	}
	if (mapa[prevY][prevX] == '_') { setCColor(color[1]); gotoxy(prevX, prevY); printf("%c", 250); }
	// rutina atajo
	if (ghost->_x <= 17) ghost->_x = 61;
	else if (ghost->_x >= 62)ghost->_x = 18;

	//TODO Printar el fantasma

	pintarFantasma(ghost);

	xocPacman(ghost);
	mutMov.unlock();
}


void marcador() {

	gotoxy(5, 4); printf("PUNTS");
	gotoxy(5, 6); printf("    ");
	setCColor(color[6]);
	gotoxy(5, 6); printf("%ld", punts);

	setCColor(color[1]);
	gotoxy(5, 25); printf("VIDES");
	for (int i = 0; i <= vides; i++) {
		gotoxy(5, i + 27); printf(" ");

	}
	for (int i = 0; i < vides; i++) {
		gotoxy(5, i + 27); printf("%c", 2);

	}
	setCColor(color[2]);
	gotoxy(70, 27); printf("%c", 169);

}

//Función que recibe un string y valora en función de los carácteres especiales si es valido o no
bool validate(string aName){
	bool valid = true;
	for (int i = 0; i < aName.length(); i++) {
		if (aName[i] == '?'||aName[i]=='!'||aName[i]=='.') {
			valid = false;
		}
	}
	return valid;
	}

void main() {
	jugadorT top10[10];


	for (int i = 0; i < 10; i++) {
		top10[i].name = "---";
		top10[i].score = 0;
	}

	while (!userRegistered) {
		cout << "Enter an username (case sensitive):" << endl;
		cin >> userName;

		if (validate(userName)) {
			userRegistered = true;
			sceneState = 0;
		}
		else {
			cout << "Invalid UserName" << endl;
		}
	}

	while (!mustExit) {
		if (!welcomed) {
			welcomed = true;
			cout << "Bienvenido a Pacman " << userName<<" !"<<   endl << endl;
		}
		
		if (sceneState == 0) {
			int a;
			
			cout << "Ests en el menu principal" << endl;
			cout << "¿Que desea hacer?" << endl;
			cout << "1: Jugar" << endl;
			cout << "2: Consultar Top 10" << endl;
			cout << "3: Consultar Puntuaciones personales" << endl;
			cout << "4: Consultar logros" << endl;
			cout << "5: Salir" << endl;
			cin >> a;

			if (a == 1 || a == 2 || a == 3 || a == 4 || a == 5) {
				sceneState = a;
			}
		}
		else if (sceneState == 1) {
			//Vacia la consola
			std::system("cls");

			pintar_mapa();
			srand(time(NULL));

			fantasma ghostA = inicialitzarFantasma(41, 14, 2);
			fantasma ghostB = inicialitzarFantasma(43, 14, 3);
			fantasma ghostC = inicialitzarFantasma(40, 14, 4);
			fantasma ghostD = inicialitzarFantasma(39, 14, 5);

			fantasma* pointA = &ghostA;
			fantasma* pointB = &ghostB;
			fantasma* pointC = &ghostC;
			fantasma* pointD = &ghostD;
#pragma region pacmanGame
			//while (vides > 0 && punts < 1900) {
			while (!gameOver){
				marcador();
				rand1 = rand() % 4;
				rand2 = rand() % 4;
				rand3 = rand() % 4;
				rand4 = rand() % 4;
				rand5 = rand() % 4;
				mourePacman();
				thread pacman = thread(mourePacman);
				thread ghA = thread(moureFantasma, pointA);
				thread ghB = thread(moureFantasma, pointB);
				thread ghC = thread(moureFantasma, pointC);
				thread ghD = thread(moureFantasma, pointD);

				Sleep(5);
				pacman.join();

				ghA.join();
				ghB.join();
				ghC.join();
				ghD.join();


				Sleep(100);
				GameOverCheck();
			}


		/*	for (int i = 0; i <= vides; i++) {
				gotoxy(5, i + 27);
				printf(" ");
			}
			*/
			//system("pause>NULL");
			std::system("cls");
			setCColor(0x00F);
			gameOver = false;
			vides = 3;

			cout << "Game Over!"<<endl<<endl << "Your final score was:  " << punts << endl;

			system("pause");

			std::system("cls");
			sceneState = 0;

		}
	#pragma endregion

		else if (sceneState == 2) {
			cout << "Top 10" << endl;

			system("pause");
			sceneState = 0;
		}
		else if (sceneState == 3) {
			sceneState = 0;
		}
		else if (sceneState == 4) {
			sceneState = 0;
		}
		else if (sceneState == 5) {
			sceneState = 0;
			cout << "\nGoodBye!" << endl;
			mustExit = true;
		}
			}
		}
	
