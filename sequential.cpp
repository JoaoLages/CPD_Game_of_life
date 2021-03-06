#include <iostream>
#include <vector>  // vectors aka lists
#include <string>
#include <sstream> // stringstream
#include <fstream>  //files
#include <algorithm>

using namespace std; // no need to type 'std::'

struct Node {
    int z;
    Node* left;
    Node* right;

    Node(int val) {
        this->z = val;
        this->left = NULL;
        this->right = NULL;
    }

    Node(){
      this->z = -1;
      this->left = NULL;
      this->right = NULL;
    }
};

int cube_size; // glocal variable

bool Find( Node** node, int value )
{
    if( (*node) == NULL)
        return false;
    if( (*node)->z == value )
        return true;
    if( (*node)->z > value )
        return Find( &((*node)->left), value );
    else
        return Find( &((*node)->right), value );
}

void Insert( Node** node, int value )
{
    if( *node == NULL ) {
        *node = new Node( value );
        return;
    }
    if( ((*node)->z) == value ) return; // dont allow equal values
    if( ((*node)->z) > value )
        Insert( &((*node)->left), value );
    else
        Insert( &((*node)->right), value );
}


// Count live cells around a cell
int checkLiveCells(int x, int y, int z, vector<vector<Node*>> &Cube){
  int liveCells = 0;
  int i, aux;
  int cord_vector[3], aux_cord_vector[3];

  aux_cord_vector[0] = cord_vector[0] = x;
  aux_cord_vector[1] = cord_vector[1] = y;
  aux_cord_vector[2] = cord_vector[2] = z;

  for(i=0;i<3;i++){

    aux = cord_vector[i]-1;
    if(aux==-1) aux=cube_size-1;
    aux_cord_vector[i] = aux;
    if(Find(&Cube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2])) // live cell in this position (0/-1)
      liveCells++;

    aux = cord_vector[i]+1;
    if(aux==cube_size) aux=0;
    aux_cord_vector[i] = aux;
    if(Find(&Cube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2])) // live cell in this position (0/-1)
      liveCells++;

    aux_cord_vector[i] = cord_vector[i];
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


vector<vector<Node*>> readInputFile(string filename){

  ifstream infile;
  infile.open(filename);
  string line;
  vector<vector<Node*>> Cube;

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
      int z = atoi(tokens_str[2].c_str());
      Insert(&Cube[atoi(tokens_str[0].c_str())][atoi(tokens_str[1].c_str())], z);
    }
  }
  infile.close();
  return Cube;
}

void print_in_order(Node* p, int a, int b){
  if(p != NULL){
    if(p->left) print_in_order(p->left, a, b);
    cout << a <<" "<<b<<" "<<p->z<<endl;
    if(p->right) print_in_order(p->right, a, b);
  }return;
}

// Program Output
void printCube(vector<vector<Node*>> &Cube){

  for(int a=0; a<cube_size; ++a){
    for(int b=0; b<cube_size; ++b){
      print_in_order(Cube[a][b], a, b);
    }
  }
}

void inorder(Node* p, int a, int b, vector<vector<Node*>> &Cube, vector<vector<Node*>> &newCube)
{
    if(p->left) inorder(p->left, a, b, Cube, newCube);
    // Do something with node p
    int z = p->z;

    int aux_live_cells = checkLiveCells(a, b, z, Cube);
    if(aux_live_cells>=2 and aux_live_cells<=4){ // cell lives
      Insert(&newCube[a][b], z);
    }
    // Check Neighbours
    int cord_vector[3], aux_cord_vector[3], aux;

    aux_cord_vector[0] = cord_vector[0] = a;
    aux_cord_vector[1] = cord_vector[1] = b;
    aux_cord_vector[2] = cord_vector[2] = z;
    // Check Neighbours
    for(int i=0;i<3;i++){

      aux = cord_vector[i]-1;
      if(aux==-1) aux=cube_size-1;
      aux_cord_vector[i] = aux;
      if(!Find(&Cube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2])){ // death cell in this position
        aux_live_cells = checkLiveCells(aux_cord_vector[0], aux_cord_vector[1], aux_cord_vector[2], Cube);
        if(aux_live_cells>=2 and aux_live_cells<=3){// cell lives
          Insert(&newCube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2]);
        }
      }

      aux = cord_vector[i]+1;
      if(aux==cube_size) aux=0;
      aux_cord_vector[i] = aux;
      if(!Find(&Cube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2])){ // death cell in this position
        aux_live_cells = checkLiveCells(aux_cord_vector[0], aux_cord_vector[1], aux_cord_vector[2], Cube);
        if(aux_live_cells>=2 and aux_live_cells<=3){// cell lives
          Insert(&newCube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2]);
        }
      }

      aux_cord_vector[i] = cord_vector[i];

    }

    if(p->right) inorder(p->right, a, b, Cube, newCube);
    else return;
}
int main(int argc, char *argv[]){

  if( argc != 3 ){ // 2 arguments needed
    cout<<"usage: "<< argv[0] << " <filename> <number_gen>" << endl;
    return -1;
  }

  // Read from file
  int number_gen;
  stringstream ss;
  ss << argv[2];
  ss >> number_gen;

  vector<vector<Node*>> Cube = readInputFile(argv[1]);


  for(int p=0; p<number_gen; ++p){
    // Reset newCube
    vector<vector<Node*>> newCube;
    newCube.resize(cube_size);
    for (auto &a: newCube) a.resize(cube_size);

    for(int a=0; a<cube_size; ++a){
      for(int b=0; b<cube_size; ++b){

        Node* z_tree = (Cube[a][b]);
        if (z_tree != NULL) inorder(z_tree, a, b, Cube, newCube); //Iterate in binary tree

      }
    }
    Cube = newCube;
  }
  printCube(Cube);

  return 0;
}
