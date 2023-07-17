#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <Windows.h>      
//#include <unistd.h>

char mapa[24][26] = {
    // Mapa do jogo
    "0000000000000000000000000",
    "0000000000000000000000000",
    "0000001111111111111111111",
    "0000001000000001000000001",
    "0000001011011101011101101",
    "0000001011011101011101101",
    "0000001000000000000000001",
    "0000001011010111110101101",
    "0000001000010001000100001",
    "0000001111011101011101111",
    "0000002221010000000101222",
    "0000001111010112110101111",
    "0000000000000122210000000",
    "0000001111010111110101111",
    "0000002221010000000101222",
    "0000001111010111110101111",
    "0000001000000001000000001",
    "0000001011011101011101101",
    "0000001001000000000001001",
    "0000001101010111110101011",
    "0000001000010001000100001",
    "0000001011111101011111101",
    "0000001000000000000000001",
    "0000001111111111111111111"
};

// Contagem de pontos
int placar = 0;

// Variável de fim de jogo
bool gameover = false;
bool win = false;

// Cores do mapa
int r, g, b;

// Posicao do CatMan
int posx = 15; 
int posy = 12;

// Estado de movimento do CatMan
bool cima = false;
bool baixo = false;
bool esq = false;
bool dir = false;

// Intencao de movimento do CatMan
bool querdireita = false;
bool queresquerda = false;
bool quercima = false;
bool querbaixo = false;

//Variaveis dos fantasmas
struct Fantasma {
    int fposx, fposy;
    int sorteiomov;
    bool encruzilhada = false;
};

