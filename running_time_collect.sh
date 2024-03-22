Tar=bin/parallel.run
Tar_lazy=bin/lazy.run
data=data/in.in
out_file=tmp.tmp
log_file=log.tmp

rm -f $log_file

cnt=0
while [ 1 ]
do  
    let cnt+=1

    # Run 5 times, and average the run time
    for i in {1..5}
    do 
        { time ./$Tar $cnt < $data > $out_file; } 2>>$log_file
    done

    if [ $cnt -eq 128 ]
    then 
        break
    fi
done

# Finally, measure the lazy's running time
for i in {1..5}
do 
    { time ./$Tar_lazy < $data > $out_file; } 2>>$log_file
done
