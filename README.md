# OpenMPvsMPI
***
The objective of this study is to analyze and compare the performance of four standard matrix multiplication algorithms (row by column) with different optimization techniques (MM1c, MM1f, MM1fu, MM2f) implemented with two parallel programming models (OpenMPI and OpenMP). on multiple compute nodes with multicore processors.

### General Info
***

The objective of this study is to analyze and compare the performance of four standard matrix multiplication algorithms (row by column) with different optimization techniques (MM1c, MM1f, MM1fu, MM2f) implemented with two parallel programming models (OpenMPI and OpenMP). on multiple compute nodes with multicore processors. For this, the time was evaluated Matrix multiplication execution for different sizes of matrices and number of threads in a shared memory architecture. The results show that in the OpenMP implementation the MM2f code was the most efficient in terms of performance, while in the MPI implementation, the MM2f, MM1f and MM1fu codes managed to get the shortest execution times, which means that they are the most efficient codes. more efficient in terms of performance.

## Technologies
***
List of technologies used within the project:
* [OpenMP](https://www.openmp.org/)
* [Open MPI](https://www.open-mpi.org/)

## Installation
***
To install please follow the below steps: 
```
$ git clone https://github.com/nextore28/OpenMPvsMPI.git
$ cd ../Files
Open the "Anexo_Evaluacion_Rendimiento.pdf" file to check the next steps.
```
The described process was tested using a Linux terminal. The servers were configured previously with OpenMP and an OpenMPI cluster.
