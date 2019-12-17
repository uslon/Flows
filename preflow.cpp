//
// Created by amir on 08.10.2019.
//

#include <queue>
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

void output_vector (vector <int> & a) {
    for (auto ai : a)
        std::cout << ai << ' ';
    cout << endl;
}

const long long MAXN = 1e4 + 20, MAXM = 2e4 + 20, inf = 2e6 + 200;

struct edge {
    int to, cap, flow, next;
};

int s = 0;
int t = 0;
int sz = 0;
int sze = 0;
std::vector <int> head;
std::vector <int> height;
std::vector <int> overflow;
std::queue <int> vertex_queue;
std::vector <bool> is_in_queue;

std::vector <edge> ed;

void add_edge (int u, int v, int cap) {
    ed.push_back({v, cap, 0, head[u]});
    head[u] = ed.size() - 1;
    ed.push_back({u, 0, 0, head[v]});
    head[v] = ed.size() - 1;
}

void push (int e, int flow) {
    if (flow == 0)
        return;

    ed[e].flow += flow;
    ed[e].cap -= flow;
    ed[e ^ 1].flow -= flow;
    ed[e ^ 1].cap += flow;

    int v = ed[e].to;
    int u = ed[e ^ 1].to;
    overflow[u] -= flow;
    overflow[v] += flow;
    if (!is_in_queue[v] && v != t && v != s) {
        is_in_queue[v] = true;
        vertex_queue.push(v);
    }
}

void relabel (int u) {
    int e = head[u];
    int min_height = inf;
    while (e != -1) {
        int v = ed[e].to;
        if (ed[e].cap > 0 && min_height > height[v])
            min_height = height[v];

        e = ed[e].next;
    }

    height[u] = min_height + 1;
}

void discharge (int u) {
    int e = head[u];
    while (overflow[u] > 0 && height[u] <= sz) {
        //std::cout << u << ' ' << overflow[u] << ' ' << height[u] << ' ' << v << std::endl;
        if (e == -1) {
            relabel(u);
            e = head[u];
        }
        else {
            int v = ed[e].to;
            if (ed[e].cap > 0 && height[u] == height[v] + 1)
                push(e, std::min(overflow[u], ed[e].cap));
            else
                e = ed[e].next;
        }
    }
}

void push_preflow () {
    int e = head[s];
    while (e != -1) {
        push(e, ed[e].cap);
        e = ed[e].next;
        //std::cout << e << std::endl;
    }
    height[s] = sz;
}

void init_preflow () {
    head.clear();
    height.clear();
    overflow.clear();
    is_in_queue.clear();
    head.resize(sz, -1);
    height.resize(sz);
    overflow.resize(sz);
    is_in_queue.resize(sz);
}

long long get_max_flow () {
    while (!vertex_queue.empty()) {
        /*cout << "overflow\n";
        output_vector(overflow);
        cout << "height\n";
        output_vector(height);
        cout << endl;*/
        int v = vertex_queue.front();
        vertex_queue.pop();
        is_in_queue[v] = false;
        discharge(v);
    }
    return overflow[t];
}

long long max_flow_preflow(std::ifstream & in) {
    int n, m;
    in >> n >> m >> s >> t;
    --s, --t;

    sz = n;
    init_preflow();

    int u, v, cap;
    for (int j = 0; j < m; ++j) {
        in >> u >> v >> cap;
        add_edge(--u, --v, cap);
    }
    push_preflow();

    return get_max_flow();
}