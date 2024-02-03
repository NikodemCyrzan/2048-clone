#include <iostream>
#include <ncursesw/curses.h>
#include <cmath>
#include <map>
#include <cstdlib>
#include <ctime>

#pragma region math

class float2
{
public:
    float x;
    float y;

    float2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
};

class int2
{
public:
    int x;
    int y;

    int2(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
};

enum Direction
{
    north,
    east,
    west,
    south
};

#pragma endregion

int2 screenSize(0, 0);

class Screen
{
private:
    std::map<int, int> colorsMap;
    int mapIndex = 1;
    int2 *boardBoxSize = new int2(12, 6);

    void MapColor(int number, int numberColor, int backgroundColor)
    {
        init_pair(mapIndex, numberColor, backgroundColor);
        colorsMap[number] = mapIndex;
        mapIndex++;
    }

    void MapColors()
    {
        MapColor(2, COLOR_RED, COLOR_BLACK);
        MapColor(4, COLOR_GREEN, COLOR_BLACK);
        MapColor(8, COLOR_YELLOW, COLOR_BLACK);
        MapColor(16, COLOR_BLUE, COLOR_BLACK);
        MapColor(32, COLOR_MAGENTA, COLOR_BLACK);
        MapColor(64, COLOR_CYAN, COLOR_BLACK);
        MapColor(128, COLOR_BLACK, COLOR_RED);
        MapColor(256, COLOR_BLACK, COLOR_GREEN);
        MapColor(512, COLOR_BLACK, COLOR_YELLOW);
        MapColor(1024, COLOR_BLACK, COLOR_MAGENTA);
        MapColor(2048, COLOR_BLACK, COLOR_MAGENTA);
        MapColor(4096, COLOR_BLACK, COLOR_CYAN);
    }

public:
    Screen()
    {
        MapColors();
    }

    ~Screen()
    {
        delete boardBoxSize;
    }

    void DrawBox(int2 _position, int _value)
    {
        for (int y = 0; y <= boardBoxSize->y; y++)
            for (int x = 0; x <= boardBoxSize->x; x++)
            {
                int2 gPos(x + _position.x * boardBoxSize->x, y + _position.y * boardBoxSize->y);

                if ((gPos.x % boardBoxSize->x == 0 || gPos.x == 4 * boardBoxSize->x) && gPos.y != 0)
                    mvprintw(gPos.y + 1, gPos.x, "|");
                else if (gPos.y % boardBoxSize->y == 0 || gPos.y == 4 * boardBoxSize->y)
                    mvprintw(gPos.y + 1, gPos.x, "_");
            }

        int2 textPos(
            _position.x * boardBoxSize->x + boardBoxSize->x / 2 - std::to_string(_value).length() / 2,
            _position.y * boardBoxSize->y + boardBoxSize->y / 2 + 1);

        move(textPos.y, textPos.x);
        attron(COLOR_PAIR(colorsMap[_value]));
        printw("%d", _value);
        attroff(COLOR_PAIR(colorsMap[_value]));
    }
};

class Board
{
private:
    int *board = new int[16];
    Screen *screen = new Screen();

    bool CanSetRandom()
    {
        int counter = 0;
        for (int i = 0; i < 16; i++)
            if (GetBox(IndexToPosition(i)) <= 0)
                counter++;

        return counter > 0;
    }

    int MoveNorth(bool _move = true)
    {
        int counter = 0;
        for (int y = 1; y < 4; y++)
            for (int x = 0; x < 4; x++)
            {
                int value = GetBox(int2(x, y));

                int emptyY = -1;
                for (int y2 = y - 1; y2 >= 0; y2--)
                    if (GetBox(int2(x, y2)) == 0 || GetBox(int2(x, y2)) == value)
                        emptyY = y2;
                    else
                        break;

                if (emptyY != -1)
                {
                    counter++;
                    if (!_move)
                        continue;

                    if (GetBox(int2(x, emptyY)) > 0)
                        score += GetBox(int2(x, emptyY)) + value;
                    SetBox(int2(x, emptyY), GetBox(int2(x, emptyY)) + value);
                    SetBox(int2(x, y), 0);
                }
            }
        return counter;
    }

    int MoveEast(bool _move = true)
    {
        int counter = 0;
        for (int y = 0; y < 4; y++)
            for (int x = 2; x >= 0; x--)
            {
                int value = GetBox(int2(x, y));

                int emptyX = -1;
                for (int x2 = x + 1; x2 < 4; x2++)
                    if (GetBox(int2(x2, y)) == 0 || GetBox(int2(x2, y)) == value)
                        emptyX = x2;
                    else
                        break;

                if (emptyX != -1)
                {
                    counter++;
                    if (!_move)
                        continue;

                    if (GetBox(int2(emptyX, y)) > 0)
                        score += GetBox(int2(emptyX, y)) + value;
                    SetBox(int2(emptyX, y), GetBox(int2(emptyX, y)) + value);
                    SetBox(int2(x, y), 0);
                }
            }
        return counter;
    }

