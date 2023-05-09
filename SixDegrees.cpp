/*
 * SixDegrees.cpp
 * Implementation for the SixDegrees class
 * 
 * CS 15 Project 2: Six Degrees of Collaboration
 * 
 * by Blake Dreyer (bdreye02), 14 Apr 2023
 * 
 */
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <fstream>

#include "CollabGraph.h"
#include "SixDegrees.h"

using namespace std;

const string QUIT = "quit";
const string BFS = "bfs";
const string DFS = "dfs";
const string NOT = "not";


/* SixDegrees 
 * Arguments: None
 * Purpose: Nullary Constructor */
SixDegrees::SixDegrees(){

}

/* populateGraph
 * Arguments: datafile containing lists of artists and songs
 * Purpose: Populates a CollabGraph with the datafile */
void SixDegrees::populateGraph(ifstream & datafile){
    
    vector<Artist> artists;
    string next;
    getline(datafile, next);
    int numArtists = 0;
    
    while(!datafile.eof()){
        Artist currArtist = Artist(next);
        artists.push_back(currArtist);
        numArtists++;
        //grab songs for that artist
        while(next != "*"){
            artists.back().add_song(next);
            getline(datafile, next);
        }
        //flush the star
        getline(datafile, next);
    }

    for(int i = 0; i < numArtists; i++){
        colgraph.insert_vertex(artists[i]);
    }

    string collab = "";

    //Populate graph edges while folowing invariant rules
    for(int i = 0; i < numArtists; i++){
        for(int s = 0; s < numArtists; s++){
            if(artists[i] != artists[s]){
                collab = artists[i].get_collaboration(artists[s]);
                if(collab != ""){
                    colgraph.insert_edge(artists[i], artists[s], collab);
                }
                
            }
        }
        
    }
}

/* Play
 * Arguments: An input stream and an output stream
 * Purpose: Runs the program */
void SixDegrees::run(istream &input, ostream &output){
    string command, src, des, next;
    //Loop that takes commands
    while(!getline(input, command).fail()){
        colgraph.clear_metadata();
        if (command == BFS){
            getline(input, src);
            Artist source(src);
            getline(input, des);
            Artist destination(des);
            if(enforceArtists(source, destination, output)){
                bfs(source, destination);
                print(source, destination, output);
            }

        } else if (command == DFS){
            getline(input, src);
            Artist source(src);
            getline(input, des);
            Artist destination(des);
            if(enforceArtists(source, destination, output)){
                dfs(source, destination);
                print(source, destination, output);
            }

        } else if (command == NOT){
            getline(input, src);
            Artist source(src);
            getline(input, des);
            Artist destination(des);
            vector<Artist> excluded;
            getline(input, next);
            while(next != "*"){
                excluded.push_back(Artist(next));
                getline(input, next);
            }
            if(enforceArtists(source, destination, excluded, output)){
                not_bfs(source, destination, excluded);
                print(source, destination, output);
            }

        } else if (command == QUIT){
            break;

        } else {
            output << command << " is not a command. Please try again." << endl;
        }

    }
}

// END OF PUBLIC FUNCTIONS
// ALL FUNCTIONS BELOW HERE ARE PRIVATE

/* bfs
 * Arguments: Two Artists, a source and a destination
 * Purpose: Finds the fastest route between the two artists using
 * a greedy breadth first search (Djikstra's Algorithm), if a path
 * exists */
void SixDegrees::bfs(const Artist &source, const Artist &dest){
    if(source == dest){
        return;
    }
    queue<Artist> bfsQueue;
    bfsQueue.push(source);
    
    while(!bfsQueue.empty()){
        Artist current = bfsQueue.front();
        bfsQueue.pop();
        colgraph.mark_vertex(current);
        vector<Artist> neighbors = colgraph.get_vertex_neighbors(current);
        int ncount = neighbors.size();
        for(int i = 0; i < ncount; i++){
            Artist now = neighbors.at(i);
            if(!colgraph.is_marked(now)){
                colgraph.set_predecessor(now, current);
                bfsQueue.push(now);
                if(now == dest){
                    return;
                }
            }
        }
    }
}

