4610project: shell_main.o sdisk.o filesys.o block.o shell.o table.o
	g++ shell_main.o sdisk.o filesys.o block.o shell.o table.o -o 4610project

main.o: shell_main.cpp
	g++ -c shell_main.cpp

sdisk.o: sdisk.cpp sdisk.h
	g++ -c sdisk.cpp

filesys.o: filesys.cpp filesys.h
	g++ -c filesys.cpp

block.o: block.cpp block.h
	g++ -c block.cpp

shell.o: shell.cpp shell.h
	g++ -c shell.cpp

table.o: table.cpp table.h
	g++ -c table.cpp

clean:
	rm *.o
	rm 4610project
	rm disk