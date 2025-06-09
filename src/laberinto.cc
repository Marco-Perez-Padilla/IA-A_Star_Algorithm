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

** Archivo laberinto.h: Implementación de la clase Laberinto
**      Contiene los métodos de la clase

** Historial de revisiones:
**      13/10/2024 - Creacion (primera version) del codigo
**      16/01/2025 - Arreglo del código
**/

#include <cmath>
#include <climits>
#include <algorithm>
#include <unordered_set>

#include "laberinto.h"


// Global variables
int kFactor = 3;
char kHeuristic = 'M';


/**
 * @brief Method that deletes a cell safely
 */
void Laberinto::destroy() {
  for (auto& row : getLaberinto()) {
    for (auto& casilla: row) {
      delete casilla;
      casilla = nullptr;
    }
  }
}


/**
 * @brief Public destructor that uses destroy method
 */
Laberinto::~Laberinto() {
  destroy();
}


/**
 * @brief Read method, it builds a maze
 * @param istream input file specifying a maze
 */
void Laberinto::read(std::istream& input) {
  unsigned int rows, columns;
  // Getting rows and columns
  input >> rows >> columns;
  nodos_ = rows*columns;
  // Creating the maze
  std::vector<std::vector<Casilla*>> maze(rows, std::vector<Casilla*>(columns, nullptr));
  // Additional Casilla object we'll need 
  Casilla* tempStart = nullptr;
  // For each row
  for (unsigned int i {0}; i < rows; ++i) {
    // For each column
    for (unsigned int j {0}; j < columns; ++j) {
      int wall_value;
      // If it occurs any error while reading
      if (!(input >> wall_value)) {
        for (unsigned int k = 0; k < i; ++k) {
          for (unsigned int l = 0; l < columns; ++l) {
            delete maze[k][l]; // Delete all the casillas
          }
        }
        std::cerr << "Error reading the cell (" << i << ", " << j << ")" << std::endl;
        return;
      }
      // Comparing wall_value with 1 to determine if it's a wall or not (when wall_value is 1, isWall will initialize to True)
      bool isWall {wall_value == 1};
      int initialCost = (wall_value == 3) ? 0 : INT_MAX; // 0 para inicio, INT_MAX demás
      maze[i][j] = new Casilla(i, j, isWall, 0, initialCost, 0, false);
      if (wall_value == 3) { 
        tempStart = maze[i][j]; // If we find 3, we'll allocate it as a temporal start
      } else if (wall_value == 4) {
        setEnd(maze[i][j]);  // If we find 4, we stablish it as the end
      }
    }
  }
  setStart(tempStart); // Setting the start after we've read all the matrix (otherwise we would have problems with the start)
  setLaberinto(maze); // Setting the maze
  UpdateHeuristic(kHeuristic, getEnd()); // Updating the maze with the chosen heuristic
  // If there is no start or end
  if (!getStart() || !getEnd()) {
    std::cerr << "Error: Start or End not set correctly." << std::endl;
    for (unsigned int k = 0; k < rows; ++k) {
      for (unsigned int l = 0; l < columns; ++l) {
        delete maze[k][l]; // Delete the maze
      }
    }
    return;
  }
}


/**
 * @brief Public constructor that uses the read method
 */
Laberinto::Laberinto(std::istream& input) {
  read(input);
}


/**
 * @brief Method to change the maze we're working with
 */
void Laberinto::ChangeMaze(std::istream& input) {
  // Destroy the current maze
  destroy();
  // Create a new one
  read(input);
}


/**
 * @brief Method to reset all the nodes of the maze. Preparing the maze for a new execution
 */
void Laberinto::ResetNodes() {
  for (auto& row : laberinto_) {
    for (auto& cell : row) {
      if (cell) {
        cell->setCoste(cell == start_ ? 0 : INT_MAX);
        cell->setPadre(nullptr);
        cell->setPath(false);
      }
    }
  }
  UpdateHeuristic(kHeuristic, end_);
}


/**
 * @brief Method that updates the maze to Manhattan Distance heuristic
 * @param Casilla end, to be able to calculate the new values
 */
