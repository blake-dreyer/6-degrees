/*
 * main.cpp
 *
 * CS 15 Project 2: Six Degrees of Collaboration
 * 
 * by Blake Dreyer (bdreye02), 14 Apr 2023
 * 
 */

#include <iostream>
#include <fstream>

#include "SixDegrees.h"

using namespace std;


int main(int argc, char *argv[])
{
    //Check for valid overall number of argruments
    if (argc < 2 || argc > 4){
        cerr << "Usage: ./SixDegrees dataFile [commandFile] [outputFile]" << endl;
        exit(1);
    }

    //Check for valid dataFile
    ifstream datafile(argv[1]);
    if (datafile.fail()) {
        cerr << argv[1] << " cannot be opened." << endl;
        exit(1);
    }
    //Populate Graph with data from dataFile
    SixDegrees six;
    six.populateGraph(datafile);

    //Run with no input or output files
    if(argc == 2){
       six.run(cin, cout);

    //Run with input file only
    } else if(argc == 3){
        ifstream input(argv[2]);
        if (input.fail()) {
            cerr << argv[2] << " cannot be opened." << endl;
            exit(1);
        }
        six.run(input, cout);

    //Run with input and output files
    } else {
        ifstream input(argv[2]);
        if (input.fail()) {
            cerr << argv[2] << " cannot be opened." << endl;
            exit(1);
        }
        ofstream output(argv[3]);
        if (output.fail()) {
            cerr << argv[3] << " cannot be opened." << endl;
            exit(1);
        }
        six.run(input, output);

    }


    return 0;
}
