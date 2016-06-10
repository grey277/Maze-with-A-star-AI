class MapGen {
public:
    MapGen(int height, int width) : _height(height), _width(width) {
        srand(time(NULL));
        maze = new int *[_height];
        generateMaze();
    }

    int **getMaze() { return maze; }

private:
    int _height;
    int _width;
    int **maze;

    void generateMaze() {
        for (int i = 0; i < _height; i++) {
            maze[i] = new int[_width];
            for (int j = 0; j < _width; ++j) {
                maze[i][j] = 0;
            }
        }

        int r = rand() % _height;
        if (!r)
            r = rand() % _height;
        while (r % 2) {
            r = rand() % _height;
            if (!r)
                r = rand() % _height;
        }

        int c = rand() % _width;
        if (!c)
            c = rand() % _height;
        while (c % 2) {
            c = rand() % _width;
            if (!c)
                c = rand() % _height;
        }
        maze[r][c] = 4;

        recursion(r, c);
    }

    void recursion(int r, int c) {
        int *randDirs = new int[4];
        genRandDir(randDirs);
        for (int i = 0; i < 4; ++i) {
            switch (randDirs[i]) {
                case 1:
                    if (r - 2 <= 0)
                        continue;
                    if (maze[r - 2][c] != 4) {
                        maze[r - 2][c] = 4;
                        maze[r - 1][c] = 4;
                        recursion(r - 2, c);
                    }
                    break;
                case 2:
                    if (c + 2 >= _width - 1)
                        continue;
                    if (maze[r][c + 2] != 4) {
                        maze[r][c + 2] = 4;
                        maze[r][c + 1] = 4;
                        recursion(r, c + 2);
                    }
                    break;
                case 3:
                    if (r + 2 >= _height - 1)
                        continue;
                    if (maze[r + 2][c] != 4) {
                        maze[r + 2][c] = 4;
                        maze[r + 1][c] = 4;
                        recursion(r + 2, c);
                    }
                    break;
                case 4:
                    if (c - 2 <= 0)
                        continue;
                    if (maze[r][c - 2] != 4) {
                        maze[r][c - 2] = 4;
                        maze[r][c - 1] = 4;
                        recursion(r, c - 2);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void genRandDir(int *data) {
        std::vector<int> random;
        for (int i = 0; i < 4; i++) {
            random.push_back(i + 1);
        }
        std::random_shuffle(random.begin(), random.end());
        for (int j = 0; j < 4; ++j) {
            data[j] = random.data()[j];
        }
    }

};


