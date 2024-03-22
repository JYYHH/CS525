## 0. Initilize
- all commands below should be executed under this working dir
```bash
mkdir bin
mkdir data
```

## 1. How to compile the source files
```bash
make
```

### 1.1 clean up previous targets and data
```bash
make clean
```

## 2. Run raw bin
### 2.1 Data Generator: `datagen.run`
```bash
./bin/datagen.run $number_of_operations $upper_bound_of_value > $data_file
```
- This will generate data with $number_of_operations operations into $data_file
- All the values generated will be in [1, $upper_bound_of_value]

### 2.2 (Serial) Binary Tree Runner: `lazy.run`/`normal.run`
```bash
./$target_name < $input_file (> $output_file)
```
- This will print the results of each `look_up` operation into stdout or $output_file
- Note there are two numbers in each line of input file: `the first` the the operator number (1: Insert / 2: Delete / 3: LookUP), and `the second` is the value to be operated.

### 2.3 (Parallel) Binary Tree Runner: `parallel.run`
```bash
./$target_name $MAX_THREAD_COUNT < $input_file (> $output_file)
```
- only one more parameter for target program: how many threads will run simultaneously to handle the BT operation.

## 3. Use my script
### 3.1 Correctness Benchmark
```bash
bash comp_correct.sh $number_of_operations $upper_bound_of_value $prog1 $prog2
```
- ___For the purpose of hw3's Part 3, just type `bash comp_correct.sh 3000000 1000000 lazy normal`___
- This script will run an infinite loop to compare the output of $prog1 and $prog2.
- Every time inside the loop, it will:
    1. generate data based on  $number_of_operations and $upper_bound_of_value, store it in `$DATA_DIR/in.in`
    2. feed the data into $prog1 and $prog2
    3. collect the results into `$DATA_DIR/1.out` and `$DATA_DIR/2.out`
    4. compare the two files, if different, break from the loop
- So you need to terminate it by yourself if the two programs behave identically.

### 3.2 Running time Collector
- ___(You must run 3.1 before this section, to generate the data)___
```bash
bash running_time_collect.sh
```

- Collect the running time of `parallel.run` based on data generated in 3.1, and with thread number from 1 to 128.

### 3.3 Draw figure using python
- ___(You must finish 3.2's running before this section, to generate profiling result)___
```bash
python draw_exp_result.py
```
- Show the figure