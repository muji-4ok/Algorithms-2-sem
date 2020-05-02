#include "geometry.h"
bool eq(double a, double b) {
  return std::abs(a - b) < ERROR;
}
double Ellipse::perimeter() const {
  double a = majorSemiAxis();
  double b = minorSemiAxis();
  return M_PI * (3 * (a + b) - std::sqrt(10 * a * b + 3 * (a * a + b * b)));
}
double Ellipse::area() const {
  double a = majorSemiAxis();
  double b = minorSemiAxis();
  return M_PI * a * b;
}
bool Ellipse::operator==(const Shape &other) const {
  auto otherEllipse = dynamic_cast<const Ellipse *>(&other);

  if (otherEllipse) {
    return center() == otherEllipse->center() && eq(dist, otherEllipse->dist);
  } else {
    return false;
  }
}
bool Ellipse::operator!=(const Shape &other) const {
  return !(*this == other);
}
bool Ellipse::isCongruentTo(const Shape &other) const {
  auto otherEllipse = dynamic_cast<const Ellipse *>(&other);

  if (otherEllipse) {
    return (f2 - f1).sqrDist() == (otherEllipse->f2 - otherEllipse->f1).sqrDist()
        && dist == otherEllipse->dist;
  } else {
    return false;
  }
}
bool Ellipse::isSimilarTo(const Shape &other) const {
  auto otherEllipse = dynamic_cast<const Ellipse *>(&other);

  if (otherEllipse) {
    double a = majorSemiAxis();
    double b = minorSemiAxis();
    double otherA = otherEllipse->majorSemiAxis();
    double otherB = otherEllipse->minorSemiAxis();
    double coeff = a / b;
    return otherA / otherB == coeff || otherB / otherA == coeff;
  } else {
    return false;
  }
}
bool Ellipse::containsPoint(Point point) const {
  return (f1 - point).dist() + (f2 - point).dist() <= dist;
}
void Ellipse::rotate(Point _center, double angle) {
  Point onEllipse = center() + getUpVector() * minorSemiAxis();
  f1.rotate(_center, angle);
  f2.rotate(_center, angle);
  onEllipse.rotate(_center, angle);
  dist = (f1 - onEllipse).dist() + (f2 - onEllipse).dist();
}
void Ellipse::reflex(Point _center) {
  Point onEllipse = center() + getUpVector() * minorSemiAxis();
  f1.reflex(_center);
  f2.reflex(_center);
  onEllipse.reflex(_center);
  dist = (f1 - onEllipse).dist() + (f2 - onEllipse).dist();
}
void Ellipse::reflex(Line axis) {
  Point onEllipse = center() + getUpVector() * minorSemiAxis();
  f1.reflex(axis);
  f2.reflex(axis);
  onEllipse.reflex(axis);
  dist = (f1 - onEllipse).dist() + (f2 - onEllipse).dist();

}
void Ellipse::scale(Point _center, double coeff) {
  Point onEllipse = center() + getUpVector() * minorSemiAxis();
  f1.scale(_center, coeff);
  f2.scale(_center, coeff);
  onEllipse.scale(_center, coeff);
  dist = (f1 - onEllipse).dist() + (f2 - onEllipse).dist();

}
double Ellipse::minorSemiAxis() const {
  return std::sqrt(dist * dist / 4.0 - (center() - f1).sqrDist());
}
double Ellipse::majorSemiAxis() const {
  return (dist - (f1 - f2).dist()) / 2.0 + (center() - f1).dist();
}
std::pair<Point, Point> Ellipse::focuses() const {
  return std::make_pair(f1, f2);
}
std::pair<Line, Line> Ellipse::directrices() const {
  assert(f1 != f2);

  Point upVec = getUpVector();
  Point sideVec = (f1 - center());
  sideVec.normalize();

  double a = majorSemiAxis();
  double e = eccentricity();

  Point edge1 = center() + sideVec * (a / e);
  Point edgeOffset1 = edge1 + upVec;
  Point edge2 = center() - sideVec * (a / e);
  Point edgeOffset2 = edge2 + upVec;

  return std::make_pair(Line(edge1, edgeOffset1), Line(edge2, edgeOffset2));
}
double Ellipse::eccentricity() const {
  double a = majorSemiAxis();
  double b = minorSemiAxis();
  return sqrt(1 - b * b / (a * a));
}
Point Ellipse::center() const {
  return (f1 + f2) * 0.5;
}
Ellipse::Ellipse(Point f1, Point f2, double dist) : f1(f1), f2(f2), dist(dist) {
  assert(dist > 0);
}
Point Ellipse::getUpVector() const {
  if (f1 == f2) {
    return Point(0.0, 1.0);
  } else {
    Point normal = (f2 - f1).orthogonal();
    normal.normalize();
    return normal;
  }
}
Point &Point::operator+=(const Point &other) {
  x += other.x;
  y += other.y;
  return *this;
}
Point &Point::operator-=(const Point &other) {
  x -= other.x;
  y -= other.y;
  return *this;
}
Point &Point::operator*=(double scale) {
  x *= scale;
  y *= scale;
  return *this;
}
Point &Point::operator/=(double scale) {
  assert(scale != 0.0);
  x /= scale;
  y /= scale;
  return *this;
}
Point Point::operator+(const Point &other) const {
  Point copy(*this);
  copy += other;
  return copy;
}
Point Point::operator-(const Point &other) const {
  Point copy(*this);
  copy -= other;
  return copy;
}
Point Point::operator*(double scale) {
  Point copy(*this);
  copy *= scale;
  return copy;
}
Point Point::operator/(double scale) {
  Point copy(*this);
  copy /= scale;
  return copy;
}
void Point::rotate(Point center, double angle) {
  *this -= center;
  angle *= -M_PI / 180.0;

  double s = std::sin(angle);
  double c = std::cos(angle);

  double newX = x * c - y * s;
  double newY = x * s + y * c;
  x = newX;
  y = newY;

  *this += center;
}
void Point::reflex(Point center) {
  *this += (center - *this) * 2.0;
}
void Point::scale(Point center, double coeff) {
  Point diff = *this - center;
  diff *= coeff;
  *this = center + diff;
}
void Point::reflex(Line axis) {
  Point projection = axis.projectOn(*this);
  *this += (projection - *this) * 2.0;
}
double Point::dot(Point other) const {
  return x * other.x + y * other.y;
}
bool Point::isCollinear(Point other) const {
  double coeff = !eq(x, 0.0) ? other.x / x : other.y / y;
  return eq(x * coeff, other.x) && eq(y * coeff, other.y);
}
int Point::orientation(Point other) const {
  double product = cross(other);

  if (product < 0.0)
    return 1;
  else if (product > 0.0)
    return -1;
  else
    return 0;
}
double Point::cross(Point other) const {
  return x * other.y - y * other.x;
}
Point Line::projectOn(Point p) const {
  p -= p1;
  return p1 + (direction() * (p.dot(direction()) / direction().sqrDist()));
}
Circle::Circle(Point _center, double radius) : Ellipse(_center, _center, radius * 2.0) {
}
double Circle::radius() const {
  return dist * 0.5;
}
bool Polygon::operator==(const Shape &other) const {
  auto otherP = dynamic_cast<const Polygon *>(&other);

  if (otherP) {
    if (points.size() != otherP->points.size())
      return false;

    for (int toReverse = 0; toReverse < 2; ++toReverse) {
      for (int shift = 0; shift < points.size(); ++shift) {
        for (int i = 0; i < points.size(); ++i) {
          int j;

          if (toReverse)
            j = modulo(points.size() - (i + 1) + shift, points.size());
          else
            j = modulo(i + shift, points.size());

          if (points[i] != otherP->points[j])
            goto nextShift;
        }

        return true;

        nextShift:;
      }
    }

    return false;
  } else {
    return false;
  }
}
bool Polygon::operator!=(const Shape &other) const {
  return !(*this == other);
}
double Polygon::perimeter() const {
  double result = 0.0;

  for (int i = 0; i < points.size(); ++i) {
    int j = (i + 1) % (int) points.size();
    result += (points[j] - points[i]).dist();
  }

  return result;
}
double Polygon::area() const {
  double result = 0.0;

  for (int i = 0; i < points.size(); ++i) {
    int j = (i + 1) % (int) points.size();
    // Ориентированная площадь треугольника (0, p1, p2)
    result += points[i].cross(points[j]) * 0.5;
  }

  return std::abs(result);
}
bool Polygon::isCongruentTo(const Shape &other) const {
  auto otherP = dynamic_cast<const Polygon *>(&other);

  if (otherP) {
    return isSimilarTo(otherP, true);
  } else {
    return false;
  }
}
bool Polygon::isSimilarTo(const Shape &other) const {
  auto otherP = dynamic_cast<const Polygon *>(&other);

  if (otherP) {
    return isSimilarTo(otherP, false);
  } else {
    return false;
  }
}
int modulo(int x, int mod) {
  return (x + mod) % mod;
}
bool Polygon::isSimilarTo(const Polygon *other, bool strictLengths) const {
  if (points.size() != other->points.size())
    return false;

  for (int toReverse = 0; toReverse < 2; ++toReverse) {
    for (int shift = 0; shift < points.size(); ++shift) {
      int other0, other1;
      if (toReverse) {
        other0 = modulo(points.size() - (1 + shift), points.size());
        other1 = modulo(points.size() - (2 + shift), points.size());
      } else {
        other0 = shift;
        other1 = modulo(1 + shift, points.size());
      }

      double coeff =
          strictLengths ?
          1.0 :
          (points[1] - points[0]).dist() / (other->points[other1] - other->points[other0]).dist();
      bool canReflect = true;
      double reflection = 1.0;

      for (int cur = 0; cur < points.size(); ++cur) {
        int prev = modulo(cur - 1, points.size());
        int next = modulo(cur + 1, points.size());

        int prevOther, curOther, nextOther;

        if (toReverse) {
          prevOther = modulo(points.size() - (prev + 1 + shift), points.size());
          curOther = modulo(points.size() - (cur + 1 + shift), points.size());
          nextOther = modulo(points.size() - (next + 1 + shift), points.size());
        } else {
          prevOther = modulo(prev + shift, points.size());
          curOther = modulo(cur + shift, points.size());
          nextOther = modulo(next + shift, points.size());
        }

        Point curEdge = points[next] - points[cur];
        Point curEdgeOther = other->points[nextOther] - other->points[curOther];

        // Подобны ли стороны
        if (!eq(curEdge.dist(), curEdgeOther.dist() * coeff))
          goto nextShift;

        Point prevEdge = points[cur] - points[prev];
        Point prevEdgeOther = other->points[curOther] - other->points[prevOther];
        curEdge.normalize();
        prevEdge.normalize();
        curEdgeOther.normalize();
        prevEdgeOther.normalize();

        double angle = prevEdge.cross(curEdge);
        double angleOther = prevEdgeOther.cross(curEdgeOther);

        if (canReflect) {
          bool areEq = eq(angle, angleOther);
          canReflect = false;
          reflection = areEq ? 1.0 : -1.0;
        }

        if (!eq(angle, reflection * angleOther))
          goto nextShift;
      }

      return true;

      nextShift:;
    }
  }

  return false;
}
bool Polygon::isConvex() const {
  int targetOrientation = (points[1] - points[0]).orientation(points[2] - points[1]);

  for (int cur = 0; cur < points.size(); ++cur) {
    int prev = (cur - 1 + (int) points.size()) % (int) points.size();
    int next = (cur + 1) % (int) points.size();
    Point curEdge = points[next] - points[cur];
    Point prevEdge = points[cur] - points[prev];
    int orientation = prevEdge.orientation(curEdge);

    if (!orientation)
      continue;

    if (targetOrientation) {
      // Поворот в другую сторону
      if (orientation != targetOrientation)
        return false;
    } else {
      targetOrientation = orientation;
    }
  }

  return true;
}
bool segmentIntersectsPositiveRay(Point p1, Point p2, Point start) {
  // Мы сильно левее старта
  if (p1.x < start.x && p2.x < start.x)
    return false;

  // Мы сильно выше
  if (p1.y > start.y && p2.y > start.y)
    return false;

  // Мы сильно ниже
  if (p1.y < start.y && p2.y < start.y)
    return false;

  // Теперь мы точно пересекаем прямую (но не обязательно луч)

  // Мы сильно правее
  if (p1.x >= start.x && p2.x >= start.x)
    return true;

  Point segmentDir = p2 - p1;

  if (segmentDir.y < 0) {
    segmentDir.x = -segmentDir.x;
    segmentDir.y = -segmentDir.y;
    start -= p2;
  } else {
    start -= p1;
  }

  return segmentDir.orientation(start) <= 0;
}
bool Polygon::containsPoint(Point point) const {
  int intersections = 0;

  for (int i = 0; i < points.size(); ++i) {
    int j = (i + 1) % (int) points.size();

    // Луч пересекает нашу вершину => один раз посчитаем, но не больше
    if (eq(point.y, points[i].y) && points[i].x >= point.x)
      continue;

    intersections += segmentIntersectsPositiveRay(points[i], points[j], point);
  }

  return intersections % 2;
}
void Polygon::rotate(Point center, double angle) {
  for (Point &p : points)
    p.rotate(center, angle);
}
void Polygon::reflex(Point center) {
  for (Point &p : points)
    p.reflex(center);
}
void Polygon::reflex(Line axis) {
  for (Point &p : points)
    p.reflex(axis);
}
void Polygon::scale(Point center, double coeff) {
  for (Point &p : points)
    p.scale(center, coeff);
}
Polygon::Polygon(const std::vector<Point> &points) : points(points) {
  assert(points.size() >= 3);
}
Point Rectangle::center() const {
  return (points[0] + points[1] + points[2] + points[3]) / 4.0;
}
std::pair<Line, Line> Rectangle::diagonals() const {
  return std::make_pair(Line(points[0], points[2]), Line(points[1], points[3]));
}
Rectangle::Rectangle(Point corner1, Point corner2, double coeff) : Polygon(Point(),
                                                                           Point(),
                                                                           Point(),
                                                                           Point()) {
  Point diagonal = corner2 - corner1;
  double smallLen = std::sqrt(diagonal.sqrDist() / (1 + coeff * coeff));
  double bigLen = smallLen * coeff;

  if (smallLen > bigLen)
    std::swap(smallLen, bigLen);

  Point smallDir = diagonal;
  smallDir.normalize();
  smallDir.rotate(Point(0, 0), -std::atan(bigLen / smallLen) * 180.0 / M_PI);
  smallDir *= smallLen;
  Point corner3 = corner1 + smallDir;

  Point bigDir = smallDir;
  bigDir.normalize();
  bigDir.rotate(Point(0, 0), 90.0);
  bigDir *= bigLen;
  Point corner4 = corner1 + bigDir;

  points[0] = corner1;
  points[1] = corner3;
  points[2] = corner2;
  points[3] = corner4;
}
Circle Square::circumscribedCircle() const {
  return Circle(center(), (points[2] - points[0]).dist() * 0.5);
}
Circle Square::inscribedCircle() const {
  return Circle(center(), (points[1] - points[0]).dist() * 0.5);
}
Square::Square(Point corner1, Point corner2) : Rectangle(corner1, corner2, 1.0) {
}
Triangle::Triangle(Point p1, Point p2, Point p3) : Polygon(p1, p2, p3) {
}
Circle Triangle::circumscribedCircle() const {
  Point a = points[0];
  Point b = points[1];
  Point c = points[2];
  Point ab = b - a;
  Point ac = c - a;
  Point bc = c - b;

  double angleA =
      std::acos((ab.sqrDist() + ac.sqrDist() - bc.sqrDist()) / (2.0 * ab.dist() * ac.dist()));
  double sinA = std::sin(angleA);
  double r = bc.dist() / (2.0 * sinA);
  double orthoAbLen = std::sqrt(r * r - (ab * 0.5).sqrDist());
  double orthoAcLen = std::sqrt(r * r - (ac * 0.5).sqrDist());

  // Ровно одна из нормалей смотрит внутрь треугольника
  Point abNormal = ab * 0.5;
  abNormal.normalize();
  abNormal *= orthoAbLen;
  abNormal.rotate(Point(0.0, 0.0), -90.0);

  Point acNormal = ac * 0.5;
  acNormal.normalize();
  acNormal *= orthoAcLen;
  acNormal.rotate(Point(0.0, 0.0), -90.0);

  Point acCenter = (a + c) + acNormal;
  Point abCenter = (a + b) + abNormal;

  Point circC = eq(bc.dot(acCenter - (b + c) * 0.5), 0.0) ? acCenter : abCenter;

  double d0 = (circC - points[0]).dist();
  double d1 = (circC - points[1]).dist();
  double d2 = (circC - points[2]).dist();

  return Circle(circC, r);
}
Circle Triangle::inscribedCircle() const {
  Point a = points[0];
  Point b = points[1];
  Point c = points[2];
  Point ab = b - a;
  Point ac = c - a;
  Point bc = c - b;

  double angleAHalf =
      0.5 * std::acos((ab.sqrDist() + ac.sqrDist() - bc.sqrDist()) / (2.0 * ab.dist() * ac.dist()));
  double angleBHalf =
      0.5 * std::acos((ab.sqrDist() + bc.sqrDist() - ac.sqrDist()) / (2.0 * ab.dist() * bc.dist()));
  double anglePhi = M_PI - angleAHalf - angleBHalf;
  double aoLen = ab.dist() * std::sin(angleBHalf) / std::sin(anglePhi);

  ab.normalize();
  ac.normalize();

  Point biss = (ac + ab) * 0.5;
  biss.normalize();
  biss *= aoLen;

  Point insCenter = a + biss;

  Point pA = Line(b, c).projectOn(insCenter);
  Point pB = Line(a, c).projectOn(insCenter);
  Point pC = Line(a, b).projectOn(insCenter);

  double dA = (pA - insCenter).dist();
  double dB = (pB - insCenter).dist();
  double dC = (pC - insCenter).dist();

  return Circle(insCenter, (insCenter - Line(a, b).projectOn(insCenter)).dist());
}
Point Triangle::centroid() const {
  return (points[0] + points[1] + points[2]) / 3.0;
}
Point Triangle::orthocenter() const {
  Point a = points[0];
  Point b = points[1];
  Point c = points[2];
  Point hA = Line(b, c).projectOn(a);
  Point hB = Line(a, c).projectOn(b);

  double tgA = (hA - c).dist() / (a - hA).dist();
  double minusHeightLen = tgA * (a - hB).dist();
  Point heightOffset = hB - b;
  double heightLen = heightOffset.dist();
  heightOffset.normalize();
  heightOffset *= heightLen - minusHeightLen;

  Point hC = Line(a, b).projectOn(c);

  double dotA = (hA - (b + heightOffset)).dot(b - c);
  double dotB = (hB - (b + heightOffset)).dot(a - c);
  double dotC = (hC - (b + heightOffset)).dot(a - b);

  return b + heightOffset;
}
Line Triangle::EulerLine() const {
  return Line(centroid(), orthocenter());
}
Circle Triangle::ninePointsCircle() const {
  Point c = (orthocenter() + circumscribedCircle().center()) * 0.5;
  return Circle(c, ((points[0] + points[1]) * 0.5 - c).dist());
}
