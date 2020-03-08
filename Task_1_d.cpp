//
// Created by egork on 29.02.2020.
//
// https://github.com/muji-4ok/Algorithms-2-sem
#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cassert>
#include "SetSubGraph.h"
#include "SetGraph.h"
#include "EdgeSubGraph.h"

std::vector<int> fillComponent(int start,
                               const SetGraph &graph,
                               std::vector<char> &state,
                               std::vector<int> &collection) {
  std::vector<int> cycle;
  std::vector<Edge> stack;
  stack.emplace_back(start, start);
  bool hasCycles = false;

  while (!stack.empty()) {
    int vertex, parent;
    vertex = stack.back().finish;
    parent = stack.back().start;

    if (state[vertex] == 2) {
      stack.pop_back();
      continue;
    }

    state[vertex] = 1;

    for (int child : graph.getChildren(vertex)) {
      if (state[child] == 1 && child != vertex && child != parent) {
        if (!hasCycles) {
          int i = stack.size() - 1;
          std::unordered_map<int, bool> added;

          for (; stack[i].finish != child; --i) {
            int &element = stack[i].finish;

            if (state[element] == 1 && !added[element]) {
              cycle.push_back(element);
              added[element] = true;
            }
          }

          cycle.push_back(stack[i].finish);
        }

        hasCycles = true;
      } else if (state[child] == 0) {
        stack.emplace_back(vertex, child);
      }
    }

    while (!stack.empty() && state[stack.back().finish] == 1) {
      collection.push_back(stack.back().finish);
      state[stack.back().finish] = 2;
      stack.pop_back();
    }
  }

  return cycle;
}

// Returns vertices of cyclic subgraphs and a cycle for each of them
std::tuple<std::vector<std::vector<int>>,
           std::vector<std::vector<int>>> getCyclicComponents(const SetGraph &graph) {
  std::vector<char> state(graph.getNodeCount(), 0);
  std::vector<std::vector<int>> cyclicComponentsVertices;
  std::vector<std::vector<int>> cycles;

  // Find all cyclic components
  for (int i = 0; i < graph.getNodeCount(); ++i) {
    if (state[i] == 0) {
      std::vector<int> vertices;
      std::vector<int> cycle = fillComponent(i, graph, state, vertices);

      if (!cycle.empty()) {
        cyclicComponentsVertices.push_back(std::move(vertices));
        cycles.push_back(std::move(cycle));
      }
    }
  }

  return {cyclicComponentsVertices, cycles};
}

void walkBridges(int start,
                 const SetGraph &graph,
                 std::vector<int> &inTimes,
                 std::vector<int> &outTimes,
                 std::vector<char> &states,
                 std::vector<Edge> &bridges) {
  std::vector<Edge> stack;
  stack.emplace_back(start, start);
  int time = 0;

  while (!stack.empty()) {
    int vertex, parent;
    vertex = stack.back().finish;
    parent = stack.back().start;

    states[vertex] = 1;
    inTimes[vertex] = time;
    outTimes[vertex] = time;
    ++time;

    for (int child : graph.getChildren(vertex)) {
      if (states[child] >= 1 && child != vertex && child != parent) {
        outTimes[vertex] = std::min(outTimes[vertex], inTimes[child]);
      } else if (states[child] == 0) {
        stack.emplace_back(vertex, child);
      }
    }

    while (!stack.empty() && states[stack.back().finish] >= 1) {
      vertex = stack.back().finish;
      parent = stack.back().start;
      outTimes[parent] = std::min(outTimes[parent], outTimes[vertex]);

      if (outTimes[vertex] > inTimes[parent])
        bridges.push_back(stack.back());

      states[vertex] = 2;
      stack.pop_back();
    }
  }
}

std::vector<Edge> getBridges(const SetGraph &graph) {
  std::vector<int> inTimes(graph.getNodeCount());
  std::vector<int> outTimes(graph.getNodeCount());
  std::vector<char> states(graph.getNodeCount(), 0);
  std::vector<Edge> bridges;

  for (int i = 0; i < graph.getNodeCount(); ++i)
    if (states[i] == 0)
      walkBridges(i, graph, inTimes, outTimes, states, bridges);

  return bridges;
}

