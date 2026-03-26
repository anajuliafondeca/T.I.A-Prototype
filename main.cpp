#include "crow_all.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream> 
#include <sstream> 

int quemClicou = 0;

int main() {
    crow::SimpleApp app;

    crow::mustache::set_base("templates");

    // 1. ROTA DA TV (Página Principal)
    CROW_ROUTE(app, "/")([](){
        return crow::mustache::load("index.html").render();
    });

    // 2. ROTA DO CELULAR (Controle)
    CROW_ROUTE(app, "/controle")([](){
        return crow::mustache::load("controle.html").render();
    });

    // 3. RECEBE O CLIQUE DO CELULAR (Buzzer)
    CROW_ROUTE(app, "/clique")([&](const crow::request& req){
        auto t = req.url_params.get("time");
        if(t != nullptr) {
            try {
                quemClicou = std::stoi(t);
                std::cout << ">>> Time " << quemClicou << " apertou o buzzer!" << std::endl;
            } catch (...) {
                std::cout << "Erro na conversao do time." << std::endl;
            }
        }
        return crow::response(200);
    });

    // 4. TV VERIFICA SE HOUVE CLIQUE (Polling)
    CROW_ROUTE(app, "/verificar_clique")([](){
        std::string res = std::to_string(quemClicou);
        quemClicou = 0; 
        return res;
    });

    // ==========================================
    // ROTA NOVA: RECEBE A PERGUNTA E MOSTRA NO LOG
    // ==========================================
    CROW_ROUTE(app, "/log_pergunta")([&](const crow::request& req){
        auto q = req.url_params.get("q");
        auto a = req.url_params.get("a");
        
        if(q != nullptr && a != nullptr) {
            std::cout << "\n========================================" << std::endl;
            std::cout << "[ GAME MASTER ] NOVA PERGUNTA NA TELA" << std::endl;
            std::cout << "Pergunta: " << q << std::endl;
            std::cout << "Resposta Correta: " << a << std::endl;
            std::cout << "========================================\n" << std::endl;
        }
        return crow::response(200);
    });

    // 5. ROTA PARA CARREGAR ARQUIVOS ESTÁTICOS
    CROW_ROUTE(app, "/<string>")([](std::string filename){
        try {
            std::ifstream is("templates/" + filename, std::ios::in | std::ios::binary);
            if (!is) return crow::response(404);
            
            std::ostringstream contents;
            contents << is.rdbuf();
            crow::response res(contents.str());
            
            if (filename.find(".css") != std::string::npos) {
                res.set_header("Content-Type", "text/css");
            } else if (filename.find(".png") != std::string::npos) {
                res.set_header("Content-Type", "image/png");
            } else if (filename.find(".jpg") != std::string::npos || filename.find(".jpeg") != std::string::npos) {
                res.set_header("Content-Type", "image/jpeg");
            } else if (filename.find(".gif") != std::string::npos) {
                res.set_header("Content-Type", "image/gif");
            } else if (filename.find(".mp4") != std::string::npos) {
                res.set_header("Content-Type", "video/mp4"); 
            }
            
            return res;
        } catch (...) {
            return crow::response(404);
        }
    });

    char* port = getenv("PORT");
    uint16_t portNumber = port ? (uint16_t)atoi(port) : 3000;

    std::cout << "\n========================================" << std::endl;
    std::cout << "SISTEMA T.I.A INICIADO ONLINE" << std::endl;
    std::cout << "Porta utilizada: " << portNumber << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    app.port(portNumber).multithreaded().run();
}