make clean && make

echo $1
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --$1
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --$1
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --$1
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --$1
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --$1
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --$1

echo
echo $2
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --$2
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --$2
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --$2
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --$2
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --$2
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --$2

echo
echo $3
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --$3
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --$3
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --$3
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --$3
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --$3
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --$3