//TODO: better organize face managing
//class FaceManager {
// public:
//  bool hasFace(int vertex, int face) const;
//
// private:
//};

void addEdgeSegments(int start,
                     const SetGraph &graph,
                     const EdgeSubGraph &laidGraph,
                     EdgeStorage &segmentEdges,
                     std::list<std::pair<SetSubGraph, std::set<int>>> &segments,
                     const std::unordered_map<int, std::set<int>> &verticesInFaces) {
  for (int child : graph.getChildren(start)) {
    if (laidGraph.hasNode(child) && !laidGraph.hasEdge(start, child)
        && !segmentEdges.hasEdge(start, child)) {
      SetSubGraph segment{};
      std::set<int> contactPoints{start, child};

      segmentEdges.addEdge(start, child);
      segmentEdges.addEdge(child, start);

      segment.addNode(start);
      segment.addNode(child);
      segment.addEdge(start, child);
      segment.addEdge(child, start);

      segments.emplace_back(std::move(segment), std::move(contactPoints));
    }
  }
}

void addComponentSegment(
    int start,
    const SetGraph &graph,
    const EdgeSubGraph &laidGraph,
    EdgeStorage &segmentEdges,
    std::list<std::pair<SetSubGraph, std::set<int>>> &segments,
    const std::unordered_map<int, std::set<int>> &facesOfVertices
) {
  std::vector<int> stack;
  stack.push_back(start);
  std::unordered_map<int, char> states;
  SetSubGraph subGraph;
  std::set<int> contactPoints{start};
  subGraph.addNode(start);

  while (!stack.empty()) {
    int vertex = stack.back();

    if (states[vertex] == 2) {
      stack.pop_back();
      continue;
    }

    states[vertex] = 1;

    for (int child : graph.getChildren(vertex)) {
      if (segmentEdges.hasEdge(vertex, child)
          || (laidGraph.hasNode(vertex) && laidGraph.hasNode(child)))
        continue;

      if (laidGraph.hasNode(child)) {
        // We are going inside laidGraph, which we don't want to do
        subGraph.addNode(child);
        subGraph.addEdge(vertex, child);
        subGraph.addEdge(child, vertex);

        segmentEdges.addEdge(vertex, child);
        segmentEdges.addEdge(child, vertex);

        contactPoints.insert(child);
      } else {
        subGraph.addNode(child);
        subGraph.addEdge(vertex, child);
        segmentEdges.addEdge(vertex, child);

        if (states[child] == 0) {
          stack.push_back(child);
        }
      }
    }

    while (!stack.empty() && states[stack.back()] == 1) {
      states[stack.back()] = 2;
      stack.pop_back();
    }
  }

  // Actually have anything besides the starting vertex
  if (subGraph.getNodeCount() > 1)
    segments.emplace_back(std::move(subGraph), std::move(contactPoints));
}

// TODO: calculate paths when building segments

std::unordered_map<int, int> buildPaths(int start, const SetSubGraph &segment) {
  std::unordered_map<int, int> parents;
  std::unordered_map<int, char> states;
  std::vector<int> stack;
  stack.push_back(start);

  while (!stack.empty()) {
    int vertex = stack.back();

    if (states[vertex] == 2) {
      stack.pop_back();
      continue;
    }

    states[vertex] = 1;

    for (int child : segment.getChildren(vertex)) {
      if (states[child] == 0) {
        parents[child] = vertex;
        stack.push_back(child);
      }
    }

    while (!stack.empty() && states[stack.back()] == 1) {
      states[stack.back()] = 2;
      stack.pop_back();
    }
  }

  return parents;
}

std::tuple<int, int> findContacts(int face,
                                  const SetSubGraph &segment,
                                  const EdgeSubGraph &laidGraph,
                                  const std::unordered_map<int, std::set<int>> &facesOfVertices) {
  int start = -1;
  int end = -1;

  for (int v : segment.getVertices()) {
    if (laidGraph.hasNode(v) && facesOfVertices.at(v).find(face) != facesOfVertices.at(v).end()) {
      if (start == -1) {
        start = v;
      } else if (end == -1) {
        end = v;
      } else {
        break;
      }
    }
  }

  assert(start != -1 && end != -1);

  return {start, end};
}

