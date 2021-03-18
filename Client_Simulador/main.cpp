#include "httplib.h"
#include <iostream>
#include <string>
#include <ctime>

#ifdef LINUX
    #include <getopt.h>
#else
    #define __GNU_LIBRARY__
    #include "getopt.h"
#endif

void printUsage(char* argv[]) {
    std::cout <<  "Uso: " << argv[0] << " [-h] -a [ip] -p [port]" << std::endl;
    std::cout <<  "Options:" << std::endl;
    std::cout <<  "  -h          Print this help message." << std::endl;
    std::cout <<  "  -a          Optional ip address to Connect." << std::endl;
    std::cout <<  "  -p          Optional port to Connect." << std::endl;
    
    std::cout <<  std::endl << "Examples:" << std::endl;
    std::cout <<  "  > " << argv[0] << " -v -a 192.168.0.32 -p 18000" << std::endl;
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
    std::string ip = "192.168.0.40";
    int port = 18000;

    char c;
    int temp_mode = 0;
    while ( (c = getopt (argc, argv, "a:p:h")) != -1) {
        
        switch(c) {
            case 'a':
                ip = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                printUsage(argv);
                exit(0);
            default:
                break;    
        }

    }

    httplib::Client cli(ip, port);

    if (auto res = cli.Get("/")) {
        if (res->status == 200) {
            std::cout << res->body << std::endl;
        }
    } else {
        auto err = res.error();
    }

    auto res = cli.Post("/post", "nome=mostrador1&data=2021-01-31 14:51:12&tensao=120&corrente=1.4", "application/x-www-form-urlencoded");
    if (res) {
        if (res->status == 200) {
            std::cout << res->body << std::endl;
        }
    } else {
        auto err = res.error();
        std::cout << "Error: " << err << std::endl;         
    }

    std::string data;
    
    for (int c = 0; c < 20; c++) {
        std::ostringstream os;
        os << "nome=mostra" << c <<"&data=";
        os << get_now();
        os << "&tensao=127&corrente=1." << c;
        data = os.str();
        std::cout << data << std::endl; 
        res = cli.Post("/post", data, "application/x-www-form-urlencoded");
        if (res) {
            if (res->status == 200) {
                std::cout << res->body << std::endl;
            }
        } else {
            auto err = res.error();
            std::cout << "Error: " << err << std::endl;         
        }
    }
    
    httplib::MultipartFormDataItems items = {
        { "text1", "nome=mostrador1_text1&data=2021-01-31 14:51:12&tensao=120&corrente=1.4", "", "application/x-www-form-urlencoded" },
        { "text2", "nome=mostrador1_text2&data=2021-01-31 14:51:12&tensao=120&corrente=1.4", "", "application/x-www-form-urlencoded" },
        { "file1", "h\ne\n\nl\nl\no\n", "hello.txt", "text/plain" }
    };
    
    res = cli.Post("/multipart", items);
    if (res) {
        if (res->status == 200) {
            std::cout << res->body << std::endl;
        }
    } else {
        auto err = res.error();
        std::cout << "Error: " << err << std::endl;         
    }

    std::ifstream t("./leitura.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    res = cli.Put("/file", buffer.str(), "text/plain");
    if (res) {
        if (res->status == 200) {
            std::cout << res->body << std::endl;
        }
    } else {
        auto err = res.error();
        std::cout << "Error: " << err << std::endl;         
    }
}