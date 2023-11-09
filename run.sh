#!/bin/bash
max=1
declare -a arr=("config_a") 
#                "config_b"
#                "config_c")

for i in "${arr[@]}"
do
    echo "================================================================================================"
    echo "Ejecutando simulaciones para archivo de configuración: $i.json"
    echo "------------------------------------------------------------------------------------------------"
    for j in `seq 1 $max`
    do
        build/sim.out -f ./config/$i.json -o $i"_"$j
        echo "Se ejecutó la simulación número $j para el archivo de configuración $i.json"
    done
    echo "================================================================================================"
done