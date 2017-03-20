#include <iostream>
#include <vector>  // vectors aka lists
#include <string>
#include <sstream> // stringstream
#include <fstream>  //files
#include <set> // set

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
  set<string> Cube;

  while (getline(infile, line))
  {
    if(firstline){ // get firtline of the file
      stringstream aux_ss;
      aux_ss << line;
      aux_ss >> number_gen;
      firstline = false;
    }
    else{
      vector<string> tokens_str;
      tokens_str = split(line, ' '); // get x,y,z
      std::string result;
      for (auto const& s : tokens_str) result += s;
      Cube.insert(result);
    }
  }

  set<string>::iterator iter;
  for(iter=Cube.begin(); iter!=Cube.end();++iter){
    cout<<*iter<<endl;
  }
  infile.close();

  return 0;
}
