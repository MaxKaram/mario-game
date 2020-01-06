#include "game.h"

int main() {
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(512, 550), "mario");

    sf::View view;
    view.reset((sf::FloatRect(0, 200, 512, 550)));
    window.setView(view);

    sf::Texture heroTex;
    sf::Texture blockTex;
    
    heroTex.loadFromFile("textures/heroV2.png");
    blockTex.loadFromFile("textures/textures.png");

    Game game(&blockTex, &heroTex);
    int time = 0;

    while(window.isOpen()) {
        sf::Event event;
        int elapsed = clock.getElapsedTime().asMilliseconds();
        while(window.pollEvent(event)) {
            if(event.type == 0) window.close();
            if(event.type == sf::Event::KeyPressed) {
                game.setVector(event.key.code);
            }
            if(event.type == sf::Event::KeyReleased) {
                game.unsetVector(event.key.code);
            };
        }

        if(elapsed >= 35) {
            time = time == 1050 ? 35 : time + 35;
            clock.restart();
            game.performAnimations(time);
            if(!game.isGameOver()) game.move(time);
            game.changeQuestionFrame(time);
            game.setView(&view, &window);
        }

        window.clear(sf::Color(94, 145, 254));
        game.renderDecoration(&window);
        if(!game.isGameOver()) {
            game.renderAnimations(&window);
            game.renderMap(&window);
            game.drawHero(&window);
        } else {
            game.renderMap(&window);
            game.renderAnimations(&window);
        }
        window.display();
    }
}