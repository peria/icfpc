#!/usr/bin/env python3

from flask import Flask, render_template, request
import json

app = Flask(__name__)

@app.route("/")
def list_table():
    data = [{
        'timestamp': '2025/07/07 10:00:00',
        'hash': 'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855',
        'prefix': '',
        'diff': 0,
        'risk':360000,
        'rna_size': 302450,
        'elapsed_time': 27.05,
    },
    {
        'timestamp': '2025/07/07 10:00:00',
        'hash': 'e3c0e6a0d9cc04465eb70dbd9b2ddc989f55ba3cfcedb207e79334c0a4f4f5fd',
        'prefix': 'IIPIFFCPICICIICPIICIPPPICIIC',
        'diff': 0,
        'risk':360000,
        'rna_size': 36312,
        'elapsed_time': 5.52,
    }]
    # TODO: Convert prefix to Pattern/Template
    return render_template("index.html", data=data)

@app.route("/execute", methods=["POST"])
def execute():
    data = json.loads(request.get_data())
    if 'prefix' not in data:
        return "Key not found"

    prefix = data['prefix']
    # ../target/release/icfpc2007 -e ../../data/endo.dna -s --image-dir static/images -p {prefix}
    return f"OK: {prefix}"

if __name__ == "__main__":
    app.run(debug=True)
