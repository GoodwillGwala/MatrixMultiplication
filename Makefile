#Commands:
# make 			:compiles and builds program
# make clean 	:removes compiled files from folder

CC = g++

#Main Execution Command
pythonDriver: Lab1
	python3 batch.py 
 
#Compile Commands
Lab1: Matrix Utility
	$(CC)  -c Lab1.cpp
 
Matrix: MatrixHeader
	$(CC) -c Matrix.cpp
 
Utility: UtilityHeader
	$(CC) -c Utility.cpp

MatrixHeader:
	$(CC) -c Matrix.h

UtilityHeader:
	$(CC) -c Utility.h

clean:
	rm -f *.o *.h.gch all