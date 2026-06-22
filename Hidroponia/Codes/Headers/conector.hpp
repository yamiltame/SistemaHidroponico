//header de la clase conector para obtener datos de la base
#include <stdlib.h>
#include <iostream>
#include <string>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/prepared_statement.h>//Es clase heredada para el funcionamiento de nuestra conexio$
#include <cppconn/driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
using namespace std;
using namespace sql;

class conector{
        public:
                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;
                sql::ResultSet *res;
                void conectar();
                void borrar();
                conector ();
        };

