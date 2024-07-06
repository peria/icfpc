#!/usr/bin/env python3

import requests

DOMAIN = "localhost:8000"
BEARER_TOKEN = "00000000-0000-0000-0000-000000000000"

COMMUNICATE_URL = f'http://{DOMAIN}/communicate'
HEADERS = {'Authorization': f'Bearer {BEARER_TOKEN}'}


def communicate(message):
    return requests.post(COMMUNICATE_URL, headers=HEADERS, data=message).text


if __name__ == '__main__':
    print('communicator.py is not run from the command line.p')
