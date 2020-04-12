//
// Created by egork on 07.04.2020.
//
// report: https://contest.yandex.ru/contest/17594/run-report/32205639/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

struct ExchangeData {
  ExchangeData(double rate, double commission) : rate(rate), commission(commission) {}

  double rate;
  double commission;
};

template<typename T>
struct Edge {
  Edge(int start, int finish, T cost) : start(start), finish(finish), cost(cost) {}

  int start;
  int finish;
  T cost;
};

template<typename T>
class EdgeGraph {
 public:
  explicit EdgeGraph(int size, int expectedEdges = 0) : nodeCount(size) {
    if (expectedEdges)
      edges.reserve(expectedEdges);
  }

  void addEdge(int start, int finish, T cost) {
    assert(0 <= start && start < nodeCount);
    assert(0 <= finish && finish < nodeCount);
    edges.emplace_back(start, finish, cost);
  }

  const std::vector<Edge<T>> &getEdges() const {
    return edges;
  }

  int getNodeCount() const {
    return nodeCount;
  }

 private:
  int nodeCount;
  std::vector<Edge<T>> edges;
};

double calculateExchange(double money, ExchangeData exchangeData) {
  return (money - exchangeData.commission) * exchangeData.rate;
}

bool hasPositiveInputLoop(const EdgeGraph<ExchangeData> &graph,
                          int startCurrency,
                          double startAmount) {
  std::vector<double> amount(graph.getNodeCount(), 0.0);
  std::vector<double> newAmount(amount);
  amount[startCurrency] = startAmount;

  for (int i = 0; i < graph.getNodeCount() - 1; ++i) {
    newAmount = amount;

    for (Edge e : graph.getEdges())
      if (amount[e.start] > 0.0)
        newAmount[e.finish] =
            std::max(newAmount[e.finish], calculateExchange(amount[e.start], e.cost));

    std::swap(amount, newAmount);
  }

  for (Edge e : graph.getEdges())
    if (amount[e.finish] < calculateExchange(amount[e.start], e.cost))
      return true;

  return false;
}

int main() {
  int n, m, s;
  double v;
  std::cin >> n >> m >> s >> v;
  --s;
  EdgeGraph<ExchangeData> graph(n, m * 2);

  for (int i = 0; i < m; ++i) {
    int curA, curB;
    double rab, cab, rba, cba;
    std::cin >> curA >> curB >> rab >> cab >> rba >> cba;
    --curA;
    --curB;

    graph.addEdge(curA, curB, ExchangeData(rab, cab));
    graph.addEdge(curB, curA, ExchangeData(rba, cba));
  }

  std::cout << (hasPositiveInputLoop(graph, s, v) ? "YES" : "NO") << '\n';
}
