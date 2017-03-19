#include <iostream>
#include <vector>  // vectors aka lists
#include <string>
#include <sstream> // stringstream
#include <fstream>  //files

using namespace std; // no need to type 'std::'

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

// Structure for data in Zs vectors
struct data_z_axis{
  int z;
  bool alive;
};

// Structure for data in Ys vectors
struct data_y_axis{
  int y;
  vector<data_z_axis> *ptr_y;
};


int main(int argc, char *argv[]){

  if( argc != 3 ){ // 2 arguments needed
    cout<<"usage: "<< argv[0] << " <filename> <number_gen>" << endl;
    return -1;
  }

  // Read from file
  ifstream infile;
  infile.open(argv[1]);
  string line;
  bool firstline = true;

  int number_gen;
  vector<vector<data_y_axis>*> Cube_x;
  data_y_axis data_y;
  data_z_axis data_z;
  vector<string> tokens_str;

  while (getline(infile, line))
  {
    if(firstline){ // get firtline of the file
      stringstream aux_ss;
      aux_ss << line;
      aux_ss >> number_gen;
      Cube_x.reserve(number_gen); // create x axis
      for(int i=0; i<Cube_x.size(); ++i)  // Initialize with zeros TODO: Acho que os vectors vêm com random data lá dentro em vez de NULLs. Se já vier mesmo com 0s ou assim, remover isto
        Cube_x[i] = NULL;
      firstline = false;
    }

    tokens_str = split(line, ' '); // get x,y,z
    vector<int> tokens;
    for(int i=0; i<tokens_str.size(); ++i){
      stringstream aux_ss;
      int aux_int;
      aux_ss << tokens_str[i];
      aux_ss >> aux_int;
      tokens.push_back(aux_int);
    }

    if(Cube_x[tokens[0]] == NULL){ // no y axis in this x
      vector<data_y_axis> Cube_y;
      Cube_x[tokens[0]] = &Cube_y; // save pointer to y axis in x vector

      vector<data_z_axis> Cube_z;
      data_y = {tokens[1], &Cube_z};
      Cube_y.insert(Cube_y.begin(), data_y);

      data_z = {tokens[2], true};
      Cube_z.insert(Cube_z.begin(), data_z);

    } else { // there is already a pointer to a vector of y axis in this x
      vector<data_y_axis> Cube_y = *Cube_x[tokens[0]];

      bool aux_inserted = false;
      int i;
      for(i=0; i<Cube_y.size(); ++i){
        if(Cube_y[i].y > tokens[1]) break; // this specific y does not exist
        if(Cube_y[i].y == tokens[1]){ // y already exists
          vector<data_z_axis> Cube_z = *Cube_y[i].ptr_y;

          int j;
          for(j=0; j<Cube_z.size(); ++j){
            if(Cube_z[j].z > tokens[2]) break;
          }
          data_z = {tokens[2], true};
          Cube_z.insert(Cube_z.begin()+j, data_z);

          aux_inserted = true;
          break;
        }
      }
      if(!aux_inserted){
        vector<data_z_axis> Cube_z;
        data_y = {tokens[1], &Cube_z};
        Cube_y.insert(Cube_y.begin()+i, data_y);

        data_z = {tokens[2], true};
        Cube_z.insert(Cube_z.begin(), data_z);
      }
    }
  } 

  for(int i=0; i<Cube_x.size(); ++i){
    if(Cube_x[i]!=NULL){
        vector<data_y_axis> Cube_y = *Cube_x[i];
        for(int j=0; j<Cube_y.size(); ++j){
          vector<data_z_axis> Cube_z = *Cube_y[j].ptr_y;
          for(int g=0; g<Cube_z.size(); ++g){
            cout << i << " " << Cube_y[j].y << " " << Cube_z[g].z << endl;
          }
        }
    }
  }
  infile.close();

  return 0;
}
