#include <iostream>
#include <vector>  // vectors aka lists
#include <string>
#include <sstream> // stringstream
#include <fstream>  //files
#include <set> // set
#include <unordered_set> // unordered_set
#include <time.h>

using namespace std; // no need to type 'std::'


struct Cell{
  int coords[3];
};

namespace std
{
    template <> struct hash<Cell>
    {
        size_t operator()(Cell const & x) const noexcept
        {
            return (
                ((199 + std::hash<int>()(x.coords[0])) * 199
                + std::hash<int>()(x.coords[1]))* 199
                + std::hash<int>()(x.coords[2])
            );
        }
    };
}
int cube_size;

// for ordering set
inline bool operator<(const Cell& lhs, const Cell& rhs)
{
  if(lhs.coords[0] != rhs.coords[0]) return lhs.coords[0] < rhs.coords[0];
  if(lhs.coords[1] != rhs.coords[1]) return lhs.coords[1] < rhs.coords[1];
  return lhs.coords[2] < rhs.coords[2];
}

inline bool operator==(const Cell& lhs, const Cell& rhs)
{
  if(lhs.coords[0] == rhs.coords[0] and lhs.coords[1] == rhs.coords[1] and lhs.coords[2] == rhs.coords[2]) return true;
  return false;
}

// Count live cells around a cell
int checkLiveCells(Cell aux_cell, const unordered_set<Cell> &Cube){
  int liveCells = 0;
  int i, aux;
  int cord_vector[3];

  for(int i=0;i<3;i++)
    cord_vector[i] = aux_cell.coords[i];

  for(i=0;i<3;i++){

    aux = cord_vector[i]-1;
    if(aux==-1) aux=cube_size-1;
    aux_cell.coords[i] = aux;
    if(Cube.count(aux_cell)==1) // live cell in this position
      liveCells++;

    aux = cord_vector[i]+1;
    if(aux==cube_size) aux=0;
    aux_cell.coords[i] = aux;
    if(Cube.count(aux_cell)==1) // live cell in this position
      liveCells++;

    aux_cell.coords[i] = cord_vector[i];

  }

  return liveCells;
}

// Split a string depending on the separation (sep) character
vector<string> split(const string &text, char sep) {
  vector<string> tokens;
  size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}


void readInputFile(string filename, unordered_set<Cell> &Cube){

  ifstream infile;
  infile.open(filename);
  string line;

  bool firstline = true;

  while (getline(infile, line))
  {
    if(firstline){
      stringstream stream;
    	stream << line;
  		stream >> cube_size;
      firstline = false;
    }else{
      vector<string> tokens_str;
      Cell cell;
      tokens_str = split(line, ' '); // get x,y,z
      for(int i=0; i<tokens_str.size(); ++i)
        cell.coords[i] = atoi(tokens_str[i].c_str());
      Cube.insert(cell);
    }
  }
  infile.close();
}


// Program Output
void printCube(const unordered_set<Cell> &Cube){
  Cell aux_cell;
  set<Cell> orderedCube(Cube.begin(), Cube.end());

  for(auto iter=orderedCube.begin(); iter!=orderedCube.end();++iter){
    aux_cell = *iter;
    cout << aux_cell.coords[0] << " " << aux_cell.coords[1] << " " << aux_cell.coords[2] <<endl;
  }
  return;
}

int main(int argc, char *argv[]){

  clock_t start_time, end_time;

  start_time = clock();

  if( argc != 3 ){ // 2 arguments needed
    cout<<"usage: "<< argv[0] << " <filename> <number_gen>" << endl;
    return -1;
  }

  // Read from file
  unordered_set<Cell> Cube;
  int number_gen;
  stringstream ss;
  ss << argv[2];
  ss >> number_gen;

  readInputFile(argv[1], Cube);

  unordered_set<Cell> newCube;

  for(int p=0; p<number_gen; ++p){
<<<<<<< Updated upstream:task_version.cpp
    #pragma omp parallel
    {
      #pragma omp single
      {
      for(const auto& iter: Cube)
      {
        Cell aux_cell = iter;
        #pragma omp task firstprivate(aux_cell)
        {

          int aux_live_cells, aux;
          unordered_set<Cell> liveCells;
=======
    //#pragma omp parallel for private(aux_cell, iter_int, aux_live_cells)
    for( const auto& iter: Cube ) {

        aux_cell = iter;
        unordered_set<Cell> liveCells;
>>>>>>> Stashed changes:teste.cpp

          int cord_vector[3];

          for(int i=0;i<3;i++)
            cord_vector[i] = aux_cell.coords[i];

          aux_live_cells = checkLiveCells(aux_cell, Cube);

<<<<<<< Updated upstream:task_version.cpp
          if(aux_live_cells>=2 and aux_live_cells<=4) // cell lives
            liveCells.insert(aux_cell);
=======
        if(aux_live_cells>=2 and aux_live_cells<=4) // cell lives
          liveCells.insert(aux_cell);
>>>>>>> Stashed changes:teste.cpp

          // check Neighbours
          // #pragma omp parallel for TODO: não sei se rende...
          for(int j = 0;j < 3;j++){
            aux = cord_vector[j]-1;
            if(aux==-1) aux=cube_size-1;
            aux_cell.coords[j] = aux;

<<<<<<< Updated upstream:task_version.cpp
            if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
              aux_live_cells = checkLiveCells(aux_cell, Cube);
              if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
                liveCells.insert(aux_cell);
            }

            aux = cord_vector[j]+1;
            if(aux==cube_size) aux=0;
            aux_cell.coords[j] = aux;

            if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
              aux_live_cells = checkLiveCells(aux_cell, Cube);
              if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
                liveCells.insert(aux_cell);
            }

            aux_cell.coords[j] = cord_vector[j];
=======
        for(int j = 0;j < 3;j++){
          aux = cord_vector[j]-1;
          if(aux==-1) aux=cube_size-1;
          aux_cell.coords[j] = aux;
          if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
            aux_live_cells = checkLiveCells(aux_cell, Cube);
            if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
              liveCells.insert(aux_cell);
          }

          aux = cord_vector[j]+1;
          if(aux==cube_size) aux=0;
          aux_cell.coords[j] = aux;
          if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
            aux_live_cells = checkLiveCells(aux_cell, Cube);
            if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
              liveCells.insert(aux_cell);
>>>>>>> Stashed changes:teste.cpp
          }

          for(auto it=liveCells.begin(); it!= liveCells.end(); ++it){
            Cell aux = *it;
            #pragma omp critical
            newCube.insert(aux);
          }
        }
<<<<<<< Updated upstream:task_version.cpp
      }
      #pragma omp taskwait
      Cube = newCube;
      newCube.clear();
      }
=======

      //  #pragma omp critical
        {
          for(auto it=liveCells.begin(); it!= liveCells.end(); ++it){
            Cell aux = *it;
            newCube.insert(aux);}
        }
>>>>>>> Stashed changes:teste.cpp
    }
  }

  printCube(Cube);

    end_time = clock();

    cout << (float)(end_time - start_time)/ CLOCKS_PER_SEC   << endl;
  return 0;
}
