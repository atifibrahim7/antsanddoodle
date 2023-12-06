#pragma once
#include <random>
#include<iostream>
#include <fstream>
#include"SFML/System.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "menu.h"
using namespace sf;
using namespace std;
string title = "Sukodu Simulator";
class Game {

public: 
    
    Sprite gridsprite , bg ;
    Texture gridTexture , bgtex;
    Menu menu;      //menu class 
    const int BOARD_SIZE = 9;
    int** grid;         // has the grid 2d array
    int** gridsolution;  // for saving grid solution
    Game()
    {
        if (!bgtex.loadFromFile("img/bg.png"))          //loading grid as a background
            cout << "Error loading bg.png";
        bg.setTexture(bgtex);
        grid = new int* [BOARD_SIZE];       // initializing grid 
        for (int i = 0; i < BOARD_SIZE; ++i) {
           grid[i] = new int[BOARD_SIZE];
        }
        if (!gridTexture.loadFromFile("img/grid.png"))          //loading grid as a background
            cout << "Error loading grid.png";
        gridsprite.setTexture(gridTexture);
        //gridsprite.setPosition(40, 20);
        gridsprite.scale(1.1f,1.1f);

        gridsolution = new int* [BOARD_SIZE];
        for (int i = 0; i < BOARD_SIZE; ++i) {
            gridsolution[i] = new int[BOARD_SIZE];
        }

    }
    void start_game()               //main game start fucntion which contains the game loop
    {   

        srand(time(0));
        bool flag = 0;
        int option = 0;
        menu.display_menu(flag , option);           // calls menu window before starting game loop

        if (flag == 0)return;  // game end          

        RenderWindow window(VideoMode(1300, 850), title);
        window.setFramerateLimit(60);
        Clock clock, clock1;
        float timer = 0;
        Music gameplay;
        Font FONTS;
        FONTS.loadFromFile("img/ss.otf");
        // for showing text on screen
        int mistakes = 0;
        Text MistakesText;
        MistakesText.setFont(FONTS);
        MistakesText.setCharacterSize(40);
        MistakesText.setPosition(500, 800);
        MistakesText.setFillColor(sf::Color(255, 255, 255, 130));
        // show time on screen 
        Text Time ;
        Time.setFont(FONTS);
        Time.setCharacterSize(80);
        Time.setPosition(1030, 370);
        Time.setFillColor(sf::Color(255, 255, 255, 130));
        Music gameplay_music;
        if (!gameplay_music.openFromFile("img/music.wav"))
        gameplay_music.setVolume(50); // set the volume to 50%
        gameplay_music.setLoop(true);
        gameplay_music.play();

        
        string filename;
        bool inputflag = false;
        // check which option is selected from menu class
        switch (option)
        {case 1 : 
            filename = "board1.txt"; break;
        case 2 :
            filename = "board2.txt"; break;
        case 3:
            filename = "board3.txt"; break;
        default:
            inputflag = true;
            break;
        }
        //input flag as false means user selected one of three default maps  
        if (inputflag == false)
        {

            loadBoardFromFile(filename);                // loading map from a .txt file
            //Copy the same board to the solution : Note this method is for File Reading and not input via screen 
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                for (int j = 0; j < BOARD_SIZE; j++)
                {
                    gridsolution[i][j] = grid[i][j];
                }
            }
            solveSudokuSolution();          // populates out gridsolution array 
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                for (int j = 0; j < BOARD_SIZE; j++)
                {
                    cout << gridsolution[i][j] << " ";
                }
                cout << endl;
            }
        
        }
        // Section A
        // This part of code allows you to enter the grid on the screen and save it in 2d grid array !
        // ENter Grid user entered grid : 
        std::string Boardstr;

        if (inputflag)
        {

            bool filesaved = 0;
            Text enterNameText;
            enterNameText.setFont(FONTS);
            enterNameText.setCharacterSize(50);
            enterNameText.setFillColor(Color::Yellow);
            enterNameText.setString("Enter Your Board:");
            enterNameText.setPosition(100, 100); // Adjust the position as needed

            Text nameText;
            nameText.setFont(FONTS);
            nameText.setCharacterSize(50);
            nameText.setFillColor(Color::Yellow);
            nameText.setPosition(200, 200); // Adjust the position as needed

            int counter = 0;


            srand(time(0));
            bool enteringBoard = true;
            sf::Event event;

            while (enteringBoard && window.isOpen()) {
                window.clear();
                window.draw(enterNameText);
                nameText.setString(Boardstr);
                window.draw(nameText);
                window.display();

                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        enteringBoard = false;
                    }
                    else if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode < 128) {
                            if (event.text.unicode == '\b' && !Boardstr.empty()) {
                                // Handle backspace
                                Boardstr.pop_back();
                                counter--;
                            }
                            else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                                // Handle enter key
                                enteringBoard = false;
                            }
                            else {
                                // Add the entered character to the name string
                                Boardstr += static_cast<char>(event.text.unicode);
                                counter++;
                                if (counter == 9) {
                                    Boardstr += '\n';
                                    counter = 0;
                                }
                            }
                        }
                    }
                }
            }

        //    cout << "Board string " << Boardstr;
            removeNewlines(Boardstr);
          //  cout << "\n";
           // cout << "Board string " << Boardstr;

            // load from board string to grid 
            // Now convert the entered string to a 9x9 2D array
            loadBoardFromGrid(Boardstr);
          
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                for (int j = 0; j < BOARD_SIZE; j++)
                {
                    gridsolution[i][j] = grid[i][j];
                }
            }
            solveSudokuSolution();


        }

       

        // game loop 
        while (window.isOpen())
        {
            float time = clock.getElapsedTime().asSeconds();


            clock.restart();
            timer += time;
            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed) // If cross/close is clicked/pressed
                    window.close(); //close the game 
                if (e.type == sf::Event::KeyPressed)
                {
                    if (e.key.code == sf::Keyboard::Num0)
                    {
                       
                    Reset : 
                        if (option == 4)
                        {   
                            timer = 0;
                            loadBoardFromGrid(Boardstr);

                        }
                        else {
                        loadBoardFromFile(filename);
                        mistakes = 0; 
                        timer = 0;
                        }
                    }
                    else if (e.key.code >= sf::Keyboard::Num1 && e.key.code <= sf::Keyboard::Num9)   // SFML logic to take input on screen
                    {
                       // cout << "key pressed";
                        // Input a number (1-9) at the current position (0-based)
                        int inputNumber = e.key.code - sf::Keyboard::Num1 + 1;

                        int selectedRow, selectedCol;
                        if (findEmptyLocation(selectedRow, selectedCol) && isSafe(selectedRow, selectedCol, inputNumber))
                        {
                            // If the cell is empty and the input is safe, place the number only if it matches the input 
                            if (gridsolution[selectedRow][selectedCol] == inputNumber)
                            {
                            grid[selectedRow][selectedCol] = inputNumber;
                            }
                            else
                            {
                                mistakes++;
                                grid[selectedRow][selectedCol] = 0;
                            }
                        }
                        else {
                            mistakes++;
                        }
                    }
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
            // This part of code deals with mouse poitner inputs on the screen . This may change if youre using  operating system other than windows

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)         //sfml mouse input logic
            {
                Vector2i mousePos = Mouse::getPosition(window);
                //cout << mousePos.x << " " << mousePos.y<<endl;

                if (mousePos.x >= 790 && mousePos.x <= 900 && mousePos.y >= 270 && mousePos.y <= 380)
                {
                    solveSudoku();
                }
                if (mousePos.x >= 794 && mousePos.x <= 877 && mousePos.y >= 400 && mousePos.y <= 515)
                {
                    if (option != 4) {
                        loadBoardFromFile(filename);
                        timer = 0;
                        mistakes = 0;
                    }
                    else {
                        loadBoardFromGrid(Boardstr);
                        timer = 0;
                        mistakes = 0;
                    }
                }
            }
            
          
            // Below we clear screen , draw sprites and the just window display
            
            window.clear(sf::Color::White);
            window.draw(bg);
            window.draw(gridsprite);
           MistakesText.setString("MISTAKES :" + to_string(mistakes) + "/ 5");
            window.draw(MistakesText);
            Time.setString(to_string(int(timer) ));
            window.draw(Time);
            sf::Text textongrid;
            textongrid.setFont(FONTS);
            textongrid.setCharacterSize(40);
            textongrid.setFillColor(sf::Color::Black);

            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    // Convert int to string for drawing
                     if (grid[i][j] == 0) textongrid.setString(" ");
                    else 
                    textongrid.setString(std::to_string(grid[i][j]));

                    // Set position based on grid position
                    textongrid.setPosition(j * 70.0f + 130 , i * 70.0f + 120);

                    // Draw the text
                    window.draw(textongrid);
                }
            }
            // if mistakes are greater than five print a lost message and reset
            if (mistakes >= 5)
            {
                cout << "You Lost" << endl;
                mistakes = 0;
                timer = 0;
                goto Reset;
            }
            window.display();
        }
    }

    void loadBoardFromFile(const std::string& filename) {
        std::fstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (file >> grid[i][j]) {
                    // Successfully read a value from the file
                }
                else {
                    std::cerr << "Error reading value from file.\n";
                    return;
                }
            }
        }

        file.close();
    }


    // Function to print the Sudoku grid
    void printGrid() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++)
                cout << grid[i][j] << " ";
            cout << endl;
        }
    }

    // Function to check if a number can be placed in the given cell
    bool isSafe(int row, int col, int num) {
        // Check if the number is present in the row or column
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (grid[row][i] == num || grid[i][col] == num) {
                return false;
            }
        }

        // Check within the 3x3 subgrid
        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[i + startRow][j + startCol] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    // Function to find an empty cell in the Sudoku grid
    bool findEmptyLocation(int& row, int& col) {
        for (row = 0; row < BOARD_SIZE; row++) {
            for (col = 0; col < BOARD_SIZE; col++) {
                if (grid[row][col] == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    // Function to solve the Sudoku puzzle using backtracking
    bool solveSudoku() {
        int row, col;

        if (!findEmptyLocation(row, col)) {
            return true;
        }

        for (int num = 1; num <= 9; num++) {
            if (isSafe(row, col, num)) {
                grid[row][col] = num;

                if (solveSudoku()) {
                    return true;
                }

                grid[row][col] = 0;
            }
        }

        return false;
    }

    // For filling the solution of grid


    bool isSafeSolution(int row, int col, int num) {
        // Check if the number is present in the row or column
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (gridsolution[row][i] == num || gridsolution[i][col] == num) {
                return false;
            }
        }

        // Check within the 3x3 subgrid
        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (gridsolution[i + startRow][j + startCol] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    // Function to find an empty cell in the Sudoku grid
    bool findEmptyLocationSolution(int& row, int& col) {
        for (row = 0; row < BOARD_SIZE; row++) {
            for (col = 0; col < BOARD_SIZE; col++) {
                if (gridsolution[row][col] == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    // Function to solve the Sudoku puzzle using backtracking
    bool solveSudokuSolution() {
        int row, col;

        if (!findEmptyLocationSolution(row, col)) {
            return true;
        }

        for (int num = 1; num <= 9; num++) {
            if (isSafeSolution(row, col, num)) {
                gridsolution[row][col] = num;

                if (solveSudokuSolution()) {
                    return true;
                }

                gridsolution[row][col] = 0;
            }
        }

        return false;
    }
    ~Game() {
        // Deallocate memory for the Sudoku board
        for (int i = 0; i < BOARD_SIZE; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
    }
    void removeNewlines(std::string& str) {
        size_t pos = str.find('\n');
        while (pos != std::string::npos) {
            str.erase(pos, 1);  // Erase one character at the found position
            pos = str.find('\n', pos);  // Find the next occurrence
        }
    }

    void loadBoardFromGrid(string& Boardstr)
    {
          size_t index = 0;

            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    // Convert character to integer
                    int value = Boardstr[index] - '0';

                    // Store the value in the grid
                    grid[i][j] = value;

                    // Move to the next character in the input string
                    index++;
                }
            }
    }
};

