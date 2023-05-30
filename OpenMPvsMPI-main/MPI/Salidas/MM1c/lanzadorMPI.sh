#!/bin/bash

# Execution Parameters
source parametrosMPI.txt

# Iteration executions
for ((i=0; i<10; i++)); 
do
echo "Main FOR - Execute i = ${i}"
    for thread in "${threads[@]}";
    do
    echo "Thread - Execute Thread = ${thread}"
        for matrix in "${matrixes[@]}";
        do
        echo "Matrix - Execute matrix = ${matrix} thread = ${thread}"
            for program in "${programs[@]}";
            do
            # Commands
            filename="MPI-${program}-${matrix}-TH-${thread}.txt"
            echo "Execute MPI-${program}-${matrix}-TH-${thread}"
            mpirun --host worker4:${thread} mpi${program} ${matrix} >> ${filename}
            #mpirun --mca btl_tcp_if_include eno1 --host worker4:${thread},worker5:${thread} mpi${program} ${matrix} >> ${filename}
            #Comando de referencia:
            #Se debe incluir el parametro --mca btl_tcp_if_include eno1 para
            #restringir el socket de red que se puede usar, pues el proceso
            #estaba evaluando todos, incluido el de docker instalado
            #recientemente.
            #mpirun --mca btl_tcp_if_include eno1 --host worker4:2,worker5:2 ./mpiMM1c 300
            
            #echo "Ejecutando $cmd"
            #$cmd > "salida_${program}_${thread}_${matrix}.txt"
            done
        done
    done
done
echo "Todas las ejecuciones han finalizado."