std::vector<int> buildChain(int face,
                            const SetSubGraph &segment,
                            const EdgeSubGraph &laidGraph,
                            const std::unordered_map<int, std::set<int>> &facesOfVertices) {
  int start, end;
  std::tie(start, end) = findContacts(face, segment, laidGraph, facesOfVertices);
  std::unordered_map<int, int> parents = buildPaths(start, segment);
  std::vector<int> chain;

  for (int i = end; i != start; i = parents[i])
    chain.push_back(i);

  chain.push_back(start);

  return chain;
}

std::tuple<int, int> findSplits(const std::vector<int> &splitted, int p1, int p2) {
  int first = -1;
  int second = -1;

  for (int i = 0; i < splitted.size(); ++i) {
    if (splitted[i] == p1 || splitted[i] == p2) {
      if (first == -1)
        first = i;
      else if (second == -1)
        second = i;
      else
        assert(false);
    }
  }

  assert(first != -1 && second != -1);

  if (second < first)
    std::swap(first, second);

  return {first, second};
}

void splitFace(int face,
               const std::vector<int> &chain,
               std::unordered_map<int, std::vector<int>> &faces,
               std::unordered_map<int, std::set<int>> &facesOfVertices) {
  int start, end;
  std::tie(start, end) = findSplits(faces[face], chain.front(), chain.back());
  int newFace = faces.size();

  for (auto &it : facesOfVertices) {
    for (int i = start + 1; i < end; ++i) {
      if (it.first == faces[face][i]) {
        it.second.erase(face);
        it.second.insert(newFace);
      }
    }
  }

  for (int v : chain) {
    facesOfVertices[v].insert(face);
    facesOfVertices[v].insert(newFace);
  }

  std::vector<int> bufferFaceCycle;

  for (int i = 0; i < start; ++i)
    bufferFaceCycle.push_back(faces[face][i]);

  for (int v : chain)
    bufferFaceCycle.push_back(v);

  for (int i = end + 1; i < faces[face].size(); ++i)
    bufferFaceCycle.push_back(faces[face][i]);

  std::vector<int> bufferNewFaceCycle;

  for (int i = start + 1; i < end; ++i)
    bufferNewFaceCycle.push_back(faces[face][i]);

  for (int i = chain.size() - 1; i >= 0; --i)
    bufferNewFaceCycle.push_back(chain[i]);

  faces[face] = bufferFaceCycle;
  faces[newFace] = bufferNewFaceCycle;
}

template<typename T>
void intersectWith(std::set<T> &intersected, const std::set<T> &intersection) {
  std::set<T> outSet;
  std::set_intersection(intersected.begin(),
                        intersected.end(),
                        intersection.begin(),
                        intersection.end(),
                        std::inserter(outSet, outSet.begin()));
  intersected = std::move(outSet);
}

std::set<int> getCommonFaces(const std::set<int> &contactPoints,
                             const std::unordered_map<int, std::set<int>> &facesOfVertices) {
  std::set<int> out{facesOfVertices.at(*contactPoints.begin())};

  for (int v : contactPoints)
    intersectWith(out, facesOfVertices.at(v));

  return out;
}

void laySegment(std::list<std::pair<SetSubGraph, std::set<int>>>::iterator segmentIndex,
                std::list<std::pair<SetSubGraph, std::set<int>>> &segments,
                EdgeSubGraph &laidGraph,
                EdgeStorage &segmentEdges,
                std::unordered_map<int, std::set<int>> &facesOfVertices,
                std::unordered_map<int, std::vector<int>> &faces,
                std::vector<int> &addedVertices) {
  SetSubGraph &segment = segmentIndex->first;
  std::set<int> contactPoints = segmentIndex->second;
  int face = *getCommonFaces(contactPoints, facesOfVertices).begin();
  std::vector<int> chain = buildChain(face, segment, laidGraph, facesOfVertices);

  // Segment completely removed, addedVertices are then processed
  // Everything from segment deleted from segmentEdges
  for (int v : segment.getVertices())
    for (int u : segment.getChildren(v))
      segmentEdges.removeEdge(v, u);

  segments.erase(segmentIndex);

  for (int v : chain) {
    if (!laidGraph.hasNode(v)) {
      addedVertices.push_back(v);
      laidGraph.addNode(v);
    }
  }

  for (int i = 1; i < chain.size(); ++i) {
    laidGraph.addEdge(chain[i - 1], chain[i]);
    laidGraph.addEdge(chain[i], chain[i - 1]);
  }

  splitFace(face, chain, faces, facesOfVertices);
}

