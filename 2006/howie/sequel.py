#!/usr/bin/python

import re
import subprocess

RE_item = re.compile('[Tt]here is an? ([^\.]+)')

class Umix(object):
  def __init__(self, initial_files=None):
    self._umix = subprocess.Popen(['../um/um', '../um/umix.um'],
                                  stdin=subprocess.PIPE,
                                  stdout=subprocess.PIPE)
    if initial_files is None:
      return

    if not isinstance(initial_files, (list, tuple)):
      initial_files = [initial_files]
    for filename in initial_files:
      with open(filename, 'r') as f:
        self.write(f.read())

  def until(self, line):
    while self.read().find(line) < 0:
      pass

  def until_debug(self, line):
    while True:
      output = self.read()
      print output.strip()
      if output.find(line) >= 0:
        return

  def read(self):
    return self._umix.stdout.readline().strip()

  def read_section(self):
    section = []
    line = ''
    while line == '':
      line = self.read()

    while line != '':
      section.append(line)
      line = self.read()
    return section

  def write(self, line):
    if isinstance(line, list):
      self._umix.stdin.write('\n'.join(line) + '\n')
    else:
      self._umix.stdin.write(line.strip() + '\n')


def get_items_list(umix):
  items = []
  while True:
    lines = ' '.join(umix.read_section()).split('. ')
    if lines[-1].find('south') >= 0:
      break
    if len(lines) == 1 and lines[0].find('>:') >= 0:
      continue
    if lines[0].find('You are in') < 0:
      continue

    for line in lines:
      m = RE_item.search(line)
      if not m:
        continue
      item = m.group(1).split()
      if item[-1] == 'here':
        item = item[:-1]
      if item[0] == '(broken)':
        item = item[1:]
      if len(item) == 1:
        item = [''] + item
      assert len(item) == 2
      items.append(item)
      umix.write('get %s %s' % (item[0], item[1]))
      umix.write('inc %s %s' % (item[0], item[1]))
      # print('get %s %s' % (item[0], item[1]))
      # print('inc %s %s' % (item[0], item[1]))
    umix.write('l')

  return items


def parse_missing(tokens):
    lvl = 0
    missing = []
    ts = []
    for token in tokens:
      lvl += token.count('(') - token.count(')')
      ts.append(token)
      if lvl == 0:
        if len(ts) == 1:
          ts = ts[0]
        else:
          ts[0] = ts[0][1:]  # Drop '('
          ts[-1] = ts[-1][:-1]  # Drop ')'
          ts = [t for t in ts if t != '']
          ts = parse_missing(ts)
        missing.append(ts)
        ts = []
    return missing

def get_items_details(umix, items):
  for item in items:
    umix.write('l %s %s' % (item[0], item[1]))
    umix.until('>: ')
    line = ' '.join(umix.read_section())
    if line.find('broken:') < 0:
      item.append(item[1])
      continue
    missing_str = line[line.find('broken:') + 14:-1]  # skip "broken: it is " and "."
    tokens = [t.replace('(a', '(')
              for t in missing_str.split(' ') if t not in ['a', 'missing', 'and']]
    missing = parse_missing(tokens)
    item.append(missing)

  for item in items:
    print item[2]
  return items


def main():
  targets = ['race car', 'pousse game', 'package robot',
             'carmine robber', 'finite-state machine', 'codex']

  for index, target in enumerate(targets):
    if index != 5:
      continue

    room_name = 'Part %d' % (index + 1)
    umix = Umix(['input3.txt'])
    umix.write(['n'] * (index + 1))
    umix.until(room_name)
    items = get_items_list(umix)

    umix = Umix(['input3.txt'])
    umix.write(['n'] * (index + 1))
    umix.until(room_name)
    items = get_items_details(umix, items)

if __name__ == '__main__':
  main()
