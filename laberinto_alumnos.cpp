/**
 *
 * @file laberinto_alumnos.cpp
 *
 * @brief Programa para la generacion y resolucion de un laberinto.
 *
 * Generación mediante una cadena generada de manera aleatoria siguiendo el backtracking
 * y una resolución siguiendo la regla de la mano derecha
 *
 * @version 1.4
 *
 * @author Mario Gil Domingo, Luis Razvan Petrisor
 * 
 * @date 27 Diciembre 2021
 *
 * @copyright Universidad de Valencia
 *
 */
 
#include <iostream>
///< Sirve para crear, leer y guardar los archivos en textos
#include <fstream> 
///< ctime y windows.h sirve para crear un numero aleatorio
#include <ctime>
#include <windows.h>
///< Sirven para crear la función sleep_for()
#include <thread>      
#include <chrono> 
///< La biblioteca string sirve para cadenas de caracteres
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
	// Esto no hay que hacer nada
	HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo (h, &cursor);
	system("MODE CON COLS=42 LINES=26");
    system ("color E0");
    
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
					// system ("cls");
					Estado e = {TAM - 1, 0, 'N'}; //posicion actual en la E
					Resolver (e, n, a, h);	//siempre encuentra salida
					IrA (TAM, 0, h);		
				}
		}
		system("pause");
		system ("cls");
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
void Inicializar (MNodos n, MParedes a) 
{
	///< Inicializamos el laberinto
	srand(time(NULL) * time (NULL)); ///< Crea un numero aleatorio
	unsigned int i, j;
	///< Asignamos false a todas las paredes
	for (i = 0; i < TAM * TAM; i++)
	{
		for (j = 0; j < TAM * TAM; j++)
		{
			a[i][j] = false;
		}
	}
	/* Tenemos que crear dos bucles, ya que uno es de tamaño TAM y el otro de TAM*TAM
	 Inicializamos todos los id, y los bordes les damos el valor de -1.*/
	for (i = 0; i < TAM; i++)
	{
		
		for (j = 0; j < TAM; j++)
		{
			///< Primero definiremos las celdas
			n[i][j].v = false;
			n[i][j].idN = (i - 1) * TAM + j;
			n[i][j].idE = i * TAM + j + 1;
			n[i][j].idO = i * TAM + j - 1;
			n[i][j].idS = (i + 1) * TAM + j;			
			///< Esto sirve para los bordes del laberinto
			///< Si 'i' vale 0 esta en la posicion más al norte, por lo que al norte de esta no existe laberinto
			if(i == 0) 
				n[i][j].idN = -1;
			///< Si 'j' vale 0 esta en la posición más al oeste, por lo que al oeste de esta no existe laberinto
			if (j == 0)
				n[i][j].idO = -1;
			///< Si 'i' vale TAM - 1, es decir 9, esta mas al sur , por lo que al sur de esta no existe laberinto
			if (i == TAM - 1)
				n[i][j].idS = -1;
			///< Si 'j' vale TAM - 1 esta mas al este, por lo que al este de esta no existe laberinto
			if (j == TAM - 1)
				n[i][j].idE = -1; 
				
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
	/* Crea el laberinto rellenando la matriz de adyacencias 
	* y eliminando tabiques usando una busqueda en profundida, usa retroceso.
    * Marcamos la celda como visitada, desde la id, pasamos a
    * coordenadas, obteniendo asi las filas y las columnas. */
    unsigned int fil,colum,siguienteVecina;
    fil = id / TAM;
    colum = id % TAM;
    n[fil][colum].v = true;

    ///< Mientras quedan vecinas de celda no visitadas hacer
    while(NoVisitada(id,n,a, siguienteVecina))
	{		    
    		///< eliminar la pared entre celda y siguienteCelda
    		a[id][siguienteVecina] = true;
    		a[siguienteVecina][id] = true;
    		this_thread::sleep_for (chrono::milliseconds(265)); ///< Ponemos un delay, para apreciar con claridad como se van 'levantando' tabiques
    		Mostrar (a,h);
     		///< Crear (siguienteCelda)
     		Crear (siguienteVecina,n,a,h);
    		//fin_mientras		
    }

	return;
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
    ///< Necesitaremos variable para guardar los valores de nuestro nodo
    int fila,columna,count,aux;
    ///< Una string donde guardar la direccion aleatoria y otro para substraer la primera letra de la cadena
    string dir, letra;
    ///< la variable count guardará el numero de nodos vecinos validos, y si es 0 no quedan vecinos por confirmar
    count = 0;
    ///< Guardamos los datos de nuestro nodo
    fila = id / TAM;
    columna = id % TAM;
    ///< Comprobamos si el vecino norte es valido
    aux = n[fila][columna].idN;
    if(aux != -1 && !n[aux/TAM][aux%TAM].v) 
		count++;
    ///< Comprobamos si el vecino este es valido
    aux = n[fila][columna].idE;
    if(aux != -1 && !n[aux/TAM][aux%TAM].v) 
		count++;
    ///< Comprobamos si el vecino sur es valido
    aux = n[fila][columna].idS;
    if(aux != -1 && !n[aux/TAM][aux%TAM].v) 
		count++;
    ///< Comprobamos si el vecino Oeste es valido
    aux = n[fila][columna].idO;
    if(aux != -1 && !n[aux/TAM][aux%TAM].v) 
		count++;
    if(!count) ///< Si no hay ningún válido (count == 0) devolvemos que no quedan casillas NoVisitadas, por lo que todas estarían visitadas
		return false;
    
	bool encontrado = false; ///< Creamos un booleano para que se repita el bucle hasta que encuentre una celda valida
    while(!encontrado)
	{
        dir = AleatorizarDir(); 						///< Obtenemos la cadena aleatoria
        letra = dir.substr(0,1); 						///< Substramos la primera letra de la cadena
        dir = dir.erase(0); 							///< Borramos la primera letra de la cadena porque no nos hace más falta
        if(letra == "N")
		{
                aux = n[fila][columna].idN; 			///< Aux será en nodo que tenga al norte
                if(aux != -1 && !n[aux/TAM][aux%TAM].v) ///< Se ejecuta si existe este nodo, es decir, aux != -1 y no ha sido visitada
				{
                    vecinaId = aux; 					///< Al cumplirse las condiciones la celda auxiliar pasa a ser la siguienteVecina
                    encontrado = true; 					///< Encontrado es true, y sale del bucle while
                } 										///< Esto se repite para cada caso, es decir, N,S,E,O.			
		}
        else if(letra == "S")
		{
                aux = n[fila][columna].idS;
                if(aux != -1 && !n[aux/TAM][aux%TAM].v)
				{
                    vecinaId = aux;
                    encontrado = true; 
                }			
		}
        else if(letra == "E")
		{
                aux = n[fila][columna].idE;
                if(aux != -1 && !n[aux/TAM][aux%TAM].v)
				{
                    vecinaId = aux;
                    encontrado = true; 
                }
		}
        else if(letra == "O")
		{
                aux = n[fila][columna].idO;
                if(aux != -1 && !n[aux/TAM][aux%TAM].v)
				{
                    vecinaId = aux;
                    encontrado = true; 
                }			
		}
    }
    return true;
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
void EjecutarMov (MNodos n, MParedes a, Estado & p, string ori, HANDLE h)
{
	int id = p.f * TAM + p.c; ///< Obteniendo de la estrucutra Estado las filas y las columnas las pasamos al id.
	bool verificar = false;  ///< Creamos un booleano para que se repita el bucle hasta que se ponga a true y
    while(verificar == false)
	{
		for (unsigned int i = 0; i< ori.length(); i++)
		{
///< Si la letra es N, ademas entre la celda y su vecina al norte es true, por lo que no hay tabique y ademas es diferente de -1, por lo que si que hay celda al norte
			if (ori[i] == 'N' && a[id][n[p.f][p.c].idN] == true && n[p.f][p.c].idN != -1) 
			{
				p.f = p.f - 1;
				p.mira = 'N';
				verificar = true;
			}
			else if (ori[i] == 'O' && a[id][n[p.f][p.c].idO] == true && n[p.f][p.c].idO != -1)
			{
				p.c = p.c - 1;
				p.mira = 'O';
				verificar = true;			
			}
			else if (ori[i] == 'E' && a[id][n[p.f][p.c].idE] == true && n[p.f][p.c].idE != -1)
			{
				p.c = p.c + 1;
				p.mira = 'E';
				verificar = true;				
			}
			else if (ori[i] == 'S' && a[id][n[p.f][p.c].idS] == true && n[p.f][p.c].idS != -1)
			{
				p.f = p.f + 1;
				p.mira = 'S';
				verificar = true;			
			}
		}
	}
	return;
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
	
	/* Volvemos a inicializar los Nodos, ya que si leemos desde fichero del laberinto, y 
	* le damos a Resolver, entonces los ids de la matriz MNodos están sin 
	* inicializar, y no resuelve el laberinto. */
	unsigned int i,j;
	for (i = 0; i < TAM; i++)
	{
		
		for (j = 0; j < TAM; j++)
		{
			///< Primero definiremos las celdas
			n[i][j].v = false;
			n[i][j].idN = (i - 1) * TAM + j;
			n[i][j].idE = i * TAM + j + 1;
			n[i][j].idO = i * TAM + j - 1;
			n[i][j].idS = (i + 1) * TAM + j;			
			///< Esto sirve para los bordes del laberinto
			///< Si 'i' vale 0 esta en la posicion más al norte, por lo que al norte de esta no existe laberinto
			if(i == 0) 
				n[i][j].idN = -1;
			///< Si 'j' vale 0 esta en la posición más al oeste, por lo que al oeste de esta no existe laberinto
			if (j == 0)
				n[i][j].idO = -1;
			///< Si 'i' vale TAM - 1, es decir 9, esta mas al sur , por lo que al sur de esta no existe laberinto
			if (i == TAM - 1)
				n[i][j].idS = -1;
			///< Si 'j' vale TAM - 1 esta mas al este, por lo que al este de esta no existe laberinto
			if (j == TAM - 1)
				n[i][j].idE = -1; 
				
		}
	}
	
	Mostrar (a,h);	
	MostrarEstado (p,h);		
	///< Algoritmo Resolver Laberinto Regla Mano Derecha
	///< mientras no estemos en la celda de salida hacer
	while (p.f != 0 || p.c != TAM - 1) 
	{
		MostrarEstado (p,h); ///< Mostramos la flechita
		Mostrar(a,h); ///< Mostramos el laberinto		
		switch (p.mira)
		{
			///< si mira al Norte entonces EjecutarMov (estado, "ENOS"); 
			case 'N':
					EjecutarMov (n,a,p,"ENOS",h);	
					
				break;
			///< si mira al Oeste entonces EjecutarMov (estado, "NOSE");
			case 'O':
					EjecutarMov (n,a,p,"NOSE",h);
				
				break;
			///< si mira al Sur entonces EjecutarMov (estado, "OSEN");		
			case 'S':
					EjecutarMov (n,a,p,"OSEN",h);
				
				break;
			///< si mira al Este entonces EjecutarMov (estado, "SENO");						
			case 'E':
					EjecutarMov (n,a,p,"SENO",h);
				
				break;			 	
		}
	Mostrar(a,h);		
	MostrarEstado(p,h);
	}
	///< Poner flecha direccion norte al final
	p.mira = 'N';
	MostrarEstado(p,h);
	return;
}

/**
 *
 * Muestra en la reticula la fila, la columna y la orientacion actual
 *
 * @param [in] p estado actual
 * @param [in] h handle de la ventana donde se muestra el laberinto
 *
 */
void MostrarEstado (Estado p, HANDLE h)
{
	IrA (p.f, p.c, h);
	///< Metemos un sleep_for para que haya un delay, y se aprecie la flechita
	this_thread::sleep_for (chrono::milliseconds(265));
	 
	/* switch o if elses para sacara la posicion de la flecha en p.mira
	* Para cada caso es un cout mirando en direccion a la flecha e ir a la posicion en cada caso (funcion IrA)
	* el char de norte = "30", sur = "31", este = "16", oeste ="17" */
	
	if (p.mira == 'N')
		cout << char(30);
	else if (p.mira == 'O')
		cout << char(17);
	else if (p.mira == 'E')
		cout << char(16);
	else if (p.mira == 'S')
		cout << char (31);					
	
	return;
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
	for (unsigned int i = 0; i < TAM*TAM; i++)
	{
		for (unsigned int j = 0; j < TAM*TAM; j++)
		{
			f >> a[i][j];
		}
	}
	
	return;
	
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
	///< Hay que hacer dos bucles for y guardar los booleanos con f <<
	for (unsigned int i = 0; i< TAM*TAM; i++)
	{
		for (unsigned int j = 0; j < TAM*TAM; j++)
		{
			f << a[i][j] << " ";
		}
		f << "\n";
	}
	
	return;
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
	int aleatorio,pos = 0;
	for (unsigned int i = 0; i < ori.length(); i++)
	{
		while (pos != -1) ///< Mientras no encuentre la letra que se repita
		{ 
			aleatorio = rand() % ori.length(); ///< El numero aleatorio esta comprendido entre 1 y 4
			switch(aleatorio)
			{
				case 0:
					pos = d.find("N"); 		///< Busca la letra
					if (pos == -1) 			///< Si la letra no está, su posición es -1
						d = "N" + d; 		///< Se añade la letra al principio de la cadena			
					break;
				case 1:
					pos = d.find("S");
					if (pos == -1) 
						d = "S" + d;
					break;
				case 2:
					pos = d.find("E");
					if (pos == -1)
						d = "E" + d;
					break;
				case 3:
					pos = d.find("O");
					if (pos == -1)
						d = "O" + d;
					break;		 	
			}
		}
	}	
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