/* dfs
 * Arguments: Two Artists, a source and a destination
 * Purpose: Finds a route between the two artists using depth first
 * search, if a path exists */
void SixDegrees::dfs(const Artist &source, const Artist &dest){
    colgraph.mark_vertex(source);
    if(source == dest){
        return;
    } else {
        vector<Artist> neighbors = colgraph.get_vertex_neighbors(source);
        int ncount = neighbors.size();
        for(int i = 0; i < ncount; i++){
            Artist now = neighbors.at(i);
            if(!colgraph.is_marked(now)){
                colgraph.set_predecessor(now, source);
                dfs(now, dest);
            }
        }
    }
    return;
}

/* not_bfs
 * Arguments: Two Artists, a source and a destination. A vector of Artists to exclude
 * Purpose: Finds the fastest route between the two artists using
 * a greedy breadth first search (Djikstra's Algorithm), if a path
 * exists. Excludes vertexes represented by artists in the exclusion list */
void SixDegrees::not_bfs(const Artist &source, const Artist &dest, vector<Artist> excluded){
    while(!excluded.empty()){
        if(excluded.back() == source || excluded.back() == dest){
            return;
        }
        colgraph.mark_vertex(excluded.back());
        excluded.pop_back();
    }
    bfs(source, dest);
}

/* print
 * Arguments: Two Artists, a source and a destination, along with an outputstream
 * Purpose: Uses the find_path function in CollabGraph to print an identified path from
 * one Artist to another. */
void SixDegrees::print(const Artist &source, const Artist &dest, ostream &output){
    stack<Artist> path = colgraph.report_path(source, dest);
    if((source == dest) || path.empty()){
        output << "A path does not exist between " << '"' << source.get_name() << '"'
        << " and " << '"' << dest.get_name() << '"' << "." << endl;
    } else {
        int cnt = (path.size()) - 1;
        for(int i = 0; i < cnt; i++){
            Artist top = path.top();
            path.pop();
            output << '"' << top.get_name() << '"' << " collaborated with " << '"' << path.top().get_name() << '"'
            << " in " << '"' << colgraph.get_edge(top, path.top()) << '"' << "." << endl;
        }
        output << "***" << endl;
    }
}

/* enforceArtists
 * Arguments: Two Artists, a source and a destination, along with an outputstream
 * Returns: Boolean value indicating whether or not both Artists are in the graph
 * Purpose: Ensures that Artists requested by the user exist in the graph. Prints
 * an error statement if they do not.  */
bool SixDegrees::enforceArtists(const Artist &source, const Artist &dest, ostream &output){
    if(!colgraph.is_vertex(source)){
        output << '"' << source.get_name() << '"' << " was not found in the dataset :(" << endl;
        return false;
    } 
    if(!colgraph.is_vertex(dest)){
        output << '"' << dest.get_name() << '"' << " was not found in the dataset :(" << endl;
        return false;
    }
    return true;
}

/* enforceArtists (For not Command)
 * Arguments: Two Artists, a source and a destination, along with an outputstream and a vector
 * of Artists to be excluded in the search.
 * Returns: Boolean value indicating whether or not both Artists are in the graph along with
 * all artists on the exclusion list.
 * Purpose: Ensures that Artists requested by the user exist in the graph. Prints
 * an error statement if they do not.  */
bool SixDegrees::enforceArtists(const Artist &source, const Artist &dest, vector<Artist> excluded, ostream &output){
    if(!colgraph.is_vertex(source)){
        output << '"' << source.get_name() << '"' << " was not found in the dataset :(" << endl;
        return false; 
    }
    if(!colgraph.is_vertex(dest)){
        output << '"' << dest.get_name() << '"' << " was not found in the dataset :(" << endl;
        return false;
    }
    int cnt = excluded.size();
    for (int i = 0; i < cnt; i++){
        if(!colgraph.is_vertex(excluded[i])){
            output << '"' << excluded[i].get_name() << '"' << " was not found in the dataset :(" << endl;
            return false;
        }
    }
    return true;
}


