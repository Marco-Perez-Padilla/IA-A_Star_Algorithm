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

** Archivo laberinto.h: Declaración de la clase Laberinto
**      Contiene los métodos de la clase

** Historial de revisiones:
**      13/10/2024 - Creacion (primera version) del codigo
**      16/01/2025 - Arreglo del código
**/

#ifndef LABERINTO_H
#define LABERINTO_H

#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <set>

#include "casilla.h"


extern int kFactor; // Global variable to select the multiply factor for the Manhattan Distance
extern char kHeuristic; // Global variable to store the heuristic selection

enum Direction {N, NE, E, SE, S, SW, W, NW};

//                   N    NE    E   SE    S   SW    W   NW
const short i_d[] = {-1,  -1,   0,   1,   1,   1,   0,  -1};
const short j_d[] = {0,    1,   1,   1,   0,  -1,  -1,  -1};


// Comparator to be used in or priority queue
struct CasillaComparator {
  bool operator() (const Casilla* casilla1, const Casilla* casilla2) const {return casilla1->getFunction() > casilla2->getFunction();}
};

// Class Laberinto. Main class
class Laberinto {
 private:
  // Atributes
  std::vector<std::vector<Casilla*>> laberinto_; // Matrix that represents the maze
  Casilla* start_; // Start
  Casilla* end_; // End
  int nodos_; // Number of elements of the matrix

  // Private methods to build and destroy a maze
  void read(std::istream& input); // Build
  void destroy(); // Destroy
 public:
  // Constructors
  Laberinto(std::vector<std::vector<Casilla*>> laberinto) : laberinto_(laberinto), start_(nullptr), end_(nullptr) {}
  Laberinto (std::istream& input);
  Laberinto () = default;
  // Destructor
  ~Laberinto();
  // Method to change a maze
  void ChangeMaze(std::istream& input);
  // Getters
  Casilla* getStart () const {return start_;}
  Casilla* getEnd () const {return end_;}
  std::vector<std::vector<Casilla*>>& getLaberinto () {return laberinto_;}
  Casilla* getCasilla (int row, int column) {return laberinto_[row][column];}
  const int getNodos () const {return nodos_;}
  void ResetNodes();
  // Setters
  void setStart(Casilla* casilla);
  void setEnd(Casilla* casilla);
  void setLaberinto(const std::vector<std::vector<Casilla*>>& maze) {laberinto_ = maze;}
  // Change methods
  bool ChangeStart(long unsigned int StartRow, long unsigned int StartColumn);
  bool ChangeEnd(long unsigned int EndRow, long unsigned int EndColumn);
  // Update the heuristic of all the maze
  void UpdateManhattan(Casilla* end);
  void UpdateEuclidean(Casilla* end);
  void UpdateHeuristic(char heuristic, Casilla* end);
  // Update atributes methods for a cell 
  void UpdateFunction(Casilla* cell);
  void UpdateCosts (Casilla* current, Casilla* neigbour, Direction direction);
  long double Heuristic(char heuristic, int start_row, int end_row, int start_column, int end_column, int factor);
  const long unsigned int ManhattanDistance (int start_row, int end_row, int start_column, int end_column, int factor);
  const long double EuclideanDistance(int start_row, int end_row, int start_column, int end_column);
  // A* Algorithm
  bool AStarRecursive(Casilla* current, Casilla* end, std::priority_queue<Casilla*, std::vector<Casilla*>, CasillaComparator>& border, std::set<Casilla*>& closed, std::vector<Casilla*>& border_vector,std::unordered_set<Casilla*>& in_border, std::vector<Casilla*>& closed_vector, std::vector<Casilla*>& path, long unsigned int iteracion, std::ofstream& archivo);
  // Print methods
  void MostrarIteracion(unsigned iteracion, const std::vector<Casilla*>& closed_vector, const std::vector<Casilla*>& border_vector, std::ofstream& archivo) const;
  void MostrarCamino (std::vector<Casilla*>& path, std::ofstream& archivo) const;
};

#endif