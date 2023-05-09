/*
 * SixDegrees.h
 * Interface for the SixDegrees class
 * 
 * CS 15 Project 2: Six Degrees of Collaboration
 * 
 * by Blake Dreyer (bdreye02), 14 Apr 2023
 * 
 */
#ifndef __SIX_DEGREES__
#define __SIX_DEGREES__

#include <iostream>
#include <stack>
#include <vector>
#include <fstream>

#include "CollabGraph.h"

using namespace std;

class SixDegrees {
    public:

    /* Nullary Constructor */
    SixDegrees();
    //Public function that passes in datafile to populate the graph
    void populateGraph(ifstream &datafile);
    //Main function that activates the program
    void run(istream &input, ostream &output);
    
    private:

    //Main Commands
    void bfs(const Artist &source, const Artist &dest);
    void dfs(const Artist &source, const Artist &dest);
    void not_bfs(const Artist &source, const Artist &dest, vector<Artist> excluded);
    //Helpers
    void print(const Artist &source, const Artist &dest, ostream &output);
    bool enforceArtists(const Artist &source, const Artist &dest, ostream &output);
    bool enforceArtists(const Artist &source, const Artist &dest, vector<Artist> excluded, ostream &output);


    //Data Members
    CollabGraph colgraph;

};

#endif /* __SIX_DEGREES__*/