bool isPlanar(const SetGraph &graph,
              const std::vector<int> &vertices,
              const std::vector<int> &cycle) {
  std::unordered_map<int, std::vector<int>> faces;
  faces[0] = cycle;
  faces[1] = cycle;
  std::unordered_map<int, std::set<int>> facesOfVertices;

  for (int v : cycle) {
    facesOfVertices[v].insert(0);
    facesOfVertices[v].insert(1);
  }

  EdgeSubGraph laidGraph(cycle);
  EdgeStorage segmentEdges;
  std::list<std::pair<SetSubGraph, std::set<int>>> segments;
  std::vector<int> addedVertices{cycle};

  while (true) {
    for (int v : addedVertices) {
      addEdgeSegments(v, graph, laidGraph, segmentEdges, segments, facesOfVertices);
      addComponentSegment(v, graph, laidGraph, segmentEdges, segments, facesOfVertices);
    }

    addedVertices.clear();

    if (segments.empty())
      return true;

//    std::cout << "Segments: \n";
//
//    for (const auto &it : segments) {
//      std::cout << it.first.toString();
//
//      std::cout << "Contact faces: ";
//
//      for (int v : it.second)
//        std::cout << v << ' ';
//
//      std::cout << '\n';
//    }
//
//    std::cout << '\n';

    auto minSegment = segments.begin();

//    std::cout << "Min Segment: \n";
//    std::cout << minSegment->first.toString();

    for (auto it = segments.begin(); it != segments.end(); ++it)
      if (getCommonFaces(it->second, facesOfVertices)
          < getCommonFaces(minSegment->second, facesOfVertices))
        minSegment = it;

    if (getCommonFaces(minSegment->second, facesOfVertices).empty())
      return false;

    laySegment(minSegment,
               segments,
               laidGraph,
               segmentEdges,
               facesOfVertices,
               faces,
               addedVertices);

//    std::cout << "LaidGraph: \n";
//    std::cout << laidGraph.toString() << '\n';
//
//    std::cout << "SegmentEdges: \n";
//    std::cout << segmentEdges.toString() << '\n';
  }
}

bool solve(SetGraph &graph) {
  std::vector<Edge> bridges = getBridges(graph);

  for (const Edge &e : bridges) {
    graph.removeEdge(e.start, e.finish);
    graph.removeEdge(e.finish, e.start);
  }

//  for (Edge &e : bridges) {
//    std::cout << e.start << ' ' << e.finish << '\n';
//    graph.removeEdge(e.start, e.finish);
//    graph.removeEdge(e.finish, e.start);
//  }
//
//  std::cout << '\n';

  std::vector<std::vector<int>> cyclicComponentsVertices;
  std::vector<std::vector<int>> cycles;
  std::tie(cyclicComponentsVertices, cycles) = getCyclicComponents(graph);

  for (int i = 0; i < cycles.size(); ++i) {
    std::vector<int> &vertices = cyclicComponentsVertices[i];
    std::vector<int> &cycle = cycles[i];

//    for (int v : vertices)
//      std::cout << v << ' ';
//
//    std::cout << '\n';
//
//    for (int v : cycle)
//      std::cout << v << ' ';
//
//    std::cout << '\n';

    if (!isPlanar(graph, vertices, cycle))
      return false;
  }

  return true;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  SetGraph graph(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;

    if (u != v) {
      graph.addEdge(u, v);
      graph.addEdge(v, u);
    }
  }

  if (solve(graph))
    std::cout << "YES\n";
  else
    std::cout << "NO\n";
}
