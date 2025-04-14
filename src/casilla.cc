/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Asignatura: Inteligencia Artificial
** Curso: 3º
** Practica 2: Algoritmo A*
** Autor: Marco Pérez Padilla
** Correo: alu0101469348@ull.edu.es
** Fecha: 13/10/2024

** Archivo cya-P02-strings.cc: Implementación de la clase Casilla.
**      Contiene la sobrecarga del operador <

** Historial de revisiones:
**      13/10/2024 - Creacion (primera version) del codigo
**/

#include <iostream>

#include "casilla.h"


/**
 * @brief Overload of < operator, We will consider a casilla smaller than other one if its function is smaller
 * @param Casilla
 * @param Casilla
 * @return bool-type. True if casilla1 is smaller than casilla2
 */
bool operator<(const Casilla& casilla1, const Casilla& casilla2) {
  return casilla1.getFunction() < casilla2.getFunction();
}


/**
 * @brief Overload of << operator. We'll use it to debug the code
 * @param Casilla
 * @param ostream
 * @return ostream
 */
std::ostream& operator<<(std::ostream& os, const Casilla& casilla) {
    os << "Casilla(" << casilla.getRow() << ", " << casilla.getColumn() 
       << ", Estimation: " << casilla.getEstimation() 
       << ", Coste: " << casilla.getCoste() 
       << ", Function: " << casilla.getFunction() 
       << ", Wall: " << (casilla.getWall() ? "Yes" : "No")
       << ", Path: " << (casilla.getPath() ? "Yes" : "No") << ")";
    return os;
}


/**
 * @brief Compares casillas
 * @param Casilla
 * @param Casilla
 * @return bool-type. True if two casillas are the same. False otherwise
 */
bool operator==(const Casilla& casilla1, const Casilla& casilla2) {
  return (casilla1.getRow() == casilla2.getRow() && casilla1.getColumn() == casilla2.getColumn() && casilla1.getCoste() == casilla2.getCoste() && casilla1.getFunction() == casilla2.getFunction() && casilla1.getPadre() == casilla2.getPadre());
}
