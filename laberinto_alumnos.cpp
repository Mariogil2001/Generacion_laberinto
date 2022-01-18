/**
 *
 * @file laberinto_alumnos.cpp
 *
 * @brief 
 *
 * Programa para la generacion y resolucion de un laberinto.
 *
 * @version 1
 * @author Mario Gil Domingo
 * @author Luis Razvan Petrisor
 * @date 27 Diciembre 2021
 *
 * @copyright Universidad de Valencia
 */
 
#include <iostream>
// Sirve para crear, leer y guardar los archivos en textos
#include <fstream> 
// ctime y windows.h sirve para crear un numero aleatorio
#include <ctime>
#include <windows.h>
#include <thread>      
#include <chrono> 
// La biblioteca string sirve para cadenas de caracteres
#include <string>
 
using namespace std;

const int TAM = 10; 

/** 
 * 
 * @struct Estado
 * Registro para guardar la estado actual al recorrer el laberinto: fila, columna y orientacion.
 *
 * @var Estado::f
 * Almacena la fila
 * @var Estado::c
 * Almacena la columna
 * @var Estado::mira
 * Almacena la orientacion hacia donde se mira: N, S, E, O
 */
struct Estado
{
	unsigned int f;
	unsigned int c; // fila y columna
	char mira; // orientacion
};

/** 
 * 
 * @struct Nodo
 * Registro para guardar informacion de un Nodo
 *
 * @var Nodo::v
 * Almacena si la celda ya ha sido visitada
 * @var Nodo::idN
 * Almacena el identificador del vecino al Norte
 * @var Nodo::idE
 * Almacena el identificador del vecino al Este
 * @var Nodo::idS
 * Almacena el identificador del vecino al Sur
 * @var Nodo::idO
 * Almacena el identificador del vecino al Oeste
 */
struct Nodo
{
	bool v;
	int idN, idE, idS, idO;
};

/**
*
* @brief Definición de tipo de dato MNodos de tamaño 10x10 elementos de tipo Nodo
*
* @typedef MNodos
*/
typedef Nodo MNodos [TAM] [TAM];
/**
*
* @brief Definición de tipo de dato MParedes de tamaño 100x100 elementos de tipo bool,
* matriz de adyacencias
*
* @typedef MParedes
*/
typedef bool MParedes [TAM * TAM][TAM * TAM]; 

string AleatorizarDir ();
void Inicializar (MNodos, MParedes);
void Crear (unsigned int, MNodos, MParedes, HANDLE);
bool NoVisitada (unsigned int, MNodos, MParedes, unsigned int &);
void EjecutarMov(MNodos, MParedes, Estado &, string, HANDLE);
void Resolver (Estado &, MNodos, MParedes, HANDLE);
void Mostrar (MParedes, HANDLE);
void Leer (ifstream &, MParedes);
void Guardar (ofstream &, MParedes);
char Menu();
void IrA (unsigned int, unsigned int, HANDLE);
void MostrarEstado (Estado, HANDLE);

int main ()
{
	HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo (h, &cursor);
	system("MODE CON COLS=42 LINES=26");
    //system ("color F0");
    
	MNodos n; 
	MParedes a; 
	char op;
	string nombre;
	ifstream f_in;
	ofstream f_out;
					
	do
	{	
		op = Menu();
		switch (op)
		{
			case 'A':
				Inicializar (n, a);
				Crear ((TAM - 1) * TAM, n, a, h); //id de la Entrada
				Mostrar (a, h); 
				break;
			case 'B':
				cout << "Introduce el nombre: ";
				cin >> nombre;
				f_in.open (nombre.c_str());
				if (!f_in) cout << "Error al abrir el fichero " << nombre;
				else
				{
					Leer (f_in, a);
					f_in.close ();
				}
				break;	
			case 'C':
				cout << "Introduce el nombre: ";
				cin >> nombre;
				f_out.open (nombre.c_str());
				if (!f_out) cout << "Error al abrir el fichero " << nombre;
				else 
				{
					Guardar (f_out, a);
					f_out.close ();
				}
				break;	
			case 'D':
				Mostrar (a, h);
				break;
			case 'E':
				{
					Estado e = {TAM - 1, 0, 'N'}; //posicion actual en la E
					Resolver (e, n, a, h);	//siempre encuentra salida
					IrA (TAM, 0, h);
				}
		}
		system("pause");
	} 
	while (op != 'F');

	return 0;
}

/**
 *
 * Inicializa la matriz de nodos y adyacencias.
 *
 * @param [out] n matriz de nodos 
 * @param [out] v matriz de adyacencias
 *
 */
void Inicializar (MNodos n, MParedes a) // Inicializamos el laberinto
{
	srand(time(NULL)); // Creamos un numero aleatorio
	for (unsigned int i = 0; i < TAM; i++)
	{
		for (unsigned int j = 0; j < TAM; j++)
		{
			n[i][j].idN = 0 + rand()%(256);	// variable = limite_inferior + rand() % (limite_superior + 1 - limite_inferior);
		}
	}
	
	
	return;
}

/**
 *
 * Crea el laberinto rellenando la matriz de adyacencias y eliminano tabiques usando 
 * una busqueda en profundida, usa retroceso.
 *
 * @param [in] id identificador de la celda actual
 * @param [out] n matriz de nodos 
 * @param [out] a matriz de adyacencias
 * @param [in] h handle de la ventana donde se muestra el laberinto
 *
 */
