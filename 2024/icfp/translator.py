#!/usr/bin/env python3

__icfp_ascii = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!"#$%&\'()*+,-./:;<=>?@[\\]^_`|~ \n'
__icfp_index = {}

if len(__icfp_index) == 0:
    for i, c in enumerate(__icfp_ascii):
        __icfp_index[c] = chr(i + ord('!'))


def icfp_to_ascii(icfp):
    ascii = ''
    for c in icfp:
        ascii += __icfp_ascii[ord(c) - ord('!')]
    return ascii


def ascii_to_icfp(ascii):
    icfp = ''
    for c in ascii:
        icfp += __icfp_index[c]
    return icfp


def translate(message):
    return 'S' + ascii_to_icfp(message)


if __name__ == '__main__':
    print('translator.py is not run from the command line.p')
