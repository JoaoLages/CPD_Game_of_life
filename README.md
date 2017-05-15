# CPD_Game_of_life

Compilar OMP:
g++ for_version.cpp -fopenmp -std=c++11 -g -o for_cpd
g++ task_version.cpp -fopenmp -std=c++11 -g -o task_cpd

MPI:
mpic++ -o mpi mpi.cpp -std=c++11
mpirun -np 4 mpi life3D-instances/s5e50.in 10

TODO:
- ver se o vector n é nulo antes de chamar .front()!
