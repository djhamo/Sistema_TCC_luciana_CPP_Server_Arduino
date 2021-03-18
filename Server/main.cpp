#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "sqlite3pp.h"
#include <string>
#include <vector>
#include <ctime>
#ifdef LINUX
    #include <getopt.h>
#else
    #define __GNU_LIBRARY__
    #include "getopt.h"
#endif


void printUsage(char* argv[]) {
    std::cout <<  "Uso: " << argv[0] << " [-hv] -a [ip] -p [port] -d [database]" << std::endl;
    std::cout <<  "Options:" << std::endl;
    std::cout <<  "  -h          Print this help message." << std::endl;
    std::cout <<  "  -v          Optional verbose flag." << std::endl;
    std::cout <<  "  -a          Optional ip address to Bind." << std::endl;
    std::cout <<  "  -p          Optional port to Bind." << std::endl;
    std::cout <<  "  -d          Optional Database file" << std::endl;
    
    std::cout <<  std::endl << "Examples:" << std::endl;
    std::cout <<  "  > " << argv[0] << " -v -a 0.0.0.0 -p 8000 -d medicao.db" << std::endl;
    exit(0);
}

std::string get_now() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    std::ostringstream os;
    os << 1900 + ltm->tm_year << "-";
    std::string tmes;
    if (1 + ltm->tm_mon > 9) 
        tmes = std::to_string( 1 + ltm->tm_mon);
    else {
        tmes = "0" + std::to_string( 1 + ltm->tm_mon);
    }       
    os << tmes << "-";
    std::string tdia;
    if (ltm->tm_mday > 9) 
        tdia = std::to_string( ltm->tm_mday);
    else {
        tdia = "0" + std::to_string( ltm->tm_mday);
    }       
    os << tdia << " ";
    std::string thora;
    if (1+ltm->tm_hour > 9) 
        thora = std::to_string( 1+ltm->tm_hour );
    else {
        thora = "0" + std::to_string( 1+ltm->tm_hour);
    }
    os << thora << ":" ;       
    std::string tmin;
    if (1+ltm->tm_min > 9) 
        tmin = std::to_string( 1+ltm->tm_min);
    else {
        tmin = "0" + std::to_string( 1+ltm->tm_min);
    }      
    os << tmin << ":" ;
    std::string tseg;
    if (ltm->tm_sec > 9) 
        tseg = std::to_string( ltm->tm_sec );
    else {
        tseg = "0" + std::to_string( ltm->tm_sec );
    }       
    os << tseg ;

    return os.str(); 
}

