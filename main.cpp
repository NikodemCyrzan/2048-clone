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

#pragma endregion

#pragma region board

int *board = new int[16];
int2 boardBoxSize(12, 6);
int score = 0;

int GetBoardBox(int2 _position)
{
    return board[_position.x + _position.y * 4];
}

void SetBoardBox(int2 _position, int _value)
{
    board[_position.x + _position.y * 4] = _value;
}

int2 IndexToPosition(int _index)
{
    return int2(_index % 4, (int)(_index / 4.0f));
}

bool CanSetRandom()
{
    int counter = 0;
    for (int i = 0; i < 16; i++)
        if (GetBoardBox(IndexToPosition(i)) <= 0)
            counter++;

    return counter > 0;
}

void SetRandomBoardBox()
{
    if (!CanSetRandom())
        return;

    int x = std::rand() % 4;
    int y = std::rand() % 4;
    while (GetBoardBox(int2(x, y)) != 0)
    {
        x = std::rand() % 4;
        y = std::rand() % 4;
    }

    printw("%d", x);
    printw(" ");
    printw("%d", y);

    SetBoardBox(int2(x, y), 2);
}

#pragma endregion

#pragma region screen

int2 screenSize(0, 0);
std::map<int, int> colorsMap;

void DrawBoardBox(int2 _position)
{
    int value = GetBoardBox(_position);

    for (int y = 0; y <= boardBoxSize.y; y++)
        for (int x = 0; x <= boardBoxSize.x; x++)
        {
            int2 gPos(x + _position.x * boardBoxSize.x, y + _position.y * boardBoxSize.y);

            if ((gPos.x % boardBoxSize.x == 0 || gPos.x == 4 * boardBoxSize.x) && gPos.y != 0)
                mvprintw(gPos.y + 1, gPos.x, "|");
            else if (gPos.y % boardBoxSize.y == 0 || gPos.y == 4 * boardBoxSize.y)
                mvprintw(gPos.y + 1, gPos.x, "_");
        }

    int2 textPos(
        _position.x * boardBoxSize.x + boardBoxSize.x / 2 - std::to_string(value).length() / 2,
        _position.y * boardBoxSize.y + boardBoxSize.y / 2 + 1);

    move(textPos.y, textPos.x);
    attron(COLOR_PAIR(colorsMap[value]));
    printw("%d", value);
    attroff(COLOR_PAIR(colorsMap[value]));
}

void DrawView()
{
    for (int i = 0; i < 16; i++)
        DrawBoardBox(IndexToPosition(i));

    move(0, 0);
    printw("Score: ");
    printw("%d", score);
}

void MapColors()
{
    init_pair(1, COLOR_RED, COLOR_BLACK);
    colorsMap[2] = 1;
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    colorsMap[4] = 2;
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    colorsMap[8] = 3;
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    colorsMap[16] = 4;
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    colorsMap[32] = 5;
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    colorsMap[64] = 6;
    init_pair(7, COLOR_BLACK, COLOR_RED);
    colorsMap[128] = 7;
    init_pair(8, COLOR_BLACK, COLOR_GREEN);
    colorsMap[256] = 8;
    init_pair(9, COLOR_BLACK, COLOR_YELLOW);
    colorsMap[512] = 9;
    init_pair(10, COLOR_BLACK, COLOR_BLUE);
    colorsMap[1024] = 10;
    init_pair(11, COLOR_BLACK, COLOR_MAGENTA);
    colorsMap[2048] = 11;
    init_pair(12, COLOR_BLACK, COLOR_CYAN);
    colorsMap[4096] = 12;
}

