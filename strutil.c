#define _POSIX_C_SOURCE 200809L

#include "strutil.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


/* ******************************************************************
 *                   FUNCIONES AUXILIARES ALUMNO
 * *****************************************************************/

size_t contar_partes(const char* str, size_t largo, char sep){
    size_t cantidad_separadores = 0;
    size_t pos = 0;
    while (str[pos] != '\0'){
		if (str[pos] == sep){
			cantidad_separadores++;
		}
		pos++;
	}
    //la cantidad de partes va a ser 2 mas que la cantidad de separadores porque cuento el NULL
	return cantidad_separadores + 2;
}

bool guardar_parte(const char* str, size_t* inicio, size_t* fin, char** vector_de_partes, size_t contador_de_partes){
	char* copia_parte = strndup(str + *inicio, *fin - *inicio);
	if (!copia_parte) return false;
	vector_de_partes[contador_de_partes] = copia_parte;
	if (!vector_de_partes[contador_de_partes]) return false;
	return true;
}

/* ******************************************************************
 *                   FUNCIONES STRUTIL
 * *****************************************************************/

char *substr(const char *str, size_t n){
    char *buffer = (char*)calloc(n + 1, sizeof(char));
    if (!buffer) 
		return NULL;
    strncpy(buffer, str, n);
    return buffer;
}

void free_strv(char *strv[]){
	size_t parte = 0; 
	while(strv[parte]) { 
		free(strv[parte++]);
	}
	free(strv);
}

char **split(const char *str, char sep){
	size_t largo = strlen(str);                                         
	size_t cantidad_de_partes = contar_partes(str, largo, sep);
	char** vector_de_partes = (char **)calloc(cantidad_de_partes, sizeof(char*));
	if (!vector_de_partes) return NULL;
	size_t pos_inicial_cursor = 0;                                                  
	size_t pos_final_cursor = 0;                                                
	size_t* inicio = &pos_inicial_cursor;                                           
	size_t* fin = &pos_final_cursor;
	size_t contador_de_partes = 0;
	for (int i=0; i < largo + 1; i ++){
		if(str[i] == sep || str[i] == '\0'){
			bool guardado = guardar_parte(str, inicio, fin, vector_de_partes, contador_de_partes);
			if (!guardado) {
				free(vector_de_partes);
				return NULL;
			}
			*inicio= *fin + 1;
			contador_de_partes++;
		}
		(*fin)++;
	}
	vector_de_partes[contador_de_partes] = NULL;
	return vector_de_partes;
}

char *join(char **strv, char sep){
	// inicializo un contador para contar cuantas partes tendre que unir
	size_t contador_de_partes = 0;
	// inicializo un contador para contar los caracteres que tengo en total
	size_t total_de_caracteres = 0;
	// mientras la parte no sea null voy contando la cantidad de partes
	while (strv[contador_de_partes]) {
		// recorro cada parte y voy acumulando la cantidad de caracteres en un contador
		for (size_t j=0; j < strlen(strv[contador_de_partes]); j++) {
			total_de_caracteres++;
		}
		contador_de_partes++;
	}
	// la cantidad de separadores que voy a tener que agregar siempre es una menos que la cantidad de partes
	size_t num_de_separadores = contador_de_partes - 1;
	// pido memoria para guardar la nueva cadena que sera la union de todas las partes
	char* nueva_cadena = (char*)calloc(total_de_caracteres + num_de_separadores + 1, sizeof(char));
	// si falla el pedido de memoria devuelvo null
	if (!nueva_cadena) 
		return NULL;
	// vuelvo a recorrer todo el arreglo copiando cada cadena en una nueva
	char* puntero_a_final = nueva_cadena;
	for (size_t i = 0; i < contador_de_partes; i++) {
		puntero_a_final = stpcpy(puntero_a_final, strv[i]);
		if(i != contador_de_partes - 1){
			puntero_a_final = stpcpy(puntero_a_final, &sep);
		}
	}
	return nueva_cadena;
}




























