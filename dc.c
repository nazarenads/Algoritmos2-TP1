#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strutil.h"
#include <stdbool.h>
#include "pila.h"
#include "math.h"
#define SUMA '+'
#define RESTA '-'
#define DIVISION '/'
#define PRODUCTO '*'
#define POTENCIA '^'
#define OPERADOR_TERNARIO '?'
#define RAIZ "sqrt"
#define LOGARITMO "log"

/* ******************************************************************
 *                   OPERACIONES
 * *****************************************************************/

long suma(long a, long b, bool* error){
    return a+b;
}

long resta(long a, long b, bool* error){
    return a-b;
}

long division(long a, long b, bool* error){
    if(b == 0) *error = true;
    return a/b;
}

long producto(long a, long b, bool* error){
    return a*b;
}

long potencia(long base, long exponente, bool* error){
    if (exponente < 0) *error = true;
    double resultado = pow(base, exponente);
    long resultado_final = (long) resultado;
    return resultado_final;
}

long logaritmo(long exponente, long base, bool* error){
    if (base < 2) *error = true;
    double resultado = log(exponente) / log(base);
    long resultado_final = (long) resultado; 
    return resultado_final;
}

/* ******************************************************************
 *                   FUNCIONES DC
 * *****************************************************************/

bool leer_linea(FILE* archivo, char** linea, size_t* capacidad){
    size_t caracteres = getline(linea, capacidad, archivo);
    if (caracteres == -1) return false;
    return true;
}

bool realizar_operacion_binaria(pila_t* pila, long (*operacion)(long factor1, long factor2, bool* error), bool* error){
    long* ultimo_factor = pila_desapilar(pila);
    if (pila_esta_vacia(pila)){
        free(ultimo_factor);
        return false;
    }
    long* penultimo_factor = pila_desapilar(pila);
    long* resultado = malloc(sizeof(long));
    *resultado = operacion(*penultimo_factor, *ultimo_factor, error);
    bool apilar = pila_apilar(pila, resultado);
    if (!apilar){
        free(resultado);
        return false;
    }
    free(ultimo_factor);
    free(penultimo_factor);
    return true;
}

bool operador_ternario(pila_t* pila){
    long* ultimo_factor = pila_desapilar(pila);
    if(pila_esta_vacia(pila)) {
        free(ultimo_factor);
        return false;
    }
    long* penultimo_factor = pila_desapilar(pila);
    if(pila_esta_vacia(pila)) {
        free(penultimo_factor);
        free(ultimo_factor);
        return false;
    }
    long* antepenultimo_factor = pila_desapilar(pila);
    long* resultado = malloc(sizeof(long));
    *resultado = *antepenultimo_factor ? *penultimo_factor : *ultimo_factor;
    bool apilar = pila_apilar(pila, resultado);
    if (!apilar) {
        free(resultado);
        return false;
    }
    free(ultimo_factor);
    free(penultimo_factor);
    free(antepenultimo_factor);
    return true;
}

bool calcular_raiz(pila_t* pila, bool* error){
    long* radicando = pila_desapilar(pila);
    if (*radicando < 0) *error = true;
    double resultado = sqrt((double)(*radicando));
    long* resultado_final = malloc(sizeof(long));
    *resultado_final = (long)resultado;
    bool apilar = pila_apilar(pila, resultado_final);
    if (!apilar) return false;
    free(radicando);
    return true;
}


bool reducir_factores_apilados(pila_t* pila, char* operador, bool* error){
    if (*operador == SUMA){
        bool operacion_suma = realizar_operacion_binaria(pila, suma, error);
        if(!operacion_suma) return false;
    }else if(*operador == RESTA){
        bool operacion_resta = realizar_operacion_binaria(pila, resta, error);
        if(!operacion_resta) return false;
    }else if(*operador == DIVISION){
        bool operacion_division = realizar_operacion_binaria(pila, division, error);
        long resultado = (long)pila_ver_tope(pila);
        if(resultado == 0) return false;
        if(!operacion_division) return false;
    }else if(*operador == PRODUCTO){
        bool operacion_producto = realizar_operacion_binaria(pila, producto, error);
        if(!operacion_producto) return false;
    }else if(strcmp(operador, RAIZ)==0){
        bool operacion_raiz = calcular_raiz(pila, error);
        if(!operacion_raiz) return false;
    }else if (*operador == POTENCIA){
        bool operacion_potencia = realizar_operacion_binaria(pila, potencia, error);
        if (!operacion_potencia) return false;
    }else if(*operador == OPERADOR_TERNARIO){
        bool operacion_ternaria = operador_ternario(pila);
        if (!operacion_ternaria) return false;
    }else if(strcmp(operador, LOGARITMO)==0){
        bool operacion_logaritmo = realizar_operacion_binaria(pila, logaritmo, error);
        if (!operacion_logaritmo) return false;
    } else {
        return false;
    }
    return true;
}

bool apilar_y_operar(char** vector_tokens, pila_t* pila_tokens, size_t i){
    char* ptr;
    long numero = strtol(vector_tokens[i], &ptr, 10);
    if (*ptr == '\0'){
        long* numero_guardado = malloc(sizeof(long));
        *numero_guardado = numero;
        bool apilar = pila_apilar(pila_tokens, numero_guardado);
        if (!apilar) {
            free(numero_guardado);
            printf("ERROR\n");
            return false;
        }
    } else {
        char* operador = vector_tokens[i];
        bool error = false;
        if(pila_esta_vacia(pila_tokens)) printf("ERROR\n");
        bool reduccion = reducir_factores_apilados(pila_tokens, operador, &error);
        if (!reduccion || error) {
            printf("ERROR\n");
            return false;
        }
    }
    return true;
}
void vaciar_pila(pila_t* pila){
    while(!pila_esta_vacia(pila)){
        long* elemento =  pila_desapilar(pila);
        free(elemento);
    }
}

void obtener_resultado(pila_t* pila){
    bool quedaron_elementos = false;
    long* res = pila_desapilar(pila);
    if(!pila_esta_vacia(pila)){
        quedaron_elementos = true;
        vaciar_pila(pila);
        printf("ERROR\n");
    }
    if(!quedaron_elementos){
        printf("%ld\n", *res);
    }
    vaciar_pila(pila);
    free(res);
}

bool procesar_linea(char* linea){
    if (!linea || linea[0] == '\n') return false;
    size_t largo_linea = strlen(linea);
    if (linea[largo_linea-1] == '\n'){
        linea[largo_linea-1] = '\0';
    }
    char** vector_tokens =  split(linea, ' ');
    size_t i = 0;
    pila_t* pila_tokens = pila_crear();
    if (!pila_tokens) {
        free_strv(vector_tokens);
        return false;
    }
    bool operacion = false;
    while(vector_tokens[i]){
        operacion = apilar_y_operar(vector_tokens, pila_tokens, i);
        if(!operacion) break;
        i++;
    }
    if(operacion){
        obtener_resultado(pila_tokens);
    }
    pila_destruir(pila_tokens);
    free_strv(vector_tokens);
    return true;
}

bool dc(FILE* archivo, char* linea, size_t capacidad){
    while(leer_linea(archivo, &linea, &capacidad)){
        bool procesamiento = procesar_linea(linea);
        if(!procesamiento) {
            free(linea);
            return false;
        }
    }
    free(linea);
    return true;
}

int main() {

    char* linea = NULL; 
    size_t capacidad = 0;
    bool calculo = dc(stdin, linea, capacidad);
    if (!calculo) printf("ERROR\n");

    return 0;
}