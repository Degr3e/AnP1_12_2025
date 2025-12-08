#include <iostream>
#include <cstddef>

namespace top {

  struct p_t {
    int x, y;
  };

  struct IDraw {
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
    virtual ~IDraw() = default;
  };

  bool operator == (p_t a, p_t b) {
    return a.x == b.x && a.y == b.y;
  }

  bool operator != (p_t a, p_t b) {
    return !(a == b);
  }

  struct Dot:IDraw {
    p_t begin() const override;
    p_t next(p_t) const override;
    p_t o;
    Dot(int x, int y);
  };

  struct LineH : IDraw {
    p_t begin() const override;
    p_t next(p_t) const override;
    p_t a, b;
    LineH(int x1, int y, int x2);
  };

  struct LineV : IDraw {
    p_t begin() const override;
    p_t next(p_t) const override;
    p_t a, b;
    LineV(int x, int y1, int y2);
  };



  struct frame_t {
    p_t left_bot;
    p_t right_top;
  };

  void make_f(IDraw ** b, size_t k); // b[0] = new Dot(0,0); b[1] = new Dot(-1, -5); b[2] = new Dot(1, 7);
  void get_points(IDraw * b, p_t ** ps, size_t & s); // p_s записать указатель на новый массив где будут старые точки + новые
  frame_t build_frame(const p_t * p, size_t s); // нужно найти максимум и минимум и сделать frame (0,0) (m, m)
  char * build_canvas(frame_t t); // нужно выделить память (max - min + 1) для каждого x и y
  void paint_canvas(char * cnv, frame_t fr, const p_t * ps, size_t k, char f); //координаты точек перевести в координаты канваса. У канваса координаты с прав нижнего угла. Найти минимальные координаты и это будет 0 в canvas
  void print_canvas(const char * cnv, frame_t fr); // вывести масив на экран. Лишние массивы выводить не надо
}

int main() {
  using namespace top;
  int err = 0;
  IDraw * f[3] = {};
  p_t * p = nullptr;
  size_t s = 0;
  char * cnv = nullptr;
  p_t ** kp = nullptr;
  try {
    make_f(f, 3);
    kp = new p_t *[0];
    for (size_t i = 0; i < 3; ++i) {
      get_points(f[i], kp, s);
    }
    frame_t fr = build_frame(p, s);
    cnv = build_canvas(fr);
    paint_canvas(cnv, fr, p, s, '#');
    print_canvas(cnv, fr);
  } catch (...) {
    err = 1;
    delete f[0];
    delete f[1];
    delete f[2];
    delete[] p;
    delete[] cnv;
    return err;
  }
}

top::frame_t top::build_frame(const p_t * p, size_t s)
{
  int xmin = p[0].x; int ymin = p[0].y;
  int xmax = xmax; int ymax = ymin;
  for (size_t i = 1; i < s; ++i) {
    xmin = std::min(xmin, p[i].x);
    ymin = std::min(ymin, p[i].y);
    xmax = std::max(xmax, p[i].x);
    ymax = std::max(ymax, p[i].y);
  }
  p_t a{xmin, ymin};
  p_t b{xmax, ymax};
  return frame_t{a, b};
}

top::Dot::Dot(int x, int y) : IDraw(), o{x, y}
{

}

top::p_t top::Dot::begin() const
{
  return o;
}

top::p_t top::Dot::next(p_t) const
{
  return begin();
}

top::LineH::LineH(int x1, int y, int x2) : a{x1, y}, b{x2, y}
{

}

top::p_t top::LineH::begin() const
{
  return a;
}

top::p_t top::LineH::next(p_t cur) const
{
  if (cur == b)
  {
    return cur;
  }

  return p_t{cur.x + (a.x < b.x ? 1 : -1), cur.y};
}


top::LineV::LineV(int x, int y1, int y2) : a{x, y1} , b{x, y2}
{

}

top::p_t top::LineV::begin() const
{
  return a;
}

top::p_t top::LineV::next(p_t cur) const
{
  if (cur == b) {
    return cur;
  }
  return p_t{cur.x, cur.y + (a.y < b.y ? 1 : -1)};
}