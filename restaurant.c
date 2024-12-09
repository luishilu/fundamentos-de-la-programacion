#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>
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
const int GANADO = 1;
const int PERDIDO = -1;
const int EN_JUEGO = 0;
const int FILAS_MESA_4 = 2;
const int COLUMNAS_MESA_4 = 2;
const char COMENSAL = 'X';
const char TOMAR_PEDIDO = 'T';
const char CUCARACHAS = 'U';
const char MILANESA_NAPOLITANA = 'M';
const char HAMBURGUESA = 'H';
const char PARRILLA = 'P';
const char RATATOUILLE = 'R';
const int TIEMPO_MILANESA = 30;
const int TIEMPO_HAMBURGUESA = 15;
const int TIEMPO_PARRILLA = 20;
const int TIEMPO_RATATOUILLE = 25;
const int NO_HAY_MESA = -1;
const int MIN_COMENSALES = 1;
const int MAX_PACIENCIA = 200;
const int MIN_PACIENCIA = 100;
const int DINERO_MESA_4 = 20000;
const int DINERO_MESA_1 = 5000;
const int DINERO_MONEDAS = 1000;
#define WHITE_BG "\033[107m"
#define DARK_GREEN "\033[38;5;22m"
#define REINICIAR "\033[0m"
#define VERDE "\033[32m"
#define NEGRITA "\033[1m"
#define SUBRAYADO "\033[4m"
const int MOVIMIENTOS_CUCARACHA = 25;
const int MOVIMIENTOS_COMENSALES = 15;

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

