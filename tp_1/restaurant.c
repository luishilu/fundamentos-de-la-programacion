#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "restaurant.h"

const char ARRIBA = 'W';
const char ABAJO = 'S';
const char DERECHA = 'D';
const char IZQUIERDA = 'A';
const char MOPA = 'O';

const char LINGUINI = 'L';
const char MESAS = 'T';
const char COCINA = 'C';
const char CHARCOS = 'H';
const char MONEDAS = 'M';
const char PATINES = 'P';
const char VACIO = ' ';
const int MAX_MOVIMIENTOS = 200;
const int META_DINERO = 150000;
const int MAX_MESAS_1 = 6;
const int MAX_MESAS_4 = 4;
const int MAX_MONEDAS = 8;
const int MAX_PATINES = 5;
const int MAX_CHARCOS = 5;
const int LUGAR_MESA_1 = 1;
const int LUGAR_MESA_4 = 4;
const int TOTAL_MESAS = 10;
const int JUEGO_TERMINADO = 1;
const int EN_JUEGO = 0;
const int FILAS_MESA_4 = 2;
const int COLUMNAS_MESA_4 = 2;

// pre: -
//post: rellena la matriz 20x20 con la constante VACIO.
void inicializar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            mapa[i][j] = VACIO;
     }
  }
}

//pre: -
//post: devuelve una posición random entre los límites de una matriz 20x20.
coordenada_t posicion_random(){
    coordenada_t posicion;
    posicion.fil = rand() % (MAX_FILAS);
    posicion.col = rand() % (MAX_COLUMNAS);
    return posicion;
}

//pre: -
//post: devuelve true si la posición ingresada no está ocupada por ningún otro elemento.
bool es_posicion_libre(char mapa[MAX_FILAS][MAX_COLUMNAS], int fila, int columna){
    return(mapa[fila][columna] == VACIO);
}

//pre: -
//post: devuelve true si el área ingresada está vacía y no excede los límites del mapa, retorna false en caso contrario.
bool entra_mesa(char mapa[MAX_FILAS][MAX_COLUMNAS], int fila_base, int columna_base, int espacio_necesario){
    bool entra_mesa = true;
    for (int i = 0; i < espacio_necesario; i++) {
            for (int j = 0; j < espacio_necesario; j++) {
                if (mapa[i + fila_base - 1][j + columna_base - 1] == MESAS || (i + fila_base -1) >= MAX_FILAS || (j + columna_base-1) >= MAX_COLUMNAS){
                    entra_mesa = false;;
                }
            }
         }
        return entra_mesa;
}

//pre: -
//post: inicializa 4 mesas de 4 en una posición aleatoria y válida en el mapa, junto con otros campos del struct juego_t.
void inicializar_mesa_de_4(juego_t* juego, char mapa[MAX_FILAS][MAX_COLUMNAS]) {
    for (int i = 0; i < MAX_MESAS_4; i++) {
        coordenada_t posicion_base;
        int posicion_mesa = 0;
        do {
            posicion_base = posicion_random();
        } while (!entra_mesa(mapa, posicion_base.fil, posicion_base.col, 4));

        for (int j = 0; j < FILAS_MESA_4; j++) {
            for (int k = 0; k < COLUMNAS_MESA_4; k++) {
                juego->mesas[i].posicion[posicion_mesa].fil = posicion_base.fil + j;
                juego->mesas[i].posicion[posicion_mesa].col = posicion_base.col + k;

                mapa[posicion_base.fil + j][posicion_base.col + k] = MESAS;
                posicion_mesa++;
            }
        }
        (juego->cantidad_mesas)++;
        juego->mesas[i].paciencia = 0;
        juego->mesas[i].pedido_tomado = false;
        juego->mesas[i].cantidad_comensales = 0;
        juego->mesas[i].cantidad_lugares = LUGAR_MESA_4;
    }
}