int main() {
    //Seed aleatoria para o movimento dos fantasmas
    srand((unsigned)time(NULL));

    // Cria a janela
    sf::RenderWindow window(sf::VideoMode(1230, 960), "Pawsitive 2.0");

    //Carregamento da fonte do jogo
    sf::Font font;
    if (!font.loadFromFile("fontio.ttf")) {
        std::cout << "Erro ao carregar fonte\n";
        return 0;
    }

    //Musica do jogo
    sf::Music music;
    if (!music.openFromFile("music.ogg")) {
        return -1;
    }
    music.setLoop(true);
    music.setVolume(60.f);
    music.play();

    // Texto do nome do jogo
    sf::Text text;
    text.setFont(font);
    text.setPosition(352, 20);
    text.setString("Pawsitive 2.0");
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);

    // Texto do placar
    sf::Text score;
    score.setFont(font);
    score.setPosition(50, 480);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::White);
    score.setStyle(sf::Text::Bold);
    score.setLineSpacing(1.5f);

    // Tela do fim do jogo
    sf::RectangleShape endgame(sf::Vector2f(1000, 800));
    endgame.setFillColor(sf::Color(0, 0, 0));

    // Texto de gameover
    sf::Text endgamelost;
    endgamelost.setFont(font);
    endgamelost.setPosition(340, 400);
    endgamelost.setString("GAME OVER");
    endgamelost.setCharacterSize(60);
    endgamelost.setFillColor(sf::Color::White);
    endgamelost.setStyle(sf::Text::Bold);

    // Texto de gamewin
    sf::Text endgamewin;
    endgamewin.setFont(font);
    endgamewin.setPosition(280, 400);
    endgamewin.setString("YOU WIN!!!!");
    endgamewin.setCharacterSize(60);
    endgamewin.setFillColor(sf::Color::White);
    endgamewin.setStyle(sf::Text::Bold);

    //Sprite da taca do vencedor
    sf::Texture texture10;
    if (!texture10.loadFromFile("award.png")) {
        std::cout << "Erro lendo imagem award.png\n";
        return 0;
    }
    sf::Sprite spriteaward;
    spriteaward.setTexture(texture10);

    // Shape da parede
    sf::RectangleShape rectangle(sf::Vector2f(30, 30));
    rectangle.setFillColor(sf::Color(255, 255, 255));
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(sf::Color(169, 169, 169));

    // Sprite da pilula
    sf::Texture texture5;
    if (!texture5.loadFromFile("milk.png")) {
        std::cout << "Erro lendo imagem milk.png\n";
        return 0;
    }
    sf::Sprite spritepill;
    spritepill.setTexture(texture5);

    // Sprite do CatMan
        // Direita
    sf::Texture texture;
    if (!texture.loadFromFile("catRight.png")) {
        std::cout << "Erro lendo imagem catRight.png\n";
        return 0;
    }
    sf::Sprite spritedir;
    spritedir.setScale(1.5f, 1.5f);
    spritedir.setTexture(texture);

        // Esquerda
    sf::Texture texture2;
    if (!texture2.loadFromFile("catLeft.png")) {
        std::cout << "Erro lendo imagem catLeft.png\n";
        return 0;
    }
    sf::Sprite spriteesq;
    spriteesq.setScale(1.5f, 1.5f);
    spriteesq.setTexture(texture2);

        // Cima
    sf::Texture texture3;
    if (!texture3.loadFromFile("catUp.png")) {
        std::cout << "Erro lendo imagem catUp.png\n";
        return 0;
    }
    sf::Sprite spritecim;
    spritecim.setScale(1.5f, 1.5f);
    spritecim.setTexture(texture3);

        // Baixo
    sf::Texture texture4;
    if (!texture4.loadFromFile("catDown.png")) {
        std::cout << "Erro lendo imagem catDown.png\n";
        return 0;
    }
    sf::Sprite spritebai;
    spritebai.setScale(1.5f, 1.5f);
    spritebai.setTexture(texture4);

    // Sprite dos fantasmas
    Fantasma fantasmas[4];

        //Skull
    fantasmas[0].fposx = 7;
    fantasmas[0].fposy = 3;
    sf::Texture texture6;
    if (!texture6.loadFromFile("skull.png")) {
        std::cout << "Erro lendo imagem skull.png\n";
        return 0;
    }
    sf::Sprite spritef1;
    spritef1.setTexture(texture6);

        //Slime
    fantasmas[1].fposx = 7;
    fantasmas[1].fposy = 22;
    sf::Texture texture7;
    if (!texture7.loadFromFile("slime.png")) {
        std::cout << "Erro lendo imagem slime.png\n";
        return 0;
    }
    sf::Sprite spritef2;
    spritef2.setTexture(texture7);

        //Spike
    fantasmas[2].fposx = 23;
    fantasmas[2].fposy = 22;
    sf::Texture texture8;
    if (!texture8.loadFromFile("spike.png")) {
        std::cout << "Erro lendo imagem spike.png\n";
        return 0;
    }
    sf::Sprite spritef3;
    spritef3.setTexture(texture8);

        //Mount
    fantasmas[3].fposx = 23;
    fantasmas[3].fposy = 3;
    sf::Texture texture9;
    if (!texture9.loadFromFile("mount.png")) {
        std::cout << "Erro lendo imagem mount.png\n";
        return 0;
    }
    sf::Sprite spritef4;
    spritef4.setTexture(texture9);

    // Cria um relógio para medir o tempo
    sf::Clock clock;

    // Executa o programa enquanto a janela está aberta
    while (window.isOpen()) {

        // Verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // Evento "fechar" acionado: fecha a janela
            if (event.type == sf::Event::Closed)
                window.close();

            // Tecla pressionada
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    queresquerda = true;   // left key: CatMan tem a intencao de se mover para a esquerda
                    querdireita = quercima = querbaixo = false;
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    querdireita = true;   // right key: CatMan tem a intencao de se mover para a direita
                    queresquerda = quercima = querbaixo = false;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    quercima = true;   // up key: CatMan tem a intencao de se mover para cima
                    querdireita = querbaixo = queresquerda = false;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    querbaixo = true;   // down key: CatMan tem a intencao de se mover para baixo
                    querdireita = quercima = queresquerda = false;
                }
            }
        }

        //Definicao das cores do mapa com base no placar
             //A mudanca comeca a ser mais rapida conforme aumenta o placar
        double tempo = 0.1;
        if (placar > 50 && placar < 120) {
            if (clock.getElapsedTime() > sf::seconds(tempo)) {
                r = rand() % 50;  
                g = rand() % 105; 
                b = rand() % 105;
                tempo += 0.05;
            }
        }
        else if (placar >= 120 && placar < 190) {
            if (clock.getElapsedTime() > sf::seconds(tempo)) {
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
                tempo += 0.1;
            }
        }

        //Conferindo se o CatMan foi pego por algum fantasma
        for (int i = 0; i < 4; i++) {
            if (fantasmas[i].fposx == posx && fantasmas[i].fposy == posy) {
                gameover = true;
                win = false;
                break;
            }
        }

        //Conferindo se o CatMan pegou todos as pilulas
        if (placar == 191) {
            win = true;
            gameover = false;
        }

        if (!gameover && !win) {
            // Muda a posição de todos os personagens a cada 0.4 segundos
            if (clock.getElapsedTime() > sf::seconds(0.4)) {
                clock.restart();      // Recomeça contagem do tempo

                //Fecha a casinha do CatMan depois dele sair dela
                if (posy == 11 && posx == 15) {
                    mapa[11][15] = '1';
                }

                // Movimentação do Pacman
                if (quercima && mapa[posy - 1][posx] != '1') { //Se o CatMan quer e pode se mover, ele se move
                    cima = true;
                    esq = dir = baixo = false;
                    quercima = false;
                }
                if (querbaixo && mapa[posy + 1][posx] != '1') {
                    baixo = true;
                    esq = dir = cima = false;
                    querbaixo = false;
                }
                if (queresquerda && mapa[posy][posx - 1] != '1') {
                    esq = true;
                    cima = dir = baixo = false;
                    queresquerda = false;
                }
                if (querdireita && mapa[posy][posx + 1] != '1') {
                    dir = true;
                    esq = cima = baixo = false;
                    querdireita = false;
                }

                //Se o CatMan pode continuar se movendo, ele se move
                if (cima && mapa[posy - 1][posx] != '1')
                    posy--;
                if (baixo && mapa[posy + 1][posx] != '1')
                    posy++;
                if (esq && mapa[posy][posx - 1] != '1')
                    posx--;
                if (dir && mapa[posy][posx + 1] != '1')
                    posx++;

                //Passagem do CatMan pelos tuneis
                if (posx > 24)
                    posx = 6;
                if (posx < 6)
                    posx = 24;

                //Tira as pilulas do mapa quando o CatMan passa por elas
                if (mapa[posy][posx] == '0') {
                    mapa[posy][posx] = '2';
                    placar++;
                }

                //Movimento dos fantasmas
                int cont;

                for (int i = 1; i < 4; i++) { //Movimenta tres fantasmas aleatoriamente 
                    cont = 0;

                    std::vector<int> movimentosValidos; // Lista de movimentos válidos


                    if (mapa[fantasmas[i].fposy - 1][fantasmas[i].fposx] != '1') {
                        if (fantasmas[i].sorteiomov != 1) {
                            movimentosValidos.push_back(0); // Cima
                        }
                        cont++;
                    }
                    if (mapa[fantasmas[i].fposy + 1][fantasmas[i].fposx] != '1') {
                        if (fantasmas[i].sorteiomov != 0) {
                            movimentosValidos.push_back(1); // Baixo
                        }
                        cont++;
                    }
                    if (mapa[fantasmas[i].fposy][fantasmas[i].fposx - 1] != '1') {
                        if (fantasmas[i].sorteiomov != 3) {
                            movimentosValidos.push_back(2); // Esquerda
                        }
                        cont++;
                    }
                    if (mapa[fantasmas[i].fposy][fantasmas[i].fposx + 1] != '1') {
                        if (fantasmas[i].sorteiomov != 2) {
                            movimentosValidos.push_back(3); // Direita
                        }
                        cont++;
                    }

                    //Se o fantasmas tem mais de uma opcao de caminho, ele esta em uma encruzilhada
                    if (cont > 1)
                        fantasmas[i].encruzilhada = true;
                    else
                        fantasmas[i].encruzilhada = false;


                    // Se existem movimentos válidos e o fantasma esta em uma encruzilhada, escolhe aleatoriamente um para seguir
                    if (!movimentosValidos.empty() && fantasmas[i].encruzilhada) {
                        int randomIndex = rand() % movimentosValidos.size();
                        fantasmas[i].sorteiomov = movimentosValidos[randomIndex];
                    }

                    //Muda posicao do fantasma conforme sorteio
                    switch (fantasmas[i].sorteiomov) {
                    case 0:
                        if (mapa[fantasmas[i].fposy - 1][fantasmas[i].fposx] != '1') {
                            fantasmas[i].fposy--;
                        }
                        break;
                    case 1:
                        if (mapa[fantasmas[i].fposy + 1][fantasmas[i].fposx] != '1') {
                            fantasmas[i].fposy++;
                        }
                        break;
                    case 2:
                        if (mapa[fantasmas[i].fposy][fantasmas[i].fposx - 1] != '1') {
                            fantasmas[i].fposx--;
                        }
                        break;
                    case 3:
                        if (mapa[fantasmas[i].fposy][fantasmas[i].fposx + 1] != '1') {
                            fantasmas[i].fposx++;
                        }
                        break;
                    default:
                        break;
                    }

                    //Passagem dos fantasmas pelos tuneis
                    if (fantasmas[i].fposx > 24)
                        fantasmas[i].fposx = 6;
                    if (fantasmas[i].fposx < 6)
                        fantasmas[i].fposx = 24;
                }

                //Movimento do fantasma perseguidor
                int direcaox = posx - fantasmas[0].fposx; //Calcula a distancia entre CatMan e perseguidor no eixo X
                int direcaoy = posy - fantasmas[0].fposy; //Calcula a distancia entre CatMan e perseguidor no eixo Y

                bool andou = false; //Garante que o fantasma mudara a posicao x OU y por vez

                if (direcaox > 0 && mapa[fantasmas[0].fposy][fantasmas[0].fposx + 1] != '1') {
                    //Se a distancia X e positiva, o fantasma precisa ir para direita
                    fantasmas[0].fposx++;
                    andou = true;
                }
                else if (direcaox < 0 && mapa[fantasmas[0].fposy][fantasmas[0].fposx - 1] != '1') {
                    //Se a distancia X e negativa, o fantasma precisa ir para esquerda
                    fantasmas[0].fposx--;
                    andou = true;
                }

                if (direcaoy > 0 && mapa[fantasmas[0].fposy + 1][fantasmas[0].fposx] != '1' && !andou) {
                    //Se a distancia Y e positiva, o fantasma precisa ir para baixo
                    fantasmas[0].fposy++;
                }
                else if (direcaoy < 0 && mapa[fantasmas[0].fposy - 1][fantasmas[0].fposx] != '1' && !andou) {
                    //Se a distancia Y e negativa, o fantasma precisa ir para cima
                    fantasmas[0].fposy--;
                }

                //Passagem do perseguidor pelos tuneis
                if (fantasmas[0].fposx > 24)
                    fantasmas[0].fposx = 6;
                if (fantasmas[0].fposx < 6)
                    fantasmas[0].fposx = 24;
            }
        }

        // Limpa a janela com a cor preta
        window.clear(sf::Color::Black);

        // Desenha tudo aqui...

        // Desenha o jogo se ele ainda esta acontecendo (CatMan nao ganhou nem perdeu ainda)
        if (!gameover && !win) {

            //Desenha o nome do jogo
            window.draw(text);

            // Desenha o mapa
            for (int i = 0; i < 24; i++) {
                for (int j = 0; j < 25; j++) {
                    if (mapa[i][j] == '1') {
                        //DEsenha as paredes
                        if (placar <= 50) { 
                            //Mapa multicolorido ate os 50 pontos
                            r = rand() % 256;
                            g = rand() % 256;
                            b = rand() % 256;
                        }
                        else if (placar >= 190) {
                            //Mapa "desaparece" antes de pegar a ultima pilula
                            r = 0;
                            g = 0;
                            b = 0;
                            rectangle.setOutlineColor(sf::Color(20, 20, 20));
                        }
                        rectangle.setFillColor(sf::Color(r, g, b));
                        rectangle.setPosition(j * 40, i * 40);
                        window.draw(rectangle);
                    }
                    else if (i > 1 && j > 5 && mapa[i][j] == '0') {
                        //Desenha as pilulas
                        spritepill.setPosition(j * 40.5, i * 40.5);
                        window.draw(spritepill);
                    }
                }
            }

            // Desenha PacMan
            if (!dir && !esq && !cima && !baixo) {  // Posição inicial
                spritedir.setPosition(posx * 40, posy * 40);
                window.draw(spritedir);
            }
            if (dir) { //Direita
                spritedir.setPosition(posx * 40, posy * 40);
                window.draw(spritedir);
            }
            else if (esq) { //Esquerda
                spriteesq.setPosition(posx * 40, posy * 40);
                window.draw(spriteesq);
            }
            else if (cima) { //Cima
                spritecim.setPosition(posx * 40, posy * 40);
                window.draw(spritecim);
            }
            else if (baixo) { //Baixo
                spritebai.setPosition(posx * 40, posy * 40);
                window.draw(spritebai);
            }

            //Desenha os fantasmas
            spritef1.setPosition(fantasmas[0].fposx * 40, fantasmas[0].fposy * 40);
            window.draw(spritef1);

            spritef2.setPosition(fantasmas[1].fposx * 40, fantasmas[1].fposy * 40);
            window.draw(spritef2);

            spritef3.setPosition(fantasmas[2].fposx * 40, fantasmas[2].fposy * 40);
            window.draw(spritef3);

            spritef4.setPosition(fantasmas[3].fposx * 40, fantasmas[3].fposy * 40);
            window.draw(spritef4);

            //Desenha o placar
            char s[20];
            snprintf(s, 20, "Score\n %.3d", placar);
            score.setString(s);
            window.draw(score);
        }
        else if (gameover){
            //Tela de gameover
            Sleep(1000);
            window.draw(endgame);
            window.draw(endgamelost);
            music.pause();
        }
        else if (win) {
            //Tela de gamewin
            window.draw(endgame);
            window.draw(endgamewin);
            spriteaward.setPosition(500, 240);
            window.draw(spriteaward);
            music.pause();
        }

        // Termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
