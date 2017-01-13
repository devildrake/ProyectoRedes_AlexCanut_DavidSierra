#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct jugadores {
	string name;
	int score;
};
void selection_sort(jugadores arr[], int length) {
	int minPos;
	jugadores tmp;
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

void CheckInsert(jugadores arr[],int length,jugadores toInsert) {
	jugadores tmp;
	jugadores tmp2;
	bool found = false;

	for (int i = 0; i < length; i++) {
		if (arr[i].score<toInsert.score&&!found) {
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
	jugadores arrayVacio[10];
	for (int i = 0; i < 10; i++) {
		arrayVacio[i].name = "---";
		arrayVacio[i].score = 0;
	}
	ofstream archivoSalidas;
	archivoSalidas.open("Top10.txt");
	archivoSalidas << "top10\nplayer\n" << arrayVacio[0].name << "\n" << to_string(arrayVacio[0].score) << "\nplayer\n" << arrayVacio[1].name << "\n" << to_string(arrayVacio[1].score) << "\nplayer\n" << arrayVacio[2].name << "\n" << to_string(arrayVacio[2].score) << "\nplayer\n" << arrayVacio[3].name << "\n" << to_string(arrayVacio[3].score) << "\nplayer\n" << arrayVacio[4].name << "\n" << to_string(arrayVacio[4].score) << "\nplayer\n" << arrayVacio[5].name << "\n" << to_string(arrayVacio[5].score) << "\nplayer\n" << arrayVacio[6].name << "\n" << to_string(arrayVacio[6].score) << "\nplayer\n" << arrayVacio[7].name << "\n" << to_string(arrayVacio[7].score) << "\nplayer\n" << arrayVacio[8].name << "\n" << to_string(arrayVacio[8].score) << "\nplayer\n" << arrayVacio[9].name << "\n" << to_string(arrayVacio[9].score) << "\n";
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
	string line;
	if (!exist("Top10.txt")) {
		CreateTop10();
	}
	ifstream myfile("Top10.txt");
	ofstream outPutFile;
	jugadores losPlayers[10];
	int newScore;
	bool mustRead=false;
	bool hasReadName=false;
	string newName;

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
	//selection_sort(losPlayers, 10);
	//CheckInsert(losPlayers, 10, jugadorAInsertar);
	selection_sort(losPlayers, 10);
	outPutFile.open("example.txt");
	outPutFile << "top10\nplayer\n" << losPlayers[0].name << "\n" << to_string(losPlayers[0].score) << "\nplayer\n" << losPlayers[1].name << "\n" << to_string(losPlayers[1].score) << "\nplayer\n" << losPlayers[2].name << "\n" << to_string(losPlayers[2].score) << "\nplayer\n" << losPlayers[3].name << "\n" << to_string(losPlayers[3].score) << "\nplayer\n" << losPlayers[4].name << "\n" << to_string(losPlayers[4].score) << "\nplayer\n" << losPlayers[5].name << "\n" << to_string(losPlayers[5].score) << "\nplayer\n" << losPlayers[6].name << "\n" << to_string(losPlayers[6].score) << "\nplayer\n" << losPlayers[7].name << "\n" << to_string(losPlayers[7].score) << "\nplayer\n" << losPlayers[8].name << "\n" << to_string(losPlayers[8].score) << "\nplayer\n" << losPlayers[9].name << "\n" << to_string(losPlayers[9].score) << "\n";
	outPutFile.close();
}