void Laberinto::UpdateManhattan(Casilla* end) {
  // Managing errors
  if (!end || end->getEstimation() != 0) {
    std::cerr << "Error: End isn't defined, or given cell isn't the end" << std::endl;
    return;
  }
  // Getting the end coordinates
  unsigned int endRow = end->getRow();
  unsigned int endColumn = end->getColumn();
  // For each row
  for (auto& row : getLaberinto()) {
    // For each object in the row
    for (auto& cell : row) {
      // If it has a value (not nullptr)
      if (cell) {
        // Set the new heuristic value
        unsigned heuristic = ManhattanDistance(cell->getRow(), endRow, cell->getColumn(), endColumn, kFactor);
        cell->setEstimation(heuristic);
        cell->setPath(false);
      }
    }
  }
}


/**
 * @brief Method that updates the maze to Euclidean Distance heuristic
 * @param Casilla end, to be able to calculate the new values
 */
void Laberinto::UpdateEuclidean(Casilla* end) {
  // Managing errors
  if (!end || end->getEstimation() != 0) {
    std::cerr << "Error: End isn't defined, or given cell isn't the end" << std::endl;
    return;
  }
  // Getting the end coordinates
  unsigned int endRow = end->getRow();
  unsigned int endColumn = end->getColumn();
  // For each row
  for (auto& row : getLaberinto()) {
    // For each object in the row
    for (auto& cell : row) {
      // If it has a value (not nullptr)
      if (cell) {
        // Set the new heuristic value
        unsigned heuristic = EuclideanDistance(cell->getRow(), endRow, cell->getColumn(), endColumn);
        cell->setEstimation(heuristic);
        cell->setPath(false);
      }
    }
  }
}


/**
 * @brief Method to determine what Update to do
 * @param char chosen heuristic option
 * @param Casilla end to do the calculations
 */
void Laberinto::UpdateHeuristic(char heuristic, Casilla* end) {
  if (heuristic == 'M') {
    UpdateManhattan(end);
  } else if (heuristic == 'E') {
    UpdateEuclidean(end);
  }
}


/**
 * @brief Start setter
 * @param Casilla to set as start
 */
void Laberinto::setStart(Casilla* casilla) {
  // If it isn't nullptr
  if (casilla) { 
    // Change the start
    start_ = casilla;
    // New cost is 0
    getStart()->setCoste(0);
    // New Estimation (depending on the chosen heuristic)
    getStart()->setEstimation(Heuristic(kHeuristic, getStart()->getRow(), getEnd()->getRow(), getStart()->getColumn(), getEnd()->getColumn(), kFactor));  
    // Update the function value 
    UpdateFunction(getStart());
  } else {
    std::cerr << "Error: Trying to set Start pointer to invalid memory area." << std::endl;
  }
}


/**
 * @brief Method to change the start given new coordinates
 * @param int type, row
 * @param int type, column
 * @return bool-type. True if everything has gone right. False otherwise
 */
bool Laberinto::ChangeStart(long unsigned int StartRow, long unsigned int StartColumn) {
  // If the coordinates are valid
  if (StartRow < 0 || StartRow >= getLaberinto().size() || StartColumn < 0 || StartColumn >= getLaberinto()[0].size()) {
    std::cerr << "Error: Start coordenates out of range" << std::endl;
    return false;
  }
  // Create the new Start
  Casilla* newStart = getCasilla(StartRow, StartColumn);
  // If it isn't a wall
  if (newStart) { 
    // Set it
    setStart(newStart);
    getStart()->setWall(0);
    UpdateHeuristic(kHeuristic, getEnd());
    return true;
  } else { // If nullptr, error message
    std::cerr << "Error: Trying to set Start pointer to invalid memory area." << std::endl;
    return false;
  }
}


/**
 * @brief Start setter
 * @param Casilla to set as start
 */
void Laberinto::setEnd(Casilla* casilla) {
  // If not nullptr
  if (casilla) { 
    // Set the end, with estimation 0
    end_ = casilla;
    getEnd()->setEstimation(0);  
    UpdateFunction(casilla);
  } else {
    std::cerr << "Error: Trying to set End pointer to invalid memory area." << std::endl;
  }
}


/**
 * @brief Method to change the start given new coordinates
 * @param int type, row
 * @param int type, column
 * @return bool-type. True if everything has gone right. False otherwise
 */
