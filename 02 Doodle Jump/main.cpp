#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream>

using namespace sf;

struct point { int x, y; };

enum State { MENU, GAME, ABOUT };

int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(400, 533), "Doodle Game!");
    app.setFramerateLimit(60);

    // Load Textures
    Texture t1, t2, t3, tMenu, tAbout;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/platform.png");
    t3.loadFromFile("images/doodle.png");
    tMenu.loadFromFile("images/menu.png");
    tAbout.loadFromFile("images/about.png");

    Sprite sBackground(t1), sPlat(t2), sPers(t3), sMenu(tMenu), sAbout(tAbout);

    // Font for Score
    Font font;
    font.loadFromFile("fonts/arial.ttf"); // Ensure you have a font file in a fonts folder
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::Black);

    point plat[20];
    for (int i = 0; i < 10; i++) {
        plat[i].x = rand() % 400;
        plat[i].y = rand() % 533;
    }

    int x = 100, y = 100, h = 200;
    float dx = 0, dy = 0;
    int score = 0;
    State currentState = MENU;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            // Menu Navigation
            if (e.type == Event::KeyPressed) {
                if (currentState == MENU) {
                    if (e.key.code == Keyboard::P) currentState = GAME;
                    if (e.key.code == Keyboard::A) currentState = ABOUT;
                    if (e.key.code == Keyboard::E) app.close();
                }
                else {
                    // Back to menu from Game or About
                    if (e.key.code == Keyboard::M || e.key.code == Keyboard::Escape) {
                        currentState = MENU;
                    }
                }
            }
        }

        if (currentState == GAME) {
            // --- Movement Logic ---
            if (Keyboard::isKeyPressed(Keyboard::Right)) x += 3;
            if (Keyboard::isKeyPressed(Keyboard::Left)) x -= 3;

            dy += 0.2;
            y += dy;
            if (y > 500) dy = -10; // Bounce off bottom (Cheat/Save)

            // Scroll the screen and add to score
            if (y < h) {
                for (int i = 0; i < 10; i++) {
                    y = h;
                    float scrollAmount = -dy;
                    plat[i].y = plat[i].y + scrollAmount;
                    if (plat[i].y > 533) { 
                        plat[i].y = 0; 
                        plat[i].x = rand() % 400; 
                        score += 10; // Gain points for every platform passed
                    }
                }
            }

            // Platform Collision
            for (int i = 0; i < 10; i++)
                if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68)
                    && (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0)) dy = -10;

            sPers.setPosition(x, y);

            // Update Score Display
            std::stringstream ss;
            ss << "Score: " << score;
            scoreText.setString(ss.str());
            scoreText.setPosition(10, 10);
        }

        // --- Drawing ---
        app.clear();

        if (currentState == MENU) {
            app.draw(sMenu);
        }
        else if (currentState == ABOUT) {
            app.draw(sAbout);
        }
        else if (currentState == GAME) {
            app.draw(sBackground);
            app.draw(sPers);
            for (int i = 0; i < 10; i++) {
                sPlat.setPosition(plat[i].x, plat[i].y);
                app.draw(sPlat);
            }
            app.draw(scoreText);
        }

        app.display();
    }

    return 0;
}
