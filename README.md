# CPD_Game_of_life

Implementation of Game of Life in C++ done for the course of Distributed and Parallel Computing.

The sequential version is pretty good, running faster than the openMP version. Therefore the openMP.cpp is just an example. We would suggest starting with the sequential version if you are interested in using this repo.

**Compile and run sequential.cpp:**<br />
_g++ sequential.cpp -std=c++11 -g -o sequential<br />
./sequential life3D-instances/s5e50.in 10_

**Compile and run openMP.cpp:**<br />
_g++ openMP.cpp -fopenmp -std=c++11 -g -o openmp<br />
./openmp life3D-instances/s5e50.in 10_

**Compile and run mpi.cpp (with 4 processes):**<br />
_mpic++ -o mpi mpi.cpp -std=c++11<br />
mpirun -np 4 mpi life3D-instances/s5e50.in 10_
