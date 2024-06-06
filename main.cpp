#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const int ROWS = 6;
const int COLS = 7;
const float CELL_SIZE = 100.f;
const int WINNING_COUNT = 4;

enum Player { NONE = 0, PLAYER1, PLAYER2 };

class ConnectFour {
public:
    ConnectFour();
    void run();

private:
    void processEvents();
    void render();
    void handlePlayerInput(sf::Vector2i mousePos);
    int dropDisc(int column);
    bool checkWin(int row, int col, Player player);
    bool checkDirection(int startX, int startY, int deltaX, int deltaY, Player player);
    bool isBoardFull();

    sf::RenderWindow mWindow;
    sf::Font mFont;
    sf::Text mVictoryText;
    std::vector<std::vector<Player>> mGrid;
    Player mCurrentPlayer;
    bool mIsGameOver;
    std::string mWinnerText;
};

ConnectFour::ConnectFour()
    : mWindow(sf::VideoMode(COLS * CELL_SIZE, ROWS * CELL_SIZE), "Connect Four"),
      mGrid(ROWS, std::vector<Player>(COLS, NONE)),
      mCurrentPlayer(PLAYER1),
      mIsGameOver(false) {
    if (!mFont.loadFromFile("vinque rg.otf")) {
        std::cerr << "Failed to load font\n";
    }
    mVictoryText.setFont(mFont);
    mVictoryText.setCharacterSize(50);
    mVictoryText.setFillColor(sf::Color::Blue);
    mVictoryText.setPosition(COLS * CELL_SIZE / 2 - 150, ROWS * CELL_SIZE / 2 - 50);

}

void ConnectFour::run() {
    while (mWindow.isOpen()) {
        processEvents();
        render();
    }
}

void ConnectFour::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        if (event.type == sf::Event::MouseButtonPressed && !mIsGameOver) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handlePlayerInput(sf::Mouse::getPosition(mWindow));
            }
        }
    }
}


void ConnectFour::render() {
    mWindow.clear();
    
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 5, CELL_SIZE - 5));
            cell.setPosition(col * CELL_SIZE + 2.5f, row * CELL_SIZE + 2.5f);

            if (mGrid[row][col] == PLAYER1)
                cell.setFillColor(sf::Color::Red);
            else if (mGrid[row][col] == PLAYER2)
                cell.setFillColor(sf::Color::Yellow);
            else
                cell.setFillColor(sf::Color::White);

            mWindow.draw(cell);
        }
    }

    if (mIsGameOver) {
        mVictoryText.setString(mWinnerText);
        mWindow.draw(mVictoryText);
    }

    mWindow.display();
}

void ConnectFour::handlePlayerInput(sf::Vector2i mousePos) {
    if (mIsGameOver) return;

    int column = mousePos.x / CELL_SIZE;
    if (column >= 0 && column < COLS) {
        int row = dropDisc(column);
        if (row != -1) {
            if (checkWin(row, column, mCurrentPlayer)) {
                mIsGameOver = true;
                mWinnerText = (mCurrentPlayer == PLAYER1) ? "Player 1 Wins!" : "Player 2 Wins!";
            } else if (isBoardFull()) {
                mIsGameOver = true;
                mWinnerText = "It's a Draw!";
            } else {
                mCurrentPlayer = (mCurrentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
            }
        }
    }
}

int ConnectFour::dropDisc(int column) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (mGrid[row][column] == NONE) {
            mGrid[row][column] = mCurrentPlayer;
            return row;
        }
    }
    return -1;
}

bool ConnectFour::checkWin(int row, int col, Player player) {
    return checkDirection(row, col, 1, 0, player) ||
           checkDirection(row, col, 0, 1, player) ||
           checkDirection(row, col, 1, 1, player) ||
           checkDirection(row, col, 1, -1, player);
}

bool ConnectFour::checkDirection(int startX, int startY, int deltaX, int deltaY, Player player) {
    int count = 1;
    int currentX = startX + deltaX;
    int currentY = startY + deltaY;

    while (currentX >= 0 && currentX < ROWS && currentY >= 0 && currentY < COLS &&
           mGrid[currentX][currentY] == player) {
        count++;
        currentX += deltaX;
        currentY += deltaY;
    }

    currentX = startX - deltaX;
    currentY = startY - deltaY;

    while (currentX >= 0 && currentX < ROWS && currentY >= 0 && currentY < COLS &&
           mGrid[currentX][currentY] == player) {
        count++;
        currentX -= deltaX;
        currentY -= deltaY;
    }

    return count >= WINNING_COUNT;
}



bool ConnectFour::isBoardFull() {
    for (int col = 0; col < COLS; ++col) {
        if (mGrid[0][col] == NONE) {
            return false;
        }
    }
    return true;
}

int main() {
    ConnectFour game;
    game.run();
    return 0;
}
