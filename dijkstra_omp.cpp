// dijkstra_omp.cpp — Dijkstra paralelo (OpenMP) para la Prueba Recuperativa
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <cmath>
#include <cctype>
#include <omp.h>

constexpr int INF = std::numeric_limits<int>::max()/2;

/* Convierte la cadena "[[0,10,0],[0,0,5],...]" en vector<int> */
static std::vector<int> parse_numbers(const std::string& s) {
    std::vector<int> out;
    std::string num;
    bool neg = false;
    for (char c : s) {
        if (c == '-') { neg = true; }
        else if (std::isdigit(c)) { num += c; }
        else if (!num.empty()) {
            out.push_back((neg ? -1 : 1) * std::stoi(num));
            num.clear(); neg = false;
        }
    }
    if (!num.empty()) out.push_back((neg ? -1 : 1) * std::stoi(num));
    return out;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " \"[[matriz]]\" vertice_origen salida.txt\n";
        return 1;
    }

    /* 1. Parseo de argumentos */
    const std::vector<int> nums = parse_numbers(argv[1]);
    const int n = static_cast<int>(std::sqrt(nums.size()));
    if (n * n != static_cast<int>(nums.size())) {
        std::cerr << "La matriz no es cuadrada.\n";  return 2;
    }
    const int src = std::stoi(argv[2]);
    if (src < 0 || src >= n) {
        std::cerr << "Vértice de origen fuera de rango.\n";  return 3;
    }

    /* 2. Construir matriz de adyacencia */
    std::vector<std::vector<int>> w(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            w[i][j] = nums[i * n + j];

    /* 3. Estructuras de Dijkstra */
    std::vector<int> dist(n, INF), visited(n, 0);
    dist[src] = 0;

    /* 4. Dijkstra paralelo explícito */
    for (int iter = 0; iter < n; ++iter) {

        /* 4a. Seleccionar vértice mínimo NO visitado — reducción manual */
        int u = -1, min_val = INF;
        #pragma omp parallel
        {
            int local_u = -1, local_min = INF;
            #pragma omp for nowait
            for (int v = 0; v < n; ++v) {
                if (!visited[v] && dist[v] < local_min) {
                    local_min = dist[v];
                    local_u   = v;
                }
            }
            #pragma omp critical
            {
                if (local_min < min_val) {
                    min_val = local_min;
                    u       = local_u;
                }
            }
        }
        if (u == -1) break;          // Quedan vértices inalcanzables
        visited[u] = 1;

        /* 4b. Relajar vecinos de u en paralelo */
        #pragma omp parallel for schedule(static)
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && w[u][v] > 0 && dist[u] + w[u][v] < dist[v]) {
                #pragma omp atomic write
                dist[v] = dist[u] + w[u][v];
            }
        }
    }

    /* 5. Escribir resultado */
    std::ofstream fout(argv[3]);
    if (!fout) { std::cerr << "No pude abrir " << argv[3] << "\n"; return 4; }
    for (int v = 0; v < n; ++v)
        fout << v << '\t' << (dist[v] == INF ? -1 : dist[v]) << '\n';
    return 0;
} 