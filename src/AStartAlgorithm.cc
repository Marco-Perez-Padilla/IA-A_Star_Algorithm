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

** Archivo AStarAlgorithm.cc: programa cliente para ejecutar una búsqueda en A*
**

** Historial de revisiones:
**      13/10/2024 - Creacion (primera version) del codigo
**      16/01/2024 - Desarrollo del menú
**/

#include <iostream>

#include "general_functions.h"
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <string>
#include "laberinto.h"

int main(int argc, char* argv[]) {
  // Verifying the command line is valid
  ValidateCommand(argc, argv);

  // Opening the input file and checking if any error occurred while opening it
  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
    std::cerr << "Error al abrir el archivo: " << argv[1] << std::endl;
    pressanykey();
    clrscr();
    return 1;
  }

  // Opening the output file
  std::ofstream outputFile(argv[2]);

  // Message to let the user know the file could be read
  std::cout << "Maze " << std::string(argv[1]) << " has been read correctly" << std::endl;
  pressanykey();
  clrscr();

  // Creating a Maze object
  Laberinto laberinto(inputFile);

  char opcion; // Option to be chosen in the menu
  std::string newfile = std::string(argv[1]); // Additional variable for the user to be able to change the maze without stopping the program
  std::ifstream newinput; 

  const auto& maze = laberinto.getLaberinto(); //Reference which will be used to print the maze

  // Do-while menu loop 
  do {
    menu(opcion);

    switch (opcion) {
      // Show unsolved maze in the standart output stream
      case 'm' :
        clrscr();
        std::cout << "The input maze is:" << std::endl;
        
        for (const auto& row : maze) {
          for (const auto& cell : row) {
            if (cell->getWall()) { // Printing walls
              std::cout << "▒";  
            } else if (cell == laberinto.getStart()) { // Printing Start
              std::cout << "S";  
            } else if (cell == laberinto.getEnd()) { // Printing End
              std::cout << "E";  
            } else if (cell->getPath() == true) { // Printing Path
              std::cout << "*";
            } else {
              std::cout << " ";  // Printing free spaces to move inside the maze
            }
          }
          std::cout << std::endl; 
        }

        std::cout << "Maze generated correctly" << std::endl;
        pressanykey();
        clrscr();
        break;
      
      // Change the maze
      case 'c' :
        clrscr();
        std::cout << "Please insert the name of the new file. For example: data/input/M_2.txt" << std::endl;
        // Ask user for new input
        std::cin >> newfile;

        newinput = std::ifstream(newfile);
        // Managing errors
        if (!newinput.is_open()) {
          std::cout << "Error: Unable to open the file " << newfile << std::endl;
          pressanykey();  
          clrscr();  
          break;  
        }

        //Changing the maze
        laberinto.ChangeMaze(newinput);
        std::cout << "New file " << newfile << " has been charged correctly" << std::endl;
        pressanykey();
        clrscr();
        break;
      
      // Change the heuristic
      case 'h': 
        clrscr();
        // Invoke heuristic intern menu
        HeuristicMenu(kHeuristic);
        switch (kHeuristic) {
          // Euclidean distance heuristic
          case 'E':
            kHeuristic = 'E';
            break;
          // Manhattan Distance heuristic
          case 'M' :
            kHeuristic = 'M';
            break;
          // Quit
          case 'q' :
            break;
        }
        laberinto.UpdateHeuristic(kHeuristic, laberinto.getEnd());
        std::cout << "Heuristic succesfully changed: " << kHeuristic << std::endl;
        pressanykey();
        clrscr();
        break;

      // Generate basic inform explaining the basics of the code
      case 'g' : 
        clrscr();
        // If the file is already opened, close it and open it in append (managing accidental possible removes)
        if (outputFile.is_open()) {
          outputFile.close();  
          outputFile.open(argv[2], std::ios::app); // append mode
          // Write the inform
          WriteInform(outputFile);
        } else { // if it wasn't opened
          outputFile.open(argv[2]); // open it
          // Write the inform
          WriteInform(outputFile);
        }
        std::cout << "Inform generated correctly" << std::endl;
        pressanykey();
        clrscr();
        break; 

      // Change start
      case 's' : 
        clrscr(); {
          // Ask the user for new coordinates
          long unsigned int startrow {0};
          long unsigned int startcolumn {0};
          std::cout << "Changing Start coordinates." << std::endl;
          std::cout << "Introduce new Start row (It must be only one number): ";
          std::cin >> startrow;
          std::cout << "Introduce new Start column (It must be only one number): ";
          std::cin >> startcolumn;
          bool result = laberinto.ChangeStart(startrow, startcolumn);
          // If true, let it know. If false, the function will print it on the screen
          if (result == true) {
            std::cout << "Change successful" << std::endl;
          }
          pressanykey();
          clrscr();
        }
        break;

      // Change finish
      case 'f' :
        clrscr(); {
          // Ask the user for new coordinates
          long unsigned int endrow {0};
          long unsigned int endcolumn {0};
          std::cout << "Changing End coordinates." << std::endl;
          std::cout << "Introduce new End row (It must be only one number): ";
          std::cin >> endrow;
          std::cout << "Introduce new End column (It must be only one number): ";
          std::cin >> endcolumn;
          bool result = laberinto.ChangeEnd(endrow, endcolumn);
          // If true, let it know. If false, the function will print it on the screen
          if (result == true) {
            std::cout << "Change successful" << std::endl;
          }
          pressanykey();
          clrscr();
        }
        break;

      // Change the value of the factor for Manhattan Distance
      case 'v' :
        clrscr();
        // Ask for new multiply factor
        std::cout << "Select a new factor to multiply the manhattan distance heuristic (standard is 3)" << std::endl;
        std::cin >> kFactor;
        std::cout << "Change successful to " << kFactor << std::endl;
        pressanykey();
        clrscr();
        break;
        
      // Print the maze in the output file
      case 'p' :
      clrscr();
      // If the file is already opened, close it and open it in append (managing accidental possible removes)
        if (outputFile.is_open()) { 
          outputFile.close();  
          outputFile.open(argv[2], std::ios::app); // Abrir en modo append
          outputFile << std::endl;
          outputFile << "Maze: " << newfile << std::endl; // Printing the name of the maze
          for (const auto& row : maze) {
            for (const auto& cell : row) {
              if (cell->getWall()) { // Printing walls
                outputFile << "▒";  
              } else if (cell == laberinto.getStart()) { // Printing Start
                outputFile << "S";  
              } else if (cell == laberinto.getEnd()) { // Printing End
                outputFile << "E";  
              } else if (cell->getPath() == true) { // Printing Path
                outputFile << "*";
              } else {
                outputFile << " ";  // Printing free spaces to move inside the maze
              }
            }
            outputFile << std::endl; 
          }          
        } else { // if it wasn't opened
          outputFile.open(argv[2]); // open it
          outputFile << std::endl;
          outputFile << "Maze: " << newfile << std::endl; // Printing the name of the maze
          for (const auto& row : maze) {
            for (const auto& cell : row) {
              if (cell->getWall()) { // Printing walls
                outputFile << "▒";  
              } else if (cell == laberinto.getStart()) { // Printing Start
                outputFile << "S";  
              } else if (cell == laberinto.getEnd()) { // Printing End
                outputFile << "E";  
              } else if (cell->getPath() == true) { // Printing Path
                outputFile << "*";
              } else {
                outputFile << " ";  // Printing free spaces to move inside the maze
              }
            }
            outputFile << std::endl; 
          }          
        }
        std::cout << "Maze generated correctly" << std::endl;
        pressanykey();
        clrscr();
        break;

      // Execute A* Algorithm
      case 'a' :
        clrscr(); {
          //initialization 
          std::priority_queue<Casilla *, std::vector<Casilla *>, CasillaComparator> border; // Priority queue
          border.push(laberinto.getStart()); // Push the start before the search starts
          std::set<Casilla*> closed; 
          std::vector<Casilla*> border_vector(laberinto.getNodos());
          std::vector<Casilla*> closed_vector(laberinto.getNodos());
          std::vector<Casilla*> path;
          std::unordered_set<Casilla*> in_border;
          unsigned int iteration {0};

          if (outputFile.is_open()) { 
            laberinto.ResetNodes();
            outputFile.close();  
            outputFile.open(argv[2], std::ios::app); 
            outputFile << std::endl;
            outputFile << newfile << std::endl;
            // Execute the search
            laberinto.AStarRecursive(laberinto.getStart(), laberinto.getEnd(), border, closed, border_vector, in_border, closed_vector, path, iteration, outputFile);
            // Show the path
            laberinto.MostrarCamino(path, outputFile);
          } else {
            laberinto.ResetNodes();
            outputFile.open(argv[2]);
            outputFile << std::endl;
            outputFile << newfile << std::endl;
            // Execute the search
            laberinto.AStarRecursive(laberinto.getStart(), laberinto.getEnd(), border, closed, border_vector, in_border, closed_vector, path, iteration, outputFile);
            // Show the path
            laberinto.MostrarCamino(path, outputFile);
          }
          pressanykey();
          clrscr();
        }
        break;
    }
  } while (opcion != 'q'); // Finish the loop when 'q' is pressed

  // Clearing terminal and finishing the program
  clrscr();
  std::cout << "Finishing program" << std::endl;
  pressanykey();
  clrscr();

  return 0;
}
