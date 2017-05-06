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
  int z;
  //bool alive;
};

namespace std
{
    template <> struct hash<Cell>
    {
        size_t operator()(Cell const & x) const noexcept
        {
            return (
                ((199 + std::hash<int>()(x.z)) * 199)
            );
        }
    };
}
int cube_size;

// for ordering set
inline bool operator<(const Cell& lhs, const Cell& rhs)
{
  return lhs.z < rhs.z;
}

inline bool operator==(const Cell& lhs, const Cell& rhs)
{
  if(lhs.z == rhs.z) return true;
  return false;
}

// Count live cells around a cell
int checkLiveCells(int x, int y, Cell aux_cell, vector<vector<set<Cell>>> Cube){
  int liveCells = 0;
  int i, aux;
  int cord_vector[3], aux_cord_vector[3];

  aux_cord_vector[0] = cord_vector[0] = x;
  aux_cord_vector[1] = cord_vector[1] = y;
  aux_cord_vector[2] = cord_vector[2] = aux_cell.z;

  for(i=0;i<3;i++){

    aux = cord_vector[i]-1;
    if(aux==-1) aux=cube_size-1;
    if(i==2) aux_cell.z = aux;
    else aux_cord_vector[i] = aux;
    if(Cube[aux_cord_vector[0]][aux_cord_vector[1]].count(aux_cell)==1) // live cell in this position
      liveCells++;

    aux = cord_vector[i]+1;
    if(aux==cube_size) aux=0;
    if(i==2) aux_cell.z = aux;
    else aux_cord_vector[i] = aux;
    if(Cube[aux_cord_vector[0]][aux_cord_vector[1]].count(aux_cell)==1) // live cell in this position
      liveCells++;

    if(i==2) aux_cell.z = cord_vector[i];

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


vector<vector<set<Cell>>>  readInputFile(string filename){

  ifstream infile;
  infile.open(filename);
  string line;
  vector<vector<set<Cell>>> Cube;

  bool firstline = true;

  while (getline(infile, line))
  {
    if(firstline){
      stringstream stream;
    	stream << line;
  		stream >> cube_size;
      Cube.resize(cube_size);
      for (auto &a: Cube) a.resize(cube_size);
      firstline = false;
    }else{
      vector<string> tokens_str;
      tokens_str = split(line, ' '); // get x,y,z
      Cell cell;
      cell.z = atoi(tokens_str[2].c_str());
      Cube[atoi(tokens_str[0].c_str())][atoi(tokens_str[1].c_str())].insert(cell);
    }
  }
  infile.close();
  return Cube;
}


// Program Output
void printCube(const vector<vector<set<Cell>>> Cube){

  for(int a=0; a<cube_size; ++a){
    for(int b=0; b<cube_size; ++b){
      set<Cell> set = Cube[a][b];

      for(auto it=set.begin(); it!=set.end(); ++it){
        Cell aux_cell = *it;
        cout << a << " " << b << " " << aux_cell.z <<endl;
      }
    }
  }
}

int main(int argc, char *argv[]){

  float start = omp_get_wtime();

  if( argc != 3 ){ // 2 arguments needed
    cout<<"usage: "<< argv[0] << " <filename> <number_gen>" << endl;
    return -1;
  }

  // Read from file
  int number_gen;
  stringstream ss;
  ss << argv[2];
  ss >> number_gen;

  vector<vector<set<Cell>>> Cube = readInputFile(argv[1]);
  vector<vector<set<Cell>>> newCube;
  newCube.resize(cube_size);
  for (auto &a: newCube) a.resize(cube_size);

  for(int p=0; p<number_gen; ++p){
    //#pragma omp parallel for
      for(int a=0; a<cube_size; ++a)
      {
        for(int b=0; b<cube_size; ++b){

        set<Cell> z_set = Cube[a][b];

        for(auto it=z_set.begin(); it!=z_set.end(); ++it){ //
          Cell aux_cell = *it;

          int aux_live_cells = checkLiveCells(a, b, aux_cell, Cube);
          if(!(aux_live_cells>=2 and aux_live_cells<=4)) // cell lives
            newCube[a][b].insert(aux_cell);

          int cord_vector[3], aux_cord_vector[3], aux;

          aux_cord_vector[0] = cord_vector[0] = a;
          aux_cord_vector[1] = cord_vector[1] = b;
          aux_cord_vector[2] = cord_vector[2] = aux_cell.z;
          // Check Neighbours
          for(int i=0;i<3;i++){

            aux = cord_vector[i]-1;
            if(aux==-1) aux=cube_size-1;
            if(i==2) aux_cell.z = aux;
            else aux_cord_vector[i] = aux;
            if(Cube[aux_cord_vector[0]][aux_cord_vector[1]].count(aux_cell)==0){ // death cell in this position
              aux_live_cells = checkLiveCells(a, b, aux_cell, Cube);
              if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
                newCube[aux_cord_vector[0]][aux_cord_vector[1]].insert(aux_cell);
            }

            aux = cord_vector[i]+1;
            if(aux==cube_size) aux=0;
            if(i==2) aux_cell.z = aux;
            else aux_cord_vector[i] = aux;
            if(Cube[aux_cord_vector[0]][aux_cord_vector[1]].count(aux_cell)==0){ // death cell in this position
              aux_live_cells = checkLiveCells(a, b, aux_cell, Cube);
              if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
                newCube[aux_cord_vector[0]][aux_cord_vector[1]].insert(aux_cell);
            }

            if(i==2) aux_cell.z = cord_vector[i];

          }
        }


        /*
        Cell aux_cell = Cube_vector[a];

          int aux_live_cells, aux;
          unordered_set<Cell> liveCells;

          int cord_vector[3];

          for(int i=0;i<3;i++)
            cord_vector[i] = aux_cell.coords[i];

          aux_live_cells = checkLiveCells(aux_cell, Cube);

          if(aux_live_cells>=2 and aux_live_cells<=4) // cell lives
            liveCells.insert(aux_cell);

          // check Neighbours
          // #pragma omp parallel for TODO: não sei se rende...
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
            }


            aux_cell.coords[j] = cord_vector[j];
          }

          for(auto it=liveCells.begin(); it!= liveCells.end(); ++it){
            Cell aux = *it;
            #pragma omp critical (set)
              newCube.insert(aux);
            #pragma omp critical (vec)
              newCube_vector.push_back(aux);

          }*/

      }}
      Cube = newCube;
      newCube.clear();
  }

  printCube(Cube);

  float end = omp_get_wtime();

  cout << end-start << endl;
  return 0;
}
