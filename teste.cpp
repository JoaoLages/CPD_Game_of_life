#include <iostream>
#include <vector>  // vectors aka lists
#include <string>
#include <sstream> // stringstream
#include <fstream>  //files
#include <set> // set

using namespace std; // no need to type 'std::'

int checkLiveCells(string aux_cell, const set<string> &Cube){
  int liveCells = 0;
  int x_aux, y_aux, z_aux;
  int x,y,z;

  x = aux_cell[0]-'0';
  y = aux_cell[1]-'0';
  z = aux_cell[2]-'0';

  // (x-1, y, z)
  x_aux = x-1;
  aux_cell[0] = '0' + x_aux;
  if(Cube.count(aux_cell)==1){ // live cell in this position
    liveCells++;
  }

  // (x+1, y, z)
  x_aux = x+1;
  aux_cell[0] = '0' + x_aux;
  if(Cube.count(aux_cell)==1){ // live cell in this position
    liveCells++;
  }

  aux_cell[0] = '0' + x; //reset

  // (x, y-1, z)
  y_aux = y-1;
  aux_cell[1] = '0' + y_aux;
  if(Cube.count(aux_cell)==1){ // live cell in this position
    liveCells++;
  }

  // (x, y+1, z)
  y_aux = y+1;
  aux_cell[1] = '0' + y_aux;
  if(Cube.count(aux_cell)==1){ // live cell in this position
    liveCells++;
  }

  aux_cell[1] = '0' + y; // reset

  // (x, y, z-1)
  z_aux = z-1;
  aux_cell[2] = '0' + z_aux;
  if(Cube.count(aux_cell)==1){ // live cell in this position
    liveCells++;
  }

  // (x, y, z+1)
  z_aux = z+1;
  aux_cell[2] = '0' + z_aux;
  if(Cube.count(aux_cell)==1){ // live cell in this position
    liveCells++;
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

int main(int argc, char *argv[]){

  if( argc != 3 ){ // 2 arguments needed
    cout<<"usage: "<< argv[0] << " <filename> <number_gen>" << endl;
    return -1;
  }

  // Read from file
  ifstream infile;
  infile.open(argv[1]);
  string line;

  int number_gen;
  stringstream ss;
  ss << argv[2];
  ss >> number_gen;
  set<string> Cube;
  bool firstline = true;

  while (getline(infile, line))
  {
    if(firstline){
      firstline = false;
    }else{
      vector<string> tokens_str;
      tokens_str = split(line, ' '); // get x,y,z
      std::string result;
      for (auto const& s : tokens_str) result += s;
      Cube.insert(result);
    }
  }
  infile.close();

  set<string>::iterator iter;
  set<string> newCube;
  int aux_live_cells, x, x2, x_aux, y, y2, y_aux, z, z2, z_aux, aux;
  string aux_cell;

  for(int i=0; i<number_gen; ++i){
    for(iter=Cube.begin(); iter!=Cube.end();++iter){

        aux_cell = *iter;
        x = aux_cell[0]-'0';
        y = aux_cell[1]-'0';
        z = aux_cell[2]-'0';

        aux_live_cells = checkLiveCells(aux_cell, Cube);

        if(aux_live_cells>=2 and aux_live_cells<=4) // cell lives
          newCube.insert(aux_cell);

        // check Dead Neighbours

        // (x-1, y, z)
        x_aux = x-1;
        aux_cell[0] = '0' + x_aux;
        if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
          aux_live_cells = checkLiveCells(aux_cell, Cube);
          if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
            newCube.insert(aux_cell);
        }

        // (x+1, y, z)
        x_aux = x+1;
        aux_cell[0] = '0' + x_aux;
        if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
          aux_live_cells = checkLiveCells(aux_cell, Cube);
          if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
            newCube.insert(aux_cell);
        }

        aux_cell[0] = '0' + x; //reset

        // (x, y-1, z)
        y_aux = y-1;
        aux_cell[1] = '0' + y_aux;
        if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
          aux_live_cells = checkLiveCells(aux_cell, Cube);
          if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
            newCube.insert(aux_cell);
        }

        // (x, y+1, z)
        y_aux = y+1;
        aux_cell[1] = '0' + y_aux;
        if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
          aux_live_cells = checkLiveCells(aux_cell, Cube);
          if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
            newCube.insert(aux_cell);
        }

        aux_cell[1] = '0' + y; // reset

        // (x, y, z-1)
        z_aux = z-1;
        aux_cell[2] = '0' + z_aux;
        if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
          aux_live_cells = checkLiveCells(aux_cell, Cube);
          if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
            newCube.insert(aux_cell);
        }

        // (x, y, z+1)
        z_aux = z+1;
        aux_cell[2] = '0' + z_aux;
        if(Cube.count(aux_cell)==0 and newCube.count(aux_cell)==0){ // death cell in this position and not inserted
          aux_live_cells = checkLiveCells(aux_cell, Cube);
          if(aux_live_cells>=2 and aux_live_cells<=3) // cell lives
            newCube.insert(aux_cell);
        }
    }
    Cube = newCube;
    newCube.clear();
  }

  for(iter=Cube.begin(); iter!=Cube.end();++iter){
    aux_cell = *iter;
    cout << aux_cell[0] << " " << aux_cell[1] << " " << aux_cell[2] <<endl;
  }

  return 0;
}