//pre: -
//post: inicializa 6 mesas de 1 en una posición aleatoria y válida en el mapa, junto con otros campos del struct juego_t.
void inicializar_mesa(juego_t* juego, char mapa[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = MAX_MESAS_4; i < TOTAL_MESAS; i++){
        int posicion_mesa = 0;
        do {
            juego->mesas[i].posicion[posicion_mesa] = posicion_random();
        }while (!entra_mesa(mapa, juego->mesas[i].posicion[posicion_mesa].fil, juego->mesas[i].posicion[posicion_mesa].col, 3));

        mapa[juego->mesas[i].posicion[posicion_mesa].fil][juego->mesas[i].posicion[posicion_mesa].col] = MESAS;
        (juego->cantidad_mesas)++;
        juego->mesas[i].cantidad_lugares = LUGAR_MESA_1;
        juego->mesas[i].paciencia = 0;
        juego->mesas[i].pedido_tomado = false;
        juego->mesas[i].cantidad_comensales = 0;
 }
}


//pre: el mapa debe haber sido inicializado con la constante VACIO.
//post: inicializa el mozo en una posición aleatoria y válida en el mapa, junto con otros campos del struct juego_t. 
void inicializar_linguini(juego_t* juego, char mapa[MAX_FILAS][MAX_COLUMNAS]){
    do{
        juego->mozo.posicion = posicion_random();
    } while(!es_posicion_libre(mapa, juego->mozo.posicion.fil, juego->mozo.posicion.col));

    mapa[juego->mozo.posicion.fil][juego->mozo.posicion.col] = LINGUINI;
    juego->mozo.cantidad_pedidos = 0;
    juego->mozo.cantidad_bandeja = 0;
    juego->mozo.cantidad_patines = 0;
    juego->mozo.tiene_mopa = false;
}

//pre: el mapa debe haber sido inicializado con la constante VACIO.
//post: inicializa la cocina en una posición aleatoria y válida en el mapa
void inicializar_cocina(juego_t* juego, char mapa[MAX_FILAS][MAX_COLUMNAS]){
    do{
        juego->cocina.posicion = posicion_random();
    } while(!es_posicion_libre(mapa,juego->cocina.posicion.fil,juego->cocina.posicion.col));

    mapa[juego->cocina.posicion.fil][juego->cocina.posicion.col] = COCINA;
}

//pre: el mapa debe haber sido inicializado con la constante VACIO.
//post: inicializa 8 monedas en una posición aleatoria y válida en el mapa.
void inicializar_monedas(juego_t* juego, char mapa[MAX_FILAS][MAX_COLUMNAS]){
    juego->cantidad_herramientas = 0;
    for(int i = 0; i < MAX_MONEDAS; i++){
        do{
            juego->herramientas[juego->cantidad_herramientas].posicion = posicion_random();
        }while(!es_posicion_libre(mapa, juego->herramientas[juego->cantidad_herramientas].posicion.fil, juego->herramientas[juego->cantidad_herramientas].posicion.col));

        mapa[juego->herramientas[juego->cantidad_herramientas].posicion.fil][juego->herramientas[juego->cantidad_herramientas].posicion.col] = MONEDAS;
        juego->herramientas[juego->cantidad_herramientas].tipo = MONEDAS;
        (juego->cantidad_herramientas)++;
    }
    }

//pre: el mapa debe haber sido inicializado con la constante VACIO.
//post: inicializa una mopa en una posición aleatoria y válida en el mapa.
void inicializar_mopa(juego_t* juego, char mapa[MAX_FILAS][MAX_COLUMNAS]){
    juego->herramientas[juego->cantidad_herramientas].tipo = MOPA; 
    do{
        juego->herramientas[juego->cantidad_herramientas].posicion = posicion_random();
    }while(!es_posicion_libre(mapa, juego->herramientas[juego->cantidad_herramientas].posicion.fil, juego->herramientas[juego->cantidad_herramientas].posicion.col));

    mapa[juego->herramientas[juego->cantidad_herramientas].posicion.fil][juego->herramientas[juego->cantidad_herramientas].posicion.col] = MOPA;
    (juego->cantidad_herramientas)++;

}

