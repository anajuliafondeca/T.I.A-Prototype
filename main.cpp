#include "crow_all.h"
#include <iostream>
#include <string>
#include <cstdlib>

int quemClicou = 0;

int main() {
    crow::SimpleApp app;

    // Configura o Crow para buscar HTML, CSS e Imagens dentro da pasta 'templates'
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

    // 5. ROTA PARA CARREGAR ARQUIVOS ESTÁTICOS (CSS, PNG, JPG)
    // Isso busca os arquivos dentro da pasta 'templates' conforme definido no set_base
    CROW_ROUTE(app, "/<string>")([](std::string filename){
        try {
            auto page = crow::mustache::load(filename);
            crow::response res(page.render());
            
            // Define o tipo de conteúdo para o navegador entender o que é cada arquivo
            if (filename.find(".css") != std::string::npos) {
                res.set_header("Content-Type", "text/css");
            } else if (filename.find(".png") != std::string::npos) {
                res.set_header("Content-Type", "image/png");
            } else if (filename.find(".jpg") != std::string::npos || filename.find(".jpeg") != std::string::npos) {
                res.set_header("Content-Type", "image/jpeg");
            }
            return res;
        } catch (...) {
            return crow::response(404);
        }
    });

    // CONFIGURAÇÃO DE PORTA PARA O RENDER
    // O Render atribui uma porta dinâmica, por isso usamos getenv("PORT")
    char* port = getenv("PORT");
    uint16_t portNumber = port ? (uint16_t)atoi(port) : 3000;

    std::cout << "\n========================================" << std::endl;
    std::cout << "SISTEMA T.I.A INICIADO ONLINE" << std::endl;
    std::cout << "Porta utilizada: " << portNumber << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Inicia o servidor
    app.port(portNumber).multithreaded().run();
}