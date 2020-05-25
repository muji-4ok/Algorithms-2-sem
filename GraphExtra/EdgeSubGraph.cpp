//
// Created by egork on 07.03.2020.
//

#include "EdgeSubGraph.h"

EdgeSubGraph::EdgeSubGraph(std::vector<int> cycle) {
  for (int v : cycle)
    vertices.insert(v);

  for (int i = 1; i < cycle.size(); ++i) {
    addEdge(cycle[i - 1], cycle[i]);
    addEdge(cycle[i], cycle[i - 1]);
  }

  addEdge(cycle.back(), cycle.front());
  addEdge(cycle.front(), cycle.back());
}
void EdgeStorage::addEdge(int start, int finish) {
  addEdge(Edge{start, finish});
}
void EdgeStorage::addEdge(Edge edge) {
  edges.insert(edge);
}
void EdgeSubGraph::addNode(int vertex) {
  vertices.insert(vertex);
}
bool EdgeStorage::hasEdge(int start, int finish) const {
  return hasEdge(Edge{start, finish});
}
bool EdgeStorage::hasEdge(Edge edge) const {
  return edges.find(edge) != edges.end();
}
void EdgeStorage::removeEdge(int start, int finish) {
  removeEdge(Edge{start, finish});
}
void EdgeStorage::removeEdge(Edge edge) {
  edges.erase(edge);
}
std::string EdgeStorage::toString() const {
  std::string out;
  std::unordered_map<int, std::unordered_map<int, bool>> printed;

  for (const auto &it : edges) {
    int v = it.start;
    int u = it.finish;

    if (!printed[v][u]) {
      out += std::to_string(v) + " <---> " + std::to_string(u) + '\n';
      printed[v][u] = true;
      printed[u][v] = true;
    }
  }

  return out;
}
int EdgeStorage::getEdgeCount() const {
  return edges.size();
}
bool EdgeSubGraph::hasNode(int vertex) const {
  return vertices.find(vertex) != vertices.end();
}
int EdgeSubGraph::getNodeCount() const {
  vertices.size();
}
bool Edge::operator==(const Edge &other) const {
  return start == other.start && finish == other.finish;
}
std::size_t EdgeHasher::operator()(const Edge &edge) const noexcept {
  using std::size_t;
  return static_cast<size_t>(edge.start)
      + (static_cast<size_t>(edge.finish) << (sizeof(size_t) / 2));
}
