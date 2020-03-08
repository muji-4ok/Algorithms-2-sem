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
void EdgeSubGraph::addEdge(int start, int finish) {
  addEdge(Edge{start, finish});
}
void EdgeSubGraph::addEdge(Edge edge) {
  edges.insert(edge);
}
void EdgeSubGraph::addNode(int vertex) {
  vertices.insert(vertex);
}
bool EdgeSubGraph::hasEdge(int start, int finish) const {
  return hasEdge(Edge{start, finish});
}
bool EdgeSubGraph::hasEdge(Edge edge) const {
  return edges.find(edge) != edges.end();
}
bool EdgeSubGraph::hasNode(int vertex) const {
  return vertices.find(vertex) != vertices.end();
}
bool Edge::operator==(const Edge &other) const {
  return start == other.start && finish == other.finish;
}
std::size_t EdgeHasher::operator()(const Edge &edge) const noexcept {
  using std::size_t;
  return static_cast<size_t>(edge.start)
      + (static_cast<size_t>(edge.finish) << (sizeof(size_t) / 2));
}
