#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <random>
class MazeSolver {
private:
    std::vector<std::vector<int>> maze; int rows, cols;
    std::pair<int, int> start, end;
    void generateMaze() {
        maze.assign(rows, std::vector<int>(cols, 1));
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        std::random_device rd; std::mt19937 gen(rd()); maze[1][1] = 0; visited[1][1] = true;
        std::stack<std::pair<int, int>> stack; stack.push({1, 1});
        std::vector<std::pair<int, int>> dirs = {{-2, 0}, {0, 2}, {2, 0}, {0, -2}};
        while (!stack.empty()) {
            auto [x, y] = stack.top(); std::vector<int> unvisited;
            for (int i = 0; i < 4; i++) {
                int nx = x + dirs[i].first, ny = y + dirs[i].second;
                if (nx > 0 && nx < rows-1 && ny > 0 && ny < cols-1 && !visited[nx][ny])
                    unvisited.push_back(i);
            }
            if (!unvisited.empty()) {
                int dir = unvisited[gen() % unvisited.size()];
                int nx = x + dirs[dir].first, ny = y + dirs[dir].second;
                int wallX = x + dirs[dir].first/2, wallY = y + dirs[dir].second/2;
                maze[wallX][wallY] = maze[nx][ny] = 0; visited[nx][ny] = true;
                stack.push({nx, ny}); } else stack.pop();
        }
        start = {1, 1}; end = {rows-2, cols-2};
    }
    bool solveDFS(int x, int y, std::vector<std::vector<int>>& sol) {
        if (x < 0 || x >= rows || y < 0 || y >= cols || maze[x][y] == 1 || sol[x][y] == 1)
            return false; sol[x][y] = 1;
        if (x == end.first && y == end.second) return true;
        return solveDFS(x+1, y, sol) || solveDFS(x-1, y, sol) || 
               solveDFS(x, y+1, sol) || solveDFS(x, y-1, sol) || (sol[x][y] = 0, false);
    }
    bool solveBFS(std::vector<std::vector<int>>& sol) {
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        std::vector<std::vector<std::pair<int, int>>> parent(rows, 
            std::vector<std::pair<int, int>>(cols, {-1, -1}));
        std::queue<std::pair<int, int>> q;
        q.push(start); visited[start.first][start.second] = true;
        std::vector<std::pair<int, int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            if (x == end.first && y == end.second) {
                int px = x, py = y;
                while (px != -1 && py != -1) {
                    sol[px][py] = 1; auto [npx, npy] = parent[px][py];
                    px = npx; py = npy;
                }
                return true;
            }
            for (auto [dx, dy] : dirs) {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && 
                    maze[nx][ny] == 0 && !visited[nx][ny]) {
                    visited[nx][ny] = true; parent[nx][ny] = {x, y};
                    q.push({nx, ny});
                }
            }
        }
        return false;
    }
    void print(const std::vector<std::vector<int>>& sol = {}) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                char ch = (i == start.first && j == start.second) ? 'S' :
                         (i == end.first && j == end.second) ? 'E' :
                         (maze[i][j] == 1) ? '#' :
                         (!sol.empty() && sol[i][j] == 1) ? '*' : '.';
                std::cout << ch << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
public:
    MazeSolver(int r, int c) : rows(r % 2 ? r : r+1), cols(c % 2 ? c : c+1) {
        generateMaze();
    }
    void solve(bool useBFS = false) {
        std::vector<std::vector<int>> solution(rows, std::vector<int>(cols, 0));
        std::cout << (useBFS ? "BFS Solution:\n" : "DFS Solution:\n");
        bool found = useBFS ? solveBFS(solution) : solveDFS(start.first, start.second, solution);
        found ? print(solution) : (std::cout << "No solution!\n", void());
    }
    void demo() {
        std::cout << "Generated Maze:\n"; print(); solve(false); solve(true);
    }
};
bool solveCustomMaze() {
    int rows, cols; std::cout << "Enter maze dimensions (rows cols): ";
    std::cin >> rows >> cols;
    std::vector<std::vector<int>> maze(rows, std::vector<int>(cols));
    std::cout << "Enter maze (0=path, 1=wall):\n";
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            std::cin >> maze[i][j]; int startX, startY, endX, endY;
    std::cout << "Enter start (row col): "; std::cin >> startX >> startY;
    std::cout << "Enter end (row col): "; std::cin >> endX >> endY;
    std::vector<std::vector<int>> solution(rows, std::vector<int>(cols, 0));
    std::function<bool(int, int)> solve = [&](int x, int y) -> bool {
        if (x < 0 || x >= rows || y < 0 || y >= cols || maze[x][y] == 1 || solution[x][y] == 1)
            return false; solution[x][y] = 1;
        if (x == endX && y == endY) return true;
        return solve(x+1, y) || solve(x-1, y) || solve(x, y+1) || solve(x, y-1) || 
               (solution[x][y] = 0, false); };
    if (solve(startX, startY)) {
        std::cout << "\nSolution:\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                char ch = (i == startX && j == startY) ? 'S' :
                         (i == endX && j == endY) ? 'E' :
                         (maze[i][j] == 1) ? '#' :
                         (solution[i][j] == 1) ? '*' : '.'; std::cout << ch << ' ';
            }
            std::cout << '\n';
        }
        return true;
    }
    return false;
}
void quickDemo() {
    std::cout << "Quick demo with predefined 7x7 maze:\n";
    std::vector<std::vector<int>> demo = {
        {1,1,1,1,1,1,1}, {1,0,1,0,0,0,1}, {1,0,1,0,1,0,1},
        {1,0,0,0,1,0,1}, {1,1,1,0,1,0,1}, {1,0,0,0,0,0,1}, {1,1,1,1,1,1,1}
    };
    std::vector<std::vector<int>> sol(7, std::vector<int>(7, 0));
    std::function<bool(int, int)> solve = [&](int x, int y) -> bool {
        if (x < 0 || x >= 7 || y < 0 || y >= 7 || demo[x][y] == 1 || sol[x][y] == 1)
            return false; sol[x][y] = 1;
        if (x == 5 && y == 5) return true;
        return solve(x+1, y) || solve(x-1, y) || solve(x, y+1) || solve(x, y-1) || 
               (sol[x][y] = 0, false); };
    if (solve(1, 1)) {
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                char ch = (i == 1 && j == 1) ? 'S' : (i == 5 && j == 5) ? 'E' :
                         (demo[i][j] == 1) ? '#' : (sol[i][j] == 1) ? '*' : '.';
                std::cout << ch << ' ';
            }
            std::cout << '\n';
        }
    }
}
int main() {
    std::cout << "=== C++ Maze Solver Project ===\n";
    std::cout << "1. Random maze  2. Custom maze  3. Quick demo\nChoice: ";
    int choice; std::cin >> choice;
    switch(choice) {
        case 1: {
            int rows, cols; std::cout << "Enter dimensions: ";
            std::cin >> rows >> cols; MazeSolver solver(rows, cols);
            solver.demo(); break;
        }
        case 2:
            if (!solveCustomMaze()) std::cout << "No solution!\n";
            break;
        case 3:
            quickDemo(); break;
        default:
            std::cout << "Invalid choice!\n";
    }
    return 0;
}