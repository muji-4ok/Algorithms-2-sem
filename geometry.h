//
// Created by egork on 12.04.2020.
//
#pragma once
#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>

constexpr double ERROR = 1e-8;

bool eq(double a, double b);

class Line;

struct Point {
  explicit Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}

  double dist() const { return std::hypot(x, y); }
  double sqrDist() const { return x * x + y * y; }
  void normalize() { if (*this) *this /= dist(); }
  void rotate(Point center, double angle);
  void reflex(Point center);
  void reflex(Line axis);
  void scale(Point center, double coeff);
  double dot(Point other) const;
  bool isCollinear(Point other) const;
  Point orthogonal() const { return Point(-y, x); }
  // У нас правостороняя система координат
  // Если поворот *this -> other по меньшему по модулю углу:
  // 1) по часовой: return = 1
  // 2) против часовой: return = -1
  // 3) векторы коллинеарны, угол 0/180: return 0
  int orientation(Point other) const;
  // Ведет себя как угол * какой-то коэффициент
  double cross(Point other) const;

  explicit operator bool() const { return x != 0.0 || y != 0.0; }

  bool operator==(const Point &other) const { return eq(x, other.x) && eq(y, other.y); }
  bool operator!=(const Point &other) const { return !(*this == other); }

  Point &operator+=(const Point &other);
  Point &operator-=(const Point &other);
  Point &operator*=(double scale);
  Point &operator/=(double scale);
  Point operator+(const Point &other) const;
  Point operator-(const Point &other) const;
  Point operator*(double scale);
  Point operator/(double scale);

  double x;
  double y;
};

class Line {
 public:
  Line(Point p1, Point p2) : p1(p1), p2(p2) { assert(p1 != p2); }
  Line(double tan, double shift) : Line(Point(0.0, shift), tan) {
  }
  Line(Point p, double tan) : p1(p), p2(p.x + 1.0, p.y + tan) {}

  // Не нормализованы
  Point orthogonal() const { return direction().orthogonal(); }
  Point direction() const { return p2 - p1; }
  bool contains(Point p) const { return (p1 - p2).isCollinear(p1 - p); }
  Point projectOn(Point p) const;

  bool operator==(const Line &other) const {
    Point diff = p1 - p2;
    Point diffOther = other.p1 - other.p2;
    return diff.isCollinear(diffOther) && contains(other.p1);
  }

  bool operator!=(const Line &other) const { return !(*this == other); }

 protected:
  Point p1;
  Point p2;
};

struct Shape {
  virtual ~Shape() = default;

  virtual bool operator==(const Shape &other) const = 0;
  virtual bool operator!=(const Shape &other) const = 0;

  virtual double perimeter() const = 0;
  virtual double area() const = 0;
  virtual bool isCongruentTo(const Shape &other) const = 0;
  virtual bool isSimilarTo(const Shape &other) const = 0;
  virtual bool containsPoint(Point point) const = 0;

  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coeff) = 0;
};

class Ellipse : public Shape {
 public:
  Ellipse(Point f1, Point f2, double dist);
  ~Ellipse() override = default;

  bool operator==(const Shape &other) const override;
  bool operator!=(const Shape &other) const override;

  double perimeter() const override;
  double area() const override;
  virtual double minorSemiAxis() const;
  virtual double majorSemiAxis() const;

  std::pair<Point, Point> focuses() const;
  std::pair<Line, Line> directrices() const;
  double eccentricity() const;
  Point center() const;

  bool isCongruentTo(const Shape &right) const override;
  bool isSimilarTo(const Shape &right) const override;
  bool containsPoint(Point point) const override;
  void rotate(Point _center, double angle) override;
  void reflex(Point _center) override;
  void reflex(Line axis) override;
  void scale(Point _center, double coeff) override;

 protected:
  // Нормальный
  Point getUpVector() const;

  Point f1;
  Point f2;
  double dist;
};

class Circle : public Ellipse {
 public:
  Circle(Point _center, double radius);

  double radius() const;
};

int modulo(int x, int mod);

// Считаем что наш луч идет по направлению x -> +oo
bool segmentIntersectsPositiveRay(Point p1, Point p2, Point start);

class Polygon : public Shape {
 public:
  explicit Polygon(const std::vector<Point> &points);
  template<typename... T>
  explicit Polygon(T... points) : points({points...}) {
    assert(this->points.size() >= 3);
  }
  ~Polygon() override = default;

  bool operator==(const Shape &other) const override;
  bool operator!=(const Shape &other) const override;

  double perimeter() const override;
  double area() const override;

  bool isCongruentTo(const Shape &other) const override;
  bool isSimilarTo(const Shape &other) const override;
  bool containsPoint(Point point) const override;

  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coeff) override;

  int verticesCount() const { return points.size(); }
  const std::vector<Point> &getVertices() const { return points; }
  bool isConvex() const;

 protected:
  std::vector<Point> points;

 private:
  bool isSimilarTo(const Polygon *other, bool strictLengths) const;
};

class Rectangle : public Polygon {
 public:
  Rectangle(Point corner1, Point corner2, double coeff);

  std::pair<Line, Line> diagonals() const;
  Point center() const;
};

class Square : public Rectangle {
 public:
  Square(Point corner1, Point corner2);

  Circle circumscribedCircle() const;
  Circle inscribedCircle() const;
};

class Triangle : public Polygon {
 public:
  Triangle(Point p1, Point p2, Point p3);

  Circle circumscribedCircle() const;
  Circle inscribedCircle() const;
  Point centroid() const;
  Point orthocenter() const;
  Line EulerLine() const;
  Circle ninePointsCircle() const;
};