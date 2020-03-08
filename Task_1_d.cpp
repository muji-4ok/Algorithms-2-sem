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

std::vector<int> findCycle(int start, const SetSubGraph &graph) {
  std::vector<Edge> stack;
  stack.emplace_back(start, start);
  std::unordered_map<int, char> states;
  std::vector<int> cycle;

  while (!stack.empty()) {
    int vertex, parent;
    vertex = stack.back().finish;
    parent = stack.back().start;

    if (states[vertex] == 2) {
      stack.pop_back();
      continue;
    }

    states[vertex] = 1;

    for (int child : graph.getChildren(vertex)) {
      if (states[child] == 1 && child != parent) {
        int i = stack.size() - 1;
        std::unordered_map<int, bool> added;

        for (; stack[i].finish != child; --i) {
          int &element = stack[i].finish;

          if (states[element] == 1 && !added[element]) {
            cycle.push_back(element);
            added[element] = true;
          }
        }

        cycle.push_back(stack[i].finish);

        return cycle;
      } else if (states[child] == 0) {
        stack.emplace_back(vertex, child);
      }
    }

    while (!stack.empty() && states[stack.back().finish] == 1) {
      states[stack.back().finish] = 2;
      stack.pop_back();
    }
  }

  return cycle;
}

SetSubGraph fillComponent(int start,
                          const SetGraph &graph,
                          const std::unordered_set<int> &cutPoints,
                          std::vector<char> &states) {
  std::vector<Edge> stack;
  stack.emplace_back(start, start);
  SetSubGraph subGraph;
  subGraph.addNode(start);

  while (!stack.empty()) {
    int vertex, parent;
    vertex = stack.back().finish;
    parent = stack.back().start;

    if (states[vertex] == 2) {
      stack.pop_back();
      continue;
    }

    states[vertex] = 1;
    bool canAddNew = cutPoints.find(vertex) == cutPoints.end();

    for (int child : graph.getChildren(vertex)) {
      if ((states[child] == 1 || states[child] == 2) && child != parent) {
        subGraph.addEdge(vertex, child);
        subGraph.addEdge(child, vertex);
      } else if (states[child] == 0 && canAddNew) {
        subGraph.addNode(child);
        subGraph.addEdge(vertex, child);
        subGraph.addEdge(child, vertex);
        stack.emplace_back(vertex, child);
      }
    }

    while (!stack.empty() && states[stack.back().finish] == 1) {
      states[stack.back().finish] = 2;
      stack.pop_back();
    }
  }

  for (int v : subGraph.getVertices())
    if (cutPoints.find(v) == cutPoints.end())
      states[v] = 3;
    else
      states[v] = 0;

  return subGraph;
}

// Returns vertices of cyclic subgraphs and a cycle for each of them
std::tuple<std::vector<SetSubGraph>, std::vector<std::vector<int>>> getCyclicComponents(
    const SetGraph &graph,
    const std::unordered_set<int> &cutPoints
) {
  std::vector<char> states(graph.getNodeCount(), 0);
  std::vector<SetSubGraph> cyclicSubGraphs;
  std::vector<std::vector<int>> cycles;

  // Find all cyclic components
  for (int i = 0; i < graph.getNodeCount(); ++i) {
    if (states[i] == 0 && cutPoints.find(i) == cutPoints.end()) {
      SetSubGraph subGraph = fillComponent(i, graph, cutPoints, states);

//      std::cout << subGraph.toString();

      std::vector<int> cycle = findCycle(i, subGraph);

      if (!cycle.empty()) {
        cyclicSubGraphs.push_back(std::move(subGraph));
        cycles.push_back(cycle);
      }
    }
  }

  return {cyclicSubGraphs, cycles};
}

