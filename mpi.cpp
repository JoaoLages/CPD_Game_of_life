#include <iostream>
#include <vector>  // vectors aka lists
#include <string>
#include <sstream> // stringstream
#include <fstream>  //files
#include <set> // set
#include <unordered_set> // unordered_set
#include <algorithm>
#include <mpi.h>

#define TAG 123

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
  if( (*node) == NULL) return false;
  if( (*node)->z == value )return true;
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
// used to read cube from file
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


//reads cube from file a returns the cube data structure
vector<vector<int>> readInputFile(string filename){

  ifstream infile;
  infile.open(filename);
  string line;
  vector<vector<int>> Cube;  // lists of y,z (ex: Cube[0]= [y,z,y,z,y,z])

  bool firstline = true;

  while (getline(infile, line))
  {
    if(firstline){
      stringstream stream;
      stream << line;
      stream >> cube_size;
      Cube.resize(cube_size);

      firstline = false;
    }else{
      vector<string> tokens_str;
      tokens_str = split(line, ' '); // get x,y,z
      int z = atoi(tokens_str[2].c_str());
      Cube[atoi(tokens_str[0].c_str())].push_back(atoi(tokens_str[1].c_str())); //insert y
      Cube[atoi(tokens_str[0].c_str())].push_back(atoi(tokens_str[2].c_str())); //insert z
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

void inorder(Node* p, int a, int b, vector<vector<Node*>> &Cube, vector<vector<Node*>> &newCube, vector<vector<int>> &toSendCube, bool insert)
{
  if(p->left) inorder(p->left, a, b, Cube, newCube, toSendCube, insert);
  // Do something with node p
  int z = p->z;

  int aux_live_cells = checkLiveCells(a, b, z, Cube);
  if(aux_live_cells>=2 and aux_live_cells<=4){ // cell lives
    Insert(&newCube[a][b], z);
    if(insert){
      if(toSendCube.size()>2){
        toSendCube[a].push_back(b); // insert y
        toSendCube[a].push_back(z); // insert z
      }else{
        if(a==1){
          toSendCube[0].push_back(b); // insert y
          toSendCube[0].push_back(z); // insert z
        }else{
          toSendCube[1].push_back(b); // insert y
          toSendCube[1].push_back(z); // insert z
        }
      }
    }
  }
  // Check Neighbours
  int cord_vector[3], aux_cord_vector[3], aux;

  aux_cord_vector[0] = cord_vector[0] = a;
  aux_cord_vector[1] = cord_vector[1] = b;
  aux_cord_vector[2] = cord_vector[2] = z;

  for(int i=0;i<3;i++){

    aux = cord_vector[i]-1;
    if(!(i==0 and aux==0)){ //Dont compute firstline in x
      if(aux==-1) aux=cube_size-1;
      aux_cord_vector[i] = aux;
      if(!Find(&Cube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2])){ // death cell in this position
        aux_live_cells = checkLiveCells(aux_cord_vector[0], aux_cord_vector[1], aux_cord_vector[2], Cube);
        if(aux_live_cells>=2 and aux_live_cells<=3){// cell lives
          Insert(&newCube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2]);
          if(insert){
            if(toSendCube.size()>2){ // only insert in the last iteration
              toSendCube[a].push_back(b); // insert y
              toSendCube[a].push_back(z); // insert z
            }
          }
        }
      }
    }

    aux = cord_vector[i]+1;
    if(!(i==0 and aux==cube_size-1)){ //Dont compute lastline in x
      if(aux==cube_size) aux=0;
      aux_cord_vector[i] = aux;
      if(!Find(&Cube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2])){ // death cell in this position
        aux_live_cells = checkLiveCells(aux_cord_vector[0], aux_cord_vector[1], aux_cord_vector[2], Cube);
        if(aux_live_cells>=2 and aux_live_cells<=3){// cell lives
          Insert(&newCube[aux_cord_vector[0]][aux_cord_vector[1]], aux_cord_vector[2]);
          if(insert){
            if(toSendCube.size()>2){ // only insert in the last iteration
              toSendCube[a].push_back(b); // insert y
              toSendCube[a].push_back(z); // insert z
            }
          }
        }
      }
    }

    aux_cord_vector[i] = cord_vector[i];

  }

  if(p->right) inorder(p->right, a, b, Cube, newCube, toSendCube, insert);
  else return;
}

