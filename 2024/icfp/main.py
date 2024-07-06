#!/usr/bin/env python3

import sys
import evaluator
import communicator


def main():
    lines = []
    for line in sys.stdin:
        lines.append(line)
    message = '\n'.join(lines)
    response = communicator.communicate(message)
    result = evaluator.evaluate(response)
    print(result)


if __name__ == '__main__':
    main()
