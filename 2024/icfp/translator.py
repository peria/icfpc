#!/usr/bin/env python3

__icfp_ascii = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!"#$%&\'()*+,-./:;<=>?@[\\]^_`|~ \n'
__icfp_index = {}

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


def icfp_to_int(icfp):
    value = 0
    for c in icfp:
        value = value * 94 + ord(c) - ord('!')
    return value


def int_to_ascii(value):
    ascii = ''
    while value > 0:
        ascii = __icfp_ascii[value % 94] + ascii
        value //= 94
    return ascii


def translate(message):
    return 'S' + ascii_to_icfp(message)


if __name__ == '__main__':
    print('translator.py is not run from the command line.p')