//pre: el mapa debe haber sido inicializado con la constante VACIO.
//post: inicializa 5 patines en una posición aleatoria y válida en el mapa.
void inicializar_patines(juego_t* juego, char mapa[MAX_FILAS][MAX_COLUMNAS]) {
    for (int i = 0; i < MAX_PATINES ; i++) {
        do{
            juego->herramientas[juego->cantidad_herramientas].posicion = posicion_random();
        }while(!es_posicion_libre(mapa, juego->herramientas[juego->cantidad_herramientas].posicion.fil, juego->herramientas[juego->cantidad_herramientas].posicion.col));

        mapa[juego->herramientas[juego->cantidad_herramientas].posicion.fil][juego->herramientas[juego->cantidad_herramientas].posicion.col] = PATINES;
        juego->herramientas[juego->cantidad_herramientas].tipo = PATINES;
        (juego->cantidad_herramientas)++;
        }
}

//pre: el mapa debe haber sido inicializado con la constante VACIO.
//post: inicializa 5 charcos en una posición aleatoria y válida en el mapa.
void inicializar_charcos(juego_t* juego, char mapa[MAX_FILAS][MAX_COLUMNAS]){
    juego->cantidad_obstaculos = 0;
    for(int i = 0; i < MAX_CHARCOS; i++){
        do{
            juego->obstaculos[juego->cantidad_obstaculos].posicion = posicion_random();
        }while(!es_posicion_libre(mapa, juego->obstaculos[juego->cantidad_obstaculos].posicion.fil,juego->obstaculos[juego->cantidad_obstaculos].posicion.col));

        mapa[juego->obstaculos[juego->cantidad_obstaculos].posicion.fil][juego->obstaculos[juego->cantidad_obstaculos].posicion.col] = CHARCOS;
        juego->obstaculos[juego->cantidad_obstaculos].tipo = CHARCOS;
        (juego->cantidad_obstaculos)++;
    }
}

//pre: -
//post: muestra el mapa actualizado hasta la última función llamada por arriba de ésta. 
void mostrar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS]){
    printf("\n");
    for(int i = 0;  i < MAX_FILAS; i++){
        printf("\t");
        for(int j = 0; j < MAX_COLUMNAS; j++){
            printf("| %c ", mapa[i][j]);
        }
        printf("| \n ");
    }
}

//pre: las mesas deben haber sido inicializadas.
//post: devuelve false si la posición a la que quiere acceder el usuario se excede de los límites del mapa o si es una mesa,
//retorna true en caso contrario.
bool accion_posible(juego_t* juego, int fila, int columna){
    bool es_posible = true;

    if(fila >= MAX_FILAS || columna >= MAX_COLUMNAS || fila < 0 || columna < 0){
        return false;
    }

    for (int i = 0; i < juego->cantidad_mesas; i++) {
        for (int j = 0; j < juego->mesas[i].cantidad_lugares; j++) { 
            if ((juego->mesas[i].posicion[j].fil) == fila && (juego->mesas[i].posicion[j].col) == columna) {
                es_posible = false;
               }
         }
    }
    return es_posible;
}

//pre: el mozo debe haber sido inicializado.
//post: si accion_posible() devuelve true, Linguini se ubica una fila más arriba, actualizando en juego_t posición y movimientos.
void moverse_arriba(juego_t* linguini){
        int nueva_fila =  linguini->mozo.posicion.fil - 1;
        int col = linguini->mozo.posicion.col;

        if(accion_posible(linguini, nueva_fila, col)){
            linguini->mozo.posicion.fil = nueva_fila;
            (linguini->movimientos)++;
        } 
}

