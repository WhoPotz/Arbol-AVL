#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#define COLOR_GREEN 2
#define COLOR_RED 4
#define COLOR_LIGHTBLUE 9
#define COLOR_LIGHTRED 12
#define COLOR_WHITE 15

using namespace std;

/*---------  Estructura del arbol  -------*/
typedef struct nodo
{
    int nro;
    int altura;
    struct nodo *izq;
    struct nodo *der;
    
} nodo;

int auxX = 0; //variable global


/*--------- declaraciones ---------*/
void preorden(nodo *);
void inorden(nodo *);
nodo *borrar(nodo *, int);
nodo *insertar(nodo *, int);
bool buscarNodoArbol(nodo *&raiz,int x);
int altura(nodo *);
int Balance_factor(nodo *);
nodo *rotacion_derecha(nodo *);
nodo *rotacion_izquierda(nodo *);
nodo *derecha_derecha(nodo *);
nodo *izquierda_izquierda(nodo *);
nodo *izquierda_derecha(nodo *);
nodo *derecha_izquierda(nodo *);

//funcion de posiciones en el eje x ,y
void gotoxy(int x, int y)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

//funcion de mostrar en eje x , y
void MostrarArbol(nodo *arbol, int auxY)
{
    if (arbol == NULL)
    { //Saber si el arbol esta vacio.
        return;
    }
    auxX += 5;
    MostrarArbol(arbol->izq, auxY + 2);
    gotoxy(10 + auxX - auxY, 15 + auxY);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
    cout << "[" << arbol->nro << "]" << endl
         << endl
         << endl;
    MostrarArbol(arbol->der, auxY + 2);
}

//funcion de buscar un dato en arbol
bool buscarNodoArbol(nodo *&arbol,int x){
    if(arbol==NULL){ //si arbol esta vacio devuelve falso
        return false;
    }else if(arbol->nro==x){ //busca el dato en los punteros del arbol
        return true;
    }else if(x<arbol->nro){
        return buscarNodoArbol(arbol->izq,x);
    }else{
        return buscarNodoArbol(arbol->der,x);
    }
}

//se insertan los datos a la funcion
nodo *insertar(nodo *T, int x)
{
    if (T == NULL) //si el arbol esta vacio
    {
        T = (nodo *)malloc(sizeof(nodo));
        T->nro = x;
        T->izq = NULL;
        T->der = NULL;
        
    }
    else if (x > T->nro) // insertar sub-arbol derecho
    {
        T->der = insertar(T->der, x);
        if (Balance_factor(T) == -2) //balancea el arbol mediante van entrando los datos
            if (x > T->der->nro)
                T = derecha_derecha(T);
            else
                T = derecha_izquierda(T);
        cout<<"Se Ingreso a la derecha"<<endl;
    }
    else if (x < T->nro) // insertar sub-arbol izquierdo
    {
        T->izq = insertar(T->izq, x);
        if (Balance_factor(T) == 2)
            if (x < T->izq->nro)
                T = izquierda_izquierda(T);
            else
                T = izquierda_derecha(T);
        cout<<"Se inserto a la izquierda"<<endl;       
    }
    T->altura = altura(T);
    return (T);
}

//funcion de eliminar un dato del arbol
nodo *borrar(nodo *T, int x)
{
    nodo *p;
    if (T == NULL)
    {
        return NULL;
    }
    else if (x > T->nro) 
    {
        T->der = borrar(T->der, x);
        if (Balance_factor(T) == 2)
            if (Balance_factor(T->izq) >= 0)
                T = izquierda_izquierda(T);
            else
                T = izquierda_derecha(T);
    }
    else if (x < T->nro)
    {
        T->izq = borrar(T->izq, x);
        if (Balance_factor(T) == -2) //Reequilibrio
            if (Balance_factor(T->der) <= 0)
                T = derecha_derecha(T);
            else
                T = derecha_izquierda(T);
    }
    else
    {
        //se encuentra nro para ser eliminado
        if (T->der != NULL)
        { //eliminar su sucesor en orden
            p = T->der;
            while (p->izq != NULL)
                p = p->izq;
            T->nro = p->nro;
            T->der = borrar(T->der, p->nro);
            if (Balance_factor(T) == 2) //rebalance
                if (Balance_factor(T->izq) >= 0)
                    T = izquierda_izquierda(T);
                else
                    T = izquierda_derecha(T);
        }
        else
            return (T->izq);
    }
    T->altura = altura(T);
    return (T);
}