/*My_x is the value of x that the process starts with*/
// Transform Cube vector<vector<int>> to vector<vector<Node*>>
// to be sent to others nodes???
vector<vector<Node*>> cube_dismember(vector<vector<int>> Cube){

  vector<vector<Node*>> Cubinho;

  Cubinho.resize(cube_size);

  for(int k=0;k<cube_size;++k)
  Cubinho[k].resize(cube_size);

  for(int i=0;i<Cube.size();++i){
    for(int j=0;j<Cube[i].size();++j){

      if(j%2==0){
        Insert(&Cubinho[i][Cube[i][j]], Cube[i][j+1]);
      }
    }
  }
  return Cubinho;
}

vector<vector<Node*>> joinCubes(vector<vector<Node*>> CubeA, vector<vector<int>> CubeB){
  int pos; //position to insert in CubeA
  for(int i=0;i<2;++i){
    if(i==0) pos = 0;
    else pos = CubeA.size()-1;

    for(int j=0;j<CubeB[i].size();++j){

      if(j%2==0){
        Insert(&CubeA[pos][CubeB[i][j]], CubeB[i][j+1]);
      }
    }
  }
  return CubeA;
}

int main(int argc, char *argv[]){

  MPI_Status status;
  //float start = omp_get_wtime();

  if( argc != 3 ){ // 2 arguments needed
    cout<<"usage: "<< argv[0] << " <filename> <number_gen>" << endl;
    return -1;
  }
  //MPI vars
  vector<vector<int>> Cube;
  int me, nprocs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);

  // Read from file
  int number_gen;
  stringstream ss;
  ss << argv[2];
  ss >> number_gen;

  //Master node
  if(me == 0){

    Cube = readInputFile(argv[1]);

    vector<int> vector_int; //n_linhas to send to slave
    vector_int.resize(nprocs);

    for(int i = 0; i <nprocs;i++)
    vector_int[i] = cube_size/nprocs;

    if(cube_size%nprocs != 0){
      for(int i=0; i< cube_size%nprocs; i++)
      vector_int[i]++;
    }
    int aux_n_linha = 0; //linha a mandar
    // sends to all slaves ????

    for(int c=0; c<vector_int.size(); c++){
      vector<int> infos;
      infos.resize(3);

      infos[0]=cube_size;
      infos[1]=vector_int[c]; //number of lines to send to slave process
      infos[2]=aux_n_linha;

      MPI_Send(&infos.front(), infos.size(), MPI_INT, c, TAG, MPI_COMM_WORLD); // send infos to slave
      // Send firstline of line group
      if(c == 0){
        int aux = Cube.back().size();
        MPI_Send(&aux, 1, MPI_INT, c, TAG, MPI_COMM_WORLD); // send dimension of line
        MPI_Send(&Cube.back().front(), aux, MPI_INT, c, TAG, MPI_COMM_WORLD); // send line
      }else{
        int aux = Cube[aux_n_linha].size();
        MPI_Send(&aux, 1, MPI_INT, c, TAG, MPI_COMM_WORLD); // send dimension of line
        MPI_Send(&Cube[aux_n_linha].front(), aux, MPI_INT, c, TAG, MPI_COMM_WORLD); // send line
      }
      for(int q=0; q<vector_int[c]; q++){ //send q lines to slave
        int aux = Cube[aux_n_linha].size();
        MPI_Send(&aux, 1, MPI_INT, c, TAG, MPI_COMM_WORLD); // send dimension of line
        MPI_Send(&Cube[aux_n_linha].front(), aux, MPI_INT, c, TAG, MPI_COMM_WORLD); // send line
        aux_n_linha++;
      }
      // Send last line of line group
      if(c == (vector_int.size()-1)){
        int aux = Cube[0].size();
        MPI_Send(&aux, 1, MPI_INT, c, TAG, MPI_COMM_WORLD); // send dimension of line
        MPI_Send(&Cube[0].front(), aux, MPI_INT, c, TAG, MPI_COMM_WORLD); // send line
      }else{
        int aux = Cube[aux_n_linha].size();
        MPI_Send(&aux, 1, MPI_INT, c, TAG, MPI_COMM_WORLD); // send dimension of line
        MPI_Send(&Cube[aux_n_linha].front(), aux, MPI_INT, c, TAG, MPI_COMM_WORLD); // send line
      }

    }
  }else{ // Slave code
    vector<int> infos;
    infos.resize(3);
    MPI_Recv(&infos.front(), infos.size(), MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);

    Cube.resize(infos[1]+2); //infos vai ter cube_size, n_lines e my_x por esta ordem

    cube_size=infos[0];

    int aux_ldim; // dimension of the line
    for(int i=0; i<(infos[1]+2); i++){
      MPI_Recv(&aux_ldim, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
      Cube[i].resize(aux_ldim); // redimension line
      MPI_Recv(&Cube[i].front(), Cube[i].size(), MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
    }

    vector<vector<Node*>> Cubinho = cube_dismember(Cube);


    for(int p=0;p<number_gen; ++p){
      // Reset toSendCube
      vector<vector<int>> toSendCube;
      vector<vector<int>> receivedCube;
      if(p==(number_gen-1)) // last iteration => send all Cube
      {toSendCube.resize(cube_size);
        receivedCube.resize(cube_size);}
        else //else, send first and lastline only
        {toSendCube.resize(2);
          receivedCube.resize(2);}

          // Reset newCube ???? MANEL pq se faz isto todas as gerações?
          // pq nao penas 1 vez de depois so se fazer resize
          vector<vector<Node*>> newCube;
          newCube.resize(cube_size);
          for (auto &a: newCube) a.resize(cube_size);

          // Compute secondline in x
          for(int b=0; b<cube_size; ++b){
            Node* z_tree = (Cubinho[1][b]);
            if (z_tree != NULL) inorder(z_tree, 1, b, Cubinho, newCube, toSendCube, true); //Iterate in binary tree
          }
          // Compute second lastline in x
          for(int b=0; b<cube_size; ++b){
            Node* z_tree = (Cubinho[(cube_size-2)][b]);
            if (z_tree != NULL) inorder(z_tree, cube_size-2, b, Cubinho, newCube, toSendCube, true); //Iterate in binary tree
          }

          // TODO: A mudar para Irecv e Isend
          // Send first and lastline via MPI, if not last iteration
          // Receive second and second last via MPI, if not last iteration
          if(p!=(number_gen-1)){
            int aux_ldim; // dimension of the line
            int slave_n; // number of slave to send
            for(int c=0; c<2; ++c){
              if(c==0){ // send to previous
                slave_n = me - 1;
                if(slave_n == 0) slave_n = nprocs-1;
              }else{ // send to next
                slave_n = me + 1;
                if(slave_n == nprocs) slave_n = 1;
              }

              // correr para cada cubo


              MPI_Request requests[4];
              MPI_Status statuses[2];

              int aux = toSendCube[c].size();

              MPI_Irecv(&aux_ldim, 1, MPI_INT, slave_n, TAG, MPI_COMM_WORLD, &requests[0]); // receive dimension
              MPI_Irecv(&receivedCube[c].front(), aux_ldim, MPI_INT, slave_n, TAG, MPI_COMM_WORLD, &requests[1]); // receive line

              MPI_Isend(&aux, 1, MPI_INT, slave_n, TAG, MPI_COMM_WORLD, &requests[2]); // send dimension of line
              cout <<"first arg: "<< aux<<", Process: "<< me <<endl;

              MPI_Isend(&toSendCube[c].front(), aux, MPI_INT, slave_n, TAG, MPI_COMM_WORLD, &requests[3]); // send line
              cout << "second arg: "<<p<<", Process: "<< me <<endl;

              MPI_Waitall(4,requests, statuses);
              cout << "hello" << endl;
            }
          }

          // Correr Cubinho entre 2 e size-2 -> nao mexe nas  2 primeiras e  2 ultimas linhas
          bool insert = false;
          if(p==(number_gen-1)) insert = true;
          for(int a=2; a<(cube_size-2); ++a){
            for(int b=0; b<cube_size; ++b){
              Node* z_tree = (Cubinho[a][b]);
              if (z_tree != NULL) inorder(z_tree, a, b, Cubinho, newCube, toSendCube, insert); //Iterate in binary tree
            }
          }
          newCube = joinCubes(newCube, receivedCube);
          Cubinho = newCube;
        }
      }
      //float end = omp_get_wtime();
      //cout << end-start << endl;

      MPI_Finalize();
      return 0;
    }
