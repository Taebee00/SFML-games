#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <time.h>
#include <string>
#include <vector>

using namespace sf;
using namespace std;

#define cellsz 20
#define width 30
#define height 30

int stage = 3;
int period = 0;
int speed = 5;
int level = 1;
bool invisible = 0;

array<string, 21> rmaps{
   " ################### ",
   " #........#........# ",
   " #.##.###.#.###.##.# ",
   " #.................# ",
   " #.##.#.#.#.#.#.##.# ",
   " #....#...#...#....# ",
   " ####.###.#.###.#### ",
   "    #.#.......#.#    ",
   "#####.#.##.##.#.#####",
   "#   #...........#   #",
   "#####.#.##.##.#.#####",
   "    #.#.......#.#    ",
   " ####.#.#.#.#.#.#### ",
   " #........#........# ",
   " #.##.###.#.###.##.# ",
   " #o.#...........#..# ",
   " ##.#.#.#.#.#.#.#.## ",
   " #....#.......#....# ",
   " #.######.#.######.# ",
   " #.................# ",
   " ################### "
};

array<string, 21> maps = rmaps;

class pacman
{
public:
    int period = 0;
    int atedots = 0;
    CircleShape pacmanshape;
    Vector2f position;
    pacman(float sz, Vector2f pos)
    {
        pacmanshape = CircleShape(sz);
        pacmanshape.setPosition(pos.x, pos.y);
        pacmanshape.setFillColor(Color::Yellow);
    }
    void moveleft()
    {
        if (maps[(int)pacmanshape.getPosition().y / 20][(int)pacmanshape.getPosition().x / 20 - 1] == '#')
        {
            return;
        }
        pacmanshape.move(-20, 0);
    }
    void moveright()
    {
        if (maps[(int)pacmanshape.getPosition().y / 20][(int)pacmanshape.getPosition().x / 20 + 1] == '#')
        {
            return;
        }
        pacmanshape.move(20, 0);
    }
    void moveup()
    {
        if (maps[(int)pacmanshape.getPosition().y / 20 - 1][(int)pacmanshape.getPosition().x / 20] == '#')
        {
            return;
        }
        pacmanshape.move(0, -20);
    }
    void movedown()
    {
        if (maps[(int)pacmanshape.getPosition().y / 20 + 1][(int)pacmanshape.getPosition().x / 20] == '#')
        {
            return;
        }
        pacmanshape.move(0, 20);
    }
    void draw(RenderWindow& _window)
    {
        _window.draw(pacmanshape);
    }
    void mapupdate()
    {
        if (maps[(int)pacmanshape.getPosition().y / 20][(int)pacmanshape.getPosition().x / 20] == '.')
        {
            maps[(int)pacmanshape.getPosition().y / 20][(int)pacmanshape.getPosition().x / 20] = ' ';
            atedots++;
        }
        if (maps[(int)pacmanshape.getPosition().y / 20][(int)pacmanshape.getPosition().x / 20] == 'o')
        {
            invisible = 1;
            maps[(int)pacmanshape.getPosition().y / 20][(int)pacmanshape.getPosition().x / 20] = ' ';
        }
        if (atedots == 190)
        {
            cout << "STAGE " << stage << " CLEAR!" << endl;
            stage++;
            maps = rmaps;
            pacmanshape.setPosition(200, 200);
            level = 1;
            atedots = 0;
        }
        if (invisible == 1)
        {
            period++;
            pacmanshape.setFillColor(Color::Cyan);
            if (period % 500 == 0)
            {
                invisible = 0;
                pacmanshape.setFillColor(Color::Yellow);
            }
        }
    }
};

