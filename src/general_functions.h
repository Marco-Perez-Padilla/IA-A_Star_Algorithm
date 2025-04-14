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

** Archivo general_functions.h: Declaración de funciones de ayuda
**      

** Historial de revisiones:
**      13/10/2024 - Creacion (primera version) del codigo
**      16/01/2024 - Adicion de todas las funciones excepto Usage y Help
**/

#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

#include <iostream>
#include <fstream>

void Usage();
void Help ();
void menu (char &opcion);
void HeuristicMenu (char &opcion);
void WriteInform (std::ofstream& outfile);
bool ValidateFile (const std::string& name);
void CheckFileError (const std::string& name);
void ValidateCommand(int argc, char* argv[]);
void pressanykey();
void clrscr();

#endif