#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// Grid A*. x indexes ROW (world width), y indexes COL (world height) -- the names are
// backwards but every use agrees, so they are left alone.
class PathFinderxd {
private:
    int ROW, COL;
    vector<int> dx, dy;
    // Was a vector of blocked coordinates scanned linearly on every neighbour test: with
    // a world full of solid tiles that is millions of comparisons per search. A grid
    // makes the test O(1).
    vector<vector<bool>> blocked;

public:
    PathFinderxd(int ROW, int COL)
        : ROW(ROW < 0 ? 0 : ROW), COL(COL < 0 ? 0 : COL),
        blocked(ROW < 0 ? 0 : ROW, vector<bool>(COL < 0 ? 0 : COL, false)) {}

    struct Node {
        int x, y;
        int g, h;
        Node(int x, int y, int g, int h) : x(x), y(y), g(g), h(h) {}
        bool operator<(const Node& n) const { return g + h > n.g + n.h; }
    };

    bool inBounds(int x, int y) const {
        return x >= 0 && x < ROW && y >= 0 && y < COL;
    }

    void setBlocked(int x, int y) {
        if (inBounds(x, y))
            blocked[x][y] = true;
    }

    bool isBlocked(int x, int y) const {
        return !inBounds(x, y) || blocked[x][y];
    }

    void setNeighbors(vector<int> dx, vector<int> dy) {
        this->dx = dx;
        this->dy = dy;
    }

    vector<pair<int, int>> aStar(int startX, int startY, int finishX, int finishY) {
        // The caller passes the player position and a punched tile straight from a
        // packet, so either can sit outside the world. Indexing the grid with those
        // wrote out of bounds and crashed the proxy.
        if (!inBounds(startX, startY) || !inBounds(finishX, finishY))
            return {};
        if (isBlocked(finishX, finishY))
            return {};
        if (dx.empty() || dx.size() != dy.size())
            return {};

        vector<vector<int>> g(ROW, vector<int>(COL, INT_MAX));
        vector<vector<int>> h(ROW, vector<int>(COL, INT_MAX));
        vector<vector<pair<int, int>>> cameFrom(ROW, vector<pair<int, int>>(COL, { -1, -1 }));
        priority_queue<Node> q;

        g[startX][startY] = 0;
        h[startX][startY] = max(abs(finishX - startX), abs(finishY - startY));
        q.push(Node(startX, startY, g[startX][startY], h[startX][startY]));

        while (!q.empty()) {
            Node curr = q.top();
            q.pop();

            if (curr.x == finishX && curr.y == finishY) {
                vector<pair<int, int>> path;
                int x = curr.x, y = curr.y;
                // Walk the parent links back to the start. Bounded by the grid size so a
                // broken chain cannot spin forever.
                for (int steps = 0; steps <= ROW * COL; steps++) {
                    if (x == startX && y == startY)
                        break;
                    path.push_back({ x, y });
                    const pair<int, int> prev = cameFrom[x][y];
                    if (prev.first < 0)
                        return {};           // no parent recorded: unusable path
                    x = prev.first;
                    y = prev.second;
                }
                path.push_back({ startX, startY });
                reverse(path.begin(), path.end());
                return path;
            }

            for (size_t i = 0; i < dx.size(); ++i) {
                int x_ = curr.x + dx[i], y_ = curr.y + dy[i];
                if (!inBounds(x_, y_))
                    continue;
                if (isBlocked(x_, y_))
                    continue;

                int gScore = g[curr.x][curr.y] + 1;
                if (gScore >= g[x_][y_])
                    continue;

                cameFrom[x_][y_] = { curr.x, curr.y };
                g[x_][y_] = gScore;
                h[x_][y_] = abs(finishX - x_) + abs(finishY - y_);
                q.push(Node(x_, y_, g[x_][y_], h[x_][y_]));
            }
        }
        return {};
    }
};
