#!/usr/bin/python3

import json
import jinja2
import os
import sys

_ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../'))
_JSON_PATH = os.path.join(_ROOT_DIR, 'solutions', 'score.json')


# TODO: Use external template file and then use CSS framework.
tmpl = jinja2.Template("""
<!DOCTYPE html>
<style>
.value { text-align: right; }
</style>
<table>
  <tr>
    <th>
    {% for engine in engines %}
    <th id="{{engine.name}}">{{engine.name}}
    {% endfor %}
  {% for i in range(300) %}
  <tr>
    <th>
    {{'prob-{:03d}'.format(i+1)}}
    {% for engine in engines %}
    <td class="value">{{engine.times[i]}}
    {% endfor %}
  {% endfor %}
</table>
""")

dp = {}
with open(_JSON_PATH, 'r') as f:
    db = json.load(f)

engines_context = []
for engine, times in db.items():
    engine_context = {
        'name': engine,
        'times': times,
    }
    engines_context.append(engine_context)

html = tmpl.render(engines=engines_context)
print('Content-Type: text/html; charset=utf-8\n')
print(html)