int main(int argc, char *argv[]) {

    // HTTP
    httplib::Server svr;
    std::string db_name = "test.db";
    std::string ip = "0.0.0.0";
    int port = 8000;
    bool verbose = false;

    std::string sql_insert_leitura = "INSERT INTO LEITURAS (NOME_MEDIDOR, DATA_MEDICAO, VALOR_TENSAO, VALOR_CORRENTE ) VALUES (?, ?, ?, ?)";

    char c;
    int temp_mode = 0;
    while ( (c = getopt (argc, argv, "a:p:d:vh")) != -1) {
        
        switch(c) {
            case 'a':
                ip = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;                
            case 'd':
                db_name = optarg;
                break;                
            case 'v':
                verbose = true;
                break;
            case 'h':
                printUsage(argv);
                exit(0);
            default:
                break;    
        }

    }

    if (verbose) {
        std::cout << "Iniciando" << std::endl;
    }

    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("<html><h1>Teste de Servidor</h1></html>", "text/html");
    });

    svr.Get("/help", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("<html><h1>Página de Ajuda</h1></html>", "text/html");
    });

    svr.Get("/init", [&](const httplib::Request &, httplib::Response &res) {
        
        try {
            sqlite3pp::database db(db_name.c_str());
            {
                db.execute(R"(
                    CREATE TABLE IF NOT EXISTS LEITURAS (
                        ID INTEGER PRIMARY KEY AUTOINCREMENT,
                        NOME_MEDIDOR           TEXT    NOT NULL,
                        DATA_MEDICAO            DATETIME     NOT NULL,
                        VALOR_TENSAO        REAL,
                        VALOR_CORRENTE         REAL
                    )
                )");
                if (verbose) {
                    std::cout << "Tabela criada com Sucesso" << std::endl;
                }
            }
        } catch (std::exception& ex) {
            if (verbose) {
                std::cout << "SQL error: " << ex.what() << std::endl;
            }
            res.set_content("NOK", "text/plain");
        }    
   
        res.set_content("OK", "text/plain");
    });
 
    svr.Post("/post", [&](const httplib::Request &req, httplib::Response &res) {
    
        auto nome = req.get_param_value("nome");
        //auto data = req.get_param_value("data");
        auto data = get_now();
        auto tensao = req.get_param_value("tensao");
        auto corrente = req.get_param_value("corrente");
                
        if (verbose) {
            std::cout << "Nome: " << nome << std::endl;
            std::cout << "Data: " << data << std::endl;
            std::cout << "Tensão: " << tensao << std::endl;
            std::cout << "Corrente: " << corrente << std::endl;
        }

        try {
            sqlite3pp::database db(db_name.c_str());
            {
                sqlite3pp::transaction xct(db);
                sqlite3pp::command cmd(db, sql_insert_leitura.c_str());

                cmd.bind(1, nome, sqlite3pp::copy);
                cmd.bind(2, data, sqlite3pp::copy);
                cmd.bind(3, tensao, sqlite3pp::copy);
                cmd.bind(4, corrente, sqlite3pp::copy);
                if (verbose) {
                    std::cout << "Sql " << cmd.getSQL() << std::endl;
                }
                cmd.execute();
                
                xct.commit();
            }
            
            db.disconnect();
            res.set_content("OK", "text/plain");

        } catch (std::exception& ex) {
            if (verbose) {
                std::cout << "SQL error: " << ex.what() << std::endl;
            }
            res.set_content("NOK", "text/plain");
        }

    });
    
    svr.Put("/file", [&](const httplib::Request &req, httplib::Response &res) {
        auto text_file = req.body.c_str();

        if (verbose) {
            std::cout << "text " << std::endl << text_file << std::endl;
        }

        try {

            sqlite3pp::database db(db_name.c_str());
            {
                sqlite3pp::transaction xct(db);

                sqlite3pp::command cmd(db, sql_insert_leitura.c_str());

                std::stringstream ss( text_file );

                while( ss.good() ) {
                    std::string substr;
                    getline( ss, substr, '\n' );

                    if (verbose) {
                        std::cout << substr << std::endl << std::endl;
                    }
                    std::stringstream ss_int( substr );
                    std::vector<std::string> line;
                    while( ss_int.good() ) {
                        std::string substr2;
                        getline( ss_int, substr2, ',' );
                        line.push_back( substr2 );
                    }

                    //tratamento da entrada

                    if (line.size() == 4) {
                        cmd.binder() << line[0] << line[1] << line[2] << line[3];
                        if (verbose) {
                            std::cout << "Sql " << cmd.getSQL() << std::endl;
                        }
                
                        cmd.execute();
                        cmd.reset();
                    }

                }

                xct.commit();
            }

            db.disconnect();
            res.set_content("OK", "text/plain");

        } catch (std::exception& ex) {
            if (verbose) {
                std::cout << "SQL error: " << ex.what() << std::endl;
            }
            res.set_content("NOK", "text/plain");
        }

   });

    svr.Get("/stop", [&](const httplib::Request& req, httplib::Response& res) {
        svr.stop();
    });

    if (verbose) {
        std::cout << "Iniciando Serviço: " << ip.c_str() << ":" << port << std::endl;
    }
    try {
        svr.listen(ip.c_str(), port);
    } catch (const std::exception& e) {
        std::cout << "HTTP Error: " << e.what() << std::endl;
    }    
    return 0;
}