//
// Created by egork on 06.04.2020.
//
// report: https://contest.yandex.ru/contest/17594/run-report/32058694/
#include <vector>
#include <set>
#include <utility>
#include <iostream>
#include <algorithm>

typedef unsigned long long ull;

int findMinTravelDistance(ull source, ull destination, ull nodesCount, int costA, int costB) {
  std::vector<int> distance(nodesCount, INT32_MAX);
  std::vector<bool> used(nodesCount, false);
  // (dist, vertex)
  std::set<std::pair<int, ull>> queue;
  queue.emplace(0, source);
  distance[source] = 0;

  while (!queue.empty()) {
    std::set<std::pair<int, ull>>::node_type bestNode = queue.extract(queue.begin());
    ull vertex = bestNode.value().second;

    if (used[vertex])
      continue;

    used[vertex] = true;

    ull childA = (vertex + 1) % nodesCount;
    ull childB = (vertex * vertex + 1) % nodesCount;
    int newDistA = std::min(distance[childA], distance[vertex] + costA);
    int newDistB = std::min(distance[childB], distance[vertex] + costB);

    if (distance[childA] == INT32_MAX)
      queue.emplace(newDistA, childA);

    if (distance[childB] == INT32_MAX)
      queue.emplace(newDistB, childB);

    distance[childA] = newDistA;
    distance[childB] = newDistB;
  }

  return distance[destination];
}

int main() {
  int a, b;
  ull M, x, y;
  std::cin >> a >> b >> M >> x >> y;
  std::cout << findMinTravelDistance(x, y, M, a, b) << '\n';
}
