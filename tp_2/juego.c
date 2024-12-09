#include <stdio.h>
#include "restaurant.h"
#include <time.h>
#include <stdlib.h>
static const int GANADO = 1;
static const int JUGANDO = 0;
static const int OBJETIVO_DINERO = 150000;
static const int MOVIMIENTOS_MAX = 200;
static const char ARRIBA = 'W';
static const char ABAJO = 'S';
static const char DERECHA = 'D';
static const char IZQUIERDA = 'A';
static const char MOPA = 'O';
static const char PATINES = 'P';
static const char TOMAR_PEDIDO = 'T';
#define REINICIAR "\033[0m"
#define VERDE "\033[32m"
#define FONDO_BLANCO "\033[107m"
#define NEGRO "\033[30m"
#define NEGRITA "\033[1m"
#define SUBRAYADO "\033[4m"


//pre: -
//post: guarda la acción que el usuario quiere realizar.
void pedir_movimiento(char* accion){
    printf("Ingresá tu movimiento" VERDE" ▶ "REINICIAR);
    scanf(" %c", accion);
}

//pre: -
//post: devuelve true si la tecla que ingresó el usuario es una acción válida, false en caso contrario.
bool es_accion_valida(char accion){
   return(accion == ARRIBA || accion == ABAJO || accion == DERECHA || accion == IZQUIERDA || accion == MOPA || accion == PATINES || accion == TOMAR_PEDIDO);
}

//pre: -
//post: guarda la acción que el usuario quiere realizar con los patines puestos.
void direccion_patines(char* accion){
    do {
        printf("¡Tenés los patines! ¿Cuál es tu próxima acción?" VERDE" ▶ "REINICIAR);
        scanf(" %c", accion);
    } while(*accion != ARRIBA && *accion != ABAJO && *accion != DERECHA && *accion != IZQUIERDA && *accion != MOPA && *accion != TOMAR_PEDIDO);
}

//pre: -
//post: guarda la acción que el usuario quiere realizar con los patines puestos después de agarrar la mopa.
void direccion_patines_mopa(char* accion, juego_t juego){
    do {
        if(juego.mozo.tiene_mopa)
            printf("¡Agarraste la mopa!\n");
        else
            printf("No estás sobre la mopa para agarrarla.\n");

        printf("Tenes los patines! ¿Cuál es tu próxima acción?" VERDE" ▶ "REINICIAR);
        scanf(" %c", accion);
    } while(*accion != ARRIBA && *accion != ABAJO && *accion != DERECHA && *accion != IZQUIERDA);
}

//pre: -
//post: guarda la acción que el usuario quiere realizar con los patines puestos después de tomar un pedido.
void direccion_patines_pedido(char* accion){
    do {
        printf("¡Tenés los patines! ¿Cuál es tu próxima acción?" VERDE" ▶ "REINICIAR);
        scanf(" %c", accion);
    } while(*accion != ARRIBA && *accion != ABAJO && *accion != DERECHA && *accion != IZQUIERDA && *accion != MOPA);

}

int main(){
    srand((unsigned)time(NULL)); 
    juego_t juego;
    char accion = '-';
    inicializar_juego(&juego);
    int estado = estado_juego(juego);

    do{
        system("clear");

        mostrar_juego(juego);

        if(juego.movimientos < MOVIMIENTOS_MAX){
            pedir_movimiento(&accion);

            if(es_accion_valida(accion)){
                if(accion == PATINES && juego.mozo.cantidad_patines > 0 && !juego.mozo.tiene_mopa){
                    realizar_jugada(&juego, accion);
                    direccion_patines(&accion);
                    if(accion == TOMAR_PEDIDO){
                        realizar_jugada(&juego, accion);
                        direccion_patines_pedido(&accion);
                    }
                    if(accion == MOPA){
                        realizar_jugada(&juego, accion);
                        direccion_patines_mopa(&accion, juego);
                    } 
                }
                realizar_jugada(&juego, accion);
            }
            estado = estado_juego(juego);
        }
    } while(estado == JUGANDO); 
    
    if(estado == GANADO){
        system("clear");
        printf(SUBRAYADO NEGRITA"\n\tGanaste!! :D\n" REINICIAR);
        printf("Lograste juntar $150000!!");
        printf("\n");
        printf("⋆ ｡  ﾟ ☁︎ ｡ ⋆ ｡ ﾟ ☾  ﾟ ｡ ⋆ ☾ ⋆ ｡ ﾟ\n");


    } else{
        system("clear");
        printf(SUBRAYADO NEGRITA"\n\tPerdiste :(\n" REINICIAR);
        printf("Te faltaron juntar $%i\n", (OBJETIVO_DINERO - juego.dinero));
        printf("⋆ ｡  ﾟ ☁︎ ｡ ⋆ ｡ ﾟ ☾  ﾟ ｡ ⋆ ☾ ⋆ ｡ ﾟ\n");
        printf("\n");

    }

    destruir_juego(&juego);

    return 0;
}