//pre: el mozo debe haber sido inicializado.
//post: si accion_posible() devuelve true, Linguini se ubica una fila más abajo, actualizando en juego_t posición y movimientos.
void moverse_abajo( juego_t* linguini){
        int nueva_fila =  linguini->mozo.posicion.fil + 1;
        int col = linguini->mozo.posicion.col;

        if(accion_posible(linguini, nueva_fila, col)){
            linguini->mozo.posicion.fil = nueva_fila;
            (linguini->movimientos)++;
        }
    }


//pre: el mozo debe haber sido inicializado.
//post: si accion_posible() devuelve true, Linguini se ubica una columna hacia la derecha, actualizando en juego_t posición y movimientos.
void moverse_derecha(juego_t* linguini){
        int nueva_columna =  linguini->mozo.posicion.col + 1;
        int fil = linguini->mozo.posicion.fil;

        if(accion_posible(linguini, fil, nueva_columna)){
            linguini->mozo.posicion.col = nueva_columna;
            (linguini->movimientos)++;
            }
    }


//pre: el mozo debe haber sido inicializado.
//post: si accion_posible() devuelve true, Linguini se ubica una columna hacia la izquierda, actualizando en juego_t posición y movimientos.
void moverse_izquierda(juego_t* linguini){
        int nueva_columna =  linguini->mozo.posicion.col - 1;
        int fil = linguini->mozo.posicion.fil;

        if(accion_posible(linguini, fil, nueva_columna)){
            linguini->mozo.posicion.col = nueva_columna;
            (linguini->movimientos)++;

        }
    
}

//pre: la mopa debe haber sido inicializada.
//post: devuelve true si la posición de la mopa y el mozo coinciden y si el usuario ingresó la constante MOPA, false en caso contrario.
bool puede_agarrar_mopa(juego_t* juego) {
    bool agarrar_mopa = false;
    for(int i = 0; i < juego->cantidad_herramientas; i++) {
        if (juego->herramientas[i].tipo == MOPA && 
            juego->herramientas[i].posicion.fil == juego->mozo.posicion.fil && 
            juego->herramientas[i].posicion.col == juego->mozo.posicion.col && 
            juego->mozo.tiene_mopa == false) {
            agarrar_mopa = true;
        }
    }
    return agarrar_mopa; 
}


//pre: las herramientas y obstáculos deben haber sido inicializados.
//post: mientras la posición del mozo no coincida con la de las herramientas, cocina ni obstáculos, posea una mopa y el usuario haya ingresado
//la constante MOPA, devuelve true, false en caso contrario.
bool puede_soltar_mopa(juego_t* juego){
    bool puede_soltar = true;

    for (int i = 0; i < juego->cantidad_herramientas; i++) {
        if (juego->herramientas[i].posicion.fil == juego->mozo.posicion.fil &&
            juego->herramientas[i].posicion.col == juego->mozo.posicion.col) {
            puede_soltar = false;  
        }
    }
    for (int i = 0; i < juego->cantidad_obstaculos; i++) {
        if (juego->obstaculos[i].posicion.fil == juego->mozo.posicion.fil &&
            juego->obstaculos[i].posicion.col == juego->mozo.posicion.col) {
            puede_soltar = false;  
        }
    }

    if(juego->cocina.posicion.fil == juego->mozo.posicion.fil &&
        juego->cocina.posicion.col == juego->mozo.posicion.col) {
        puede_soltar = false;  
        }

    return puede_soltar;
}

// pre: -
// post: elimina la mopa del vector de herramientas.
void eliminar_mopa(juego_t* juego){
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        if(juego->herramientas[i].tipo == MOPA){
            for (int j = i; j < juego->cantidad_herramientas - 1; j++) {
                juego->herramientas[j] = juego->herramientas[j + 1]; 
            }
            juego->cantidad_herramientas--; 
            i--; 
        }
    }
}

