from database import Database
from flask import Flask, render_template, request
import datetime
import hashlib
import json
import subprocess
import time


app = Flask(__name__)
mydb = Database()

def convert(entry: dict) -> dict:
    entry = entry.copy()
    timestamp = datetime.datetime.fromtimestamp(entry['timestamp'])
    entry['timestamp'] = timestamp.strftime('%Y/%m/%d %H:%M:%S')
    entry['hash'] = hashlib.sha256(entry['prefix'].encode()).hexdigest()
    entry['risk'] = 10 * entry['diff'] + len(entry['prefix'])
    return entry

@app.route("/")
def list_table():
    data = map(convert, mydb.values())
    return render_template("index.html", data=data)

@app.route("/execute", methods=["POST"])
def execute():
    data = json.loads(request.get_data())
    if 'prefix' not in data:
        return "Key not found"

    prefix = data['prefix']
    timestamp = time.time()
    result = subprocess.run([
        '../target/release/icfpc2007',
        '-s',
        '-e', '../../data/endo.dna',
        '--image-dir', 'static/images',
        '--target-path', '../../image/target.png',
        '-p', prefix], encoding='utf-8', stdout=subprocess.PIPE)
    result = result.stdout
    print(f"STDOUT> {result}")
    result = json.loads(result)
    mydb.insert(prefix = prefix,
                timestamp = int(timestamp),
                diff = result['diff'],
                rna_size = result['rna'],
                elapsed_time = result['dna2rna'])
    return f"OK: {prefix}"

if __name__ == "__main__":
    app.run(debug=True)