class ghost
{
public:
    Texture ghosttext;
    Sprite ghostsprite;
    int dir;
    ghost(string _string, Vector2f _pos)
    {
        ghosttext.loadFromFile(_string);
        ghostsprite.setTexture(ghosttext);
        ghostsprite.setPosition(_pos);
        srand((unsigned)time(NULL));
    };
    void moveleft()
    {
        if (maps[(int)ghostsprite.getPosition().y / 20][(int)ghostsprite.getPosition().x / 20 - 1] == '#')
        {
            return;
        }
        ghostsprite.move(-20, 0);
    }
    void moveright()
    {
        if (maps[(int)ghostsprite.getPosition().y / 20][(int)ghostsprite.getPosition().x / 20 + 1] == '#')
        {
            return;
        }
        ghostsprite.move(20, 0);
    }
    void moveup()
    {
        if (maps[(int)ghostsprite.getPosition().y / 20 - 1][(int)ghostsprite.getPosition().x / 20] == '#')
        {
            return;
        }
        ghostsprite.move(0, -20);
    }
    void movedown()
    {
        if (maps[(int)ghostsprite.getPosition().y / 20 + 1][(int)ghostsprite.getPosition().x / 20] == '#')
        {
            return;
        }
        ghostsprite.move(0, 20);
    }
    void randmove()
    {
        if (period % (21 - (2 * level)) == 0)
        {
            dir = rand() % 4;
            if (dir == 0)
            {
                moveleft();
            }
            else if (dir == 1)
            {
                moveright();
            }
            else if (dir == 2)
            {
                moveup();
            }
            else if (dir == 3)
            {
                movedown();
            }

        }
    }
};

class ghosts
{
public:
    vector <ghost> ghostarr;
    ghosts()
    {
        ghost ghost0("Ghost0.png", Vector2f(180, 140));
        ghostarr.push_back(ghost0);
        ghost ghost1("Ghost1.png", Vector2f(200, 140));
        ghostarr.push_back(ghost1);
        ghost ghost2("Ghost2.png", Vector2f(220, 140));
        ghostarr.push_back(ghost2);
    }
    void draw(RenderWindow& _window)
    {
        for (int i = 0; i < stage; i++)
        {
            _window.draw(ghostarr[i].ghostsprite);
        }
    }
    void randmove()
    {
        for (int i = 0; i < stage; i++)
        {
            ghostarr[i].randmove();
        }
    }
    void crash(pacman _pacman, RenderWindow& _window)
    {
        if (invisible == 0)
        {
            for (int i = 0; i < stage; i++)
            {
                if (_pacman.pacmanshape.getGlobalBounds().intersects(ghostarr[i].ghostsprite.getGlobalBounds()))
                {
                    cout << "STAGE " << stage << " FAILED" << endl;
                    _pacman.pacmanshape.setFillColor(Color::Red);
                    _window.close();
                }
            }
        }
    }
};


int main()
{
    // 팩맨 만들기
    pacman pac(10, Vector2f(200, 220));
    // 맵 만들기
    RectangleShape walls(Vector2f(cellsz, cellsz));
    // 점 만들기
    CircleShape dots(2);
    // 투명점 만들기
    CircleShape bigdot(5);
    // 고스트 만들기
    ghosts ghost;
    RenderWindow window(VideoMode(cellsz * 21, cellsz * 21), "PacmanGame");
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        Event event;
        {
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }
                else if (event.type == Event::KeyPressed)
                {
                    if (event.key.code == Keyboard::Left)
                    {
                        pac.moveleft();
                    }
                    else if (event.key.code == Keyboard::Right)
                    {
                        pac.moveright();
                    }
                    else if (event.key.code == Keyboard::Up)
                    {
                        pac.moveup();
                    }
                    else if (event.key.code == Keyboard::Down)
                    {
                        pac.movedown();
                    }
                }
            }
            period++;
            if (period % 500 == 0)
            {
                if (level < 10)
                {
                    level++;
                }
                cout << "LEVEL " << level << endl;
            }
            for (int i = 0; i < 21; i++)
            {
                for (int j = 0; j < 21; j++)
                {
                    walls.setPosition(cellsz * j, cellsz * i);
                    dots.setPosition(cellsz * j + 9, cellsz * i + 9);
                    bigdot.setPosition(cellsz * j + 5, cellsz * i + 5);
                    if (maps[i][j] == '#')
                    {
                        walls.setFillColor(Color::Blue);
                        window.draw(walls);
                    }
                    else if (maps[i][j] == '.')
                    {
                        dots.setFillColor(Color::White);
                        window.draw(dots);
                    }
                    else if (maps[i][j] == 'o')
                    {
                        bigdot.setFillColor(Color::White);
                        window.draw(bigdot);
                    }
                }
            }
            pac.mapupdate();
            pac.draw(window);
            ghost.randmove();
            ghost.draw(window);
            ghost.crash(pac, window);
            window.display();
            window.clear();

        }
    }
    return 0;
}
