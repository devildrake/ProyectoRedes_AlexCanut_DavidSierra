#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct jugadoresT {
	string name;
	int score;
};

struct jugador {
	string name;
	int topInterno[10];
	bool logros[5];
};

void selection_sort(jugadoresT arr[], int length) {
	int minPos;
	jugadoresT tmp;
	for (int i = 0; i < length; i++) {
		minPos = i;

		for (int j = i + 1; j < length; j++) {
			if (arr[i].score < arr[j].score) {
				minPos = j;
			}
		}
		tmp = arr[i];
		arr[i] = arr[minPos];
		arr[minPos] = tmp;
	}
}

void selection_sortInteger(int arr[], int length) {
	int minPos;
	int tmp;
	for (int i = 0; i < length; i++) {
		minPos = i;

		for (int j = i + 1; j < length; j++) {
			if (arr[i] < arr[j]) {
				minPos = j;
			}
		}
		tmp = arr[i];
		arr[i] = arr[minPos];
		arr[minPos] = tmp;
	}
}

void CheckInsert(jugadoresT arr[],int length, jugadoresT toInsert,bool* shouldWriteBack) {
	jugadoresT tmp;
	jugadoresT tmp2;
	bool found = false;

	for (int i = 0; i < length; i++) {
		if (arr[i].score<toInsert.score&&!found) 
		{
			*shouldWriteBack = true;
			tmp = arr[i];
			arr[i] = toInsert;
			found = true;
			i++;
		}
		else if (found) {
			tmp2 = arr[i];
			arr[i] = tmp;
			tmp = tmp2;
		}
	}
	selection_sort(arr, length);
}

void CreateTop10() {
	jugadoresT arrayVacio[10];
	for (int i = 0; i < 10; i++) {
		arrayVacio[i].name = "---";
		arrayVacio[i].score = 0;
	}
	ofstream archivoSalidas;
	archivoSalidas.open("Top10.txt");
	archivoSalidas << "top10\nplayer\n" << arrayVacio[0].name << "\n" << to_string(arrayVacio[0].score) << "\nplayer\n" << arrayVacio[1].name << "\n" << to_string(arrayVacio[1].score) << "\nplayer\n" << arrayVacio[2].name << "\n" << to_string(arrayVacio[2].score) << "\nplayer\n" << arrayVacio[3].name << "\n" << to_string(arrayVacio[3].score) << "\nplayer\n" << arrayVacio[4].name << "\n" << to_string(arrayVacio[4].score) << "\nplayer\n" << arrayVacio[5].name << "\n" << to_string(arrayVacio[5].score) << "\nplayer\n" << arrayVacio[6].name << "\n" << to_string(arrayVacio[6].score) << "\nplayer\n" << arrayVacio[7].name << "\n" << to_string(arrayVacio[7].score) << "\nplayer\n" << arrayVacio[8].name << "\n" << to_string(arrayVacio[8].score) << "\nplayer\n" << arrayVacio[9].name << "\n" << to_string(arrayVacio[9].score) << "\n";
	archivoSalidas.close();
}

void CheckPersonalRecord(jugador* aPlayer,int newScore,bool* writeBackPersonal) {
	int tmp;
	int tmp2;
	bool found = false;

	for (int i = 0; i < 10; i++) {
		if (aPlayer->topInterno[i]<newScore && !found)
		{
			*writeBackPersonal = true;
			tmp = aPlayer->topInterno[i];
			aPlayer->topInterno[i] = newScore;
			found = true;
			i++;
		}
		else if (found) {
			tmp2 = aPlayer->topInterno[i];
			aPlayer->topInterno[i] = tmp;
			tmp = tmp2;
		}
	}
	selection_sortInteger(aPlayer->topInterno, 10);
}

void CreatePlayerFile(string playerName) {
	// Logros
	//1. No fer cap punt i morir
	//2. Aconseguir 50 punts(o una quantitat moderada de punts)
	//3. Aconseguir 100 punts(o una quantitat considerable de punts)
	//4. Sobreviure mig minut en una partida
	//5. Sobreviure 1 minut en una partida
	jugador aPlayer;
	aPlayer.name = playerName;
	for (int i = 0; i < 10; i++) {
		if (i < 5) {
			aPlayer.logros[i] = false;
		}
		aPlayer.topInterno[i] = 0;
	}

	playerName += ".txt";
	ofstream archivoSalidas;
	archivoSalidas.open(playerName);
	archivoSalidas << "Top10\n" << aPlayer.topInterno[0] << "\n" << aPlayer.topInterno[1] << "\n" << aPlayer.topInterno[2] << "\n" << aPlayer.topInterno[3] << "\n" << aPlayer.topInterno[4] << "\n" << aPlayer.topInterno[5] << "\n" << aPlayer.topInterno[6] << "\n" << aPlayer.topInterno[7] << "\n" << aPlayer.topInterno[8] << "\n" << aPlayer.topInterno[9] << "\nLogros\n" << aPlayer.logros[0]<<"\n"<<aPlayer.logros[1] << "\n" << aPlayer.logros[2] << "\n" << aPlayer.logros[3] << "\n" << aPlayer.logros[4];
	archivoSalidas.close();
}


