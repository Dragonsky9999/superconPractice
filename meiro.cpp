#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

const int H = 15; // セルの行数
const int W = 50; // セルの列数

int playerY = 0, playerX = 0;

// セルごとに上下左右の壁の有無を記録
struct Cell {
    bool top = true;
    bool right = true;
    bool bottom = true;
    bool left = true;
    bool visited = false;
};

vector<vector<Cell>> grid(H, vector<Cell>(W));
vector<vector<bool>> visted(H, vector<bool>(W, false)); 

void dfs(int y, int x) {
    grid[y][x].visited = true;

    int dirs[] = {0, 1, 2, 3}; // 上右下左
    for (int i = 3; i > 0; i--) swap(dirs[i], dirs[rand() % (i + 1)]);

    for (int d : dirs) {
        int ny = y + (d == 0 ? -1 : d == 2 ? 1 : 0);
        int nx = x + (d == 1 ? 1 : d == 3 ? -1 : 0);
        if (ny < 0 || ny >= H || nx < 0 || nx >= W || grid[ny][nx].visited) continue;

        // 壁を壊す
        if (d == 0) { grid[y][x].top = false; grid[ny][nx].bottom = false; }
        if (d == 1) { grid[y][x].right = false; grid[ny][nx].left = false; }
        if (d == 2) { grid[y][x].bottom = false; grid[ny][nx].top = false; }
        if (d == 3) { grid[y][x].left = false; grid[ny][nx].right = false; }

        dfs(ny, nx);
    }
}

void drawMaze(pair<int, int>) {
    // 最上部の境界線
    for (int x = 0; x < W; x++) {
        cout << "+-";
    }
    cout << "+" << endl;

    for (int y = 0; y < H; y++) {
        // 左側の壁 + 各マスの右側
        for (int x = 0; x < W; x++) {
            cout << (grid[y][x].left ? "|" : " ");
            string cell = " ";
            if (visted[y][x]) cell = "•";
         if (y == playerY && x == playerX) cell = "P";
            if (y == 0 && x == 0) cell = "S";
            if (y == H-1 && x == W-1) cell = "G";
            
            cout << cell;
        }
        cout << "|" << endl;

        // 下の壁を描画
        for (int x = 0; x < W; x++) {
            cout << "+";
            cout << (grid[y][x].bottom ? "-" : " ");
        }
        cout << "+" << endl;
    }
}

void solveMaze(){
    
    int dirs[] = {0, 1, 2, 3}; //上右下左
    for (int i = 3; i > 0; i--) swap(dirs[i], dirs[rand() % (i + 1)]);    

    for (int d : dirs){
        int ny = playerY + (d == 0 ? -1 : d == 2 ? 1 : 0);
        int nx = playerX + (d == 1 ? 1 : d == 3 ? -1 : 0);

        if (ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
        // if (visted[ny][nx]) continue;

        if (d == 0 && !grid[playerY][playerX].top) playerY--; 
        else if (d == 1 && !grid[playerY][playerX].right) playerX++;
        else if (d == 2 && !grid[playerY][playerX].bottom) playerY++;
        else if (d == 3 && !grid[playerY][playerX].left) playerX--;

        break;
    }

}

int main() {
    srand(time(0));
    dfs(0, 0); // 迷路生成
    pair<int, int> goal = {H-1, W-1};

    while (true) {
        system("clear"); 
        drawMaze(goal);

        visted[playerY][playerX] = true;

        if (playerY == goal.first && playerX == goal.second) {
            cout << "🎉 ゴールに到達！おめでとう！" << endl;
            break;
        }

        usleep(15000);
        solveMaze();
        // cout << "WASDで移動（Enterで決定）> ";
        // char c;
        // cin >> c;

        // // 移動処理（壁チェックつき）
        // if ((c == 'w' || c == 'W') && !grid[playerY][playerX].top) playerY--;
        // else if ((c == 's' || c == 'S') && !grid[playerY][playerX].bottom) playerY++;
        // else if ((c == 'a' || c == 'A') && !grid[playerY][playerX].left) playerX--;
        // else if ((c == 'd' || c == 'D') && !grid[playerY][playerX].right) playerX++;
    }

    return 0;
}