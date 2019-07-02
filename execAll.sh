echo "========================================================================" > results
echo "   RESULTADOS         GULOSO " >> results
echo "========================================================================" >> results

for tam in 1 3 10;
do
  max=3
  if [ $tam -eq 3 ];
  then
    max=4;
  fi;
  for inst in $(seq 0 $max); #instancias
  do
    echo "Instancia $inst de tamanho $tam x100 vertices" >> results
    ./main instances/V"$tam"00E"$tam"000/$inst/Test/Problem.dat_"$tam"00_"$tam"000_"$inst" 0 0  >> results
  done;
done;

for alpha in 0.1 0.3 0.5;
do
  echo "========================================================================" >> results
  echo "   RESULTADOS         GULOSO RANDOMIZADO ($alpha) " >> results
  echo "========================================================================" >> results
  for tam in 1 3 10;
  do
    max=2
    if [ $tam -eq 3 ]
    then
      max=3;
    fi;
    for inst in $(seq 0 $max); #instancias
    do
      echo "Instancia $inst de tamanho $tam x100 vertices" >> results
      for exec in `seq 1 1 30`;
      do
        ./main instances/V"$tam"00E"$tam"000/$inst/Test/Problem.dat_"$tam"00_"$tam"000_"$inst" 1 0 $alpha 1  >> results
      done;
    done;
  done;
done;

echo "========================================================================" >> results
echo "   RESULTADOS         GULOSO RANDOMIZADO REATIVO " >> results
echo "========================================================================" >> results

for tam in 1 3 10;
do
  max=3
  if [ $tam -eq 3 ]
  then
    max=4;
  fi;
  for inst in $(seq 0 $max); #instancias
  do
    echo "Instancia $inst de tamanho $tam x100 vertices" >> results
    for reps in 10 100 1000 10000;
    do
      ./main instances/V"$tam"00E"$tam"000/$inst/Test/Problem.dat_"$tam"00_"$tam"000_"$inst" 2 0 0 $reps  >> results
    done;
  done;
done;
