병렬분산 숙제 #1 
README 
20161619 이세진

사용한 hostile : host
cspro1.sogang.ac.kr slots=15
cspro2.sogang.ac.kr slots=15

개발환경 : cspro 

[문제 2번]
Source 코드는 problem2 폴더에 위치한 p2_mscan.c, p2_unblock_sr.c, p2_block_rc.c
으로 총 3개의 C파일이다. 
1. p2_mscan.c : MPI_Scan을 사용하여 prefix sum 구현 
   컴파일 : mpicc p2_mscan.c -o mscan
   실행 : mpirun -np { # process } -mca btl ^openib -hostfile host mscan

2. p2_unblock_sr.c : MPI_Isend 와 MPI_Irecv 를 통해 prefix sum 구현 
   컴파일 : mpicc  p2_unblock_sr.c -o unblock
   실행 : mpirun -np { # process } -mca btl ^openib -hostfile host unblock

3. p2_block_sr.c :  MPI_Isend 와 MPI_Recv 를 통해 prefix sum 구현 
   컴파일 : mpicc  p2_block_sr.c -o block
   실행 : mpirun -np { # process } -mca btl ^openib -hostfile host block


[문제 3번]

Source 코드는 problem3 폴더에 위치한 p3_serial.c 와 p3_parallel.c 이며 이미지는 problem3/image에 있는 이미지로 실행하였다. Ppm image viewer는 OpenSeelt.exe를 사용하였다.

1. p3_serial.c : 3가지 image processing을 sequential 하게 구현함 
   컴파일 : gcc p3_serial.c -o serial
   실행 : ./serial {ppm file name} {1,2 or 3}
-> argv[2]에 1을 입력하면 flip 을 수행하고 결과 값 flip.ppm 생성
-> argv[2]에 2를 입력하면 grayscale 을 수행하고 결과 값 grayscale.ppm 생성
-> argv[2]에 3을 입력하면 smooth을 수행하고 결과 값 smooth.ppm 생성

1. p3_parallel.c : 3가지 image processing을 parallel 하게 구현함 
   컴파일 : gcc p3_parallel.c -o parallel
   실행 : ./parallel {ppm file name} {1,2 or 3}
-> argv[2]에 1을 입력하면 flip 을 수행하고 결과 값 flip.ppm 생성
-> argv[2]에 2를 입력하면 grayscale 을 수행하고 결과 값 grayscale.ppm 생성
-> argv[2]에 3을 입력하면 smooth을 수행하고 결과 값 smooth.ppm 생성
