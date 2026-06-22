#include "Headers/conector.hpp"
using namespace std;
using namespace sql;

void conector::conectar(){
	try {
		//crear conexion
		con = driver->connect("tcp://localhost:3306", "root", "qwerty");
		//escoger la base
		con->setSchema("betha");
		stmt = con->createStatement();

	}catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}

//funcion que limpia las variables
void conector::borrar(){
	delete con;
	delete stmt;
	delete res;
	}

//constructor inicializa el driver
conector::conector(){
	driver = get_driver_instance();
	}

