////////////////////////////////////////////////////////////////////////
//Title:       Main.h
//Author:      Kristina Klinkner
//Date:        March 20, 2002
//Description: Header file for Main.cpp
////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//    20/03/2020:
//       Haohua Li changed Main.h, the details of this modification can 
//       be viewed in the commits of CSSR-Matlab.  
//////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_H
#define MAIN_H

#include "Common.h"
#include "AllStates.h"
#include "ParseTree.h"
#include "Hash2.h"
#include "Machine.h"

void PrintCopyrightInfo() {
  std::cout << "CSSR-Matlab is a non-official distribution of CSSR, comes with ABSOLUTELY NO WARRANTY; " << 
  "This is free software, and you are welcome to redistribute it under certain conditions.  " << 
  "Read accompanying file 'LICENSE' for details. " << std::endl  << 
  "CSSR version 0.1.1, Copyright (C) 2002 Kristina Klinkner and Cosma Shalizi. Visit http://bactra.org/CSSR/ for details."
  << std::endl << std::endl;
}

#endif

