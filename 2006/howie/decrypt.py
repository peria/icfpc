#!/usr/bin/python

import subprocess

def setup_umix():
  # 1. setup UM(UMIX) to ommunicate in standard I/O.
  umix = subprocess.Popen(['../um/um', '../um/umix.um'],
                          stdin=subprocess.PIPE,
                          stdout=subprocess.PIPE)

  # 2. Get upload to be ready. (stdin < input.txt and uploader.adv)
  for filename in ['input.txt', 'uploader.adv']:
    with open(filename, 'r') as f:
      for l in f:
        umix.stdin.write(l)
  for _ in xrange(1383):
    umix.stdout.readline()

  return umix


def main():
  rml = None
  with open('decrypt.rml') as f:
    rml = f.read().split('\n')

  room_line = 4
  case_line = None
  for i in xrange(len(rml)):
    if rml[i].find('CASE') >= 0:
      case_line = i
      break

  # room, length, items = ('Rotunda', 130, [])  # blueprint
  # room, length, items = ('Room With a Door', 120, [])  # manifesto
  room, length, items = ('Machine Room M4', 1110, ['crowbar', 'note'])  # console

  rml[room_line] = 'let ROOMNAME = "%s".' % room
  umix = setup_umix()

  # Upload RML
  umix.stdin.write('\n'.join(rml) + '\n')

  for item in items:
    umix.stdin.write('get %s\ninc %s\n' % (item, item))

  plain = ''
  for i in xrange(length):
    p = None
    for c in 'eatiosnrhldcumpfgywbvkjxqz1234567890!"$%&\'()=|@;:,./?<>_':
      umix.stdin.write('speak %s\n' % ('x' * i + c))

      output = ''
      while output.find('You speak the words') < 0 and output.find('your thoughts') < 0:
        output = umix.stdout.readline().strip()
      if output.find('You speak') >= 0:
        plain += p
        break
      p = c
    if p == '_':
      plain += '_'

    print '[%d] %s' % (i, plain)
    umix.stdin.write('get uploader\n')

  rml[case_line] = '          let code = string_charat (content, index) + 32.'
  umix.stdin.write('\n'.join(rml) + '\n')
  for i in xrange(length):
    if plain[i] != '_':
      continue

    p = None
    for c in 'EATIOSNRHLDCUMPFGYWBVKJXQZ_':
      umix.stdin.write('speak %s\n' % ('x' * i + c))
      output = ''
      while output.find('You speak the words') < 0 and output.find('your thoughts') < 0:
        output = umix.stdout.readline().strip()
      if output.find('You speak') >= 0:
        plain = plain[:i] + p + plain[i+1:]
        break
      p = c
    if p == '_':
      plain = plain[:i] + ' ' + plain[i+1:]

    print '[%d] %s' % (i, plain)
    umix.stdin.write('get uploader\n')

if __name__ == '__main__':
  main()
