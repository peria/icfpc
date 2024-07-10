#!/usr/bin/env python3

import sys
import evaluator
import communicator
import translator
import argparse


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--no-communicate',
        action='store_false', dest='communicate')
    parser.add_argument(
        '--no-translate', action='store_false', dest='translate')
    parser.add_argument(
        '--no-evaluate', action='store_false', dest='evaluate')
    args = parser.parse_args()

    lines = []
    for line in sys.stdin:
        lines.append(line)
    message = '\n'.join(lines).strip()
    if args.translate:
        message = translator.translate(message)
    # TODO: Compress 'message'
    if args.communicate:
        message = communicator.communicate(message)
    if args.evaluate:
        message = evaluator.evaluate(message)
    print(message)


if __name__ == '__main__':
    main()
