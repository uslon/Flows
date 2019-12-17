#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::min;

const long long inf = 1e18, edge_inf = 1e9;

struct Edge {
    int to, next;
    long long cap, flow;
};

struct Network {
    vector <Edge> ed;
    vector <int> head;
    int source, target;

    void add_edge(int u, int v, int cap) {
        ed.push_back({v, head[u], cap, 0});
        head[u] = ed.size() - 1;
        ed.push_back({u, head[v], 0, 0});
        head[v] = ed.size() - 1;
    }

    size_t size() {
        return head.size();
    }
};


int rev(int e) {
    return e ^ 1;
} // O(1)


void remove_edge(Network & net, int v, int e) {
    int cur = net.head[v];
    if (cur == -1)
        return;

    int next = net.ed[cur].next;
    if (cur == e) {
        net.head[v] = next;
        return;
    }

    while (next != e && next != -1) {
        cur = next;
        next = net.ed[next].next;
    }
    if (next != -1)
        net.ed[cur].next = net.ed[next].next;
} // O(deg(v))

void remove_vertex(Network & net, vector <long long> &pin, vector <long long> &pout, int v) {
    for (int e = net.head[v]; e != -1; e = net.ed[e].next) {
        int u = net.ed[e].to;
        remove_edge(net, u, rev(e));
        pout[u] -= net.ed[rev(e)].cap;
        pin[u] -= net.ed[e].cap;
    }
} // O(deg(v))


void push(Network & net, int e, long long flow) {
    net.ed[e].cap -= flow;
    net.ed[e].flow += flow;
    net.ed[rev(e)].cap += flow;
    net.ed[rev(e)].flow -= flow;
} // O(1)

void push_both(Network & old, Network & net, vector <int> old_edges, int e, long long flow) {
    push(old, old_edges[e], flow);
    net.ed[e].cap -= flow;
    net.ed[e].flow += flow;
} // O(1)


bool bfs(Network & net, vector <int> & dist, vector <bool> &active) {
    fill(dist.begin(), dist.end(), edge_inf);
    fill(active.begin(), active.end(), false);
    dist[net.source] = 0;
    queue <int> q;
    q.push(net.source);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        active[v] = true;
        for (int e = net.head[v]; e != -1; e = net.ed[e].next) {
            int u = net.ed[e].to;
            if (dist[u] <= dist[v] + 1 || !net.ed[e].cap)
                continue;
            dist[u] = dist[v] + 1;
            q.push(u);
        }
    }
    return dist[net.target] < edge_inf;
} // O(n + m) = O(n^2)

void rev_bfs(Network & net, vector <int> & dist, vector <bool> & active) {
    fill(active.begin(), active.end(), false);
    queue <int > q;
    q.push(net.target);
    active[net.target] = true;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int e = net.head[v]; e != -1; e = net.ed[e].next) {
            int u = net.ed[e].to;
            if (net.ed[rev(e)].cap && !active[u] && dist[u] + 1 == dist[v]) {
                active[u] = true;
                q.push(u);
            }
        }
    }
} // O(n + m) = O(n^2)

void make_and(vector <bool> & a, vector <bool> & b) {
    for (int j = 0; j < a.size(); ++j)
        a[j] = a[j] && b[j];
} // O(n)

void fill_graph(Network & net, std::ifstream & fin) {
    int n, m;
    fin >> n >> m;
    net.ed.reserve(m << 1);
    net.head.resize(n, -1);
    fin >> net.source >> net.target;
    --net.source, --net.target;
    int u, v, cap;
    for (int j = 0; j < m; ++j) {
        fin >> u >> v >> cap;
        --u, --v;
        net.add_edge(u, v, cap);
    }
} // O(n + m) = O(n^2)


void init_sup(Network &net, Network &sup, vector <int> &dist, vector <int> &old_edge, vector <bool> & active) { // O(m) = O(n^2)
    for (int e = 0; e < net.ed.size(); e += 2) {
        int u = net.ed[rev(e)].to, v = net.ed[e].to;
        if (!active[u] || !active[v])
            continue;
        if (dist[u] == dist[v] + 1 && net.ed[rev(e)].cap) {
            sup.ed.push_back({u, sup.head[v], net.ed[rev(e)].cap, 0});
            sup.head[v] = sup.ed.size() - 1;
            old_edge.push_back(rev(e));
            sup.ed.push_back({v, sup.head[u], 0, 0});
            sup.head[u] = sup.ed.size() - 1;
            old_edge.push_back(e);
        }
        else if (dist[v] == dist[u] + 1 && net.ed[e].cap) {
            sup.ed.push_back({v, sup.head[u], net.ed[e].cap, 0});
            sup.head[u] = sup.ed.size() - 1;
            old_edge.push_back(e);
            sup.ed.push_back({u, sup.head[v], 0, 0});
            sup.head[v] = sup.ed.size() - 1;
            old_edge.push_back(rev(e));
        }
    }
} // O(m) = O(n^2)


