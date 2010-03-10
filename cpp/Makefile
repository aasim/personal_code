all: array_merge random_prime_ssn

array_merge: array_merge.o
	g++ ../tmp/array_merge.o -o ../bin/array_merge

array_merge.o: array_merge.cpp
	g++ array_merge.cpp -c -o ../tmp/array_merge.o

random_prime_ssn: random_prime_ssn.o ../bin/check_prime.o
	g++ ../tmp/random_prime_ssn.o ../bin/check_prime.o -o ../bin/random_prime_ssn

random_prime_ssn.o: random_prime_ssn.cpp
	g++ random_prime_ssn.cpp -c -o ../tmp/random_prime_ssn.o

clean:
	rm ../tmp/array_merge.o ../tmp/random_prime_ssn.o ../bin/array_merge ../bin/random_prime_ssn
