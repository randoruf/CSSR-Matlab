/////////////////////////////////////////////////////////////////////////////
//Title:	Main.cpp (for program CSSR)
//Author:	Kristina Klinkner
//Date:		July 23, 2003
//Description:	Creates separate causal states for each history of data
//		with a singular probability distribution.  History length
//		increases incrementally until cutoff point is reached.  Then
//              removes transient states, determinizes remaining states, and
//              calculates various metrics for the resulting state machine.
//              Outputs a file of states, a file of state sequences, a dot
//              file, and an information file with the metrics.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2002 Kristina Klinkner
//    This file is part of CSSR
//
//    CSSR is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    CSSR is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with CSSR; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//    20/03/2020:
//       Haohua Li changed Main.cpp, the details of this modification can 
//       be viewed in the commits of CSSR-Matlab.  
//////////////////////////////////////////////////////////////////////////////


#include "Main.h"


/////////////////////////////////////////////////////////////////////////////
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  //Sample Usage:
  //    CSSR(ofilename,   alphabet,   data,     maxlength,    siglevel)
  //    CSSR(ofilename,   alphabet,   data,     maxlength,    siglevel,     true)
  //         prhs[0]      prhs[1]     prhs[2]   prhs[3]       prhs[4]       prhs[5]
  //prhs[0] is name of output files, 
  //prhs[1] is a alphabet character array 
  //prhs[2] is a cell array which stores multiple time series from the same source, 
  //prhs[3] is maximum length of strings in the causal state, 
  //prhs[4] is the significance level (must be specified), 
  //prhs[5] is use of chi-squared test (optional)

  HashTable2 *alphaHash;
  bool stateRemoved = false; //dummy
  Machine *machine;


  // Check the number of inputs (on the right hand side )
  if (nrhs != 5 && nrhs != 6) {
    mexErrMsgIdAndTxt("MATLAB:CSSR:nargin", "Improper number of parameters. See https://github.com/randoruf/CSSR-Matlab for more help. ");
  }

  // name of output files
  char *ofilename = mxArrayToString(prhs[0]);
  if (ofilename == NULL){
    mexErrMsgIdAndTxt("MATLAB:CSSR:conversionFailed", "Could not convert output name.  See https://github.com/randoruf/CSSR-Matlab for more help. ");
  } 
  // mxFree(ofilename);  // remember to clean up to avoid memory leak.

  // alphabet must be a string 
  char *alphabet = mxArrayToString(prhs[1]);
  if (alphabet == NULL){
    mexErrMsgIdAndTxt("MATLAB:CSSR:conversionFailed", "Could not convert alphabet set.  See https://github.com/randoruf/CSSR-Matlab for more help. ");
  } 
  // mxFree(alphabet);  // remember to clean up to avoid memory leak.

  //input data must be a cell array,   
  //identify whether to use multiple-line mode or not by check the number of rows in data. 
  if (!mxIsCell (prhs[2])){
    mexErrMsgTxt("The input data must be a cell array.  See https://github.com/randoruf/CSSR-Matlab for more help.");
  } 
  if (mxGetN(prhs[2]) != 1){
    mexErrMsgTxt("Each line can only store a distinct time series.  See https://github.com/randoruf/CSSR-Matlab for more help.");
  }
  bool isMulti = false;
  size_t M = mxGetM(prhs[2]);     // prhs[2] is a cell array. 
  char **data = new char* [M];    // an array that store pointer of char*
  // multiple distinct time series...
  if (M > 1){ 
    isMulti = true;  
    char *buffer = NULL; // initilise a pointer to a size-fixed buffer.
    int  buffer_len = MAX_LINE_SIZE + END_STRING; 
    buffer = (char*) mxMalloc(buffer_len*sizeof(char)); // allocate memory space for the buffer
    for (size_t i = 0; i < M; i++){ //convert each line of time series, with buffer (limit is defined in MAX_LINE_SIZE+1).....
      if (mxGetString(mxGetCell(prhs[2], i), buffer, buffer_len) == 0) { //0 on success or if strlen == 0, and 1 on failure.
        data[i] = (char*) mxMalloc((strlen(buffer)+END_STRING)*sizeof(char)); // .... create and allocate memory space for data[i]
        strcpy(data[i], buffer); // .... copy string from buffer to data[i]
      }else { 
        mexErrMsgIdAndTxt("MATLAB:CSSR:conversionFailed", "Could not convert string data. Try to modify `MAX_LINE_SIZE` in ParseTree.h and then recompile or see https://github.com/randoruf/CSSR-Matlab for more help.");
      } 
    }
    mxFree(buffer);
    // mxFree(data[i]);  // remember to clean up to avoid memory leak.

  // single-line mode (by default)
  }else if (M == 1){ 
    isMulti = false; 
    data[0] = mxArrayToString(mxGetCell(prhs[2], 0));
    if(data[0] == NULL){
      mexErrMsgIdAndTxt( "MATLAB:CSSR:conversionFailed", "Could not convert the time series to string.  See https://github.com/randoruf/CSSR-Matlab for more help.");
    }
    // mxFree(data[0]);  // remember to clean up to avoid memory leak.
  }else{
    mexErrMsgTxt("The input data is empty...  See https://github.com/randoruf/CSSR-Matlab for more help. "); 
  }

  //print the original data series 
  char *data_txtfile = new char[strlen(ofilename) + 16 + END_STRING]; 
  sprintf(data_txtfile, "%s_data_series.txt", ofilename); 
  ofstream data_txtfile_fp(data_txtfile, ios::out);
  if (!data_txtfile_fp) {
    mexErrMsgTxt(" the data series output file cannot be opened "); 
  }else{
    for (size_t i = 0; i < M; i++){
      data_txtfile_fp << data[i] << "\n";  // 'LF' format by default 
    }
  }
  data_txtfile_fp.close();
  
  for (size_t i = 0; i < M; i++){
    mxFree(data[i]);
  }
  delete[] data;

  //read maximum history length 
  if (!mxIsDouble(prhs[3]) || !mxIsScalar(prhs[3]) || mxIsComplex(prhs[3])) { 
    mexErrMsgIdAndTxt("MATLAB:CSSR:typeargin", "Maximum history length has to be scalar, non-complex and numerical.  See https://github.com/randoruf/CSSR-Matlab for more help. ");
  }
  double raw_max_length = mxGetScalar(prhs[3]); 
  int max_length = (int) raw_max_length; // cast the original history length into integer.
  if (raw_max_length - max_length != 0){
    mexErrMsgIdAndTxt("MATLAB:CSSR:typeargin", "Maximum history length has to an integer.  See https://github.com/randoruf/CSSR-Matlab for more help.");
  }
  //valid history length of CSSR
  if (max_length < 2){
    mexErrMsgIdAndTxt("MATLAB:CSSR:typeargin", "Maximum history length has to be larger than 1. "); 
  }

  //read significance levels (a double scalar)
  if (!mxIsDouble(prhs[4]) || !mxIsScalar(prhs[4]) || mxIsComplex(prhs[4])) { 
    mexErrMsgIdAndTxt("MATLAB:CSSR:typeargin", "Significance level has to be real, double and scalar. See https://github.com/randoruf/CSSR-Matlab for more help. ");
  }
  double sigLevel = mxGetScalar(prhs[4]);  
  if (sigLevel > 0 && sigLevel < 1){ 
    //std::cout << "Significance level set to " << sigLevel << "." << std::endl;
  }else{
    mexErrMsgIdAndTxt("MATLAB:CSSR:typeargin", "Significance level has to be in range of (0, 1). See https://github.com/randoruf/CSSR-Matlab for more help. ");
  }

  // check if to use Chi-Squared significance test 
  bool isChi = false;
  if (nrhs == 6){
    char *significanceTestMode = mxArrayToString(prhs[5]);
    if (significanceTestMode == NULL){
      mexErrMsgIdAndTxt("MATLAB:CSSR:conversionFailed", "Could not convert significance test mode to string. See https://github.com/randoruf/CSSR-Matlab for more help.");
    }
    // choose chi-square or ks ? 
    if (strcmp(significanceTestMode, "ch") == 0 && isChi == false) {
      isChi = true;
      //std::cout << "Using Chi-squared significance test" << std::endl; 
    }else if (strcmp(significanceTestMode, "ks") == 0 && isChi == false) {
      isChi = false;
      //std::cout << "Using Kolmogorov-Smirnov significance test" << std::endl;
    }else {
      mexErrMsgIdAndTxt("MATLAB:CSSR:typeargin", "The parameter of significance test mode is incorrect. See https://github.com/randoruf/CSSR-Matlab for more help. "); 
    }
    mxFree(significanceTestMode);  // remember to clean up to avoid memory leak.
  }

  //PrintCopyrightInfo();

  //_________________________________________________________________________________________________
  //create parse tree to store all strings in data
  ParseTree parsetree(max_length);

  //if using multi-line input, read in data and enter
  //tree one line at a time
  if (isMulti) {    
    // std::cout << "Multi-line option is set." << std::endl
    // << "Max line length is " << MAX_LINE_SIZE << "."<< std::endl;
    parsetree.ReadProcessMultiLine(alphabet, data_txtfile);
  }
  else { //otherwise do data read first, then enter in tree
    //read in data and alphabet from files
    parsetree.ReadInput(alphabet, data_txtfile);
    //enter data in tree
    parsetree.FillTree();
  }

  //make hash table of alpha symbols and indices
  alphaHash = parsetree.MakeAlphaHash();

  //create array of states
  AllStates allstates(parsetree.getAlphaSize(), sigLevel, isChi);

  //calculate frequency of occurrence of symbols
  allstates.InitialFrequencies(parsetree);

  //check all possible strings up to max 
  //length and compare distributions
  for (int k = 1; k <= max_length; k++) {
    allstates.CalcNewDist(k, parsetree);
  }

  //remove shorter strings
  stateRemoved = allstates.DestroyShortHists(max_length, parsetree);

  //remove all non-recurring states
  allstates.CheckConnComponents(parsetree);

  //check futures longer than 1,
  //by using determinism of states
  allstates.Determinize(parsetree);

  //remove all non-recurring states (again, since there may be new ones)
  allstates.CheckConnComponents(parsetree);

  //store transitions from state to state
  allstates.StoreTransitions(parsetree.getMaxLength(), parsetree.getAlpha());

  //calculate distribution/frequency of states.
  //  to write state series to a text file called "state_series.txt" 
  char *output_name = new char[strlen(ofilename) + 23 + END_STRING] ; 
  sprintf(output_name, "%s_state_series.txt", ofilename);
  allstates.GetStateDistsMulti(parsetree, output_name, alphaHash, isMulti);

  //calculate information values
  machine = new Machine(&allstates);
  machine->CalcRelEnt(parsetree, alphaHash, isMulti);
  machine->CalcRelEntRate(parsetree, alphaHash, isMulti);
  machine->CalcCmu();
  machine->CalcEntRate();
  machine->CalcVariation(parsetree, alphaHash, isMulti);

  //print out states
  sprintf(output_name, "%s_results.txt", ofilename);
  allstates.PrintOut(output_name, parsetree.getAlpha());
  //print out machine and calculations
  sprintf(output_name, "%s_info.txt", ofilename);
  machine->PrintOut(output_name, alphabet, data_txtfile, max_length, sigLevel, isMulti, isChi, parsetree.getAlphaSize());
  sprintf(output_name, "%s_inf.dot", ofilename);
  machine->PrintDot(output_name, parsetree.getAlpha());
  
  // clean up to avoid memory leak.
  delete machine; 
  delete[] output_name;
  delete[] data_txtfile;
  // mxFree(alphabet); 
  mxFree(ofilename);

  /* Matlab outputs */ 
  int num_inferred_states = allstates.getArraySize();
  // allocate memory space for output, statistical complexity 
  plhs[0] = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL); // memory-allocation 
  plhs[1] = mxCreateNumericMatrix(1, num_inferred_states, mxDOUBLE_CLASS, mxREAL); 
  // write results to Matlab parameters on the left hand side. 
  double* Cmu = (double*) mxGetData(plhs[0]);  // acquire the pointer 
  *Cmu = machine->getCMu();
  // write the probability distribution of causal states. 
  double* stationary_distribution_causal_state = (double*) mxGetData(plhs[1]); 
  for (int i = 0; i < num_inferred_states; i++){
    stationary_distribution_causal_state[i] = allstates.getState(i)->getFrequency(); 
  }

  return ;
}