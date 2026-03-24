# Usa uma imagem do Ubuntu com ferramentas de compilação
FROM ubuntu:latest

# Instala o compilador G++ e bibliotecas necessárias
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

# Define a pasta de trabalho dentro do servidor
WORKDIR /app

# Copia todos os seus arquivos para dentro do Docker
COPY . .

# Compila o código C++ (ajustado para Linux/Render)
RUN g++ main.cpp -o jogo.exe -lpthread

# Expõe a porta 3000
EXPOSE 3000

# Comando para rodar o jogo
CMD ["./jogo.exe"]
FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Comando de compilação para o Linux do Render
RUN g++ main.cpp -o jogo.exe -lpthread

EXPOSE 3000

CMD ["./jogo.exe"]