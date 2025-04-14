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

** Archivo casilla.h: Declaración de la clase Casilla
**      Contiene los métodos de la clase. Se trata de una clase interfaz para ser usada por Laberinto

** Historial de revisiones:
**      13/10/2024 - Creacion (primera version) del codigo
**      16/01/2025 - Adicion de atributo path_ y su getter y setter
**/

#ifndef CASILLA_H
#define CASILLA_H

#include <iostream>
#include <utility>

class Casilla {
 private:
  int row_;
  int column_;
  bool wall_;
  int estimation_;
  int coste_;
  int function_;
  bool path_;
  Casilla* padre_;
 public:
  // Constructors
  Casilla() : row_(0), column_(0), wall_(false), estimation_(0), coste_(0), function_(0), path_(false), padre_(nullptr) {}
  Casilla(int i, int j,  bool wall, int estimation = 0, int coste = 0, int function = 0, bool path = false) : row_(i), column_(j), wall_(wall), estimation_(estimation), coste_(coste), function_(function), path_ (path) {}
  // Getters
  int getRow () const {return row_;}
  int getColumn () const {return column_;}
  int getEstimation () const {return estimation_;}
  int getCoste () const {return coste_;}
  int getFunction() const {return function_;}
  bool getWall () const {return wall_;}
  bool getPath () const {return path_;}
  Casilla* getPadre () const {return padre_;}
  const Casilla* getCasilla () const {return this;}
  // Setters
  void setRow (int i) {row_ = i;}
  void setColumn (int j) {column_ = j;}
  void setEstimation (int hn) {estimation_ = hn;}
  void setCoste (int coste) {coste_ = coste;}
  void setFunction (int function) {function_ = function;}
  void setWall (bool wall) {wall_ = wall;}
  void setPath (bool path) {path_ = path;}
  void setPadre (Casilla* padre) {padre_ = padre;}
};
// Operators
bool operator<(const Casilla& casilla1, const Casilla& casilla2);
std::ostream& operator<<(std::ostream& os, const Casilla& casilla);
bool operator==(const Casilla& casilla1, const Casilla& casilla2);

#endif