void count_potentials(Network & net, vector <long long> & pin, vector <long long> &pout) {
    std::fill(pin.begin(), pin.end(), 0);
    std::fill(pout.begin(), pout.end(), 0);
    pout[net.target] = pin[net.source] = inf;
    for (int e = 0; e < net.ed.size(); ++e) {
        int v = net.ed[e].to, u = net.ed[rev(e)].to;
        int cap = net.ed[e].cap;
        pout[u] += cap;
        pin[v] += cap;
    }
} // O(m) = O(n^2)

int argmin(vector <long long> & arr, vector <long long> & brr, vector <bool> & active) {
    int v = -1;
    for (int j = 0; j < arr.size(); ++j) {
        if (!active[j])
            continue;
        if (v == -1 || std::min(arr[v], brr[v]) > std::min(arr[j], brr[j]))
            v = j;
    }
    return v;
} // O(n)

void push_flow(Network &net, Network &old, int v, int to, long long flow, bool fwd, vector <long long> &pin, vector <long long> &pout, vector <int> &old_edges) {
    int n = net.size();
    vector <long long> ex(n, 0);

    queue <int> q;
    q.push(v);
    ex[v] = flow;
    while (!q.empty()) {
        v = q.front();
        q.pop();
        if (v == to)
            return;
        long long rem = ex[v];
        for (int e = net.head[v]; e != -1; e = net.ed[e].next) {
            int u = net.ed[e].to;
            long long pushed;
            if (fwd) {
                pushed = std::min(rem, net.ed[e].cap);
                if (pushed) {
                    pout[v] -= pushed;
                    pin[u] -= pushed;
                    if (!ex[u])
                        q.push(u);
                    ex[u] += pushed;
                    push_both(old, net, old_edges, e, pushed);
                }
            }
            else {
                pushed = std::min(rem, net.ed[rev(e)].cap);
                if (pushed) {
                    pout[u] -= pushed;
                    pin[v] -= pushed;
                    if (!ex[u])
                        q.push(u);
                    ex[u] += pushed;
                    push_both(old, net, old_edges, rev(e), pushed);
                }
            }
            rem -= pushed;
            if (!rem)
                break;
        }
    }
} // O(n + m) = O(n^2)

long long push(Network & sup, Network & old, vector <int> &old_edges, vector <bool> &active) {
    int n = sup.size();
    vector <long long> pin(n), pout(n);
    count_potentials(sup, pin, pout);

    int cur = argmin(pin, pout, active);
    if (cur == -1)
        return 0;
    long long sum = 0;
    while (long long potential = min(pin[cur], pout[cur])) {
        sum += potential;
        push_flow(sup, old, cur, sup.target, potential, true, pin, pout, old_edges);
        push_flow(sup, old, cur, sup.source, potential, false, pin, pout, old_edges);
        do {
            active[cur] = false;
            remove_vertex(sup, pin, pout, cur);
            cur = argmin(pin, pout, active);
            if (cur == -1)
                break;
        } while (min(pin[cur], pout[cur]) == 0);
        if (cur == -1)
            break;
    }
    return sum;
} // O(n^3)

long long max_flow_mkm(std::ifstream &fin) {
    Network net;
    fill_graph(net, fin);

    vector <int> dist(net.size());
    vector <bool> a(net.size()), b(net.size());
    long long sum = 0;
    while (bfs(net, dist, a)) {
        rev_bfs(net, dist, b);
        Network sup;
        sup.head.resize(net.size(), -1);
        sup.target = net.target;
        sup.source = net.source;
        vector <int> old_edges;
        make_and(a, b);
        init_sup(net, sup, dist, old_edges, a);
        sum += push(sup, net, old_edges, a);
    }
    return sum;
} // O(n^4)