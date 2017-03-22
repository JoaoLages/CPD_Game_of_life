#include <iostream>
#include <vector>  // vectors aka lists
#include <string>
#include <sstream> // stringstream
#include <fstream>  //files
#include <set> // set

using namespace std; // no need to type 'std::'
int cube_size;

struct Cell{
  int coords[3];
};

// for ordering set
inline bool operator<(const Cell& lhs, const Cell& rhs)
{
  if(lhs.coords[0] != rhs.coords[0]) return lhs.coords[0] < rhs.coords[0];
  if(lhs.coords[1] != rhs.coords[1]) return lhs.coords[1] < rhs.coords[1];
  return lhs.coords[2] < rhs.coords[2];
}

// Count live cells around a cell
int checkLiveCells(Cell aux_cell, const set<Cell> &Cube){
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


void readInputFile(string filename, set<Cell> &Cube){

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
void printCube(const set<Cell> &Cube){
  set<Cell>::iterator iter;
  Cell aux_cell;

  for(iter=Cube.begin(); iter!=Cube.end();++iter){
    aux_cell = *iter;
    cout << aux_cell.coords[0] << " " << aux_cell.coords[1] << " " << aux_cell.coords[2] <<endl;
  }
  return;
}

int main(int argc, char *argv[]){

  if( argc != 3 ){ // 2 arguments needed
    cout<<"usage: "<< argv[0] << " <filename> <number_gen>" << endl;
    return -1;
  }

  // Read from file
  set<Cell> Cube;
  int number_gen;
  stringstream ss;
  ss << argv[2];
  ss >> number_gen;

  readInputFile(argv[1], Cube);

  set<Cell>::iterator iter;
  set<Cell> newCube;
  int aux_live_cells, aux;
  Cell aux_cell;

  for(int i=0; i<number_gen; ++i){
    for(iter=Cube.begin(); iter!=Cube.end();++iter){

        aux_cell = *iter;

        int cord_vector[3];

        for(int i=0;i<3;i++)
          cord_vector[i] = aux_cell.coords[i];

        aux_live_cells = checkLiveCells(aux_cell, Cube);

        if(aux_live_cells>=2 and aux_live_cells<=4) // cell lives
          newCube.insert(aux_cell);

        // check Neighbours

        for(int j = 0;j < 3;j++){
          aux = cord_vector[j]-1;
          if(aux==-1) aux=cube_size-1;
          aux_cell.coords[j] = aux;
          if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
            aux_live_cells = checkLiveCells(aux_cell, Cube);
            if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
              newCube.insert(aux_cell);
          }

          aux = cord_vector[j]+1;
          if(aux==cube_size) aux=0;
          aux_cell.coords[j] = aux;
          if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
            aux_live_cells = checkLiveCells(aux_cell, Cube);
            if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
              newCube.insert(aux_cell);
          }

          aux_cell.coords[j] = cord_vector[j];
        }
    }
    Cube = newCube;
    newCube.clear();
  }

  printCube(Cube);

  return 0;
}
