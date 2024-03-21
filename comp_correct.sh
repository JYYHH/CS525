# $1: Number of total operation
# $2: M, all the values generated will be in [1, M], and M must <= 1e6
# $3: name of the first compared obj
# $4: name of the second compared obj

# Files
SRC_DIR=src
BIN_DIR=bin
DATA_DIR=data

# Executable
Tar_datagen=$BIN_DIR/datagen.run
Tar_1=$BIN_DIR/$3.run
Tar_2=$BIN_DIR/$4.run

# Main routine
cnt=0
while [ 1 ]
do  
    let cnt+=1
    # Step 1: generate correspond input data
    ./$Tar_datagen $1 $2 > $DATA_DIR/in.in
    # Step 2: feed the data into two programs
    ./$Tar_1 < $DATA_DIR/in.in > $DATA_DIR/1.out
    ./$Tar_2 < $DATA_DIR/in.in > $DATA_DIR/2.out
    # Step 3: compare the output of two programs
    cmp -s $DATA_DIR/1.out $DATA_DIR/2.out
    cmp_=$? 
    if [ $cmp_ -eq 0 ]
    then
        echo Round No.$cnt Test passed!
    else
        echo Round No.$cnt Test failed, please check the difference between $DATA_DIR/1.out and $DATA_DIR/2.out
        break
    fi
done 

