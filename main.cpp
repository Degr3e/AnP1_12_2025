#include <cstddef>
#include <iostream>

namespace top
{
  struct p_t
  {
    int x, y;
  };

  struct IDraw
  {
    virtual ~IDraw() = default;
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
    virtual bool is_last(p_t) const = 0;
  };

  struct Dot: IDraw
  {
    Dot(int x, int y);
    explicit Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t) const override;
    bool is_last(p_t) const override;
    p_t d;
  };

  struct Vert_line: IDraw
  {
    Vert_line(int x, int starty, int endy);
    p_t begin() const override;
    p_t next(p_t) const override;
    bool is_last(p_t) const override;

    private:
    int x;
    int sy;
    int ey;
  };

  struct Hor_line: IDraw
  {
    Hor_line(int sx, int endx, int y);
    p_t begin() const override;
    p_t next(p_t) const override;
    bool is_last(p_t) const override;

    private:
    int sx;
    int ex;
    int y;
  };

  struct Alfa_line: IDraw
  {
    Alfa_line(p_t a, p_t b);
    p_t begin() const override;
    p_t next(p_t) const override;
    bool is_last(p_t) const override;

    private:
    p_t start;
    p_t finish;
  };

  struct frame_t
  {
    p_t left_bot;
    p_t right_top;
  };

  bool operator==(p_t a, p_t b)
  {
    return a.x == b.x && a.y == b.y;
  }

  bool operator!=(p_t a, p_t b)
  {
    return !(a == b);
  }

  size_t rows(frame_t f)
  {
    return f.right_top.y - f.left_bot.y + 1;
  }

  size_t cols(frame_t f)
  {
    return f.right_top.x - f.left_bot.x + 1;
  }

  size_t get_points(IDraw* b, p_t** ps, size_t s)
  {
    size_t k = 1;
    p_t current = b->begin();

    while(!b->is_last(current))
    {
      ++k;
      current = b->next(current);
    }

    p_t* arr_p = new p_t[s + k];

    for(size_t i = 0; i < s; ++i)
    {
      arr_p[i] = (*ps)[i];
    }

    current = b->begin();
    for(size_t i = 0; i < k; ++i)
    {
      arr_p[s + i] = current;
      if(!b->is_last(current))
        current = b->next(current);
    }

    delete[] *ps;
    *ps = arr_p;
    return s + k;
  }

  frame_t build_frame(const p_t* ps, size_t s)
  {
    if(!s)
    {
      throw std::logic_error("bad size");
    }

    int minx = ps[0].x, miny = ps[0].y,
    maxx = ps[0].x, maxy = ps[0].y;

    for (size_t i = 1; i < s; ++i)
    {
      minx = (ps[i].x < minx)? ps[i].x : minx;
      maxx = (ps[i].x > maxx)? ps[i].x : maxx;
      miny = (ps[i].y < miny)? ps[i].y : miny;
      maxy = (ps[i].y > maxy)? ps[i].y : maxy;
    }
    p_t aa{minx, miny};
    p_t bb{maxx, maxy};
    frame_t ans = {aa, bb};
    return ans;
  }

  char* build_canvas(frame_t f, char fill)
  {
    size_t size = rows(f) * cols(f);
    char* n = new char[size];
    for(size_t i = 0; i < size; ++i)
    {
      n[i] = fill;
    }
    return n;
  }

  void paint_canvas(char* cnv, frame_t fr, p_t* p, char f)
  {
    int dx = p->x - fr.left_bot.x;
    int dy = fr.right_top.y - p->y;
    cnv[dy * cols(fr) + dx] = f;
  }

  void print_canvas(std::ostream& os, char* cnv, frame_t fr)
  {
    for(size_t i = 0; i < rows(fr); ++i)
    {
      for(size_t j = 0; j < cols(fr); ++j)
      {
        os << cnv[i * cols(fr) + j];
      }
      os << "\n";
    }
  }
}


int main()
{
  using namespace top;
  int err = 0;
  IDraw* f[4] = {};
  p_t * pts = nullptr;
  size_t s = 0;

  try
  {
    f[0] = new Dot(0, 0);
    f[1] = new Vert_line(5, -3, 3);
    f[2] = new Hor_line(-5, 0, -2);
    f[3] = new Alfa_line({-3, 3}, {6, 12});

    for(size_t i = 0; i < 4; ++i)
    {
      s = get_points((f[i]), &pts, s);
    }

    frame_t fr = build_frame(pts, s);
    char* cnv = build_canvas(fr, '.');

    for (size_t i = 0; i < s; ++i)
    {
      paint_canvas(cnv, fr, pts + i, '#');
    }

    print_canvas(std::cout, cnv, fr);

    delete[] cnv;
    delete[] pts;
    delete f[0];
    delete f[1];
    delete f[2];
    delete f[3];

  } catch (...) {
    err = 2;
    std::cerr << "Bad drawing\n";
  }
  return err;
}

top::Dot::Dot(int x, int y):
  IDraw(),
  d{x, y}
{}

top::Dot::Dot(p_t p):
  IDraw(),
  d{p}
{}

top::p_t top::Dot::begin() const
{
  return d;
}

top::p_t top::Dot::next(p_t prev) const
{
  return d;
}

bool top::Dot::is_last(p_t p) const
{
  return true;
}

top::Vert_line::Vert_line(int xx, int syy, int eyy):
  IDraw(),
  x{xx}, sy{syy}, ey{eyy}
{}

top::p_t top::Vert_line::begin() const
{
  return {x, sy};
}

top::p_t top::Vert_line::next(p_t p) const
{
  if (p.y < ey)
    return {x, p.y + 1};
  return {x, ey};
}

bool top::Vert_line::is_last(p_t p) const
{
  return p.y == ey;
}

top::Hor_line::Hor_line(int sxx, int exx, int yy):
  IDraw(),
  sx{sxx}, ex{exx}, y{yy}
{}

top::p_t top::Hor_line::begin() const
{
  return {sx, y};
}

top::p_t top::Hor_line::next(p_t p) const
{
  if (p.x < ex)
    return {p.x + 1, y};
  return {ex, y};
}

bool top::Hor_line::is_last(p_t p) const
{
  return p.x == ex;
}

top::Alfa_line::Alfa_line(p_t a, p_t b):
  IDraw(),
  start{a}, finish{b}
{}

top::p_t top::Alfa_line::begin() const
{
  return start;
}

top::p_t top::Alfa_line::next(p_t p) const
{
  if (p == finish) return finish;

  int dx = (finish.x > p.x) ? 1 : -1;
  int dy = (finish.y > p.y) ? 1 : -1;

  if (finish.x != p.x && finish.y != p.y)
    return {p.x + dx, p.y + dy};
  else if (finish.x != p.x)
    return {p.x + dx, p.y};
  else
    return {p.x, p.y + dy};
}

bool top::Alfa_line::is_last(p_t p) const
{
  return p == finish;
}