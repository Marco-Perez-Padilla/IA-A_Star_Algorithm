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

** Archivo general_functions.cc: Implementación de funciones de ayuda
**      

** Historial de revisiones:
**      13/10/2024 - Creacion (primera version) del codigo
**      16/01/2024 - Adicion de todas las funciones excepto Usage y Help
**/

#include <iostream>
#include <regex>
#include <limits>

// Including libreries for compatibility among operative systems
#ifdef _WIN32
  #include <conio.h>  // _getch() for pressanykey
  #define CLEAR "cls" // Macro for clrscr in Windows
#else
  #define CLEAR "clear" // Macro for clrscr in Linux
#endif

#include "general_functions.h"


/**
 * @brief brief usage message for the user
 */
void Usage() {
  std::cout << "Usage: ./AStarAlgorithm <inputfile> <outputfile>\n"
            << "Try './AStarAlgorithm --help' for further information\n";
}


/**
 * @brief Help for the user to know how the program works
 */
void Help () {
  std::cout << "./AStarAlgorithm   -- Result to a maze\n"
            << "Usage:             ./AStarAlgorithm <inputfile> <outputfile>\n"
            << "\n"              
            << "<inputfile>:      Input maze file with format .txt\n"
            << "<outputfile>:     Output file to be written with the results. Format: .txt\n"
            << "\n";
}


/**
 * @brief Start menu to choose an option of the following
 * @param char option to be used
 */
void menu (char &opcion) {
  std::cout << "m. Show the [m]aze in the standard output" << std::endl;
  std::cout << "c. [c]harge new maze" << std::endl;     
  std::cout << "h. Change the [h]euristic for the research" << std::endl;
  std::cout << "g. [g]enerate inform" << std::endl;          
  std::cout << "s. Change [s]tart coordinates" << std::endl;
  std::cout << "f. Change [f]inish coordinates" << std::endl;
  std::cout << "v. Change [v]alue factor to multiply Manhattan Distance" << std::endl;
  std::cout << "p. [p]rint the maze in the specified output file" << std::endl;
  std::cout << "a. Execute the [a]* [a]lgorithm and print the results in the output file" << std::endl;
  std::cout << "q. [q]uit the program" << std::endl;
  std::cout << "Introduce the option to execute  > ";
  std::cin >> opcion;
};


/**
 * @brief Intern heuristic menu to choose the heuristic
 * @param char heurisitic to be chosen
 */
void HeuristicMenu (char &heuristic) {
  std::cout << "M. [M]anhattan Distance" << std::endl;     
  std::cout << "E. [E]uclidean Distance" << std::endl;         
  std::cout << "q. [q]uit menu" << std::endl;
  std::cout << "Introduce the option to execute  > ";
  std::cin >> heuristic;
};


/**
 * @brief Function to write a general inform about this code
 * @param ofstream output file
 */
void WriteInform (std::ofstream& outfile) {
  outfile << "    Para la realización de ésta práctica y la implementación del algoritmo A* se ha optado por la utilización de una priority_queue utilizando como comparador el atributo de función acumulada" << std::endl;
  outfile << "de las casillas. Gracias a ello, se mantiene siempre en primera posición la Casilla que el algoritmo debe seleccionar. Como criterio de parada, no es que se encuentre la meta, sino que se" << std::endl;
  outfile << "seleccione, garantizando así que el camino es mínimo. Una vez es seleccionado, se hace baktracking y se guardan las Casillas que forman parte del camino en un vector que se podrá imprimir" << std::endl;
  outfile << "más tarde. Por otro lado, es reseñable que la clase Casilla apenas tiene funcionalidades, siendo más bien una clase interfaz." << std::endl;
  outfile << "    Por otro lado, he elegido la distancia euclídea multiplicada por dos como heurística alternativa para tener en cuenta las paredes del laberinto y al no poder usar la distancia Euclídea sola" << std::endl;
}


/**
 * @brief Validates the extension of the input text file
 * @param string  
 * @return bool-type. True if the file is accepted. False otherwise
 */
bool ValidateFile (const std::string& name) {
  if (name.find('.') == std::string::npos) {
    return false;  
  }
  std::regex pattern (R"((.*)\.(txt|pdf)$)");
  return std::regex_match(name, pattern);
}


/**
 * @brief Uses ValidateFile function. Acts in consequence of the returned type
 * @param string  
 */
void CheckFileError (const std::string& name) {
  if (ValidateFile(name)) return;
  std::cerr << "Error in input file: Wrong extension. Please, try \"p06_automata_simulator --help\" for further information" << std::endl;
  exit(EXIT_FAILURE);
}


/**
 * @brief Validates the line of commands and uses Help, Usage and CheckFileError functions
 * @param int argc  
 * @param char* argv[]
 */
void ValidateCommand(int argc, char* argv[]) {
  if (argc == 2 && std::string(argv[1]) == "--help") {
    Help();
    exit(EXIT_SUCCESS);
  }
  if (argc != 3) {
    Usage();
    exit(EXIT_FAILURE);
  }
  if (argc == 3 && (std::string(argv[1]).find('.') != std::string::npos) && (std::string(argv[2]).find('.') != std::string::npos)) {
    CheckFileError(std::string(argv[1]));
    CheckFileError(std::string(argv[2]));
    clrscr();
    return;
  } else {
    Usage();
    exit(EXIT_FAILURE);
  }
}


/**
 * @brief function that waits for the user to press any key
 */
void pressanykey() {
  #ifdef _WIN32
    _getch(); // Windows
  #else
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Linux
    std::cin.get(); 
  #endif
}


/**
 * @brief function to clear the standard ouput
 */
void clrscr() {
  system(CLEAR);
}