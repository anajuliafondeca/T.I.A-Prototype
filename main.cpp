#include "crow_all.h"
#include <iostream>
#include <string>

int quemClicou = 0;

int main() {
    crow::SimpleApp app;

    // Força o Crow a procurar na pasta onde está o executável
    crow::mustache::set_base("templates");

    // 1. ROTA DA TV (Página Principal)
    CROW_ROUTE(app, "/")([](){
        return crow::mustache::load("index.html").render();
    });

    // 2. ROTA DO CELULAR (Controle)
    CROW_ROUTE(app, "/controle")([](){
        return crow::mustache::load("controle.html").render();
    });

    // 3. RECEBE O CLIQUE DO CELULAR
    CROW_ROUTE(app, "/clique")([&](const crow::request& req){
        auto t = req.url_params.get("time");
        if(t != nullptr) {
            try {
                quemClicou = std::stoi(t);
                std::cout << ">>> Time " << quemClicou << " apertou o buzzer!" << std::endl;
            } catch (...) {
                std::cout << "Erro na conversão do time." << std::endl;
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

    // 5. ROTA PARA CARREGAR TUDO (CSS, PNG, JPG) - CORRIGIDA
    CROW_ROUTE(app, "/<string>")([](std::string filename){
        auto page = crow::mustache::load(filename);
        crow::response res(page.render());
        
        // Define o tipo de arquivo para o navegador não ignorar o CSS
        if (filename.find(".css") != std::string::npos) {
            res.set_header("Content-Type", "text/css");
        } else if (filename.find(".png") != std::string::npos) {
            res.set_header("Content-Type", "image/png");
        }
        return res;
    });

    std::cout << "\n========================================" << std::endl;
    std::cout << "SISTEMA T.I.A INICIADO" << std::endl;
    std::cout << "Acesse: http://localhost:3000" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Pega a porta do Render ou usa 3000 por padrão
    char* port = getenv("PORT");
    uint16_t portNumber = port ? (uint16_t)atoi(port) : 3000;
    
    std::cout << "Servidor iniciado na porta " << portNumber << std::endl;
    app.port(portNumber).multithreaded().run();
}