// pre: -
// post: devuelve true si el área ingresada está vacía y no excede los límites del mapa, retorna false en caso contrario.
bool entra_mesa(char mapa[MAX_FILAS][MAX_COLUMNAS], int fila_base, int columna_base, int espacio_necesario) {
    bool entra = true;
    for (int i = 0; i < espacio_necesario; i++) {
        for (int j = 0; j < espacio_necesario; j++) {
            int fila_actual = i + fila_base - 1;
            int columna_actual = j + columna_base - 1;

            if (fila_actual < 0 || fila_actual >= MAX_FILAS || columna_actual < 0 || columna_actual >= MAX_COLUMNAS || mapa[fila_actual][columna_actual] == MESAS) {
                entra = false;
            }
        }
    }
    return entra;
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

int mesa_atendida(juego_t juego){
    int i = 0;
    bool hay_mesa = false;
    int mesa = -1;
    while(i < juego.cantidad_mesas && !hay_mesa){
        if(juego.mesas[i].pedido_tomado){
            mesa = i;
            hay_mesa = true;
        }
    }
    return mesa;
}

// pre: -
// post: muestra el mapa actualizado hasta la última función llamada por arriba de ésta.
void mostrar_mapa(juego_t juego, char mapa[MAX_FILAS][MAX_COLUMNAS]) {
    printf("\n");
    for (int i = 0; i < MAX_FILAS; i++) {
        printf("\t");
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            bool mesa_atendida = false;
            if (juego.mozo.posicion.fil == i && juego.mozo.posicion.col == j) {
                printf("|" WHITE_BG DARK_GREEN NEGRITA " %c " REINICIAR, LINGUINI);
            } else {
                for (int k = 0; k < juego.cantidad_mesas; k++) {
                    for (int lugares = 0; lugares < juego.mesas[k].cantidad_comensales; lugares++) {
                        if (juego.mesas[k].posicion[lugares].fil == i && juego.mesas[k].posicion[lugares].col == j && juego.mesas[k].pedido_tomado) {
                            mesa_atendida = true;
                            printf("|" VERDE " %c " REINICIAR, COMENSAL);
                        }
                    }
                }
                if (!mesa_atendida) {
                    printf("| %c ", mapa[i][j]);
                }
            }
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
void moverse_abajo(juego_t* linguini){
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
            !juego->mozo.tiene_mopa){
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
    if(juego->cantidad_herramientas < MAX_HERRAMIENTAS && juego->mozo.tiene_mopa){
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
        agregar_mopa(juego);
        juego->mozo.tiene_mopa = false;
    }
}

void inicializar_juego(juego_t *juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    juego->mozo.patines_puestos = false;
    juego->movimientos = 0;
    juego->dinero = 0;
    juego->cantidad_mesas = 0;
    juego->cocina.cantidad_listos = 0;
    juego->cocina.cantidad_preparacion = 0;
    juego->cocina.platos_listos = NULL;
    juego->cocina.platos_preparacion = NULL;

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
//post: coloca los obstáculos según la información guardada en el struct juego_t
void colocar_obstaculos(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego) {
    for (int i = 0; i < juego.cantidad_obstaculos; i++) {
        mapa[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = juego.obstaculos[i].tipo;
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
//post: devuelve el índice de la primera mesa de 1 que encuentre o NO_HAY_MESA si todas están ocupadas.
int buscar_mesa_de_1(juego_t juego){
    int i = 0, j = 0;
    bool mesa_vacia = false;
    int mesa_posicion = NO_HAY_MESA;

    while(i < juego.cantidad_mesas && !mesa_vacia){
        while(j < juego.mesas[i].cantidad_lugares && !mesa_vacia){
            if(juego.mesas[i].cantidad_comensales == 0 && juego.mesas[i].cantidad_lugares == 1){
                mesa_posicion = i;
                mesa_vacia = true;
            }
            j++;
        }
        i++;
        j = 0;
    }
    return mesa_posicion;   
}

//pre: -
//post: devuelve el índice de la primera mesa adecuada al nro. de comensales o NO_HAY_MESA si todas están ocupadas.
int buscar_mesa_vacia(juego_t juego, int comensales){
    int i = 0, j = 0;
    bool mesa_vacia = false;
    int mesa_posicion = NO_HAY_MESA;

    while(i < juego.cantidad_mesas && !mesa_vacia){
        while(j < juego.mesas[i].cantidad_lugares && !mesa_vacia){
            if(juego.mesas[i].cantidad_comensales == 0 && juego.mesas[i].cantidad_lugares >= comensales){
                mesa_posicion = i;
                mesa_vacia = true;
            }
            j++;
        }
        i++;
        j = 0;
    }
    return mesa_posicion;
}


//pre: la acción debe ser válida.
//post: inicializa comensales cada 15 movimientos.
void generar_comensales(juego_t* juego, char accion){
    int comensales = -1;
    if(juego->movimientos % MOVIMIENTOS_COMENSALES == 0 && juego->movimientos != 0 && accion != MOPA && accion != TOMAR_PEDIDO && accion != PATINES){
        comensales = rand() % MAX_COMENSALES + MIN_COMENSALES;
    }

    int mesa_vacia = buscar_mesa_vacia(*juego, comensales);
    int mesa_de_1 = buscar_mesa_de_1(*juego);

    if(comensales == MIN_COMENSALES && mesa_de_1 != NO_HAY_MESA){
        juego->mesas[mesa_de_1].cantidad_comensales = comensales;
        juego->mesas[mesa_de_1].paciencia = rand() % MAX_PACIENCIA + MIN_PACIENCIA;
    } else if (mesa_vacia != NO_HAY_MESA && comensales != -1){
        juego->mesas[mesa_vacia].cantidad_comensales = comensales;
        juego->mesas[mesa_vacia].paciencia = rand() % MAX_PACIENCIA + MIN_PACIENCIA;
    }
}

//pre: -
//post: devuelve la distancia manhattan entre 2 elementos.
int distancia_manhattan(coordenada_t primer_elemento, coordenada_t segundo_elemento){
    return(abs(primer_elemento.fil - segundo_elemento.fil) + abs(primer_elemento.col - segundo_elemento.col));
}

//pre:-
//post: vuelve a inicializar una mesa.
void vaciar_mesa(juego_t* juego, int mesa){
    juego->mesas[mesa].cantidad_comensales = 0;
    juego->mesas[mesa].paciencia = 0;
    juego->mesas[mesa].pedido_tomado = false;
}

//pre: -
//post: elimina el pedido si los comensales se fueron
void eliminar_pedido(juego_t* juego, int mesa){
    int i = 0;
    bool encontrada = false;
    if(juego->mozo.cantidad_pedidos > 0){
        while (i < juego->mozo.cantidad_pedidos && !encontrada) {
            if (juego->mozo.pedidos[i].id_mesa == mesa) {
                encontrada = true;
                for (int j = i; j < juego->mozo.cantidad_pedidos - 1; j++) {
                    juego->mozo.pedidos[j] = juego->mozo.pedidos[j + 1];
                }
                juego->mozo.cantidad_pedidos--;
            }
            i++;
        }
    }
}

//pre: -
//post: elimina el pedido de la bandeja si los comensales se fueron.
void eliminar_bandeja(juego_t* juego, int mesa){
    int i = 0;
    bool encontrada = false;
    if(juego->mozo.cantidad_bandeja > 0){
        while (i < juego->mozo.cantidad_bandeja && !encontrada) {
            if (juego->mozo.bandeja[i].id_mesa == mesa) {
                encontrada = true;
                for (int j = i; j < juego->mozo.cantidad_bandeja - 1; j++) {
                    juego->mozo.bandeja[j] = juego->mozo.bandeja[j + 1];
                }
                juego->mozo.cantidad_bandeja--;
            }
            i++;
        }
    }
}

//pre: -
//post: elimina el pedido del vector de platos listos si los comensales se fueron.
void parar_pedido_listo(juego_t* juego, int mesa) {
    int i = 0;
    bool encontrada = false;
    if(juego->cocina.cantidad_listos > 0){
        while (i < juego->cocina.cantidad_listos && !encontrada) {
            if (juego->cocina.platos_listos[i].id_mesa == mesa) {
                encontrada = true;
                for (int j = i; j < juego->cocina.cantidad_listos - 1; j++) {
                    juego->cocina.platos_listos[j] = juego->cocina.platos_listos[j + 1];
                }
                juego->cocina.cantidad_listos--;

                if(juego->cocina.cantidad_listos >= 0)
                    juego->cocina.platos_listos = realloc(juego->cocina.platos_listos, sizeof(pedido_t) * (size_t)juego->cocina.cantidad_listos);

                if (juego->cocina.platos_listos == NULL) {
                    printf("Hubo un error al reservar memoria\n");
                    return;
                }
            }
            i++;
        }
    }
}

//pre: -
//post: elimina el pedido del vector de platos en preparación si los comensales se fueron.
void parar_pedido_preparacion(juego_t* juego, int mesa){
    int i = 0;
    bool encontrada = false;
    if(juego->cocina.cantidad_preparacion > 0){
        while (i < juego->cocina.cantidad_preparacion && !encontrada) {
            if (juego->cocina.platos_preparacion[i].id_mesa == mesa) {
                encontrada = true;
                for (int j = i; j < juego->cocina.cantidad_preparacion - 1; j++) {
                    juego->cocina.platos_preparacion[j] = juego->cocina.platos_preparacion[j + 1];
                }
                juego->cocina.cantidad_preparacion--;

                if(juego->cocina.cantidad_preparacion >= 0)
                    juego->cocina.platos_preparacion = realloc(juego->cocina.platos_preparacion, sizeof(pedido_t) * (size_t)juego->cocina.cantidad_preparacion);

                if (juego->cocina.platos_preparacion == NULL) {
                    printf("Hubo un error al reservar memoria\n");
                    return;
                }
            }
            i++;
        }
    } 
}

//pre: los comensales deben haber sido inicializados y la acción debe ser válida..
//post: cada vez que el mozo hace un movimiento válido, disminuye en uno la paciencia de la mesa, si tiene una cucaracha
//cerca, además disminuye 2. Si la paciencia llega a 0, la mesa se vacía.
void disminuir_paciencia(juego_t* juego, char accion){
    for(int i = 0; i < juego->cantidad_mesas; i++){
        if(juego->mesas[i].cantidad_comensales != 0 && accion != MOPA && accion != TOMAR_PEDIDO && accion != PATINES){
            juego->mesas[i].paciencia -= 1;

        for(int j = 0; j < juego->mesas[i].cantidad_lugares; j++){
            for(int k = 0; k < juego->cantidad_obstaculos; k++){
                if(juego->obstaculos[k].tipo == CUCARACHAS){
                    int distancia = distancia_manhattan(juego->mesas[i].posicion[j], juego->obstaculos[k].posicion);
                    if(distancia <= 2 && accion != MOPA && accion != TOMAR_PEDIDO && accion != PATINES){
                        juego->mesas[i].paciencia -= 2;
                    }
                }    
            }
        }
        if(juego->mesas[i].paciencia == 0){
            vaciar_mesa(juego, i);
            eliminar_pedido(juego, i);
            eliminar_bandeja(juego, i);
            parar_pedido_listo(juego, i);
            parar_pedido_preparacion(juego, i);
        }
        }
    }
}

//pre: las mesas deben haber sido inicializadas.
//post: coloca a los comensales en las mesas.
void colocar_comensales(juego_t juego, char mapa[MAX_FILAS][MAX_COLUMNAS]) {
    for (int i = 0; i < juego.cantidad_mesas; i++) {
        if (juego.mesas[i].cantidad_comensales > 0) {
            for (int j = 0; j < juego.mesas[i].cantidad_comensales; j++) {
                mapa[juego.mesas[i].posicion[j].fil][juego.mesas[i].posicion[j].col] = COMENSAL;
            }
        }
    }
}

//pre: -
//post: permite al mozo agarrar la moneda y ganar $1000, una vez agarrada, ésta es eliminada físicamente del vector.
void consumir_monedas(juego_t* juego){
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        if(juego->herramientas[i].tipo == MONEDAS && juego->mozo.posicion.fil == juego->herramientas[i].posicion.fil 
        && juego->mozo.posicion.col == juego->herramientas[i].posicion.col && !juego->mozo.tiene_mopa){
            juego->dinero += DINERO_MONEDAS;
            for (int j = i; j < juego->cantidad_herramientas - 1; j++) {
                juego->herramientas[j] = juego->herramientas[j + 1]; 
            }
            juego->cantidad_herramientas--; 
        }
    }
}

//pre: -
//post: devuelve true si la posición está desocupada y false si no.
bool entra_cucaracha(juego_t juego, coordenada_t cucaracha){
    bool entra = true;
    for(int i = 0; i < juego.cantidad_herramientas; i++){
        if(juego.herramientas[i].posicion.fil == cucaracha.fil && juego.herramientas[i].posicion.col == cucaracha.col){
            entra = false;
        }
    }
    if(juego.cocina.posicion.fil == cucaracha.fil && juego.cocina.posicion.col == cucaracha.col)
        entra = false;

    for(int i = 0; i < juego.cantidad_mesas; i++){
        for(int j = 0; j < juego.mesas[i].cantidad_lugares; j++){
            if(juego.mesas[i].posicion[j].fil == cucaracha.fil && juego.mesas[i].posicion[j].col == cucaracha.col)
                entra = false;
        }
    }
    for(int i = 0; i < juego.cantidad_obstaculos; i++){
        if(juego.obstaculos[i].posicion.fil == cucaracha.fil && juego.obstaculos[i].posicion.col == cucaracha.col)
            entra = false;
    }
    return entra;
}

//pre: la acción debe ser válida.
//post: inicializa una cucaracha cada 25 movimientos.
void generar_cucarachas(juego_t* juego, char accion){
    if(juego->movimientos % MOVIMIENTOS_CUCARACHA == 0 && juego->movimientos != 0 && accion != TOMAR_PEDIDO && accion != MOPA && accion != PATINES){
        do{
            juego->obstaculos[juego->cantidad_obstaculos].posicion = posicion_random();
        }while(!entra_cucaracha(*juego, juego->obstaculos[juego->cantidad_obstaculos].posicion));

        juego->obstaculos[juego->cantidad_obstaculos].tipo = CUCARACHAS;
        (juego->cantidad_obstaculos)++;
    }
}

//pre:-
//post: permite al mozo eliminar a la cucaracha y ésta es eliminada físicamente del vector.
void matar_cucaracha(juego_t* juego){
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(juego->obstaculos[i].tipo == CUCARACHAS && juego->mozo.posicion.fil == juego->obstaculos[i].posicion.fil 
        && juego->mozo.posicion.col == juego->obstaculos[i].posicion.col && !juego->mozo.tiene_mopa){
            for (int j = i; j < juego->cantidad_obstaculos - 1; j++) {
                juego->obstaculos[j] = juego->obstaculos[j + 1]; 
            }
            juego->cantidad_obstaculos--; 
        }
    }
}

//pre: -
//post: permite al mozo limpiar el charco y éste es eliminado físicamente del vector.
void limpiar_charco(juego_t* juego){
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(juego->mozo.tiene_mopa && juego->obstaculos[i].tipo == CHARCOS && juego->mozo.posicion.fil == juego->obstaculos[i].posicion.fil 
        && juego->mozo.posicion.col == juego->obstaculos[i].posicion.col){
            for (int j = i; j < juego->cantidad_obstaculos - 1; j++) {
                juego->obstaculos[j] = juego->obstaculos[j + 1]; 
            }
            juego->cantidad_obstaculos--; 
        }
    }
}

//pre:-
//post: permite al mozo agarrar patines y éstos son eliminados físicamente del vector.
void agarrar_patines(juego_t* juego){
    if(!juego->mozo.tiene_mopa){
        for(int i = 0; i < juego->cantidad_herramientas; i++){
            if(juego->herramientas[i].tipo == PATINES && juego->mozo.posicion.fil == juego->herramientas[i].posicion.fil 
            && juego->mozo.posicion.col == juego->herramientas[i].posicion.col){
                juego->mozo.cantidad_patines++;
                for (int j = i; j < juego->cantidad_herramientas - 1; j++) {
                    juego->herramientas[j] = juego->herramientas[j + 1]; 
                }
                juego->cantidad_herramientas--; 
            }
        }
    }
}

//pre: -
//post: devuelve true si la acción provocará que el mozo se salga del mapa o false en caso contrario.
bool fuera_del_mapa(char accion, juego_t juego) {
    bool fuera = false;
    if (accion == ARRIBA){
        if(juego.mozo.posicion.fil - 1 < 0 || juego.mozo.posicion.fil - 1 >= MAX_FILAS)
            fuera = true;
    } else if(accion == ABAJO){
        if(juego.mozo.posicion.fil + 1 < 0 || juego.mozo.posicion.fil + 1 >= MAX_FILAS)
            fuera = true;
    } else if(accion == IZQUIERDA){
        if(juego.mozo.posicion.col - 1 < 0 || juego.mozo.posicion.col - 1 >= MAX_COLUMNAS)
            fuera = true;
    } else if(accion == DERECHA){
        if(juego.mozo.posicion.col + 1 < 0 || juego.mozo.posicion.col + 1 >= MAX_COLUMNAS)
            fuera = true;
    }
    return fuera;
}

//pre: -
//post: devuelve true si la acción provocará que el mozo se choque con una mesa o false en caso contrario.
bool hay_mesa(char accion, juego_t juego) {
    bool hay_mesa = false;
    for (int i = 0; i < juego.cantidad_mesas; i++) {
        for (int j = 0; j < juego.mesas[i].cantidad_lugares; j++) {
            if(accion == ARRIBA){
                if (juego.mozo.posicion.fil - 1 == juego.mesas[i].posicion[j].fil && juego.mozo.posicion.col == juego.mesas[i].posicion[j].col)
                    hay_mesa = true;
            } else if(accion == ABAJO){
                if(juego.mozo.posicion.fil + 1 == juego.mesas[i].posicion[j].fil && juego.mozo.posicion.col == juego.mesas[i].posicion[j].col)
                    hay_mesa = true;
            } else if(accion == IZQUIERDA){
                if(juego.mozo.posicion.fil == juego.mesas[i].posicion[j].fil && juego.mozo.posicion.col - 1 == juego.mesas[i].posicion[j].col)
                    hay_mesa = true;
            } else if(accion == DERECHA){
                if(juego.mozo.posicion.fil == juego.mesas[i].posicion[j].fil && juego.mozo.posicion.col + 1 == juego.mesas[i].posicion[j].col)
                    hay_mesa = true;
            }
        }
    }
    return hay_mesa;
}

//pre: -
//post: devuelve true si la acción es posible y false en caso contrario.
bool es_mov_valido(juego_t juego, char accion) {
    return (!fuera_del_mapa(accion, juego) && !hay_mesa(accion, juego));
}

//pre: -
//post: genera un número del 1 al 4, cada uno representando al carácter de la comida que devuelve.
char generar_pedido(){
    int pedido = rand() % 4 + 1;
    if(pedido == 1)
        return MILANESA_NAPOLITANA;
    else if(pedido == 2)
        return HAMBURGUESA;
    else if(pedido == 3)
        return PARRILLA;
    else
        return RATATOUILLE;
}

//pre: -
//post: devuelve los movimientos de la comida representada por un char.
int tiempo_de_plato(char plato){
    if(plato == MILANESA_NAPOLITANA)
        return TIEMPO_MILANESA;
    else if (plato == HAMBURGUESA)
        return TIEMPO_HAMBURGUESA;
    else if (plato == PARRILLA)
        return TIEMPO_PARRILLA;
    else
        return TIEMPO_RATATOUILLE;
}


//pre: el tope debe ser mayor o igual a 0 y menor que MAX_PLATOS.
//post: devuelve el elemento con mayor cantidad de movimientos en un array de platos.
int hallar_movimientos(char platos[MAX_PLATOS], int tope){
    int mas_alto = 0;
    for(int i = 0; i < tope; i++){
        int tiempo_actual = tiempo_de_plato(platos[i]);
        if(tiempo_actual > mas_alto)
            mas_alto = tiempo_actual;
    }
    return mas_alto;
}

//pre: -
//post: toma el pedido de una mesa con comensales e inicializa un nuevo pedido.
void tomar_pedido(juego_t* juego){
    for(int i = 0; i < juego->cantidad_mesas; i++){
        for(int j = 0; j < juego->mesas[i].cantidad_lugares; j++){
            int distancia = distancia_manhattan(juego->mozo.posicion, juego->mesas[i].posicion[j]);
            if(distancia == 1 && juego->mesas[i].cantidad_comensales != 0 && !juego->mesas[i].pedido_tomado && !juego->mozo.tiene_mopa && juego->mozo.cantidad_pedidos < MAX_PEDIDOS){
                for(int k = 0; k < juego->mesas[i].cantidad_comensales; k++){
                    juego->mozo.pedidos[juego->mozo.cantidad_pedidos].platos[k] = generar_pedido();
                }
                juego->mozo.pedidos[juego->mozo.cantidad_pedidos].id_mesa = i;
                juego->mozo.pedidos[juego->mozo.cantidad_pedidos].cantidad_platos = juego->mesas[i].cantidad_comensales;
                int movimientos = hallar_movimientos(juego->mozo.pedidos[juego->mozo.cantidad_pedidos].platos, juego->mozo.pedidos[juego->mozo.cantidad_pedidos].cantidad_platos);
                juego->mozo.pedidos[juego->mozo.cantidad_pedidos].tiempo_preparacion = movimientos;
                juego->mesas[i].pedido_tomado = true;
                juego->mozo.cantidad_pedidos++;
                }
        }
    }
}

//pre: -
//post: ingresa el pedido de la mesa al vector de platos en preparación.
void llevar_pedido(juego_t* juego){
    if(juego->mozo.posicion.fil == juego->cocina.posicion.fil && juego->mozo.posicion.col == juego->cocina.posicion.col && juego->mozo.cantidad_pedidos != 0 && !juego->mozo.tiene_mopa){
        juego->cocina.platos_preparacion = realloc(juego->cocina.platos_preparacion, sizeof(pedido_t) * (size_t)(juego->cocina.cantidad_preparacion + juego->mozo.cantidad_pedidos));
       
        if(juego->cocina.platos_preparacion == NULL){
            printf("Hubo un error al reservar memoria");
                return;
        }

        while(juego->mozo.cantidad_pedidos > 0){
            juego->mozo.cantidad_pedidos--;
            juego->cocina.platos_preparacion[juego->cocina.cantidad_preparacion] = juego->mozo.pedidos[juego->mozo.cantidad_pedidos];
            juego->cocina.cantidad_preparacion++;
        }
    }
}


//pre: la acción debe ser válida.
//post: disminuye en uno el tiempo de preparación de cada pedido por cada movimiento válido.
void cocinar_pedido(juego_t* juego, char accion) {
    if(juego->cocina.cantidad_preparacion != 0 && accion != MOPA && accion != TOMAR_PEDIDO && accion != PATINES){
        for (int i = 0; i < juego->cocina.cantidad_preparacion; i++) {
            juego->cocina.platos_preparacion[i].tiempo_preparacion--;
        }
    }
}

// pre: -
// post: una vez que el tiempo de preparación de un plato llega a 0, pasa al vector de platos listos y el mozo ya puede ir a buscarlos.
void platos_preparados(juego_t* juego) {
    int i = 0;
    while (i < juego->cocina.cantidad_preparacion) {
        if (juego->cocina.platos_preparacion[i].tiempo_preparacion == 0) {
            juego->cocina.platos_listos = realloc(juego->cocina.platos_listos, sizeof(pedido_t) * (size_t)(juego->cocina.cantidad_listos + 1));
            if (juego->cocina.platos_listos == NULL) {
                printf("Hubo un error al reservar memoria\n");
                return;
            }
            juego->cocina.platos_listos[juego->cocina.cantidad_listos] = juego->cocina.platos_preparacion[i];
            juego->cocina.cantidad_listos++;

            for (int j = i; j < juego->cocina.cantidad_preparacion - 1; j++) {
                juego->cocina.platos_preparacion[j] = juego->cocina.platos_preparacion[j + 1];
            }
            juego->cocina.cantidad_preparacion--;

            if (juego->cocina.cantidad_preparacion >= 0) {
                juego->cocina.platos_preparacion = realloc(juego->cocina.platos_preparacion, sizeof(pedido_t) * (size_t)(juego->cocina.cantidad_preparacion));
                if (juego->cocina.platos_preparacion == NULL) {
                    printf("Hubo un error al reservar memoria\n");
                    return;
                }
            }
        } else {
            i++;
        }
    }
}

// pre: -
//post: una vez que el pedido está listo, permite al mozo agregarlo a su bandeja. 
void agarrar_pedido(juego_t* juego){
    if(juego->cocina.cantidad_listos != 0 && juego->cocina.posicion.fil == juego->mozo.posicion.fil && juego->cocina.posicion.col == juego->mozo.posicion.col 
    && juego->mozo.cantidad_bandeja < MAX_BANDEJA && !juego->mozo.tiene_mopa){

        while (juego->cocina.cantidad_listos > 0 && juego->mozo.cantidad_bandeja < MAX_BANDEJA) {
            juego->mozo.bandeja[juego->mozo.cantidad_bandeja] = juego->cocina.platos_listos[0];
            juego->mozo.cantidad_bandeja++;

            for (int j = 0; j < juego->cocina.cantidad_listos - 1; j++) {
                juego->cocina.platos_listos[j] = juego->cocina.platos_listos[j + 1];
            }

            juego->cocina.cantidad_listos--;
        }

        if(juego->cocina.cantidad_listos >= 0){
            juego->cocina.platos_listos = realloc(juego->cocina.platos_listos, sizeof(pedido_t) * (size_t)juego->cocina.cantidad_listos);
            if (juego->cocina.platos_listos == NULL) {
                printf("Hubo un error al reservar memoria\n");
                return;
            }
        }
    }
}

//pre: -
//post: si pisa un charco sin mopa, elimina los pedidos en la bandeja y vuelve a inicializar las mesas que tenían esos pedidos.
void pisar_charco(juego_t* juego){
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(juego->mozo.cantidad_bandeja != 0 && juego->obstaculos[i].tipo == CHARCOS && juego->obstaculos[i].posicion.fil == juego->mozo.posicion.fil
        && juego->obstaculos[i].posicion.col == juego->mozo.posicion.col && !juego->mozo.tiene_mopa){
            for(int j = 0; j < juego->mozo.cantidad_bandeja; j++){
                juego->mesas[juego->mozo.bandeja[j].id_mesa].cantidad_comensales = 0;
                juego->mesas[juego->mozo.bandeja[j].id_mesa].paciencia = 0;
                juego->mesas[juego->mozo.bandeja[j].id_mesa].pedido_tomado = false;
            }
            juego->mozo.cantidad_bandeja = 0;

        }
    }
}

//pre: -
//post: genera las acciones interactuables del mozo.
void acciones_mozo(juego_t* juego){
    consumir_monedas(juego);
    matar_cucaracha(juego);
    limpiar_charco(juego);
    pisar_charco(juego);
    agarrar_patines(juego);
    llevar_pedido(juego);
    agarrar_pedido(juego);
}

//pre: -
//post: devuelve true si el mozo es capaz de tomar el pedido si hay una mesa cerca, false en caso contrario.
bool tomo_el_pedido(juego_t juego){
    int i = 0, j = 0;
    bool pedido_tomado = false;
    while(i < juego.cantidad_mesas && !pedido_tomado){
        while(j < juego.mesas[i].cantidad_lugares && !pedido_tomado){
            int distancia = distancia_manhattan(juego.mozo.posicion, juego.mesas[i].posicion[j]);
            if(distancia == 1 && juego.mesas[i].cantidad_comensales != 0 && !juego.mesas[i].pedido_tomado && !juego.mozo.tiene_mopa && juego.mozo.cantidad_pedidos < MAX_PEDIDOS){
                pedido_tomado = true;
            }
            j++;
        }
        i++;   
        j = 0;
    }
    return pedido_tomado;
}

//pre: -
//post: permite al mozo avanzar hacia la dirección que quiera hasta chocarse con una mesa/pared, interactuando con los elementos.
void activar_patines(juego_t* juego, char accion){
    bool puede_pasar = es_mov_valido(*juego, accion);

    if(accion == TOMAR_PEDIDO){
        if(tomo_el_pedido(*juego)){
            printf("Pedido tomado!\n");
            tomar_pedido(juego);
        } else{
            printf("No hay mesa para tomar pedido\n");
        }
    }

    if(accion == MOPA){
        if(puede_agarrar_mopa(juego)){
            juego->mozo.tiene_mopa = true;
            eliminar_mopa(juego);
        }
    }
    
    while(accion == ARRIBA && puede_pasar){
        moverse_arriba(juego);
        acciones_mozo(juego);
        puede_pasar = es_mov_valido(*juego, accion);
        juego->movimientos--;
    }

    while(accion == ABAJO && puede_pasar){
        moverse_abajo(juego);
        acciones_mozo(juego);
        puede_pasar = es_mov_valido(*juego, accion);
        juego->movimientos--;
    }

    while(accion == DERECHA && puede_pasar){
        moverse_derecha(juego);
        acciones_mozo(juego);
        puede_pasar = es_mov_valido(*juego, accion);
        juego->movimientos--;
    }

    while(accion == IZQUIERDA && puede_pasar){
        moverse_izquierda(juego);
        acciones_mozo(juego);
        puede_pasar = es_mov_valido(*juego, accion);
        juego->movimientos--;
    }
    
    if(!puede_pasar){
        juego->movimientos += 1;
        juego->mozo.patines_puestos = false;
    }

}

//pre: -
//post: le suma al mozo la cantidad de dinero correspondiente por mesa y la vacía.
void recompensar_pedido(juego_t* juego, int id_mesa){
    if(juego->mesas[id_mesa].cantidad_lugares == 4)
        juego->dinero += DINERO_MESA_4;
    else
        juego->dinero += DINERO_MESA_1;

    vaciar_mesa(juego, id_mesa);
}

//pre: el plato entregado debe estar en la bandeja.
//post: entrega el pedido y éste es eliminado físicamente del vector, el mozo es recompensado.
void entregar_pedido(juego_t* juego){
    for(int i = 0; i < juego->cantidad_mesas; i++){
        for(int j = 0; j < juego->mesas[i].cantidad_lugares; j++){
            int distancia = distancia_manhattan(juego->mozo.posicion, juego->mesas[i].posicion[j]);
            for(int k = 0; k < juego->mozo.cantidad_bandeja; k++){
                if(distancia == 1 && i == juego->mozo.bandeja[k].id_mesa && !juego->mozo.tiene_mopa && juego->mesas[i].pedido_tomado){
                    recompensar_pedido(juego, i);
                    for(int l = k; l < juego->mozo.cantidad_bandeja - 1; l++){
                        juego->mozo.bandeja[l] = juego->mozo.bandeja[l + 1];
                    }
                    juego->mozo.cantidad_bandeja--;
                }
            }
        }
    }
}

//pre:-
//post: avisa de una cucaracha cerca cuando ésta empieza a bajar la paciencia de una mesa.
void hay_cucaracha_cerca(juego_t juego) {
    for (int i = 0; i < juego.cantidad_mesas; i++) {
        bool cucaracha_encontrada = false;
        for (int j = 0; j < juego.mesas[i].cantidad_lugares; j++) {
            for (int k = 0; k < juego.cantidad_obstaculos; k++) {
                if (juego.obstaculos[k].tipo == CUCARACHAS) {
                    int distancia = distancia_manhattan(juego.mesas[i].posicion[j], juego.obstaculos[k].posicion);
                    if (distancia <= 2 && !cucaracha_encontrada && juego.mesas[i].cantidad_comensales != 0) {
                        printf("Correeé, hay una cucaracha cerca de la mesa %i\n", (i+1));
                        cucaracha_encontrada = true; 
                    }
                }
            }
        }
    }
}

// pre: -
//post: imprime por pantalla la información del usuario.
void mostrar_info_juego(juego_t juego){
        printf("\n");
        printf("\t \t \t \t \t \t " NEGRITA VERDE "R" SUBRAYADO"atatouille  Rus"REINICIAR NEGRITA VERDE "h" REINICIAR"\n");
        printf("Sos Linguini, estás representado con una L en el mapa, movete con "NEGRITA"W"REINICIAR" (arriba), "NEGRITA"A"REINICIAR" (izquierda), "NEGRITA"D"REINICIAR" (derecha) o "NEGRITA"S"REINICIAR" (abajo). Si te \n");
        printf("ubicás por encima de una mopa ('O'), la podés agarrar con "NEGRITA"O"REINICIAR" y la podés soltar con la misma tecla solo si no estás ubicado\n");
        printf("sobre otro elemento. Además, si pasás por encima de unos patines ('P') y los activás con la letra "NEGRITA"P"REINICIAR", tu siguiente movimiento\n");
        printf("será único y podrás recorrer toda la fila/columna que elijas mientras esté a tu alcance hasta chocarte contra una mesa o la\n");
        printf("pared.\n");
        printf("Tu objetivo es juntar $150.000 hasta que termine el día, para eso vas a tener que asegurarte de no caerte sobre ningún charco\n");
        printf("porque podés tirar los pedidos de tu bandeja (¡limpialos con la mopa!), entregar los pedidos a las mesas lo más rápido posible\n");
        printf("porque los clientes pierden la paciencia y que nadie se entere  del problema de cucarachas del restaurante. ¡Suerte!\n");
        printf("\n");

        printf(VERDE"✿" REINICIAR" Dinero: " NEGRITA"$%i\n" REINICIAR, juego.dinero);
        printf(VERDE"✿" REINICIAR " Cantidad de patines: " NEGRITA"%i\n"REINICIAR, juego.mozo.cantidad_patines);

        if(juego.mozo.tiene_mopa){
            printf(VERDE"✿" REINICIAR" Tiene mopa: " NEGRITA "sí\n" REINICIAR);
        } else{
            printf(VERDE"✿" REINICIAR" Tiene mopa: " NEGRITA "no\n" REINICIAR);
        }

        printf(VERDE"✿" REINICIAR" Pedidos tomados: " NEGRITA "%i\n" REINICIAR, juego.mozo.cantidad_pedidos);
        printf(VERDE"✿" REINICIAR" Pedidos en la bandeja: " NEGRITA "%i\n" REINICIAR, juego.mozo.cantidad_bandeja);
        printf(VERDE"✿" REINICIAR" Pedidos listos la cocina: " NEGRITA "%i\n" REINICIAR, juego.cocina.cantidad_listos);

        int contador = MAX_MOVIMIENTOS - juego.movimientos;
        printf(NEGRITA"%i" REINICIAR " movimientos disponibles\n", contador);
        hay_cucaracha_cerca(juego);
}

void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    inicializar_mapa(mapa);
    colocar_mesas(mapa, juego);
    mapa[juego.cocina.posicion.fil][juego.cocina.posicion.col] = COCINA;
    colocar_obstaculos(mapa, juego);
    colocar_herramientas(mapa, &juego);
    mapa[juego.mozo.posicion.fil][juego.mozo.posicion.col] = LINGUINI;
    colocar_comensales(juego, mapa);
    mostrar_mapa(juego, mapa);
    mostrar_info_juego(juego);
}


void realizar_jugada(juego_t *juego, char accion){
    bool movimiento_valido = es_mov_valido(*juego, accion);
    if(movimiento_valido){
        if(juego->mozo.patines_puestos){
            activar_patines(juego, accion);
        } else if(accion == PATINES && juego->mozo.cantidad_patines > 0 && !juego->mozo.tiene_mopa){
            juego->mozo.cantidad_patines--;
            juego->mozo.patines_puestos = true;
        } else if (accion == ARRIBA) {
            moverse_arriba(juego);
        } else if (accion == ABAJO) {
            moverse_abajo(juego); 
        } else if (accion == IZQUIERDA) {
            moverse_izquierda(juego);
        } else if (accion == DERECHA) {
            moverse_derecha(juego);
        } else if (accion == MOPA) {
            accion_mopa(juego);
        } else if(accion == TOMAR_PEDIDO){
            tomar_pedido(juego);
        } 

        generar_comensales(juego, accion);
        limpiar_charco(juego);
        generar_cucarachas(juego, accion);
        disminuir_paciencia(juego, accion);
        consumir_monedas(juego);
        matar_cucaracha(juego);
        agarrar_patines(juego);
        pisar_charco(juego);
        cocinar_pedido(juego, accion);
        llevar_pedido(juego);
        platos_preparados(juego);
        agarrar_pedido(juego);
        entregar_pedido(juego);
    }
}

int estado_juego(juego_t juego){
    if(juego.movimientos == MAX_MOVIMIENTOS && juego.dinero >= META_DINERO){
        return GANADO;
    } else if(juego.movimientos == MAX_MOVIMIENTOS && juego.dinero < META_DINERO){
        return PERDIDO;
    } else{
        return EN_JUEGO;
    }
}


void destruir_juego(juego_t *juego){
    if (juego->cocina.platos_listos != NULL) {
        free(juego->cocina.platos_listos);
    }
    if (juego->cocina.platos_preparacion != NULL) {
        free(juego->cocina.platos_preparacion);
    }
}