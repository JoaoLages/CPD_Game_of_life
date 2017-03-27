#include <iostream>
#include <vector>  // vectors aka lists
#include <string>
#include <sstream> // stringstream
#include <fstream>  //files
#include <set> // set
#include <unordered_set> // unordered_set
#include <omp.h>

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

  float start = omp_get_wtime();

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
  int aux_live_cells, aux, iter_int;
  Cell aux_cell;

  for(int p=0; p<number_gen; ++p){
    #pragma omp parallel for private(aux_cell, iter_int, aux_live_cells)
    for(iter_int = 0; iter_int < Cube.size(); iter_int++) {
        auto iter = Cube.begin();
        advance(iter, iter_int);

        aux_cell = *iter;

        int cord_vector[3];

        for(int i=0;i<3;i++)
          cord_vector[i] = aux_cell.coords[i];

        aux_live_cells = checkLiveCells(aux_cell, Cube);

        #pragma omp critical (a)
      {  if(aux_live_cells>=2 and aux_live_cells<=4) // cell lives
          newCube.insert(aux_cell);
}
        // check Neighbours

        for(int j = 0;j < 3;j++){
          aux = cord_vector[j]-1;
          if(aux==-1) aux=cube_size-1;
          aux_cell.coords[j] = aux;
          #pragma omp critical (a)
          {if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
            aux_live_cells = checkLiveCells(aux_cell, Cube);
            if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
              newCube.insert(aux_cell);
          }}

          aux = cord_vector[j]+1;
          if(aux==cube_size) aux=0;
          aux_cell.coords[j] = aux;
          #pragma omp critical (a)
          {if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
            aux_live_cells = checkLiveCells(aux_cell, Cube);
            if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
              newCube.insert(aux_cell);
          }}

          aux_cell.coords[j] = cord_vector[j];
        }
    }
    Cube = newCube;
    newCube.clear();
  }

  printCube(Cube);

  float end = omp_get_wtime();

  cout << end-start << endl;
  return 0;
}