//funcion para saber la altura de los nodos
int altura(nodo *T)
{
    int altura_izquierda, altura_derecha;
    if (T == NULL)   //si el arbol esta vacio returna 
        return (0);
    if (T->izq == NULL)
        altura_izquierda = 0; //si el arbol esta vacio se encuentra en 0 la altura
    else
        altura_izquierda = 1 + T->izq->altura; //se le suma a la variable altura izquierda 1 cada ves que se inserta un nodo en el arbol
    if (T->der == NULL)
        altura_derecha = 0;
    else
        altura_derecha = 1 + T->der->altura; //se le suma 1 a la variable altura derecha si se ingresa un nodo al lado derecho
    if (altura_izquierda > altura_derecha) //si altura izquierda es mayor a la altura derecha
        return (altura_izquierda); //retorna el valor de altura izquierda
    return (altura_derecha); 
}


 //ayuda a calcular la direccion de las rotaciones
nodo *i(nodo *x)
{
    nodo *y;  
    y = x->izq;
    x->izq = y->der;
    y->der = x;
    x->altura = altura(x);
    y->altura = altura(y);
    return (y);
}

//rotacion simple  izquierda
nodo *rotacion_izquierda(nodo *x)
{
    nodo *y;
    y = x->der;
    x->der = y->izq;
    y->izq = x;
    x->altura = altura(x);
    y->altura = altura(y);
    return (y);
}
//rotacion simple derecha
nodo *derecha_derecha(nodo *T)
{
    T = rotacion_izquierda(T);
    return (T);
}
//rotacion compleja izquierda
nodo *izquierda_izquierda(nodo *T)
{
    T = i(T);
    return (T);
}
//rotacion compleja a la izquierda
nodo *izquierda_derecha(nodo *T)
{
    T->izq = rotacion_izquierda(T->izq);
    T = i(T);
    return (T);
}
//rotacion compljea a la derecha
nodo *derecha_izquierda(nodo *T)
{
    T->der = i(T->der);
    T = rotacion_izquierda(T);
    return (T);
}

//calcula si el arbol esta desbalanceado mas a la izquierda o a la derecha
int Balance_factor(nodo *T)
{
    int altura_izquierda, altura_derecha;
    if (T == NULL)
        return (0);

    if (T->izq == NULL)
        altura_izquierda = 0;
    else
        altura_izquierda = 1 + T->izq->altura; // se

    if (T->der == NULL)
        altura_derecha = 0;
    else
        altura_derecha = 1 + T->der->altura;

    return (altura_izquierda - altura_derecha);
}

//Muestra el recorrido en inorden
void inorden(nodo *T)
{
    if (T != NULL)
    {
        inorden(T->izq);
        printf("%d,",T->nro,"\n");
        inorden(T->der);
    }
}

//Muestra el recorrido en preorden
void preorden(nodo *T)
{
    if (T != NULL)
    {
        printf("%d,", T->nro);
        preorden(T->izq);
        preorden(T->der);
    }
}

//menu principal
int main()
{
    nodo *raiz = NULL;
    int opc, x;
    while (opc != 5)
    {
        system("cls");
        system("color 09"); // poner color a la consola
        cout << "\n\t\t  ..[ ARBOL AVL ]..  \n\n";
        cout << "\t [1]  Insertar elemento                  \n";
        cout << "\t [2]  Mostrar arbol                      \n";
        cout << "\t [3]  Buscar elemento                    \n";
        cout << "\t [4]  Eliminar elemento                  \n";
        cout << "\t [5] SALIR                              \n";

        cout << "\n\t Ingrese opcion : ";
        cin >> opc;
        switch (opc)
        {
        case 1:
            cout << "Ingrese un elemento -> ";
            cin >> x;
            raiz = insertar(raiz, x);
        
            system("pause");
            break;
        case 2:
        if (raiz==NULL)
        {   
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_LIGHTRED);
            cout<<"\n El arbol esta vacio";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_LIGHTBLUE);
        }else{
            cout <<"Inorden :"<<endl;
            inorden(raiz);
            cout <<endl<<"Preorden :"<<endl;
            preorden(raiz);
            MostrarArbol(raiz, 0);
            auxX = 0;
        }
            getch();
            break;
        case 3:
            cout << "Ingrese el numero a buscar en el arbol:";
            cin >> x;
            if (buscarNodoArbol(raiz, x) == true)
            {   
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_GREEN);
                cout << "El numero " << x << " si se encontro en el arbol\n";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_LIGHTBLUE);
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
                cout << "El numero " << x << " no se encontro en el arbol\n";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_LIGHTBLUE);
            }
            system("pause");
            break;
        case 4:
            cout << "Ingrese valor a eliminar -> ";
            cin >> x;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            cout << "El valor "<<x<<" a sido eliminado\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_LIGHTBLUE);
            raiz = borrar(raiz, x);
            system("pause");
            break;
        case 5:
            exit(0);
        }
        cout << "\n\n\n";
    }
    return 0;
}
