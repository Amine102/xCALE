/* ########################################################################
* Project :                                                               *
* Function : Create different files according to the value of the class   *
* cf. multinet usecase to use pmClassifier class                          *
* Author : Soline PONSARD                                                 *
* (c) Copyright juin 2020                                                     *
######################################################################### */

#include "createContraception.h"
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>

using namespace std;
using namespace PILGRIM;

/* function to change string to vector<string>
27;2;3;3;1;1;3;4;0  -->  [27][2][3][3][1][1][3][4][0]
*/

void split(const string &str, char delimiter, vector<string> &elem)
{
  stringstream ss(str);
  string str2;
  while (getline(ss, str2, delimiter))
  {
    elem.push_back(str2);
  }
}

vector<string> split(const string &str, char delimiter)
{
  vector<string> elem;
  split(str, delimiter, elem);
  return elem;
}


void createContraception() {

    size_t line_skipped = 1;
    size_t nb_param = 6; //A CHANGER / METTRE EN PARAM
    size_t nb_param_inc = nb_param-1;
    char delimiter = ';';
    pmScoreValueType priorWeight = 1;

    int classC = 0; // METTRE EN PARAM

    char* data_file = "../../benchmarks/data/mammographic_masses.data"; // METTRE EN PARAM
    char* copy = strdup(data_file);

    pmCSVDataSet* data = new pmCSVDataSet( data_file,
                                           line_skipped,
                                           nb_param,
                                           delimiter);

    plVariablesConjunction vars = data->observed_variables();

    plType type = vars[classC].get_type();
    vector<string> vecVar =  type.get_values_as_strings();

    // All the values of the class
    for (string str : vecVar) {

      string myFile("../../benchmarks/data/dataclass_"+str+".data");

      ifstream myFlux(copy);
      ofstream newFile(myFile.c_str());

      if(myFlux) {
        string row;
        int n = 0;

        while(getline(myFlux, row)) {
          // rewriting the header row to a file
          if (n == 0) {
            char delimiter = ';';
            vector<string> x = split(row, delimiter);
            ostringstream vts;
            x.erase(x.begin() + classC);
            auto y = x.back();
            x.pop_back();
            for (auto elem : x) {
              newFile << elem;
              newFile << ";";
            }
            newFile << y << endl;
          }
          // rewriting the row to a file if the value of the class == the class value of the data
          n=1;
          char delimiter = ';';
          vector<string> x = split(row, delimiter);

          if (x[classC]==str) {
            if(newFile) {
              ostringstream vts;
              // x.pop_back();
              x.erase(x.begin() + classC);
              auto y = x.back();
              x.pop_back();
              for (auto elem : x) {
                newFile << elem;
                newFile << ";";
              }
              newFile << y << endl;
            }
            else {
              cout << "ERROR : Unable to open the file dataclass_"+str+".data" << endl;
            }
          }
        }
      }
      else {
        cout << "ERROR : Unable to open the file." << endl;
      }
    }
  }
