#!/usr/bin/python

import profile
import sys
import copy
from PIL import Image

OUTPUT_FILE = 'fuun.png'

# Constants
WIDTH  = 600
HEIGHT = 600
# RGB colors
BLACK   = (  0,   0,   0)
RED     = (255,   0,   0)
GREEN   = (  0, 255,   0)
YELLOW  = (255, 255,   0)
BLUE    = (  0,   0, 255)
MAGENTA = (255,   0, 255)
CYAN    = (  0, 255, 255)
WHITE   = (255, 255, 255)
# Transparencies
TRANSPARENT = 0
OPAQUE      = 255

# Clockwise = +1, Counter-Clockwise = -1
EAST  = 0
SOUTH = 1
WEST  = 2
NORTH = 3

TransparentBitmap = [[(BLACK, TRANSPARENT) for _ in xrange(WIDTH)]
                     for _ in xrange(HEIGHT)]

class Rna2Fuun:
  def __init__(self):
    self.bucket = []
    self.current_pixel = None
    self.position = (0, 0)
    self.mark = (0, 0)
    self.dir = EAST
    self.bitmaps = [[[((0,0,0), 0) for _ in xrange(WIDTH)] for _ in xrange(HEIGHT)]]
    self.current_bitmap = self.bitmaps[0]

  def build(self, rnas):
    rna_size = len(rnas)
    print 'Number of RNA is %d' % rna_size

    i = 0
    percent = 10
    for rna in rnas:
      i += 1
      if rna_size * percent / 100 == i:
        print '%d%% (%d/%d) is Done' % (percent, i, rna_size)
        percent += 10

      if   rna == 'PIPIIIC':
        self.addColor(BLACK)
      elif rna == 'PIPIIIP':
        self.addColor(RED)
      elif rna == 'PIPIICC':
        self.addColor(GREEN)
      elif rna == 'PIPIICF':
        self.addColor(YELLOW)
      elif rna == 'PIPIICP':
        self.addColor(BLUE)
      elif rna == 'PIPIIFC':
        self.addColor(MAGENTA)
      elif rna == 'PIPIIFF':
        self.addColor(CYAN)
      elif rna == 'PIPIIPC':
        self.addColor(WHITE)
      elif rna == 'PIPIIPF':
        self.addColor(TRANSPARENT)
      elif rna == 'PIPIIPP':
        self.addColor(OPAQUE)
      elif rna == 'PIIPICP':
        self.bucket = []
        self.current_pixel = None
      elif rna == 'PIIIIIP':
        self.position = self.move(self.position, self.dir)
      elif rna == 'PCCCCCP':
        self.dir = self.turnCounterClockwise(self.dir)
      elif rna == 'PFFFFFP':
        self.dir = self.turnClockwise(self.dir)
      elif rna == 'PCCIFFP':
        self.mark = self.position
      elif rna == 'PFFICCP':
        self.line(self.position, self.mark)
      elif rna == 'PIIPIIP':
        self.tryfill()
      elif rna == 'PCCPFFP':
        self.addBitmap()
      elif rna == 'PFFPCCP':
        self.compose()
      elif rna == 'PFFICCF':
        self.clip()

    self.draw(self.current_bitmap, OUTPUT_FILE)

  def addColor(self, c):
    self.bucket.append(c)
    self.current_pixel = None

  def currentPixel(self):
    # Cache the result
    if self.current_pixel is None:
      rgb_size, alpha_size = (0, 0)
      rc, gc, bc = (0, 0, 0)
      ac = 0
      for color in self.bucket:
        if isinstance(color, tuple):
          rgb_size += 1
          r, g, b = color
          rc += r
          gc += g
          bc += b
        else:
          alpha_size += 1
          ac += color

      rc = (rc / rgb_size) if rgb_size > 0 else 0
      gc = (gc / rgb_size) if rgb_size > 0 else 0
      bc = (bc / rgb_size) if rgb_size > 0 else 0
      ac = (ac / alpha_size) if alpha_size > 0 else 255
      self.current_pixel = ((rc * ac / 255, gc * ac / 255, bc * ac / 255), ac)

    return self.current_pixel

  @staticmethod
  def move(pos, d):
    dx = [1, 0, WIDTH - 1, 0]
    dy = [0, 1, 0, HEIGHT - 1]
    x, y = pos
    return ((x + dx[d]) % WIDTH, (y + dy[d]) % HEIGHT)

  @staticmethod
  def turnCounterClockwise(d):
    return (d + 3) % 4

  @staticmethod
  def turnClockwise(d):
    return (d + 1) % 4

  def getPixel(self, p):
    x, y = p
    return self.current_bitmap[y][x]

  def setPixel(self, p):
    x, y = p
    self.current_bitmap[y][x] = self.currentPixel()

  def line(self, p0, p1):
    x0, y0 = p0
    x1, y1 = p1
    deltax = x1 - x0
    deltay = y1 - y0
    d = max(abs(deltax), abs(deltay))
    c = 1 if deltax * deltay <= 0 else 0
    x = x0 * d + (d - c) / 2
    y = y0 * d + (d - c) / 2
    for _ in xrange(d):
      self.setPixel((x / d, y / d))
      x += deltax
      y += deltay
    self.setPixel((x1, y1))

  def tryfill(self):
    new = self.currentPixel()
    old = self.getPixel(self.position)
    if new[0] != old[0] or new[1] != old[1]:
      self.fill(self.position, old)

  def fill(self, pos, initial):
    def check_and_register(p):
      if self.getPixel(p) == initial:
        stack.append(p)

    stack = [pos]
    while len(stack) > 0:
      p = stack.pop()
      self.setPixel(p)
      x, y = p
      if x > 0:
        check_and_register((x - 1, y))
      if x < WIDTH - 1:
        check_and_register((x + 1, y))
      if y > 0:
        check_and_register((x, y - 1))
      if y < HEIGHT - 1:
        check_and_register((x, y + 1))

  def addBitmap(self):
    if len(self.bitmaps) < 10:
      self.bitmaps.append([[((0,0,0), 0) for _ in xrange(WIDTH)] for _ in xrange(HEIGHT)])
      self.current_bitmap = self.bitmaps[-1]

  def compose(self):
    if len(self.bitmaps) >= 2:
      bitmap0 = self.bitmaps.pop()
      bitmap1 = self.bitmaps[-1]
      for y in xrange(HEIGHT):
        for x in xrange(WIDTH):
          (r0, g0, b0), a0 = bitmap0[y][x]
          (r1, g1, b1), a1 = bitmap1[y][x]
          bitmap1[y][x] = ((r0 + r1 * (255 - a0) / 255,
                            g0 + g1 * (255 - a0) / 255,
                            b0 + b1 * (255 - a0) / 255),
                           a0 + a1 * (255 - a0) / 255)
      self.current_bitmap = self.bitmaps[-1]


  def clip(self):
    if len(self.bitmaps) >= 2:
      bitmap0 = self.bitmaps.pop()
      bitmap1 = self.bitmaps[-1]
      for y in xrange(HEIGHT):
        for x in xrange(WIDTH):
          _, a0 = bitmap0[y][x]
          (r1, g1, b1), a1 = bitmap1[y][x]
          bitmap1[y][x] = ((r1 * a0 / 255,
                            g1 * a0 / 255,
                            b1 * a0 / 255),
                            a1 * a0 / 255)
      self.current_bitmap = self.bitmaps[-1]

  # Parses computed array and draw it in a PNG file.
  def draw(self, bitmap, filename):
    img = Image.new('RGB', (WIDTH, HEIGHT))
    for y in xrange(HEIGHT):
      for x in xrange(WIDTH):
        color, _ = bitmap[y][x]
        img.putpixel((x, y), color)
    img.save(filename)
    print 'output %s' % filename


def main():
  rnas = []
  for rna in sys.stdin:
    rna = rna.rstrip()  # Remove new line code
    rnas.append(rna)

  if len(sys.argv) >= 2:
    n = int(sys.argv[1])
    rnas = rnas[0:n]

  fuun = Rna2Fuun()
  fuun.build(rnas)


if __name__ == "__main__":
  main()
