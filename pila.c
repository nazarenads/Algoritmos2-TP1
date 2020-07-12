#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_INICIAL 10;
#define FACTOR_REDIMENSION 4;

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

bool pila_redimensionar(pila_t* pila, size_t nueva_capacidad) {
    void* datos_nuevo = realloc(pila->datos, nueva_capacidad * sizeof(void*));
    if (pila->capacidad > 0 && !datos_nuevo) {
        return false;
    }
	pila->capacidad = nueva_capacidad;
    pila->datos	= datos_nuevo;
    return true;
}

bool pila_esta_llena(pila_t* pila){
	return pila->capacidad == pila->cantidad;
}

bool pila_sobra_capacidad(pila_t* pila){
	size_t cantidad = pila->cantidad*FACTOR_REDIMENSION;
	return cantidad <= pila->capacidad;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void){ 
	pila_t* pila = malloc(sizeof(pila_t));
	if (!pila) {
		return NULL;
    }
	pila->cantidad = 0;
	pila->capacidad = CAPACIDAD_INICIAL;
	pila->datos = malloc(pila->capacidad * sizeof(void*));
	if (!pila->datos) {
        free(pila);
        return NULL;
    }
	return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void* valor){
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	if (pila_esta_llena(pila)){
		return pila_redimensionar(pila, pila->capacidad * 2);
	}
	return true;
}

void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	return pila->datos[(pila->cantidad)-1];
}

void* pila_desapilar(pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
	}
	pila->cantidad--;
	if (pila_sobra_capacidad(pila)){
		pila_redimensionar(pila, pila->capacidad/2);
	}
	return pila->datos[pila->cantidad];
}
