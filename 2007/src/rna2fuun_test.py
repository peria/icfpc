#!/usr/bin/python

import unittest
import rna2fuun

class Rna2FuunTest(unittest.TestCase):
  def setUp(self):
    self.fuun = rna2fuun.Rna2Fuun()

  def test_currentPixel_1(self):
    self.fuun.addColor(rna2fuun.TRANSPARENT)
    self.fuun.addColor(rna2fuun.OPAQUE)
    self.fuun.addColor(rna2fuun.OPAQUE)
    self.assertEqual(((0, 0, 0), 170),
                     self.fuun.currentPixel())

  def test_currentPixel_2(self):
    self.fuun.addColor(rna2fuun.BLACK)
    self.fuun.addColor(rna2fuun.YELLOW)
    self.fuun.addColor(rna2fuun.CYAN)
    self.assertEqual(((85, 170, 85), 255),
                     self.fuun.currentPixel())

  def test_currentPixel_3(self):
    self.fuun.addColor(rna2fuun.YELLOW)
    self.fuun.addColor(rna2fuun.TRANSPARENT)
    self.fuun.addColor(rna2fuun.OPAQUE)
    self.assertEqual(((127, 127, 0), 127),
                     self.fuun.currentPixel())

  def test_currentPixel_4(self):
    for _ in xrange(18):
      self.fuun.addColor(rna2fuun.BLACK)
    for _ in xrange(7):
      self.fuun.addColor(rna2fuun.RED)
    for _ in xrange(39):
      self.fuun.addColor(rna2fuun.MAGENTA)
    for _ in xrange(10):
      self.fuun.addColor(rna2fuun.WHITE)
    for _ in xrange(3):
      self.fuun.addColor(rna2fuun.OPAQUE)
    self.fuun.addColor(rna2fuun.TRANSPARENT)
    self.assertEqual(((143, 25, 125), 191),
                     self.fuun.currentPixel())

if __name__ == "__main__":
  suite = unittest.TestLoader().loadTestsFromTestCase(Rna2FuunTest)
  unittest.TextTestRunner(verbosity=2).run(suite)

