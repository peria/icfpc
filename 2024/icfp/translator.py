#!/usr/bin/env python3

import unittest

__icfp_ascii = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!"#$%&\'()*+,-./:;<=>?@[\\]^_`|~ \n'
__icfp_index = {}

# Initialize __icfP_index
if len(__icfp_index) == 0:
    for i, c in enumerate(__icfp_ascii):
        __icfp_index[c] = i


def icfp_to_ascii(icfp):
    ascii = ''
    for c in icfp:
        ascii += __icfp_ascii[ord(c) - ord('!')]
    return ascii


def ascii_to_icfp(ascii):
    icfp = ''
    for c in ascii:
        icfp += chr(__icfp_index[c] + ord('!'))
    return icfp


def ascii_to_int(icfp):
    value = 0
    for c in icfp:
        value = value * 94 + __icfp_index[c]
    return value


# May be never used.
def int_to_ascii(value):
    if value == 0:
        return '!'

    ascii = ''
    while value > 0:
        ascii = __icfp_ascii[value % 94] + ascii
        value //= 94
    return ascii


def icfp_to_int(icfp):
    value = 0
    for c in icfp:
        value = value * 94 + ord(c) - ord('!')
    return value


def int_to_icfp(value):
    if value == 0:
        return '!'

    ascii = ''
    while value > 0:
        ascii = chr(value % 94 + ord('!')) + ascii
        value //= 94
    return ascii


def translate(message):
    return 'S' + ascii_to_icfp(message)


class TestTranslate(unittest.TestCase):
    def test_I_code(self):
        self.assertEqual(icfp_to_int('!'), 0)
        self.assertEqual(icfp_to_int('/6'), 1337)  # from the rules
        self.assertEqual(int_to_icfp(0), '!')
        self.assertEqual(int_to_icfp(1337), '/6')


if __name__ == '__main__':
    unittest.main()