//Función que determina si el fichero con el nombre pasado como parametro existe (en la ruta señalizada por name)
inline bool exist(const string &name) {
	ifstream file(name);
	if (!file) {
		return false;
	}
	else {
		return true;
	}
}



void main() {
	int i = 0;
	bool mustWriteBackTop10 = false;
	string line;
	if (!exist("Top10.txt")) {
		CreateTop10();
	}
	ifstream myfile("Top10.txt");
	ofstream outPutFile;
	jugadoresT losPlayers[10];
	int newScore;
	bool mustRead = false;
	bool hasReadName = false;
	string newName;

	//Variables para manejar entrada de datos desde archivo jugador personal
	jugador personalScore;
	int j = 0;
	bool mustReadPlayer = false;
	bool hasReadTopPlayer = false;
	bool reading = false;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (hasReadName) {
				losPlayers[i].score = stoi(line);
				i++;
				mustRead = false;
				hasReadName = false;
			}
			if (mustRead) {
				losPlayers[i].name = line;
				hasReadName = true;
			}
			if (line == "player") {
				mustRead = true;
			}
		}

		myfile.close();
	}
	//newScore = 800;
	//newName = "TontoTry2";
	//jugadores jugadorAInsertar{ newName, newScore };
	//CheckInsert(losPlayers, 10, jugadorAInsertar, &mustWriteBackTop10);
	if (mustWriteBackTop10) {
		cout << "Writing shit back.." << endl;
		selection_sort(losPlayers, 10);
		outPutFile.open("Top10.txt");
		outPutFile << "top10\nplayer\n" << losPlayers[0].name << "\n" << to_string(losPlayers[0].score) << "\nplayer\n" << losPlayers[1].name << "\n" << to_string(losPlayers[1].score) << "\nplayer\n" << losPlayers[2].name << "\n" << to_string(losPlayers[2].score) << "\nplayer\n" << losPlayers[3].name << "\n" << to_string(losPlayers[3].score) << "\nplayer\n" << losPlayers[4].name << "\n" << to_string(losPlayers[4].score) << "\nplayer\n" << losPlayers[5].name << "\n" << to_string(losPlayers[5].score) << "\nplayer\n" << losPlayers[6].name << "\n" << to_string(losPlayers[6].score) << "\nplayer\n" << losPlayers[7].name << "\n" << to_string(losPlayers[7].score) << "\nplayer\n" << losPlayers[8].name << "\n" << to_string(losPlayers[8].score) << "\nplayer\n" << losPlayers[9].name << "\n" << to_string(losPlayers[9].score) << "\n";
		outPutFile.close();
	}
	cout << "Enter your name (Case sensitive)" << endl;
	string playerName;
	cin >> playerName;
	if (!exist(playerName+".txt")) {
		cout << "Creating file named...  " << playerName << ".txt" << endl;
		CreatePlayerFile(playerName);
	}
	int linea = 0;

	if(exist(playerName+".txt")) {

		cout << "Reading file named...  " << playerName << ".txt" << endl;

		personalScore.name = playerName;
		playerName += ".txt";
		ifstream playerFile(playerName);


		if (playerFile.is_open())
		{
			while (getline(playerFile, line))
			{
				if (hasReadTopPlayer) {
					cout << "Linea numero " << linea << endl;
					
					if (reading) {

						int temp = stoi(line);

						if (temp == 1) {
							personalScore.logros[j] = true;
						}

						else {
							personalScore.logros[j] = false;
						}
						j++;
						linea++;
						if (j > 5) {
							hasReadTopPlayer = false;
							reading = false;
						}
					}
					if (hasReadTopPlayer) {
						reading = true;
					}
				}
				if (mustReadPlayer) {
					personalScore.topInterno[j] = stoi(line);
					cout << "assiging variable with " << stoi(line) << " to " << j<<endl;
					j++;
					linea++;
					if (j > 9) {
						mustReadPlayer = false;
						hasReadTopPlayer = true;
						j = 0;
					}
				}
				if (line == "Top10") {
					linea++;
					mustReadPlayer = true;
				}
			}
			
			playerFile.close();
		}
	}
	cout << "Name: " << personalScore.name << endl;
	cout << "Puntuaciones: " << endl;
	for (int y = 0; y < 10; y++) {
		cout << y+1 << "--"<<personalScore.topInterno[y] << endl;
	}
	cout << "Logros: " << endl;
	for (int y = 0; y < 5; y++) {
		cout << y+1 << "--" << personalScore.logros[y] << endl;
	}
}