bool Laberinto::ChangeEnd(long unsigned int EndRow, long unsigned int EndColumn) {
  // If the coords are valid
  if (EndRow < 0 || EndRow >= getLaberinto().size() || EndColumn < 0 || EndColumn >= getLaberinto()[0].size()) {
    std::cerr << "Error: End coordenates out of range" << std::endl;
    return false;
  }
  // Creating new end
  Casilla* newEnd = getCasilla(EndRow, EndColumn);
  if (newEnd) { // If it isn't a wall, set it
    setEnd(newEnd);
    getEnd()->setWall(0);
    //Updating the heuristic
    UpdateHeuristic(kHeuristic, getEnd());
    return true;
  } else {
    std::cerr << "Error: Trying to set End pointer to invalid memory area." << std::endl; // If nullptr, error message
    return false;
  }
}


/**
 * @brief Method to update the acumulated cost. +5 horizontally or vertically. +7 in diagonal
 * @param Casilla current casilla
 * @param Casilla neighbour casilla
 * @param direction to move on
 */
void Laberinto::UpdateCosts (Casilla* current, Casilla* neigbour, Direction direction) {
  int move;
  if (direction == N || direction == E || direction == S || direction == W){
    move = 5;
  } else {
    move = 7;
  }
  int new_cost = current->getCoste() + move;
  if (new_cost < neigbour->getCoste()) {
    neigbour->setCoste(new_cost);
    neigbour->setPadre(current);
    UpdateFunction(neigbour);
  }
}


/**
 * @brief Method to update the function value of a given Casilla object
 * @param Casilla to be updated
 */
void Laberinto::UpdateFunction(Casilla* cell) {
  int coste = cell->getCoste();
  int heuristic = cell->getEstimation();
  int function = coste + heuristic;
  cell->setFunction(function);
}


/**
 * @brief Function that calculates the Manhattan Distance between two Casillas and multiply it by a factor
 * @param int type start row
 * @param int type end row
 * @param int type start column
 * @param int type end column
 * @param int type multiply factor
 * @return int tipe, result of the calulation
 */
const long unsigned int Laberinto::ManhattanDistance (int start_row, int end_row, int start_column, int end_column, int factor) {
  return ((std::abs(end_row - start_row) + std::abs(end_column - start_column)) * factor);
}


/**
 * @brief Function that calculates the Euclidean Distance between two Casillas
 * @param int type start row
 * @param int type end row
 * @param int type start column
 * @param int type end column
 * @return double type, result of the calulation
 */
const long double Laberinto::EuclideanDistance(int start_row, int end_row, int start_column, int end_column) {
  return (std::sqrt(pow((end_row - start_row), 2) + pow((end_column - start_column), 2)) * 2);
}


/**
 * @brief Function that choses between two possible heuristics
 * @param char chosen heuristic option
 * @param int type start row
 * @param int type end row
 * @param int type start column
 * @param int type end column
 * @param int type multiply factor
 * @return double type, result of the calulation
 */
long double Laberinto::Heuristic(char heuristic, int start_row, int end_row, int start_column, int end_column, int factor) {
  if (heuristic == 'M') {
    return ManhattanDistance(start_row, end_row, start_column, end_column, factor);
  } else if (heuristic == 'E') {
    return EuclideanDistance(start_row, end_row, start_column, end_column);
  } else { return -1;}
}


/**
 * @brief Method that executes the A* Algorithm and prints it in the output file. it also prints it in the standard error stream to help debug the function
 * @param Casilla current casilla 
 * @param Casilla end, goal to reach
 * @param priority_queue of Casillas. It stores the Casillas in a queue but priorizing those with less function values thanks to our CasillaComparator
 * @param set of Casillas. It stores the visited Casillas to do a quicker comparison
 * @param vector of Casillas, it stores the (acumulated) bordier
 * @param vector of Casillas, it stores the inspected (and so, closed) casillas
 * @param vector of Casillas, path. It will store the result path of the A* Algorithm
 * @param int type to keep a record of the iterations we do
 * @param ofstream output file to be written in
 * @return bool-type, true if the search is successful
 */
