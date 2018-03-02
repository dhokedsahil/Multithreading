repetitions=20
min_threads=30
max_threads=40
thread_increment=1
min_operations_per_thread=1000000
max_operations_per_thread=16000000
operations_per_thread_increment_factor=2

for ((reps=1;reps<=$repetitions;reps++));
do
    for((i = $min_threads; i <= $max_threads; i+= $thread_increment))
    do
        for((j = $min_operations_per_thread; j <= $max_operations_per_thread; j *= $operations_per_thread_increment_factor))
        do
            echo "Repetition : $reps"
            ./Tester threads $i operations_per_thread $j
            sleep .5
        done
    done
done