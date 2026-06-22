main: compiler drivers conector tools pruebas hidroponia manager water statistics

CPPFLAGS="-I/usr/include -I/usr/include/cppconn"
CFLAGS=-std=c++0x
CXXFLAGS=-Wall

compiler:
	g++ Codes/compiler.cpp -o comp

drivers:
	./comp

conector:
	g++ -Wall -c Codes/conector.cpp -o Objects/conector.o

tools:
	g++ -std=c++0x -Wall -c Codes/tools.cpp -o Objects/tools.o

pruebas: 
	g++ -std=c++0x -o Hardware_test Objects/*.o Codes/pruebas.cpp -lmysqlcppconn

hidroponia:
	g++ -std=c++0x -o Hidroponia Objects/*.o Codes/scheduller.cpp -lmysqlcppconn

simulacion:
	g++ -std=c++0x -o simulate Objects/*.o Codes/simulatio.cpp -lmysqlcppconn

manager:
	g++ -std=c++0x Objects/conector.o Objects/funciones.o Codes/manager.cpp -o admin -lmysqlcppconn

statistics:
	g++ -std=c++0x Objects/conector.o Codes/stats.cpp -o stats -lmysqlcppconn

water:
	g++ -std=c++0x Objects/conector.o Objects/temagua.o Codes/wateradmin.cpp -o wateradmin -lmysqlcppconn

clean:
	rm Hardware_test Hidroponia admin Objects/* wateradmin comp stats
	