void Crear (unsigned int id, MNodos n, MParedes a, HANDLE h)
{
// completar
}

/**
 *
 * Devuelve una celda vecina que no ha sido aún visitada.
 * durante la construcción del lab.
 *
 * @param [in] id identificador de la celda actual
 * @param [in] n matriz de nodos 
 * @param [out] a matriz de adyacencias
 * @param [out] vecinaId identificador de la siguiente celda a visitar
 *
 * @return verdadero si aun quedan vecinas sin visitar, falso en caso contrario
 */
bool NoVisitada (unsigned int id, MNodos n, MParedes a, unsigned int & vecinaId)
{
// completar
}

/**
 *
 * Actualiza el valor de mira y avanza si es posible 
 * usando un orden en las direcciones establecidas en funcion de la orientacion actual
 *
 * @param [in] n matriz de nodos 
 * @param [in] a matriz de adyacencias
 * @param [in, out] p estado actual
 * @param [in] ori cadena con el orden de evaluacion de los movimientos: ENOS, NOSE, OSEN, SENO
 * @param [in] h handle de la ventana donde se muestra el laberinto
 *
 */
void EjecutarMov(MNodos n, MParedes a, Estado & p, string ori, HANDLE h)
{
// completar
}

/**
 *
 * Resuelve el laberinto.
 *
 * @param [in, out] p estado actual
 * @param [in] n matriz de nodos 
 * @param [in] a matriz de adyacencias
 * @param [in] h handle de la ventana donde se muestra el laberinto
 *
 */
void Resolver (Estado & p, MNodos n, MParedes a, HANDLE h)
{
// completar	
}

/**
 *
 * Muestra en la reticula la fila, la columna y la orientacion actual
 *
 * @param [in] p estado actual
 * @param [in] h handle de la ventana donde se muestra el laberinto
 *
 */
void MostrarEstado(Estado p, HANDLE h)
{
// completar
}


/**
 *
 * Muestra el menú.
 *
 * @return la opción elegida por el usuario
 *
 */ 
char Menu ()
{
	char op;
	do
	{	cout << "         Laberinto         " << endl;
		cout << "---------------------------" << endl;
		cout << "| A. Generar.             |" << endl;
		cout << "| B. Leer desde fichero.  |" << endl;
		cout << "| C. Guardar a fichero.   |" << endl;
		cout << "| D. Mostrar.             |" << endl;
		cout << "| E. Resolver.            |" << endl;
		cout << "| F. Salir.               |" << endl;
		cout << "---------------------------" << endl;		
		cout << "Ingrese Opcion: " << endl;
		cin >> op;
	} while(op < 'A' || op > 'F' );
	return op;
}

/**
 *
 * Lee el laberinto (matriz de adyacencias) desde un fichero
 *
 * @param [out] f fichero
 * @param [in] v matriz de adyacencias
 *
 */ 
void Leer (ifstream & f, MParedes a)
{
// completar
}

/**
 *
 * Guarda el laberinto (matriz de adyacencias) en un fichero
 *
 * @param [out] f fichero
 * @param [in] a matriz de adyacencias
 *
 */ 
void Guardar (ofstream & f, MParedes a)
{
// completar
}

/**
 *
 * Devuelve una cadena con las orientaciones de busqueda al azar
 * en la generación del laberinto
 *
 * @return una cadena de orientaciones
 *
 */ 
string AleatorizarDir ()
{
	string d, ori = "ENOS"; 
// completar
	
	return d;
}

/**
 *
 *  Muestra la reticula, el laberinto
 *
 *  @param [in] a matriz de adyacencias
 *  @param [in] h handle de la ventana donde se muestra el laberinto
 *
 */ 
void Mostrar (MParedes a, HANDLE h)
{
	unsigned int i, j;
	COORD p = {0, 0};
	SetConsoleCursorPosition (h, p);  
	
	for (j = 0; j < TAM - 1; j++) cout << "+---";
    cout << "+   +" << endl; // Salida
	for (i = 0; i < TAM; i++) 
	{   
		if (i == TAM-1) cout << " "; // Entrada
		else  cout << "|";
		for (j = 0; j < TAM - 1; j++) 
    		if (a [i * TAM + j] [i * TAM + j + 1] == false) cout << "   |";
            else cout << "    ";   
        cout << "   |" << endl; 
        
        if (i < TAM-1)
		{
			for (j = 0; j < TAM; j++) 
	        	if (a [i * TAM + j] [(i + 1) * TAM + j] == false) cout <<"+---";
	       		else cout <<"+   ";
			cout << "+" << endl;
		}	
    }
    for (j = 0; j < TAM; j++) cout << "+---";
    cout << "+" << endl; 		
}

/**
 *
 * Establece una correspondencia entre filas y columnas
 * y las coordenas para colocar el cursor en la pantalla
 *
 * @param [in] fil fila de la celda
 * @param [in] col columna de la celda
 * @param [in] h handle de la ventana donde se muestra el laberinto
 *
 */ 
void IrA (unsigned int fil, unsigned int col, HANDLE h) 
{ 
    COORD p = { short(4 * col + 2), short(2 * fil + 1)};
	SetConsoleCursorPosition (h, p);   
}


