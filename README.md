# CPD_Game_of_life

Compile and run sequential.cpp:
g++ sequential.cpp -std=c++11 -g -o sequential
./sequential life3D-instances/s5e50.in 10

Compile and run openMP.cpp:
g++ openMP.cpp -fopenmp -std=c++11 -g -o openmp
./openmp life3D-instances/s5e50.in 10

Compile and run mpi.cpp (with 4 processes):
mpic++ -o mpi mpi.cpp -std=c++11
mpirun -np 4 mpi life3D-instances/s5e50.in 10

