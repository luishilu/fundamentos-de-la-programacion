#include <stdio.h>
#include "restaurant.h"
#include <time.h>
#include <stdlib.h>

static const int JUGANDO = 0;
static const int MOVIMIENTOS_MAX = 200;
static const char ARRIBA = 'W';
static const char ABAJO = 'S';
static const char DERECHA = 'D';
static const char IZQUIERDA = 'A';
static const char MOPA = 'O';

// pre: -
//post: imprime por pantalla la información del usuario.
void mostrar_info_juego(juego_t juego){
        printf("Sos Linguini, estás representado con una L en el mapa, movete con W (arriba), A (izquierda), D (derecha) o S (abajo). Si te \n");
        printf("ubicás por encima de una mopa (aparece como 'O' en el mapa), la podés agarrar con O y la podés soltar con la misma tecla solo\n");
        printf("si no estás ubicado sobre otro elemento. El día terminará cuando llegues a los 200 movimientos y ganás si lográs juntar $150.000.\n");
        printf("Dinero: $%i\n", juego.dinero);

        if(juego.mozo.tiene_mopa){
            printf("Tiene mopa: sí\n");
        } else{
            printf("Tiene mopa: no\n");
        }

        printf("Pedidos: %i\n", juego.mozo.cantidad_pedidos);

        int contador = MOVIMIENTOS_MAX - juego.movimientos;
        printf("%i movimientos disponibles\n", contador);
}

//pre: -
//post: guarda la acción que el usuario quiere realizar.
void pedir_movimiento(char* accion){
    printf("Ingresá tu movimiento: ");
    scanf(" %c", accion);
}

//pre: -
//post: devuelve true si la tecla que ingresó el usuario es una acción válida, false en caso contrario.
bool es_accion_valida(char accion){
   return(accion == ARRIBA || accion == ABAJO || accion == DERECHA || accion == IZQUIERDA || accion == MOPA);
}

int main(){
    srand((unsigned)time(NULL)); 
    juego_t juego;
    char accion = '-';
    int estado = estado_juego(juego);

    inicializar_juego(&juego);

    do{
        system("clear");
        mostrar_juego(juego);
        mostrar_info_juego(juego);

        if(juego.movimientos < MOVIMIENTOS_MAX){
            pedir_movimiento(&accion);

            if(es_accion_valida(accion))
                realizar_jugada(&juego, accion);

            estado = estado_juego(juego);
        }
    } while(estado == JUGANDO); 

    return 0;
}