bool Laberinto::AStarRecursive(Casilla* current, Casilla* end, std::priority_queue<Casilla*, std::vector<Casilla*>, CasillaComparator>& border, std::set<Casilla*>& closed, std::vector<Casilla*>& border_vector, std::unordered_set<Casilla*>& in_border, std::vector<Casilla*>& closed_vector, std::vector<Casilla*>& path, long unsigned int iteracion, std::ofstream& archivo) {
  // Base case. If border is empty, there is no solution
  if (border.empty()) {
    return false;
  }

  // Getting the Casilla with the least value of function (top of the priority queue) and pop it
  current = border.top();
  border.pop();

  // If the one we are inspecting is the goal, we've found our path. We proceed to do a backtracking
  if (current == end) {
    std::cerr << "Goal inspected: " << *current << std::endl; // Message to help debugging
    Casilla* pathNode = current;
    while (pathNode != nullptr) { // While exists 
      if (pathNode != getStart() && pathNode != getEnd()) { // If different than end and start
        pathNode->setPath(true);  // Change path atribute to true
        path.push_back(pathNode); // Store it in the result vector
      }
      pathNode = pathNode->getPadre();  // Continue the backtracking with the father
    }
    return true;
  }

  // Add the Casilla to the closed ones
  closed.insert(current);
  closed_vector.push_back(current);
  std::cerr << "Inspecting: " << *current << std::endl; // Message to help debugging

  //For each direction
  for (int dir = 0; dir < 8; ++dir) {
    int neighborRow = current->getRow() + i_d[dir];    // Getting the new coordinate in X axis
    int neighborCol = current->getColumn() + j_d[dir]; // Getting the new coordinate in Y axis

    // Verify it is within the limits of our maze
    if (neighborRow >= 0 && neighborRow < static_cast<int>(laberinto_.size()) && neighborCol >= 0 && neighborCol < static_cast<int>(laberinto_[0].size())) {
      // If it is, get the Casilla of the new coordinates
      Casilla* neighbor = getCasilla(neighborRow, neighborCol);
      // If it isn't a wall nor have been processed
      if (neighbor && !neighbor->getWall() && closed.find(neighbor) == closed.end()) {
        // Update attributes 
        UpdateCosts(current, neighbor, static_cast<Direction>(dir)); 
        UpdateFunction(neighbor);
        std::cerr << "Generating: " << *neighbor << std::endl;   // Message to help debugginf
        // Push the Casilla with new attributes to the border (add it to the queue)
        if (!in_border.contains(neighbor)) {
          border.push(neighbor);
          border_vector.push_back(neighbor);
          in_border.insert(neighbor);
          neighbor->setPadre(current);  // Get the father
        }
      }
    }
  }

  // Print the iterations
  MostrarIteracion(iteracion++, closed_vector, border_vector, archivo);

  // Recursive call
  return AStarRecursive(current, end, border, closed, border_vector, in_border, closed_vector, path, iteracion, archivo);
}


/**
 * @brief Additional method to show an iteration
 * @param int type interation
 * @param vector generated Casillas
 * @param vector inspected Casillas
 * @param ofstream name of the file to be written in
 */
void Laberinto::MostrarIteracion(unsigned iteracion, const std::vector<Casilla*>& closed_vector, const std::vector<Casilla*>& border_vector, std::ofstream& archivo) const {
  archivo << "Iteración " << iteracion << std::endl;
  archivo << "Casillas generadas: ";
  if (border_vector.empty()) {
    archivo << "-"; 
  } else {
    for (long unsigned int i = 0; i < border_vector.size(); ++i) {
      if (border_vector[i] != nullptr) {
        archivo << "(" << border_vector[i]->getRow() << ", " << border_vector[i]->getColumn() << ")";
        if (i < border_vector.size() - 1) {
          archivo << ", ";
        }
      } 
    }
  }

  archivo << std::endl;

  archivo << "Casillas inspeccionadas: ";
  if (border_vector.empty()) {
    archivo << "-"; 
  } else {
    for (long unsigned int i = 0; i < closed_vector.size(); ++i) {
      if (closed_vector[i] != nullptr) {
        archivo << "(" << closed_vector[i]->getRow() << ", " << closed_vector[i]->getColumn() << ")";
        if (i < closed_vector.size() - 1) {
          archivo << ", ";
        }
      } 
    }
  }

  archivo << std::endl;
  archivo << "____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << std::endl;
}


/**
 * @brief Additional method to print the followed path to solve the maze
 * @param vector of Casillas, result path
 * @param ofstream file to be written in
 */
void Laberinto::MostrarCamino(std::vector<Casilla*>& path, std::ofstream& archivo) const {
  archivo << "El camino encontrado es: " << std::endl;
  for (long unsigned int i {path.size()}; i > 0; --i) {
    if (path[i] != nullptr) {
      archivo << "(" << path[i]->getRow() << ", " << path[i]->getColumn() << ")";
      if (i > 1) {
        archivo << ", ";
      }
    }
  }
  archivo << std::endl;
  archivo << "Coste: ";
  archivo << getEnd()->getFunction() << std::endl;
}