void GameOver()
{
    int2 pos(
        (2 * boardBoxSize.x) - sizeof("GAME OVER") / 2,
        (2 * boardBoxSize.y));

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

#pragma endregion

#pragma region input

int MoveUp(bool _move = true)
{
    int counter = 0;
    for (int y = 1; y < 4; y++)
        for (int x = 0; x < 4; x++)
        {
            int value = GetBoardBox(int2(x, y));

            int emptyY = -1;
            for (int y2 = y - 1; y2 >= 0; y2--)
                if (GetBoardBox(int2(x, y2)) == 0 || GetBoardBox(int2(x, y2)) == value)
                    emptyY = y2;
                else
                    break;

            if (emptyY != -1)
            {
                if (!_move)
                {
                    counter++;
                    continue;
                }

                if (GetBoardBox(int2(x, emptyY)) > 0)
                    score += GetBoardBox(int2(x, emptyY)) + value;
                SetBoardBox(int2(x, emptyY), GetBoardBox(int2(x, emptyY)) + value);
                SetBoardBox(int2(x, y), 0);
            }
        }
    return counter;
}

int MoveLeft(bool _move = true)
{
    int counter = 0;
    for (int y = 0; y < 4; y++)
        for (int x = 1; x < 4; x++)
        {
            int value = GetBoardBox(int2(x, y));

            int emptyX = -1;
            for (int x2 = x - 1; x2 >= 0; x2--)
                if (GetBoardBox(int2(x2, y)) == 0 || GetBoardBox(int2(x2, y)) == value)
                    emptyX = x2;
                else
                    break;

            if (emptyX != -1)
            {
                if (!_move)
                {
                    counter++;
                    continue;
                }

                if (GetBoardBox(int2(emptyX, y)) > 0)
                    score += GetBoardBox(int2(emptyX, y)) + value;
                SetBoardBox(int2(emptyX, y), GetBoardBox(int2(emptyX, y)) + value);
                SetBoardBox(int2(x, y), 0);
            }
        }
    return counter;
}

int MoveRight(bool _move = true)
{
    int counter = 0;
    for (int y = 0; y < 4; y++)
        for (int x = 2; x >= 0; x--)
        {
            int value = GetBoardBox(int2(x, y));

            int emptyX = -1;
            for (int x2 = x + 1; x2 < 4; x2++)
                if (GetBoardBox(int2(x2, y)) == 0 || GetBoardBox(int2(x2, y)) == value)
                    emptyX = x2;
                else
                    break;

            if (emptyX != -1)
            {
                if (!_move)
                {
                    counter++;
                    continue;
                }

                if (GetBoardBox(int2(emptyX, y)) > 0)
                    score += GetBoardBox(int2(emptyX, y)) + value;
                SetBoardBox(int2(emptyX, y), GetBoardBox(int2(emptyX, y)) + value);
                SetBoardBox(int2(x, y), 0);
            }
        }
    return counter;
}

int MoveDown(bool _move = true)
{
    int counter = 0;
    for (int y = 2; y >= 0; y--)
        for (int x = 0; x < 4; x++)
        {
            int value = GetBoardBox(int2(x, y));

            int emptyY = -1;
            for (int y2 = y + 1; y2 < 4; y2++)
                if (GetBoardBox(int2(x, y2)) == 0 || GetBoardBox(int2(x, y2)) == value)
                    emptyY = y2;
                else
                    break;

            if (emptyY != -1)
            {
                if (!_move)
                {
                    counter++;
                    continue;
                }

                if (GetBoardBox(int2(x, emptyY)) > 0)
                    score += GetBoardBox(int2(x, emptyY)) + value;
                SetBoardBox(int2(x, emptyY), GetBoardBox(int2(x, emptyY)) + value);
                SetBoardBox(int2(x, y), 0);
            }
        }
    return counter;
}

bool InterpretInput(int _number)
{
    bool moved = false;
    switch (_number)
    {
    case 119: // w
        if (MoveUp(false) > 0)
        {
            MoveUp();
            moved = true;
        }
        break;
    case 115: // s
        if (MoveDown(false) > 0)
        {
            MoveDown();
            moved = true;
        }
        break;
    case 97: // a
        if (MoveLeft(false) > 0)
        {
            MoveLeft();
            moved = true;
        }
        break;
    case 100: // d
        if (MoveRight(false) > 0)
        {
            MoveRight();
            moved = true;
        }
        break;
    default:
        break;
    }
    return moved;
}

bool CanMoveInAnyDirection()
{
    return MoveUp(false) + MoveLeft(false) + MoveRight(false) + MoveDown(false) > 0;
}

#pragma endregion

void Init()
{
    srand(time(0));
    initscr();
    keypad(stdscr, true);
    getmaxyx(stdscr, screenSize.y, screenSize.x);
    curs_set(0);
    start_color();
    noecho();
}

int main()
{
    Init();

    for (int i = 0; i < 16; i++)
        SetBoardBox(IndexToPosition(i), 0);

    SetRandomBoardBox();
    MapColors();
    DrawView();

    // game loop
    while (1)
    {
        if (!CanMoveInAnyDirection())
        {
            GameOver();
            while (1)
            {
                int num = getch();
                if (num == 113) // q
                {
                    endwin();
                    return 0;
                }
            }
        }
        int input = getch();
        if (InterpretInput(input))
        {
            SetRandomBoardBox();
            clear();
            DrawView();
        }
    }

    endwin();
    return 0;
}