// pre: -
// post: agrega la mopa al vector de herramientas y aumenta su tope.
void agregar_mopa(juego_t* juego){
    if(juego->cantidad_herramientas < MAX_HERRAMIENTAS){
        juego->herramientas[juego->cantidad_herramientas].tipo = MOPA;
        juego->herramientas[juego->cantidad_herramientas].posicion.fil = juego->mozo.posicion.fil;
        juego->herramientas[juego->cantidad_herramientas].posicion.col = juego->mozo.posicion.col;
        juego->cantidad_herramientas++;
    }
}

//pre: la mopa debe haber sido inicializada.
//post: mientras se ubique en la mopa, si el usuario ingresa la constante MOPA y no tiene ninguna, procede a tenerla, si ingresa tal constante
//pero ya la tenía, la suelta mientras sea posible y en ambos casos se actualizan los movimientos en juego_t.
void accion_mopa(juego_t* juego){
    if(puede_agarrar_mopa(juego)){
        juego->mozo.tiene_mopa = true;
        eliminar_mopa(juego);
    } else if(puede_soltar_mopa(juego)){
        juego->mozo.tiene_mopa = false;
        agregar_mopa(juego);
    }
}

void inicializar_juego(juego_t *juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    juego->movimientos = 0;
    juego->dinero = 0;
    juego->cantidad_mesas = 0;
    juego->cocina.cantidad_listos = 0;
    juego->cocina.cantidad_preparacion = 0;
    
    inicializar_mapa(mapa);
    inicializar_mesa_de_4(juego, mapa);
    inicializar_mesa(juego, mapa);
    inicializar_cocina(juego, mapa);
    inicializar_linguini(juego, mapa);
    inicializar_monedas(juego, mapa);
    inicializar_mopa(juego, mapa);
    inicializar_patines(juego, mapa);
    inicializar_charcos(juego, mapa);
}




void realizar_jugada(juego_t *juego, char accion){    
    if (accion == ARRIBA) {
        moverse_arriba(juego);
    } else if (accion == ABAJO) {
        moverse_abajo(juego);
    } else if (accion == IZQUIERDA) {
        moverse_izquierda(juego);
    } else if (accion == DERECHA) {
        moverse_derecha(juego);
    } else if (accion == MOPA) {
        accion_mopa(juego);
    }
}


//pre: -
//post: coloca las mesas según la información guardada en el struct juego_t
void colocar_mesas(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego) {
    for (int i = 0; i < juego.cantidad_mesas; i++) {
        for (int j = 0; j < juego.mesas[i].cantidad_lugares; j++) {
            mapa[juego.mesas[i].posicion[j].fil][juego.mesas[i].posicion[j].col] = MESAS;
        }
    }
}

//pre: -
//post: coloca las herramientas según la información guardada en el struct juego_t
void colocar_herramientas(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t* juego) {
    for (int i = 0; i < juego->cantidad_herramientas; i++) {
        mapa[juego->herramientas[i].posicion.fil][juego->herramientas[i].posicion.col] = juego->herramientas[i].tipo;
     }
}


//pre: -
//post: coloca los obstáculos según la información guardada en el struct juego_t
void colocar_obstaculos(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego) {
    for (int i = 0; i < juego.cantidad_obstaculos; i++) {
        mapa[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = juego.obstaculos[i].tipo;
    }
}


void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    inicializar_mapa(mapa);
    colocar_mesas(mapa, juego);
    mapa[juego.cocina.posicion.fil][juego.cocina.posicion.col] = COCINA;
    colocar_herramientas(mapa, &juego);
    colocar_obstaculos(mapa, juego);
    mapa[juego.mozo.posicion.fil][juego.mozo.posicion.col] = LINGUINI;
    mostrar_mapa(mapa);
    }

int estado_juego(juego_t juego){
    if(juego.movimientos == MAX_MOVIMIENTOS && juego.dinero >= META_DINERO){
        printf("Ganaste!\n");
        return JUEGO_TERMINADO;
    } else if(juego.movimientos == MAX_MOVIMIENTOS && juego.dinero < META_DINERO){
        printf("Perdiste!\n");
        return JUEGO_TERMINADO;
    }else{
        return EN_JUEGO;
    }
}