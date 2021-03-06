#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <ncurses.h>

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;

string** crear();
void borrar(string** arreglo);
void nuevo(string**);
void imprimir(string**);
bool pawn(string**,int,int*);//mueve peon
bool tower(string**,int,int*);//mueve torre
bool horse(string**,int,int*);//mueve caballo
bool bishop(string**,int,int*);//mueve alfil
bool queen(string**,int,int*);//mueve reina
bool king(string**,int,int*);//mueve rey
void movimiento(char*,int*);//cambio el orden de como se ingresa el movimiento [A0A1] -> [0010]
bool validar(char*);
bool llamarpiezas(string** arreglo, int turno, bool jaque,int* moves);
bool isjaque(string**,int);
void guardar(string**,int,vector<string>& paths);
string** cargar(vector<string>& paths);
void guardarpaths(vector<string>&);
void abrirpaths(vector<string> paths);
bool jaquemate(string**,int);
bool confirmarjaque(string** tablero, int turno, int x, int y);
void comentarios(int,int*,string**);
void instrucciones();

int main(int argc, char const *argv[]){
	initscr();
	start_color();
	init_pair(1,COLOR_BLACK,COLOR_RED);
	init_pair(2,COLOR_GREEN,COLOR_YELLOW);
	init_pair(3,COLOR_BLUE,COLOR_MAGENTA);
	init_pair(4,COLOR_CYAN,COLOR_RED);
	init_pair(5,COLOR_RED,COLOR_BLACK);
	init_pair(6,COLOR_YELLOW,COLOR_GREEN);
	init_pair(7,COLOR_MAGENTA,COLOR_BLUE);
	init_pair(8,COLOR_RED,COLOR_CYAN);
	srand(time(NULL));
	bool jaque = false,jaquemates = false;
	bool valida = true;
	bool salir = false;
	bool neww = true;
	vector<string> paths;
	//abrirpaths(paths);
	int turno = 1;
	int menu = 3;
	char temp;
	int randomizer;
	string** tablero;
	char* moveltr = new char[4];//coordenadas con letras [A2A5]
	int* movenum = new int[4];//coordednadas con numeros [2050]
	instrucciones();
	while (menu){
		addstr("0-Salir\n1-Jugar Nueva partida\n2-Cargar partida\n");
		refresh();
		temp = getch();
		menu = temp -'0';
		if(menu == 2){
			clear();
			addstr("No logre cargar partidas sin embargo si se pueden guardar");
			refresh();
			getch();
			/*
			if(paths.size() == 0){
				addstr("No hay partidas guardadas se iniciara una nueva\n");	
				refresh();
				neww = true;
			}else{
				neww = false;
			}
			*/
			menu = 1;
			neww = true;
		}
		if(menu == 1){
			if(neww){
				tablero = crear();
				nuevo(tablero);
			}else{
				tablero = cargar(paths);
			}
			while(!jaquemates){
				if(turno == 1){
					jaque = isjaque(tablero,turno);
					if(jaque){
						addstr("Esta en jaque");
						refresh();
					}
					randomizer = rand() % 2 + 1;
					if(randomizer == 1){
						attrset(A_NORMAL);
						attron(A_BLINK | A_DIM);
					}else{
						attrset(A_NORMAL);
						attron(A_BOLD | A_STANDOUT);
					}
					randomizer = rand() % 8 + 1;
					attron(COLOR_PAIR(randomizer));
					imprimir(tablero);
					while (valida){
						addstr("PLAYER 1\nSi desea guardar la partida solo escriba 1\nAl guardar saldra de la partida\nSi desea salir y no guardar solo escriba 0\nPrimero ingrese la posicion de la pieza y luego a donde la quiere mover: ");
						refresh();
						getnstr(moveltr,4);
						if(moveltr[0] == '1'){
							salir = true;
							guardar(tablero,turno,paths);
							break;
						}else if(moveltr[0] == '0'){
							salir = true;
							break;
						}
						valida = validar(moveltr);
						if(valida){
							imprimir(tablero);
							addstr("Las letras tienen que estar en un rango de a hasta h\ny los numeros tienen que estar entre 0 y 7");
						}else{
							movimiento(moveltr,movenum);
							valida = llamarpiezas(tablero,turno,jaque,movenum);
							if (valida){
								imprimir(tablero);
								addstr("Movimiento incorrecto\n");
							}else{
								turno = 2;
								imprimir(tablero);
							}
						}
					}
					if(salir){
						break;
					}
					valida = true;
					jaquemates = jaquemate(tablero,2);
					if(jaquemates){
						addstr("PLAYER 1 HA GANADO\n");
					}
				}else if(turno == 2){
					jaque = isjaque(tablero,turno);
					if(jaque){
						addstr("Esta en jaque");
						refresh();
					}
					randomizer = rand() % 2 + 1;
					if(randomizer == 1){
						attrset(A_NORMAL);
						attron(A_BLINK | A_DIM);
					}else{
						attrset(A_NORMAL);
						attron(A_BOLD | A_STANDOUT);
					}
					randomizer = rand() % 4 + 1;
					attron(COLOR_PAIR(randomizer));
					imprimir(tablero);
					while (valida){
						addstr("PLAYER 2\nSi desea guardar la partida solo escriba 1\nAl guardar saldra de la partida\nSi desea salir y no guardar solo escriba 0\nPrimero ingrese la posicion de la pieza y luego a donde la quiere mover: ");
						getnstr(moveltr,4);
						if(moveltr[0] == '1'){
							salir = true;
							guardar(tablero,turno,paths);
							break;
						}else if(moveltr[0] == '0'){
							salir = true;
							break;
						}
						valida = validar(moveltr);
						if(valida){
							imprimir(tablero);
							addstr("Las letras tienen que estar en un rango de a hasta h\ny los numeros tienen que estar entre 0 y 7");
						}else{
							movimiento(moveltr,movenum);
							valida = llamarpiezas(tablero,turno,jaque,movenum);
							if (valida){
								imprimir(tablero);
								addstr("Movimiento incorrecto\n");
							}else{
								turno = 1;
								imprimir(tablero);
							}
						}
					}
					if(salir){
						break;
					}
					valida = true;
					jaquemates = jaquemate(tablero,1);
					if(jaquemates){
						addstr("PLAYER 2 HA GANADO\n");
					}
				}
			}
			borrar(tablero);
		}
	}
	delete[] moveltr;
	delete[] movenum;
	guardarpaths(paths);
	endwin();
	return 0;
}
void instrucciones(){
	addstr("Instrucciones del ajedrez:\nLas piezas estan nombradas de la siguiente forma: T1\nDonde T es el nombre de la pieza y 1 es a que jugador pertenece\nLas piezas son las siguientes:\nT  = Tower\nMe muevo en una linea recta para arriba o hacia abajo izquierda o derecha cuantos espacios quieras\nB = Bishop\nMe muevo en diagonal de izquierda a derecha o derecha a izquierda cuantos espacios quieras\nH = Horse\nMe muevo dos espacios para arriba o abajo, izquierda o derecha y luego 1 espacio para el lado\nQ = Queen\nMe muevo como Tower o Bishop\nP = Pawn\nMe puedo mover dos espacios en mi primer movimiento pero solo uno por el resto del juego siempre para adelante\nSe ingresa el movimiento de la siguiente manera A1A3 donde A es la columna donde esta ubicada la pieza y 1 es la fila donde esta ubicada\nEl proximo A es a que columna se va a mover y 3 es a que fila se va mover\n");
}
// TERMINA MAIN COMIENZA CREAR TABLERO-------------------------------------------------------------------
string** crear(){//crea un tablero
	string** tablero = new string*[8];
	for (int i = 0; i < 8; i++)
		tablero[i] = new string[8];
	return tablero;

}
// TERMINA CREAR TABLERO COMIENZA BORRAR TABLERO---------------------------------------------------------
void borrar(string** tablero){//borra el tablero dinamico
	for (int i = 0; i < 8; i++)
		delete[] tablero[i];
	delete[] tablero;
}
// TERMINA BORRAR TABLERO COMIENZA LLENAR TABLERO--------------------------------------------------------
void nuevo(string** arreglo){//crea un nuevo tablero con las piezas
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if ((i == 0 && j == 0) || (i == 0 && j == 7) || (i == 7 && j == 0) || (i == 7 && j == 7)){
				if(i == 0)
					arreglo[i][j] = "T1";
				else
					arreglo[i][j] = "T2";
			}else if((i == 0 && j == 1 ) || (i == 0 && j == 6) || (i == 7 && j == 1) || (i == 7 && j == 6) || (i == 7 && j == 6)){
				if(i == 0)
					arreglo[i][j] = "H1";
				else
					arreglo[i][j] = "H2";
			}else if((i == 0 && j == 2) || (i == 0 && j == 5) || (i == 7 && j == 2) || (i == 7 && j == 5)){
				if(i == 0)
					arreglo[i][j] = "B1";
				else
					arreglo[i][j] = "B2";
			}else if((i == 0 && j == 3) || (i == 7 && j == 3)){
				if(i == 0)
					arreglo[i][j] = "Q1";
				else
					arreglo[i][j] = "Q2";
			}else if((i == 0 && j == 4) || (i == 7 && j == 4)){
				if(i == 0)
					arreglo[i][j] = "K1";
				else
					arreglo[i][j] = "K2";
			}else if(i == 1 || i == 6){
				if(i == 1)
					arreglo[i][j] = "P1";
				else
					arreglo[i][j] = "P2";
			}else{
				arreglo[i][j] = "++";
			}
		}
	}

}
// TERMINA LLENAR TABLERO COMIENZA IMPRESION DE TABLERO--------------------------------------------------
void imprimir(string** arreglo){//imprime tablero
	clear();
	addstr("- A--B--C--D--E--F--G--H\n\n");
	for (int i = 0; i < 8; i++){
		if(i == 0){
			addstr("0 ");
		}else if(i == 1){
			addstr("1 ");
		}else if(i == 2){
			addstr("2 ");
		}else if(i == 3){
			addstr("3 ");
		}else if(i == 4){
			addstr("4 ");
		}else if(i == 5){
			addstr("5 ");
		}else if(i == 6){
			addstr("6 ");
		}else if(i == 7){
			addstr("7 ");
		}
		for (int j = 0; j < 8; j++){
			if(arreglo[i][j] == "T1"){
				addstr("T1-");	
			}else if(arreglo[i][j] == "B1"){
				addstr("B1-");	
			}else if(arreglo[i][j] == "H1"){
				addstr("H1-");	
			}else if(arreglo[i][j] == "Q1"){
				addstr("Q1-");	
			}else if(arreglo[i][j] == "K1"){
				addstr("K1-");	
			}else if(arreglo[i][j] == "P1"){
				addstr("P1-");	
			}else if(arreglo[i][j] == "T2"){
				addstr("T2-");	
			}else if(arreglo[i][j] == "B2"){
				addstr("B2-");	
			}else if(arreglo[i][j] == "H2"){
				addstr("H2-");	
			}else if(arreglo[i][j] == "Q2"){
				addstr("Q2-");	
			}else if(arreglo[i][j] == "K2"){
				addstr("K2-");	
			}else if(arreglo[i][j] == "P2"){
				addstr("P2-");	
			}else if(arreglo[i][j] == "++"){
				addstr("++-");	
			}
		}
		if(i == 0){
			addstr(" 0\n");
		}else if(i == 1){
			addstr(" 1\n");
		}else if(i == 2){
			addstr(" 2\n");
		}else if(i == 3){
			addstr(" 3\n");
		}else if(i == 4){
			addstr(" 4\n");
		}else if(i == 5){
			addstr(" 5\n");
		}else if(i == 6){
			addstr(" 6\n");
		}else if(i == 7){
			addstr(" 7\n");
		}
	}
	addstr("\n- A--B--C--D--E--F--G--H\n");
}
// TERMINA IMRPRESION DE TABLERO COMIENZA VALIDAR COORDENADAS--------------------------------------------
bool validar(char* move){//validacion de coordenadas
	bool valida = false;
	char j1 = move[0],j2 = move[2];
	int moves[4];//para organizarme mejor
	moves[0] = move[1] - '0';//asigno el primer i del movimiento
	moves[2] = move[3] - '0';//asigno el segundo
	if(moves[0] >= 8 || moves[0] < 0 || moves[2] >= 8 || moves[2] < 0){
		valida = true;
	}
	if (j1 == 'a' || j1 == 'A'){
		moves[1] = 0;
	}else if (j1 == 'b' || j1 == 'B'){
		moves[1] = 1;
	}else if (j1 == 'c' || j1 == 'C'){
		moves[1] = 2;
	}else if (j1 == 'd' || j1 == 'D'){
		moves[1] = 3;
	}else if (j1 == 'e' || j1 == 'E'){
		moves[1] = 4;
	}else if (j1 == 'f' || j1 == 'F'){
		moves[1] = 5;
	}else if (j1 == 'g' || j1 == 'G'){
		moves[1] = 6;
	}else if (j1 == 'h' || j1 == 'H'){
		moves[1] = 7;
	}else{
		valida = true;
	}
	if (j2 == 'a' || j2 == 'A'){
		moves[3] = 0;
	}else if (j2 == 'b' || j2 == 'B'){
		moves[3] = 1;
	}else if (j2 == 'c' || j2 == 'C'){
		moves[3] = 2;
	}else if (j2 == 'd' || j2 == 'D'){
		moves[3] = 3;
	}else if (j2 == 'e' || j2 == 'E'){
		moves[3] = 4;
	}else if (j2 == 'f' || j2 == 'F'){
		moves[3] = 5;
	}else if (j2 == 'g' || j2 == 'G'){
		moves[3] = 6;
	}else if (j2 == 'h' || j2 == 'H'){
		moves[3] = 7;
	}else{
		valida = true;
	}
	return valida;
}
// TERMINA VALIDAR COORDENADAS COMIENZA MOVIMIENTO-------------------------------------------------------
void movimiento(char* move,int* moves){//regresa las coordenadas con numeros en un arreglo
	char j1 = move[0],j2 = move[2];
	moves[0] = move[1] - '0';
	moves[2] = move[3] - '0';
	if (j1 == 'a' || j1 == 'A'){
		moves[1] = 0;
	}else if (j1 == 'b' || j1 == 'B'){
		moves[1] = 1;
	}else if (j1 == 'c' || j1 == 'C'){
		moves[1] = 2;
	}else if (j1 == 'd' || j1 == 'D'){
		moves[1] = 3;
	}else if (j1 == 'e' || j1 == 'E'){
		moves[1] = 4;
	}else if (j1 == 'f' || j1 == 'F'){
		moves[1] = 5;
	}else if (j1 == 'g' || j1 == 'G'){
		moves[1] = 6;
	}else if (j1 == 'h' || j1 == 'H'){
		moves[1] = 7;
	}
	if (j2 == 'a' || j2 == 'A'){
		moves[3] = 0;
	}else if (j2 == 'b' || j2 == 'B'){
		moves[3] = 1;
	}else if (j2 == 'c' || j2 == 'C'){
		moves[3] = 2;
	}else if (j2 == 'd' || j2 == 'D'){
		moves[3] = 3;
	}else if (j2 == 'e' || j2 == 'E'){
		moves[3] = 4;
	}else if (j2 == 'f' || j2 == 'F'){
		moves[3] = 5;
	}else if (j2 == 'g' || j2 == 'G'){
		moves[3] = 6;
	}else if (j2 == 'h' || j2 == 'H'){
		moves[3] = 7;
	}
}
// TERMINA MOVIMIENTO COMIENZA LLAMAR PIEZAS-------------------------------------------------------------
bool llamarpiezas(string** arreglo, int turno, bool jaque,int* moves){
	bool valida = false;
	int mayor,menor;
	if(turno == 1){
		if (arreglo[moves[0]][moves[1]] == "T1"){
			valida = tower(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "H1"){
			valida = horse(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "B1"){
			valida = bishop(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "Q1"){
			valida = queen(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "K1"){
			valida = king(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "P1"){
			if(moves[0] == 7){
				addstr("A que tipo de pieza desea convertir su peon\n1-Tower\n2-Bishop\n3-Queen\n4-Horse");
			refresh();
			char temp = getch();
			int opcion = temp - '0';
				if(opcion == 1){
					arreglo[moves[0]][moves[1]] = "T1";	
					valida = false;
				}else if(opcion == 2){
					arreglo[moves[0]][moves[1]] = "B1";
					valida = false;
				}else if(opcion == 3){
					arreglo[moves[0]][moves[1]] = "Q1";
					valida = false;
				}else if(opcion == 4){
					arreglo[moves[0]][moves[1]] = "H1";
					valida = false;
				}
			}else{
				valida = pawn(arreglo,turno,moves);
				comentarios(turno,moves,arreglo);
			}
		}else{
			valida = true;
		}
	}else if (turno == 2){
		if (arreglo[moves[0]][moves[1]] == "T2"){
			valida = tower(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "H2"){
			valida = horse(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "B2"){
			valida = bishop(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "Q2"){
			valida = queen(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "K2"){
			valida = king(arreglo,turno,moves);
			comentarios(turno,moves,arreglo);
		}else if (arreglo[moves[0]][moves[1]] == "P2"){
			if(moves[0] == 0){
				addstr("A que tipo de pieza desea convertir su peon\n1-Tower\n2-Bishop\n3-Queen\n4-Horse");
			refresh();
			char temp = getch();
			int opcion = temp - '0';
				if(opcion == 1){
					arreglo[moves[0]][moves[1]] = "T2";	
					valida = false;
				}else if(opcion == 2){
					arreglo[moves[0]][moves[1]] = "B2";
					valida = false;
				}else if(opcion == 3){
					arreglo[moves[0]][moves[1]] = "Q2";
					valida = false;
				}else if(opcion == 4){
					arreglo[moves[0]][moves[1]] = "H2";
					valida = false;
				}
			}else{
				valida = pawn(arreglo,turno,moves);
				comentarios(turno,moves,arreglo);
			}
		}else{
			valida = true;
		}
	}else{
		valida = true;
	}
	return valida;
}
// TERMINA EL LLAMADO A LAS PIEZA AHORA COMIENZA EL MOVIMIENTO DEL PEON----------------------------------
bool pawn(string** tablero,int turno,int* moves){
	bool valida = false;
	int i1 = moves[0], j1 = moves[1], i2 = moves[2], j2 = moves[3];
	if (turno == 1){
		if (j1 == j2 && i2 == (i1 + 1) && tablero[i2][j2] == "++" && tablero[i1][j1] == "P1"){
			//movimiento normal
			tablero[i1][j1] = "++";
			tablero[i2][j2] = "P1";
		}else if ((j2 == (j1 + 1) || j2 == (j1 - 1)) && i2 == (i1 + 1) && (tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2") && tablero[i1][j1] == "P1"){
			//comer
			tablero[i2][j2] = "P1";
			tablero[i1][j1] = "++";
		}else if(j1 == j2 && i2 == 3 && tablero[i1][j1] == "P1" && tablero[i2][j2] == "++" && i1 == 1 && tablero[2][j1] == "++"){
			//moverse dos espacios
			tablero[i2][j2] = "P1";
			tablero[i1][j1] = "++";
		}else{
			valida = true;
		}
	}else if(turno == 2){
		if (j1 == j2 && i2 == (i1 - 1) && tablero[i2][j2] == "++" && tablero[i1][j1] == "P2"){
			//movimiento normal
			tablero[i1][j1] = "++";
			tablero[i2][j2] = "P2";
		}else if ((j2 == (j1 + 1) || j2 == (j1 - 1)) && i2 == (i1 - 1) && (tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1") && tablero[i1][j1] == "P2"){
			//comer
			tablero[i2][j2] = "P2";
			tablero[i1][j1] = "++";
		}else if(j1 == j2 && i2 == 4 && tablero[i1][j1] == "P2" && tablero[i2][j2] == "++" && i1 == 6 && tablero[5][j1] == "++"){
			//moverse dos espacios
			tablero[i2][j2] = "P2";
			tablero[i1][j1] = "++";
		}else{
			valida = true;
		}
	}else{
		valida = true;
	}
	return valida;
}
// TERMINA EL MOVIMIENTO DEL PEON COMIENZA EL DE LA TORRE ------------------------------------------------
bool tower(string** tablero,int turno,int* moves){
	bool valida = false;
	int mayor,menor,x,y;
	if (turno == 1){
		if(moves[0] == moves[2]){//comparo las dos i movimiento horizontal
			x = moves[0];
			if (moves[1] > moves[3]){//movimiento de torre hacia la izquierda
				mayor = moves[1];
				menor = moves[3];
				mayor -= 1;
				for (mayor; mayor > menor; mayor--){
					if (tablero[x][mayor] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[x][mayor] == "T2" || tablero[x][mayor] == "H2" || tablero[x][mayor] == "B2" || tablero[x][mayor] == "Q2" || tablero[x][mayor] == "P2" || tablero[x][mayor] == "++"){
						tablero[x][mayor] = "T1";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else if(moves[1] < moves[3]){//movimiento de torre hacia la derecha
				mayor = moves[3];
				menor = moves[1];
				menor += 1;
				for (menor; menor < mayor; menor++){
					if (tablero[x][menor] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[x][menor] == "T2" || tablero[x][menor] == "H2" || tablero[x][menor] == "B2" || tablero[x][menor] == "Q2" || tablero[x][menor] == "P2" || tablero[x][menor] == "++"){
						tablero[x][mayor] = "T1";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else{
				valida = true;
			}
		}else if(moves[1] == moves[3]){//comparo las dos j movimiento vertical
			y = moves[1];
			if (moves[0] > moves[2]){//movimiento de torre hacia abajo
				mayor = moves[0];
				menor = moves[2];
				mayor -= 1;
				for (mayor; mayor > menor; mayor--){
					if (tablero[mayor][y] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[mayor][y] == "T2" || tablero[mayor][y] == "H2" || tablero[mayor][y] == "B2" || tablero[mayor][y] == "Q2" || tablero[mayor][y] == "P2" || tablero[mayor][y] == "++"){
						tablero[mayor][y] = "T1";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else if(moves[0] < moves[2]){//movimiento de torre hacia arriba
				mayor = moves[2];
				menor = moves[0];
				menor += 1;
				for (menor; menor < mayor; menor++){
					if (tablero[menor][y] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[menor][y] == "T2" || tablero[menor][y] == "H2" || tablero[menor][y] == "B2" || tablero[menor][y] == "Q2" || tablero[menor][y] == "P2" || tablero[menor][y] == "++"){
						tablero[menor][y] = "T1";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}else if (turno == 2){
		if(moves[0] == moves[2]){//comparo las dos i movimiento horizontal
			x = moves[0];
			if (moves[1] > moves[3]){//movimiento de torre hacia la izquierda
				mayor = moves[1];
				menor = moves[3];
				mayor -= 1;
				for (mayor; mayor > menor; mayor--){
					if (tablero[x][mayor] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[x][mayor] == "T1" || tablero[x][mayor] == "H1" || tablero[x][mayor] == "B1" || tablero[x][mayor] == "Q1" || tablero[x][mayor] == "P1" || tablero[x][mayor] == "++"){
						tablero[x][mayor] = "T2";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else if(moves[1] < moves[3]){//movimiento de torre hacia la derecha
				mayor = moves[3];
				menor = moves[1];
				menor += 1;
				for (menor; menor < mayor; menor++){
					if (tablero[x][menor] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[x][menor] == "T1" || tablero[x][menor] == "H1" || tablero[x][menor] == "B1" || tablero[x][menor] == "Q1" || tablero[x][menor] == "P1" || tablero[x][menor] == "++"){
						tablero[x][mayor] = "T2";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else{
				valida = true;
			}
		}else if(moves[1] == moves[3]){//comparo las dos j movimiento vertical
			y = moves[1];
			if (moves[0] > moves[2]){//movimiento de torre hacia abajo
				mayor = moves[0];
				menor = moves[2];
				mayor -= 1;
				for (mayor; mayor > menor; mayor--){
					if (tablero[mayor][y] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[mayor][y] == "T1" || tablero[mayor][y] == "H1" || tablero[mayor][y] == "B1" || tablero[mayor][y] == "Q1" || tablero[mayor][y] == "P1" || tablero[mayor][y] == "++"){
						tablero[mayor][y] = "T2";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else if(moves[0] < moves[2]){//movimiento de torre hacia arriba
				mayor = moves[2];
				menor = moves[0];
				menor += 1;
				for (menor; menor < mayor; menor++){
					if (tablero[menor][y] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[menor][y] == "T1" || tablero[menor][y] == "H1" || tablero[menor][y] == "B1" || tablero[menor][y] == "Q1" || tablero[menor][y] == "P1" || tablero[menor][y] == "++"){
						tablero[menor][y] = "T2";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}
	return valida;
}
// TERMINA EL MOVIMIENTO DE LA TORRE AHORA SIGUE EL CABALLO --------------------------------------------
bool horse(string** tablero,int turno,int* moves){
	int i1 = moves[0], j1 = moves[1], i2 = moves[2], j2 = moves[3];
	bool valida;
	if (turno == 1){
		if(((j1 == j2 + 1 || j1 == j2 - 1) && (i1 == i2 + 2 || i1 == i2 - 2)) || ((j1 == j2 + 2 || j1 == j2 - 2) && (i1 == i2 + 1 || i1 == i2 - 1))){
			if(tablero[i1][j1] == "H2" && (tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++")){
				tablero[i2][j2] = "H1";
				tablero[i1][j1] = "++";
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}else if(turno == 2){
		if(((j1 == j2 + 1 || j1 == j2 - 1) && (i1 == i2 + 2 || i1 == i2 - 2)) || ((j1 == j2 + 2 || j1 == j2 - 2) && (i1 == i2 + 1 || i1 == i2 - 1))){
			if(tablero[i1][j1] == "H1" && (tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++")){
				tablero[i2][j2] = "H2";
				tablero[i1][j1] = "++";
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}else{
		valida = false;
	}
	return valida;
}
//TERMINA EL MOVIMIENTO DEL CABALLO COMIENZA EL ALFIL---------------------------------------------------
bool bishop(string** tablero,int turno,int* moves){
	int i1 = moves[0], j1 = moves[1], i2 = moves[2], j2 = moves[3];	
	bool valida = false;	
	if (turno == 1){
		if(abs(i1 - i2) == abs(j1 - j2)){
			if(j1 < j2 && i1 > i2){//movimiento diagonal a la derecha-arriba
				j1 += 1;
				for(int i = j1; i < j2; i++){
					i1 -= 1;
					if(tablero[i1][i] != "++"){//verificar que no hayan piezas en el camino
						valida = true;
						break;
					}
				}
				if(!valida){
					if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "B1";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else if(j1 > j2 && i1 < i2){//movimiento diagonal a la izquierda-abajo
				i1 += 1;
				for(int i = i1; i < i2; i++){
					j1 -= 1;
					if(tablero[i][j1] != "++"){
						valida = true;
						break;
					}
				}
				if(valida){
				}else{
					if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "B1";
						tablero[moves[0]][moves[1]] = "++";
					}
				}					
			}else if(j1 > j2 && i1 > i2){//movimiento diagonal izquierda-arriba
				i1 -= 1;
				for(i1; i1 > i2; i1--){
					j1 -= 1;
					if(tablero[i1][j1] != "++"){
						valida = true;
						break;
					}	
				}
				if(!valida){
					if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "B1";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else if(j1 < j2 && i1 < i2){//movimiento diagonal derecha-abajo
				i1 += 1;
				for(i1; i1 < i2; i1++){
					j1 += 1;
					if(tablero[i1][j1] != "++"){
						valida = true;
						break;
					}	
				}
				if(!valida){
					if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "B1";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}else if(turno == 2){
		if(abs(i1 - i2) == abs(j1 - j2)){
			if(j1 < j2 && i1 > i2){//movimiento diagonal a la derecha-arriba
				j1 += 1;
				for(int i = j1; i < j2; i++){
					i1 -= 1;
					if(tablero[i1][i] != "++"){//verificar que no hayan piezas en el camino
						valida = true;
						break;
					}
				}
				if(!valida){
					if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "B2";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else if(j1 > j2 && i1 < i2){//movimiento diagonal a la izquierda-abajo
				i1 += 1;
				for(int i = i1; i < i2; i++){
					j1 -= 1;
					if(tablero[i][j1] != "++"){
						valida = true;
						break;
					}
				}
				if(!valida){
					if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "B2";
						tablero[moves[0]][moves[1]] = "++";
					}
				}					
			}else if(j1 > j2 && i1 > i2){//movimiento diagonal izquierda-arriba
				i1 -= 1;
				for(i1; i1 > i2; i1--){
					j1 -= 1;
					if(tablero[i1][j1] != "++"){
						valida = true;
						break;
					}	
				}
				if(!valida){
					if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "B2";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else if(j1 < j2 && i1 < i2){//movimiento diagonal derecha-abajo
				i1 += 1;
				for(i1; i1 < i2; i1++){
					j1 += 1;
					if(tablero[i1][j1] != "++"){
						valida = true;
						break;
					}	
				}
				if(!valida){
					if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "B2";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}else{
		valida = false;
	}
	return valida;
		
}
//TERMINA MOVIMIENTO DE ALFIL COMIENZA MOVIMIENTO DE REINA----------------------------------------------
bool queen(string** tablero,int turno,int* moves){
	int i1 = moves[0], j1 = moves[1], i2 = moves[2], j2 = moves[3];	
	int sumaini = i1 + j1,sumafin = i2 + j2;
	int mayor,menor,x,y;
	bool valida = false;	
	if(turno == 1){
		if(moves[0] == moves[2] && moves[1] != moves[3]){//comparo las dos i movimiento horizontal
			x = moves[0];
			if (moves[1] > moves[3]){//movimiento de reina hacia la izquierda
				mayor = moves[1];
				menor = moves[3];
				mayor -= 1;
				for (mayor; mayor > menor; mayor--){
					if (tablero[x][mayor] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[x][mayor] == "T2" || tablero[x][mayor] == "H2" || tablero[x][mayor] == "B2" || tablero[x][mayor] == "Q2" || tablero[x][mayor] == "P2" || tablero[x][mayor] == "++"){
						tablero[x][mayor] = "Q1";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else if(moves[1] < moves[3]){//movimiento de reina hacia la derecha
				mayor = moves[3];
				menor = moves[1];
				menor += 1;
				for (menor; menor < mayor; menor++){
					if (tablero[x][menor] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[x][menor] == "T2" || tablero[x][menor] == "H2" || tablero[x][menor] == "B2" || tablero[x][menor] == "Q2" || tablero[x][menor] == "P2" || tablero[x][menor] == "++"){
						tablero[x][mayor] = "Q1";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else{
				valida = true;
			}
		}else if(moves[1] == moves[3] && moves[0] != moves[2]){//comparo las dos j movimiento vertical
			y = moves[1];
			if (moves[0] > moves[2]){//movimiento de reina hacia abajo
				mayor = moves[0];
				menor = moves[2];
				mayor -= 1;
				for (mayor; mayor > menor; mayor--){
					if (tablero[mayor][y] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[mayor][y] == "T2" || tablero[mayor][y] == "H2" || tablero[mayor][y] == "B2" || tablero[mayor][y] == "Q2" || tablero[mayor][y] == "P2" || tablero[mayor][y] == "++"){
						tablero[mayor][y] = "Q1";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else if(moves[0] < moves[2]){//movimiento de reina hacia arriba
				mayor = moves[2];
				menor = moves[0];
				menor += 1;
				for (menor; menor < mayor; menor++){
					if (tablero[menor][y] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[menor][y] == "T2" || tablero[menor][y] == "H2" || tablero[menor][y] == "B2" || tablero[menor][y] == "Q2" || tablero[menor][y] == "P2" || tablero[menor][y] == "++"){
						tablero[menor][y] = "Q1";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else{
				valida = true;
			}
		}else if(abs(i1 - i2) == abs(j1 - j2)){
			if(j1 < j2 && i1 > i2){//movimiento diagonal a la derecha-arriba
				j1 += 1;
				for(int i = j1; i < j2; i++){
					i1 -= 1;
					if(tablero[i1][i] != "++"){//verificar que no hayan piezas en el camino
						valida = true;
						break;
					}
				}
				if(!valida){
					if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "Q1";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else if(j1 > j2 && i1 < i2){//movimiento diagonal a la izquierda-abajo
				i1 += 1;
				for(int i = i1; i < i2; i++){
					j1 -= 1;
					if(tablero[i][j1] != "++"){
						valida = true;
						break;
					}
				}
				if(!valida){
					if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "Q1";
						tablero[moves[0]][moves[1]] = "++";
					}
				}					
			}else if(j1 > j2 && i1 > i2){//movimiento diagonal izquierda-arriba
				i1 -= 1;
				for(i1; i1 > i2; i1--){
					j1 -= 1;
					if(tablero[i1][j1] != "++"){
						valida = true;
						break;
					}	
				}
				if(!valida){
					if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "Q1";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else if(j1 < j2 && i1 < i2){//movimiento diagonal derecha-abajo
				i1 += 1;
				for(i1; i1 < i2; i1++){
					j1 += 1;
					if(tablero[i1][j1] != "++"){
						valida = true;
						break;
					}	
				}
				if(!valida){
					if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "Q1";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}else if(turno == 2){
		if(moves[0] == moves[2] && moves[1] != moves[3]){//comparo las dos i movimiento horizontal
			x = moves[0];
			if (moves[1] > moves[3]){//movimiento de reina hacia la izquierda
				mayor = moves[1];
				menor = moves[3];
				mayor -= 1;
				for (mayor; mayor > menor; mayor--){
					if (tablero[x][mayor] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[x][mayor] == "T1" || tablero[x][mayor] == "H1" || tablero[x][mayor] == "B1" || tablero[x][mayor] == "Q1" || tablero[x][mayor] == "P1" || tablero[x][mayor] == "++"){
						tablero[x][mayor] = "Q2";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else if(moves[1] < moves[3]){//movimiento de reina hacia la derecha
				mayor = moves[3];
				menor = moves[1];
				menor += 1;
				for (menor; menor < mayor; menor++){
					if (tablero[x][menor] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[x][menor] == "T1" || tablero[x][menor] == "H1" || tablero[x][menor] == "B1" || tablero[x][menor] == "Q1" || tablero[x][menor] == "P1" || tablero[x][menor] == "++"){
						tablero[x][mayor] = "Q2";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else{
				valida = true;
			}
		}else if(moves[1] == moves[3] && moves[0] != moves[2]){//comparo las dos j movimiento vertical
			y = moves[1];
			if (moves[0] > moves[2]){//movimiento de reina hacia abajo
				mayor = moves[0];
				menor = moves[2];
				mayor -= 1;
				for (mayor; mayor > menor; mayor--){
					if (tablero[mayor][y] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[mayor][y] == "T1" || tablero[mayor][y] == "H1" || tablero[mayor][y] == "B1" || tablero[mayor][y] == "Q1" || tablero[mayor][y] == "P1" || tablero[mayor][y] == "++"){
						tablero[mayor][y] = "Q2";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else if(moves[0] < moves[2]){//movimiento de reina hacia arriba
				mayor = moves[2];
				menor = moves[0];
				menor += 1;
				for (menor; menor < mayor; menor++){
					if (tablero[menor][y] != "++"){
						valida = true;
						break;
					}
				}
				if(valida == false){
					if (tablero[menor][y] == "T1" || tablero[menor][y] == "H1" || tablero[menor][y] == "B1" || tablero[menor][y] == "Q1" || tablero[menor][y] == "P1" || tablero[menor][y] == "++"){
						tablero[menor][y] = "Q2";
						tablero[moves[0]][moves[1]] = "++";
					}else{
						valida = true;
					}
				}
			}else{
				valida = true;
			}
		}else if(abs(i1 - i2) == abs(j1 - j2)){
			if(j1 < j2 && i1 > i2){//movimiento diagonal a la derecha-arriba
				j1 += 1;
				for(int i = j1; i < j2; i++){
					i1 -= 1;
					if(tablero[i1][i] != "++"){//verificar que no hayan piezas en el camino
						valida = true;
						break;
					}
				}
				if(!valida){
					if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "Q2";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else if(j1 > j2 && i1 < i2){//movimiento diagonal a la izquierda-abajo
				i1 += 1;
				for(int i = i1; i < i2; i++){
					j1 -= 1;
					if(tablero[i][j1] != "++"){
						valida = true;
						break;
					}
				}
				if(!valida){
					if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "Q2";
						tablero[moves[0]][moves[1]] = "++";
					}
				}					
			}else if(j1 > j2 && i1 > i2){//movimiento diagonal izquierda-arriba
				i1 -= 1;
				for(i1; i1 > i2; i1--){
					j1 -= 1;
					if(tablero[i1][j1] != "++"){
						valida = true;
						break;
					}	
				}
				if(!valida){
					if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "Q2";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else if(j1 < j2 && i1 < i2){//movimiento diagonal derecha-abajo
				i1 += 1;
				for(i1; i1 < i2; i1++){
					j1 += 1;
					if(tablero[i1][j1] != "++"){
						valida = true;
						break;
					}	
				}
				if(!valida){
					if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
						tablero[i2][j2] = "Q2";
						tablero[moves[0]][moves[1]] = "++";
					}
				}
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}
	return valida;
}
bool king(string** tablero,int turno,int* moves){
	int i1  = moves[0],j1 = moves[1],i2 = moves[2],j2 = moves[3];
	bool valida = false;
	if(turno == 1){
		if(j1 == j2){
			if((i1 + 1) == i2){//movimiento hacia abajo
				if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
					tablero[i2][j2] = "K1";
					tablero[moves[0]][moves[1]] = "++";
				}else{
					valida = true;
				}
			}else if((i1 - 1) == i2){//movimiento hacia arriba
				if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
					tablero[i2][j2] = "K1";
					tablero[moves[0]][moves[1]] = "++";
				}else{
					valida = true;
				}
			}else{
				valida = true;
			}
		}else if(i1 == i2){
			if((j1 + 1) == j2){//movimiento hacia la derecha
				if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
					tablero[i2][j2] = "K1";
					tablero[moves[0]][moves[1]] = "++";
				}else{
					valida = true;
				}
			}else if((j1 - 1) == j2){//movimiento hacia la izquierda
				if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
					tablero[i2][j2] = "K1";
					tablero[moves[0]][moves[1]] = "++";
				}else{
					valida = true;
				}
			}else{
				valida = true;
			}
		}else if(((i1 - 1) == i2) && ((j1 - 1) == j2)){//movimiento en diagonal izquierda arriba
			if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
				tablero[i2][j2] = "K1";
				tablero[moves[0]][moves[1]] = "++";
			}else{
				valida = true;
			}
		}else if(((i1 + 1) == i2) && ((j1 + 1) == j2)){//movimiento en diagonal derecha abajo
			if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
				tablero[i2][j2] = "K1";
				tablero[moves[0]][moves[1]] = "++";
			}else{
				valida = true;
			}
		}else if(((i1 + 1) == i2) && ((j1 - 1) == j2)){//movimiento en diagonal izquierda abajo
			if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
				tablero[i2][j2] = "K1";
				tablero[moves[0]][moves[1]] = "++";
			}else{
				valida = true;
			}
		}else if(((i1 - 1) == i2) && ((j1 + 1) == j2)){//movimiento en diagonal derecha arriba
			if(tablero[i2][j2] == "T2" || tablero[i2][j2] == "H2" || tablero[i2][j2] == "B2" || tablero[i2][j2] == "Q2" || tablero[i2][j2] == "P2" || tablero[i2][j2] == "++"){
				tablero[i2][j2] = "K1";
				tablero[moves[0]][moves[1]] = "++";
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}	
	}else if(turno == 2){
		if(j1 == j2){
			if((i1 + 1) == i2){
				if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
					tablero[i2][j2] = "K2";
					tablero[moves[0]][moves[1]] = "++";
				}else{
					valida = true;
				}
			}else if((i1 - 1) == i2){
				if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
					tablero[i2][j2] = "K2";
					tablero[moves[0]][moves[1]] = "++";
				}else{
					valida = true;
				}
			}else{
				valida = true;
			}
		}else if(i1 == i2){
			if((j1 + 1) == j2){
				if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
					tablero[i2][j2] = "K2";
					tablero[moves[0]][moves[1]] = "++";
				}else{
					valida = true;
				}
			}else if((j1 - 1) == j2){
				if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
					tablero[i2][j2] = "K2";
					tablero[moves[0]][moves[1]] = "++";
				}else{
					valida = true;
				}
			}else{
				valida = true;
			}
		}else if(((i1 - 1) == i2) && ((j1 - 1) == j2)){
			if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
				tablero[i2][j2] = "K2";
				tablero[moves[0]][moves[1]] = "++";
			}else{
				valida = true;
			}
		}else if(((i1 + 1) == i2) && ((j1 + 1) == j2)){
			if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
				tablero[i2][j2] = "K2";
				tablero[moves[0]][moves[1]] = "++";
			}else{
				valida = true;
			}
		}else if(((i1 + 1) == i2) && ((j1 - 1) == j2)){
			if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
				tablero[i2][j2] = "K2";
				tablero[moves[0]][moves[1]] = "++";
			}else{
				valida = true;
			}
		}else if(((i1 - 1) == i2) && ((j1 + 1) == j2)){
			if(tablero[i2][j2] == "T1" || tablero[i2][j2] == "H1" || tablero[i2][j2] == "B1" || tablero[i2][j2] == "Q1" || tablero[i2][j2] == "P1" || tablero[i2][j2] == "++"){
				tablero[i2][j2] = "K2";
				tablero[moves[0]][moves[1]] = "++";
			}else{
				valida = true;
			}
		}else{
			valida = true;
		}
	}else{
		valida = true;
	}
	return valida;
}
//TERMINA MOVIMIENTO DE REY EMPIEZA FUNCION DE JAQUE------------------------------------------------
bool isjaque(string** tablero,int turno){
	bool salir = false;
	int x,y;
	if(turno == 1){
		for(int i = 0; i < 8; i++){
			if(salir){
				break;
			}
			for(int j = 0; j < 8; j++){//busco donde esta el rey
				if(tablero[i][j] == "K1"){
					x = i;
					y = j;
					salir = true;
					break;
				}
			}
		}
		for(int i = x; i < 8; i++){//buscar verticalmente hacia abajo si hay alguna amenaza
			if(tablero[i][y] == "T2" || tablero[i][y] == "Q2"){
				return true;
			}else if (tablero[i][y] == "T1" || tablero[i][y] == "H1" || tablero[i][y] == "B1" || tablero[i][y] == "Q1" || tablero[i][y] == "P1" || tablero[i][y] == "B2" || tablero[i][y] == "H2" || tablero[i][y] == "P2" || tablero[i][y] == "K2"){
				break;
			}
		}
		for(int i = x; i >= 0; i--){//buscar verticalmente hacia arriba
			if(tablero[i][y] == "T2" || tablero[i][y] == "Q2"){
				return true;
			}else if (tablero[i][y] == "T1" || tablero[i][y] == "H1" || tablero[i][y] == "B1" || tablero[i][y] == "Q1" || tablero[i][y] == "P1" || tablero[i][y] == "B2" || tablero[i][y] == "H2" || tablero[i][y] == "P2" || tablero[i][y] == "K2"){
				break;
			}
		}
		for(int i = y; i < 8; i++){//busco horizontalmente hacia la derecha
			if(tablero[x][i] == "T2" || tablero[x][i] == "Q2"){
				return true;
			}else if (tablero[x][i] == "T1" || tablero[x][i] == "H1" || tablero[x][i] == "B1" || tablero[x][i] == "Q1" ||tablero[x][i] == "P1" || tablero[x][i] == "B2" || tablero[x][i] == "H2" || tablero[x][i] == "P2" || tablero[x][i] == "K2"){
				break;
			}
		}
		for(int i = y; i >= 0; i--){//busco horizontalmente hacia la izquierda
			if(tablero[x][i] == "T2" || tablero[x][i] == "Q2"){
				return true;
			}else if (tablero[x][i] == "T1" || tablero[x][i] == "H1" || tablero[x][i] == "B1" || tablero[x][i] == "Q1" ||tablero[x][i] == "P1" || tablero[x][i] == "B2" || tablero[x][i] == "H2" || tablero[x][i] == "P2" || tablero[x][i] == "K2"){
				break;
			}
		}
		int x2 = x,y2 = y;
		while(true){//verificar diagonal derecha arriba
			if(x2 > 0 && y2 < 7){
				x2 -= 1;
				y2 += 1;
				if(tablero[x2][y2] == "B2" || tablero[x2][y] == "Q2"){
					return true;
				}else if (tablero[x2][y2] == "T1" || tablero[x2][y2] == "H1" ||tablero[x2][y2] == "B1" || tablero[x2][y2] == "Q1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "T2" ||tablero[x2][y2] == "H2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "K2"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal derecha abajo
			if(x2 < 7 && y2 < 7){
				x2 += 1;
				y2 += 1;
				if(tablero[x2][y2] == "B2" || tablero[x2][y] == "Q2"){
					return true;
				}else if (tablero[x2][y2] == "T1" || tablero[x2][y2] == "H1" ||tablero[x2][y2] == "B1" || tablero[x2][y2] == "Q1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" || tablero[x2][y2] == "K2"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal izquierda arriba
			if(x2 > 0 && y2 > 0){
				x2 -= 1;
				y2 -= 1;
				if(tablero[x2][y2] == "B2" || tablero[x2][y] == "Q2"){
					return true;
				}else if (tablero[x2][y2] == "T1" || tablero[x2][y2] == "H1" ||tablero[x2][y2] == "B1" || tablero[x2][y2] == "Q1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "K2"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal izquierda abajo
			if(x2 < 7 && y2 > 0){
				x2 += 1;
				y2 -= 1;
				if(tablero[x2][y2] == "B2" || tablero[x2][y] == "Q2"){
					return true;
				}else if (tablero[x2][y2] == "T1" || tablero[x2][y2] == "H1" ||tablero[x2][y2] == "B1" || tablero[x2][y2] == "Q1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" || tablero[x2][y2] == "K2"){
					break;
				}
			}else{
				break;
			}
		}
		if(x + 1 <= 7 && y + 1 <= 7){
			if(tablero[x+1][y+1] == "P2"){
				return true;
			}
		}
		if(x + 1 <= 7 && y - 1 >= 0){
			if(tablero[x+1][y-1] == "P2"){
				return true;
			}
		}
		if(x + 2 <= 7 && y - 1 >= 0){//verifico si se puede hacer la opercaion
			if(tablero[x+2][y-1] == "H2"){//miro si hay un caballo enemigo
				return true;
			}
		}
		if(x + 2 <= 7 && y + 1 <= 7){
			if(tablero[x+2][y+1] == "H2"){
				return true;
			}
		}
		if(x - 2 >= 0 && y - 1 >= 0){
			if(tablero[x-2][y-1] == "H2"){
				return true;
			}
		}
		if(x - 2 >= 0 && y + 1 <= 7){
			if(tablero[x-2][y+1] == "H2"){
				return true;
			}
		}
		if(y + 2 <= 7 && x - 1 >= 0){
			if(tablero[x-1][y+2] == "H2"){
				return true;
			}
		}
		if(y + 2 <= 7 && x + 1 <= 7){
			if(tablero[x+1][y+2] == "H2"){
				return true;
			}
		}
		if(y - 2 >= 0 && x - 1 >= 0){
			if(tablero[x-1][y-2] == "H2"){
				return true;
			}
		}
		if(y - 2 >= 0 && x + 1 <= 7){
			if(tablero[x+1][y-2] == "H2"){
				return true;
			}
		}
	}else if(turno == 2){
		for(int i = 0; i < 8; i++){
			if(salir){
				break;
			}
			for(int j = 0; j < 8; j++){//busco donde esta el rey
				if(tablero[i][j] == "K2"){
					x = i;
					y = j;
					salir = true;
					break;
				}
			}
		}
		for(int i = x; i < 8; i++){//buscar verticalmente hacia abajo si hay alguna amenaza
			if(tablero[i][y] == "T1" || tablero[i][y] == "Q1"){
				return true;
			}else if (tablero[i][y] == "T2" || tablero[i][y] == "H2" || tablero[i][y] == "B2" || tablero[i][y] == "Q2" || tablero[i][y] == "P2" || tablero[i][y] == "B1" || tablero[i][y] == "H1" || tablero[i][y] == "P1" || tablero[i][y] == "K1"){
				break;
			}
		}
		for(int i = x; i >= 0; i--){//buscar verticalmente hacia arriba
			if(tablero[i][y] == "T1" || tablero[i][y] == "Q1"){
				return true;
			}else if (tablero[i][y] == "T2" || tablero[i][y] == "H2" || tablero[i][y] == "B2" || tablero[i][y] == "Q2" || tablero[i][y] == "P2" || tablero[i][y] == "B1" || tablero[i][y] == "H1" || tablero[i][y] == "P1" || tablero[i][y] == "K1"){
				break;
			}
		}
		for(int i = y; i < 8; i++){//busco horizontalmente hacia la derecha
			if(tablero[x][i] == "T1" || tablero[x][i] == "Q1"){
				return true;
			}else if (tablero[x][i] == "T2" || tablero[x][i] == "H2" || tablero[x][i] == "B2" || tablero[x][i] == "Q2" ||tablero[x][i] == "P2" || tablero[x][i] == "B1" || tablero[x][i] == "H1" || tablero[x][i] == "P1" || tablero[x][i] == "K1"){
				break;
			}
		}
		for(int i = y; i >= 0; i--){//busco horizontalmente hacia la izquierda
			if(tablero[x][i] == "T1" || tablero[x][i] == "Q1"){
				return true;
			}else if (tablero[x][i] == "T2" || tablero[x][i] == "H2" || tablero[x][i] == "B2" || tablero[x][i] == "Q2" ||tablero[x][i] == "P2" || tablero[x][i] == "B1" || tablero[x][i] == "H1" || tablero[x][i] == "P1" || tablero[x][i] == "K1"){
				break;
			}
		}
		int x2 = x,y2 = y;
		while(true){//verificar diagonal derecha arriba
			if(x2 > 0 && y2 < 7){
				x2 -= 1;
				y2 += 1;
				if(tablero[x2][y2] == "B1" || tablero[x2][y] == "Q1"){
					return true;
				}else if (tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" ||tablero[x2][y2] == "B2" || tablero[x2][y2] == "Q2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "T1" ||tablero[x2][y2] == "H1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "K1"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal derecha abajo
			if(x2 < 7 && y2 < 7){
				x2 += 1;
				y2 += 1;
				if(tablero[x2][y2] == "B1" || tablero[x2][y] == "Q1"){
					return true;
				}else if (tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" ||tablero[x2][y2] == "B2" || tablero[x2][y2] == "Q2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "T1" ||tablero[x2][y2] == "H1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "K1"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal izquierda arriba
			if(x2 > 0 && y2 > 0){
				x2 -= 1;
				y2 -= 1;
				if(tablero[x2][y2] == "B1" || tablero[x2][y] == "Q1"){
					return true;
				}else if (tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" ||tablero[x2][y2] == "B2" || tablero[x2][y2] == "Q2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "T1" ||tablero[x2][y2] == "H1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "K1"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal izquierda abajo
			if(x2 < 7 && y2 > 0){
				x2 += 1;
				y2 -= 1;
				if(tablero[x2][y2] == "B1" || tablero[x2][y] == "Q1"){
					return true;
				}else if (tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" ||tablero[x2][y2] == "B2" || tablero[x2][y2] == "Q2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "T1" ||tablero[x2][y2] == "H1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "K1"){
					break;
				}
			}else{
				break;
			}
		}
		if(x - 1 >= 0 && y - 1 >= 0){
			if(tablero[x-1][y-1] == "P1"){
				return true;
			}
		}
		if(x - 1 >= 0 && y + 1 >= 0){
			if(tablero[x-1][y+1] == "P1"){
				return true;
			}
		}
		if(x + 2 <= 7 && y - 1 >= 0){//verifico si se puede hacer la opercaion
			if(tablero[x+2][y-1] == "H1"){//miro si hay un caballo enemigo
				return true;
			}
		}
		if(x + 2 <= 7 && y + 1 <= 7){
			if(tablero[x+2][y+1] == "H1"){
				return true;
			}
		}
		if(x - 2 >= 0 && y - 1 >= 0){
			if(tablero[x-2][y-1] == "H1"){
				return true;
			}
		}
		if(x - 2 >= 0 && y + 1 <= 7){
			if(tablero[x-2][y+1] == "H1"){
				return true;
			}
		}
		if(y + 2 <= 7 && x - 1 >= 0){
			if(tablero[x-1][y+2] == "H1"){
				return true;
			}
		}
		if(y + 2 <= 7 && x + 1 <= 7){
			if(tablero[x+1][y+2] == "H1"){
				return true;
			}
		}
		if(y - 2 >= 0 && x - 1 >= 0){
			if(tablero[x-1][y-2] == "H1"){
				return true;
			}
		}
		if(y - 2 >= 0 && x + 1 <= 7){
			if(tablero[x+1][y-2] == "H1"){
				return true;
			}
		}
	}
	return false;
}
//TERMINA JAQUE COMIENZA GUARDAR-----------------------------------------------------------------
void guardar(string** tablero, int turno, vector<string>& paths){
	ofstream escriba;
	char path[20];
	addstr("Ingrese el nombre de como desea guardar su archivo: ");
	getnstr(path,19);
	string temp = path;
	paths.push_back(temp);
	escriba.open(path);
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			escriba << tablero[i][j] << " ";
		}
	}
	escriba.close();
}
//TERMINA GUARDAR COMIENZA CARGAR---------------------------------------------------------------
string** cargar(vector<string>& paths){
	int choice;
	char temp2;
	string** tablero = crear();
	while(true){
		addstr("Ingrese la posicion del archivo que desea abrir \n");
		for(int i = 0; i < paths.size(); i++){
			printw("Posicion: %i %s",i,paths[i]);
			addstr("\n");
		}
		temp2 = getch();
		choice = temp2 - '0';
		if (choice >= 0 && choice <= paths.size()-1){
			break;
		}
	}
	string tempstr = paths[choice];
	int size = tempstr.size();
	char temp[size+1];
	for(int i = 0; i < size; i++)
		temp[i] = tempstr[i];
	ifstream archivo(temp);
	if(archivo.good()){
		archivo.open(temp);
		for (int i = 0; i < 8; i++){
			printw("i = %d",i);
			for (int j = 0; j < 8; j++){
				archivo >> tempstr;
				tablero[i][j] = tempstr;
			}
		}
	}
	return tablero;
}
void guardarpaths(vector<string>& paths){
	ofstream escriba;
	int size = paths.size();
	escriba.open("paths.txt");
	for(int i = 0; i < paths.size(); i++)
		escriba << paths[i] << " ";
	escriba.close();
	ofstream escribir;
	escribir.open("tamano.txt");
	escribir << size;
	escribir.close();
}
void abrirpaths(vector<string>& paths){
	string temp;
	ifstream archivo("tamano.txt");
	if(!archivo.fail()){
		int size;
		archivo.open("tamano.txt");
		archivo >> size;
		archivo.close();
		ifstream abrir("paths.txt");
		if(abrir.good()){
			abrir.open("paths.txt");
			for(int i = 0; i < size; i++){
				abrir >> temp;
				paths.push_back(temp);
			}
			abrir.close();
		}
	}
}	
bool jaquemate(string** tablero,int turno){
	int x,y,x2,y2;
	bool salir;
	bool evt1,evt2,evt3,evt4,evt5,evt6,evt7,evt8;
		if(turno == 1){
			for(int i = 0; i < 8; i++){
				if(salir){
					break;
				}
				for(int j = 0; j < 8; j++){//busco donde esta el rey
					if(tablero[i][j] == "K1"){
						x = i;
						y = j;
						salir = true;
						break;
					}
				}
			}	
		}else{
			for(int i = 0; i < 8; i++){
				if(salir){
					break;
				}
				for(int j = 0; j < 8; j++){//busco donde esta el rey
					if(tablero[i][j] == "K2"){
						x = i;
						y = j;
						salir = true;
						break;
					}
				}
			}
		}
		x2 = x;
		y2 = y;
		if(x2 - 1 >= 0 && y2 == y){
			evt1 = confirmarjaque(tablero,turno,x-1,y);
		}
		x2 = x;
		y2 = y;
		if(x2 + 1 <= 7 && y2 == y){
			evt2 = confirmarjaque(tablero,turno,x+1,y);
		}
		x2 = x;
		y2 = y;
		if(x2 == x && y2 + 1 <= 7){
			evt3 = confirmarjaque(tablero,turno,x,y+1);
		}
		x2 = x;
		y2 = y;
		if(x2 == x && y2 - 1 >= 0){
			evt4 = confirmarjaque(tablero,turno,x,y-1);
		}
		x2 = x;
		y2 = y;
		if(x2 + 1 <= 7 && y2 + 1 <= 7){
			evt5 = confirmarjaque(tablero,turno,x+1,y+1);
		}
		x2 = x;
		y2 = y;
		if(x2 - 1 >= 0 && y2 - 1 >= 0){
			evt6 = confirmarjaque(tablero,turno,x-1,y-1);
		}
		x2 = x;
		y2 = y;
		if(x2 - 1 >= 0 && y2 + 1 <= 7){
			evt7 = confirmarjaque(tablero,turno,x-1,y+1);
		}
		x2 = x;
		y2 = y;
		if(x2 + 1 <= 7 && y2 - 1 >= 0){
			evt8 = confirmarjaque(tablero,turno,x+1,y-1);
		}
		if(evt1 && evt2 && evt3 && evt4 && evt5 && evt6 && evt7 && evt8)
			return true;
		else
			return false;
}
bool confirmarjaque(string** tablero, int turno, int x, int y){
	if(turno == 1){
		if(x + 1 <= 7){
			for(int i = x+1; i < 8; i++){//buscar verticalmente hacia abajo si hay alguna amenaza
				if(tablero[i][y] == "T2" || tablero[i][y] == "Q2"){
					return true;
				}else if (tablero[i][y] == "T1" || tablero[i][y] == "H1" || tablero[i][y] == "B1" || tablero[i][y] == "Q1" || tablero[i][y] == "P1" || tablero[i][y] == "B2" || tablero[i][y] == "H2" || tablero[i][y] == "P2" || tablero[i][y] == "K2"){
					break;
				}
			}
		}
		if(x - 1 <= 7){
			for(int i = x-1; i >= 0; i--){//buscar verticalmente hacia arriba
				if(tablero[i][y] == "T2" || tablero[i][y] == "Q2"){
					return true;
				}else if (tablero[i][y] == "T1" || tablero[i][y] == "H1" || tablero[i][y] == "B1" || tablero[i][y] == "Q1" || tablero[i][y] == "P1" || tablero[i][y] == "B2" || tablero[i][y] == "H2" || tablero[i][y] == "P2" || tablero[i][y] == "K2"){
					break;
				}
			}
		}
		if(y + 1 <= 7){
			for(int i = y+1; i < 8; i++){//busco horizontalmente hacia la derecha
				if(tablero[x][i] == "T2" || tablero[x][i] == "Q2"){
					return true;
				}else if (tablero[x][i] == "T1" || tablero[x][i] == "H1" || tablero[x][i] == "B1" || tablero[x][i] == "Q1" ||tablero[x][i] == "P1" || tablero[x][i] == "B2" || tablero[x][i] == "H2" || tablero[x][i] == "P2" || tablero[x][i] == "K2"){
					break;
				}
			}
		}
		if(y - 1 >= 0){
			for(int i = y-1; i >= 0; i--){//busco horizontalmente hacia la izquierda
				if(tablero[x][i] == "T2" || tablero[x][i] == "Q2"){
					return true;
				}else if (tablero[x][i] == "T1" || tablero[x][i] == "H1" || tablero[x][i] == "B1" || tablero[x][i] == "Q1" ||tablero[x][i] == "P1" || tablero[x][i] == "B2" || tablero[x][i] == "H2" || tablero[x][i] == "P2" || tablero[x][i] == "K2"){
					break;
				}
			}
		}
		int x2 = x,y2 = y;
		while(true){//verificar diagonal derecha arriba
			if(x2 > 0 && y2 < 7){
				x2 -= 1;
				y2 += 1;
				if(tablero[x2][y2] == "B2" || tablero[x2][y] == "Q2"){
					return true;
				}else if (tablero[x2][y2] == "T1" || tablero[x2][y2] == "H1" ||tablero[x2][y2] == "B1" || tablero[x2][y2] == "Q1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "T2" ||tablero[x2][y2] == "H2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "K2"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal derecha abajo
			if(x2 < 7 && y2 < 7){
				x2 += 1;
				y2 += 1;
				if(tablero[x2][y2] == "B2" || tablero[x2][y] == "Q2"){
					return true;
				}else if (tablero[x2][y2] == "T1" || tablero[x2][y2] == "H1" ||tablero[x2][y2] == "B1" || tablero[x2][y2] == "Q1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" || tablero[x2][y2] == "K2"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal izquierda arriba
			if(x2 > 0 && y2 > 0){
				x2 -= 1;
				y2 -= 1;
				if(tablero[x2][y2] == "B2" || tablero[x2][y] == "Q2"){
					return true;
				}else if (tablero[x2][y2] == "T1" || tablero[x2][y2] == "H1" ||tablero[x2][y2] == "B1" || tablero[x2][y2] == "Q1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "K2"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal izquierda abajo
			if(x2 < 7 && y2 > 0){
				x2 += 1;
				y2 -= 1;
				if(tablero[x2][y2] == "B2" || tablero[x2][y] == "Q2"){
					return true;
				}else if (tablero[x2][y2] == "T1" || tablero[x2][y2] == "H1" ||tablero[x2][y2] == "B1" || tablero[x2][y2] == "Q1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" || tablero[x2][y2] == "K2"){
					break;
				}
			}else{
				break;
			}
		}
		if(x + 1 <= 7 && y + 1 <= 7){
			if(tablero[x+1][y+1] == "P2"){
				return true;
			}
		}
		if(x + 1 <= 7 && y - 1 >= 0){
			if(tablero[x+1][y-1] == "P2"){
				return true;
			}
		}
		if(x + 2 <= 7 && y - 1 >= 0){//verifico si se puede hacer la opercaion
			if(tablero[x+2][y-1] == "H2"){//miro si hay un caballo enemigo
				return true;
			}
		}
		if(x + 2 <= 7 && y + 1 <= 7){
			if(tablero[x+2][y+1] == "H2"){
				return true;
			}
		}
		if(x - 2 >= 0 && y - 1 >= 0){
			if(tablero[x-2][y-1] == "H2"){
				return true;
			}
		}
		if(x - 2 >= 0 && y + 1 <= 7){
			if(tablero[x-2][y+1] == "H2"){
				return true;
			}
		}
		if(y + 2 <= 7 && x - 1 >= 0){
			if(tablero[x-1][y+2] == "H2"){
				return true;
			}
		}
		if(y + 2 <= 7 && x + 1 <= 7){
			if(tablero[x+1][y+2] == "H2"){
				return true;
			}
		}
		if(y - 2 >= 0 && x - 1 >= 0){
			if(tablero[x-1][y-2] == "H2"){
				return true;
			}
		}
		if(y - 2 >= 0 && x + 1 <= 7){
			if(tablero[x+1][y-2] == "H2"){
				return true;
			}
		}
	}else{
		if(x + 1 <= 7){
			for(int i = x+1; i < 8; i++){//buscar verticalmente hacia abajo si hay alguna amenaza
				if(tablero[i][y] == "T1" || tablero[i][y] == "Q1"){
					return true;
				}else if (tablero[i][y] == "T2" || tablero[i][y] == "H2" || tablero[i][y] == "B2" || tablero[i][y] == "Q2" || tablero[i][y] == "P2" || tablero[i][y] == "B1" || tablero[i][y] == "H1" || tablero[i][y] == "P1" || tablero[i][y] == "K1"){
					break;
				}
			}
		}
		if(x - 1 <= 7){
			for(int i = x-1; i >= 0; i--){//buscar verticalmente hacia arriba
				if(tablero[i][y] == "T1" || tablero[i][y] == "Q1"){
					return true;
				}else if (tablero[i][y] == "T2" || tablero[i][y] == "H2" || tablero[i][y] == "B2" || tablero[i][y] == "Q2" || tablero[i][y] == "P2" || tablero[i][y] == "B1" || tablero[i][y] == "H1" || tablero[i][y] == "P1" || tablero[i][y] == "K1"){
					break;
				}
			}
		}
		if(y + 1 <= 7){
			for(int i = y+1; i < 8; i++){//busco horizontalmente hacia la derecha
				if(tablero[x][i] == "T1" || tablero[x][i] == "Q1"){
					return true;
				}else if (tablero[x][i] == "T2" || tablero[x][i] == "H2" || tablero[x][i] == "B2" || tablero[x][i] == "Q2" ||tablero[x][i] == "P2" || tablero[x][i] == "B1" || tablero[x][i] == "H1" || tablero[x][i] == "P1" || tablero[x][i] == "K1"){
					break;
				}
			}
		}
		if(y - 1 >= 0){
			for(int i = y-1; i >= 0; i--){//busco horizontalmente hacia la izquierda
				if(tablero[x][i] == "T1" || tablero[x][i] == "Q1"){
					return true;
				}else if (tablero[x][i] == "T2" || tablero[x][i] == "H2" || tablero[x][i] == "B2" || tablero[x][i] == "Q2" ||tablero[x][i] == "P2" || tablero[x][i] == "B1" || tablero[x][i] == "H1" || tablero[x][i] == "P1" || tablero[x][i] == "K1"){
					break;
				}
			}
		}
		int x2 = x,y2 = y;
		while(true){//verificar diagonal derecha arriba
			if(x2 > 0 && y2 < 7){
				x2 -= 1;
				y2 += 1;
				if(tablero[x2][y2] == "B1" || tablero[x2][y] == "Q1"){
					return true;
				}else if (tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" ||tablero[x2][y2] == "B2" || tablero[x2][y2] == "Q2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "T1" ||tablero[x2][y2] == "H1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "K1"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal derecha abajo
			if(x2 < 7 && y2 < 7){
				x2 += 1;
				y2 += 1;
				if(tablero[x2][y2] == "B1" || tablero[x2][y] == "Q1"){
					return true;
				}else if (tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" ||tablero[x2][y2] == "B2" || tablero[x2][y2] == "Q2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "T1" ||tablero[x2][y2] == "H1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "K1"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal izquierda arriba
			if(x2 > 0 && y2 > 0){
				x2 -= 1;
				y2 -= 1;
				if(tablero[x2][y2] == "B1" || tablero[x2][y] == "Q1"){
					return true;
				}else if (tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" ||tablero[x2][y2] == "B2" || tablero[x2][y2] == "Q2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "T1" ||tablero[x2][y2] == "H1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "K1"){
					break;
				}
			}else{
				break;
			}
		}
		x2 = x;
		y2 = y;
		while(true){//verificar diagonal izquierda abajo
			if(x2 < 7 && y2 > 0){
				x2 += 1;
				y2 -= 1;
				if(tablero[x2][y2] == "B1" || tablero[x2][y] == "Q1"){
					return true;
				}else if (tablero[x2][y2] == "T2" || tablero[x2][y2] == "H2" ||tablero[x2][y2] == "B2" || tablero[x2][y2] == "Q2" || tablero[x2][y2] == "P2" || tablero[x2][y2] == "T1" ||tablero[x2][y2] == "H1" || tablero[x2][y2] == "P1" || tablero[x2][y2] == "K1"){
					break;
				}
			}else{
				break;
			}
		}
		if(x - 1 >= 0 && y - 1 >= 0){
			if(tablero[x-1][y-1] == "P1"){
				return true;
			}
		}
		if(x - 1 >= 0 && y + 1 >= 0){
			if(tablero[x-1][y+1] == "P1"){
				return true;
			}
		}
		if(x + 2 <= 7 && y - 1 >= 0){//verifico si se puede hacer la opercaion
			if(tablero[x+2][y-1] == "H1"){//miro si hay un caballo enemigo
				return true;
			}
		}
		if(x + 2 <= 7 && y + 1 <= 7){
			if(tablero[x+2][y+1] == "H1"){
				return true;
			}
		}
		if(x - 2 >= 0 && y - 1 >= 0){
			if(tablero[x-2][y-1] == "H1"){
				return true;
			}
		}
		if(x - 2 >= 0 && y + 1 <= 7){
			if(tablero[x-2][y+1] == "H1"){
				return true;
			}
		}
		if(y + 2 <= 7 && x - 1 >= 0){
			if(tablero[x-1][y+2] == "H1"){
				return true;
			}
		}
		if(y + 2 <= 7 && x + 1 <= 7){
			if(tablero[x+1][y+2] == "H1"){
				return true;
			}
		}
		if(y - 2 >= 0 && x - 1 >= 0){
			if(tablero[x-1][y-2] == "H1"){
				return true;
			}
		}
		if(y - 2 >= 0 && x + 1 <= 7){
			if(tablero[x+1][y-2] == "H1"){
				return true;
			}
		}
	}
	return false;
}
void comentarios(int turno,int* moves,string** tablero){
	int i1 = moves[0],j1 = moves[1], i2 = moves[2], j2 = moves[3];
	if(turno == 1){
		if(tablero[i1][j1] == "P1"){
			addstr("Los peones son debiles hasta que llegan al otro lado\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "T1"){
			addstr("Las torres no hablan\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "H1"){
			addstr("*horse noises*(yo tampoco hablo)\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "B1"){
			addstr("Hail Hydra\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "Q1"){
			addstr("Matriarchy at work\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "K1"){
			addstr("Soy casi lo mismo que un peon...\n");
			refresh();
			getch();
		}
	}else if(turno == 2){
		if(tablero[i1][j1] == "P2"){
			addstr("Yo si soy util\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "T2"){
			addstr("*tower noises*(yo si hablo)\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "H2"){
			addstr("*human noises*(yo talvez hablo)\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "B2"){
			addstr("Hail Hydrant\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "Q2"){
			addstr("Women can do it!\n");
			refresh();
			getch();
		}else if(tablero[i1][j1] == "K2"){
			addstr("Sigo siendo inutil...\n");
			refresh();
			getch();
		}
	}
}
