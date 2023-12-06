#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;
using namespace sf;

class Menu {
public:

    Sprite menu, instruction;
    Texture menu_texture, instruction_tex;

    // Add menu attributes here
    Menu()
    {
        menu_texture.loadFromFile("img/menu.png");
          instruction_tex.loadFromFile("img/inst.png");
          instruction.setTexture(instruction_tex);
        menu.setTexture(menu_texture);
        //menu.setScale(0.5, 0.5);
      //  instruction.setScale(1, 0.5);
        //  instruction.setPosition(0, 50);

          // Constructor's body
    }
    void display_menu(bool& flag , int& option)
    {
        // Create a window to display the menu
        RenderWindow window(VideoMode(1350, 850), "Sukodu Menu");

        // Create a text object for the overview
        sf::Text overview;
        Font font;
        font.loadFromFile("img/SS.otf");
        bool difficultflag = false;
        bool insFLAG = false;
        while (window.isOpen())
        {
            // Handle events
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << " " << mousePos.y << endl;
                    if (mousePos.x >= 490 && mousePos.x <= 810 && mousePos.y >= 165 && mousePos.y <= 305)
                    {
                        cout << "Input";
                        //~Game();
                        option = 4;
                        flag = 1;
                        window.close();
                    }
                    if (mousePos.x >= 550 && mousePos.x <= 780 && mousePos.y >= 660 && mousePos.y <= 760)
                    {
                        cout << "exit";
                        //~Game();
                        flag = 0;
                        window.close();
                    }
                    if (mousePos.x >= 550 && mousePos.x <= 780 && mousePos.y >= 320 && mousePos.y <= 470)
                    {
                        cout << "Play with difficulty map ";
                        //~Game();
                        flag = 0;
                        difficultflag = !difficultflag;
                        if(difficultflag)
                            menu_texture.loadFromFile("img/menu2.png");
                        else {
                            menu_texture.loadFromFile("img/menu.png");

                        }
                    }
                    if (mousePos.x >= 855 && mousePos.x <= 1035 && mousePos.y >= 270 && mousePos.y <= 345)
                    {
                        cout << "Play with Easy map ";
                        option = 1;
                        flag = 1;
                        window.close();
                    }
                    if (mousePos.x >= 855 && mousePos.x <= 1035 && mousePos.y >= 360 && mousePos.y <= 440)
                    {
                        cout << "Play with Moderate map ";
                        option = 2;
                        flag = 1;
                        window.close();

                    }
                    if (mousePos.x >= 855 && mousePos.x <= 1035 && mousePos.y >= 455 && mousePos.y <= 530)
                    {
                        cout << "Play with Difficult map ";
                        option = 3;
                        flag = 1;
                        window.close();

                    }
                    if (mousePos.x >= 500 && mousePos.x <= 820 && mousePos.y >= 500 && mousePos.y <= 635)
                    {
                        cout << "Instructions";
                        insFLAG = true;

                    }
                    if (mousePos.x >= 28 && mousePos.x <= 271 && mousePos.y >= 30 && mousePos.y <= 138)
                    {
                        cout << "Menu back";
                        insFLAG = false;

                    }
                }
            }
            if (insFLAG)
            {
                window.clear();
                window.draw(instruction);
                window.display();
            }
            else
            {
            // Draw the menu sprite
            window.clear();
            window.draw(menu);
            window.display();
            }
        }
    }


};