    int MoveWest(bool _move = true)
    {
        int counter = 0;
        for (int y = 0; y < 4; y++)
            for (int x = 1; x < 4; x++)
            {
                int value = GetBox(int2(x, y));

                int emptyX = -1;
                for (int x2 = x - 1; x2 >= 0; x2--)
                    if (GetBox(int2(x2, y)) == 0 || GetBox(int2(x2, y)) == value)
                        emptyX = x2;
                    else
                        break;

                if (emptyX != -1)
                {
                    counter++;
                    if (!_move)
                        continue;

                    if (GetBox(int2(emptyX, y)) > 0)
                        score += GetBox(int2(emptyX, y)) + value;
                    SetBox(int2(emptyX, y), GetBox(int2(emptyX, y)) + value);
                    SetBox(int2(x, y), 0);
                }
            }
        return counter;
    }

    int MoveSouth(bool _move = true)
    {
        int counter = 0;
        for (int y = 2; y >= 0; y--)
            for (int x = 0; x < 4; x++)
            {
                int value = GetBox(int2(x, y));

                int emptyY = -1;
                for (int y2 = y + 1; y2 < 4; y2++)
                    if (GetBox(int2(x, y2)) == 0 || GetBox(int2(x, y2)) == value)
                        emptyY = y2;
                    else
                        break;

                if (emptyY != -1)
                {
                    counter++;
                    if (!_move)
                        continue;

                    if (GetBox(int2(x, emptyY)) > 0)
                        score += GetBox(int2(x, emptyY)) + value;
                    SetBox(int2(x, emptyY), GetBox(int2(x, emptyY)) + value);
                    SetBox(int2(x, y), 0);
                }
            }
        return counter;
    }

public:
    int score = 0;

    Board()
    {
        for (int i = 0; i < 16; i++)
            SetBox(IndexToPosition(i), 0);

        SetRandomBox(2);
    }

    ~Board()
    {
        delete board;
        delete screen;
    }

    int GetBox(int2 _position)
    {
        return board[PositionToIndex(_position)];
    }

    void SetBox(int2 _position, int _value)
    {
        board[PositionToIndex(_position)] = _value;
    }

    int2 IndexToPosition(int _index)
    {
        return int2(_index % 4, (int)(_index / 4.0f));
    }

    int PositionToIndex(int2 _position)
    {
        return _position.x + _position.y * 4;
    }

    void SetRandomBox(int _value)
    {
        if (!CanSetRandom())
            return;

        int x = std::rand() % 4;
        int y = std::rand() % 4;
        while (GetBox(int2(x, y)) != 0)
        {
            x = std::rand() % 4;
            y = std::rand() % 4;
        }

        printw("%d", x);
        printw(" ");
        printw("%d", y);

        SetBox(int2(x, y), _value);
    }

    void Draw()
    {
        clear();
        for (int i = 0; i < 16; i++)
        {
            int2 position = IndexToPosition(i);
            screen->DrawBox(position, GetBox(position));
        }

        move(0, 0);
        printw("Score: ");
        printw("%d", score);
    }

    bool CanMoveInAnyDirection()
    {
        return MoveNorth(false) + MoveWest(false) + MoveEast(false) + MoveSouth(false) > 0;
    }

    bool Move(Direction _direction)
    {
        if (!CanMoveInAnyDirection())
            return false;

        switch (_direction)
        {
        case north:
            return MoveNorth();
        case east:
            return MoveEast();
        case west:
            return MoveWest();
        case south:
            return MoveSouth();
        default:
            return false;
        }
    }
};

Board *board;

void InitNcurses()
{
    srand(time(0));
    initscr();
    keypad(stdscr, true);
    getmaxyx(stdscr, screenSize.y, screenSize.x);
    curs_set(0);
    start_color();
    noecho();
}

bool InterpretInput(int _number)
{
    bool moved = false;
    switch (_number)
    {
    case 119: // w
        moved = board->Move(north);
        break;
    case 115: // s
        moved = board->Move(south);
        break;
    case 97: // a
        moved = board->Move(west);
        break;
    case 100: // d
        moved = board->Move(east);
        break;
    default:
        break;
    }
    return moved;
}

void GameOver()
{
    int2 pos(
        (2 * 12) - sizeof("GAME OVER") / 2,
        (2 * 6));

    attron(COLOR_PAIR(7));
    for (int y = 0; y < 3; y++)
        for (int x = 0; x <= sizeof("GAME OVER"); x++)
        {
            move(y + pos.y, x + pos.x);
            printw(" ");
        }
    attroff(COLOR_PAIR(7));

    move(pos.y + 1, pos.x + 1);
    attron(COLOR_PAIR(7));
    printw("GAME OVER");
    attroff(COLOR_PAIR(7));
}

int main()
{
    InitNcurses();
    board = new Board();
    board->Draw();

    // game loop
    while (1)
    {
        if (!board->CanMoveInAnyDirection())
        {
            GameOver();
            while (1)
            {
                int num = getch();
                if (num == 113) // q
                {
                    endwin();
                    delete board;
                    return 0;
                }
            }
        }

        int input = getch();
        if (InterpretInput(input))
        {
            board->SetRandomBox(2);
            board->Draw();
        }
    }
}