void walkCutPoints(int start,
                   const SetGraph &graph,
                   std::vector<int> &inTimes,
                   std::vector<int> &outTimes,
                   std::vector<char> &states,
                   std::unordered_set<int> &cutPoints) {
  std::vector<Edge> stack;
  stack.emplace_back(start, start);
  int time = 0;
  int startCuts = 0;

  while (!stack.empty()) {
    int vertex, parent;
    vertex = stack.back().finish;
    parent = stack.back().start;

    states[vertex] = 1;
    inTimes[vertex] = time;
    outTimes[vertex] = time;
    ++time;

    for (int child : graph.getChildren(vertex)) {
      if (states[child] >= 1 && child != parent) {
        outTimes[vertex] = std::min(outTimes[vertex], inTimes[child]);
      } else if (states[child] == 0) {
        stack.emplace_back(vertex, child);
      }
    }

    while (!stack.empty() && states[stack.back().finish] >= 1) {
      if (states[stack.back().finish] == 1) {
        vertex = stack.back().finish;
        parent = stack.back().start;
        outTimes[parent] = std::min(outTimes[parent], outTimes[vertex]);

        if (vertex != start && parent == start)
          ++startCuts;

        if (parent != start && outTimes[vertex] >= inTimes[parent])
          cutPoints.insert(parent);

        states[vertex] = 2;
      }

      stack.pop_back();
    }
  }

  if (startCuts >= 2)
    cutPoints.insert(start);
}

std::unordered_set<int> getCutPoints(const SetGraph &graph) {
  std::vector<int> inTimes(graph.getNodeCount());
  std::vector<int> outTimes(graph.getNodeCount());
  std::vector<char> states(graph.getNodeCount(), 0);
  std::unordered_set<int> cutPoints;

  for (int i = 0; i < graph.getNodeCount(); ++i)
    if (states[i] == 0)
      walkCutPoints(i, graph, inTimes, outTimes, states, cutPoints);

  return cutPoints;
}

//TODO: better organize face managing
//class FaceManager {
// public:
//  bool hasFace(int vertex, int face) const;
//
// private:
//};

void addEdgeSegments(int start,
                     const SetSubGraph &graph,
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
    const SetSubGraph &graph,
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
               std::vector<int> chain,
               std::unordered_map<int, std::vector<int>> &faces,
               std::unordered_map<int, std::set<int>> &facesOfVertices) {
  int start, end;
  std::tie(start, end) = findSplits(faces[face], chain.front(), chain.back());

  if (faces[face][start] == chain.back())
    std::reverse(chain.begin(), chain.end());

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

bool isPlanar(const SetSubGraph &graph,
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

    for (auto it = segments.begin(); it != segments.end(); ++it)
      if (getCommonFaces(it->second, facesOfVertices)
          < getCommonFaces(minSegment->second, facesOfVertices))
        minSegment = it;

//    std::cout << "Min Segment: \n";
//    std::cout << minSegment->first.toString();

    if (getCommonFaces(minSegment->second, facesOfVertices).empty())
      return false;

    laySegment(minSegment,
               segments,
               laidGraph,
               segmentEdges,
               facesOfVertices,
               faces,
               addedVertices);

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
//
//    std::cout << "LaidGraph: \n";
//    std::cout << laidGraph.toString() << '\n';
//
//    std::cout << "SegmentEdges: \n";
//    std::cout << segmentEdges.toString() << '\n';
  }
}

bool solve(SetGraph &graph) {
  std::unordered_set<int> cutPoints = getCutPoints(graph);

//  for (int v : cutPoints)
//    std::cout << v << ' ';
//
//  std::cout << '\n';

  std::vector<SetSubGraph> cyclicSubGraphs;
  std::vector<std::vector<int>> cycles;
  std::tie(cyclicSubGraphs, cycles) = getCyclicComponents(graph, cutPoints);

  for (int i = 0; i < cycles.size(); ++i) {
    SetSubGraph &subGraph = cyclicSubGraphs[i];
    std::vector<int> &cycle = cycles[i];

//    std::cout << subGraph.toString() << '\n';
//
//    std::cout << "Cycle: ";
//
//    for (int v : cycle)
//      std::cout << v << ' ';
//
//    std::cout << '\n';

    if (!isPlanar(subGraph, cycle))
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
