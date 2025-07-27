from database import Database
from flask import Flask, render_template, request
import datetime
import hashlib
import json
import subprocess
import time


app = Flask(__name__)
mydb = Database()

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/detail")
def detail():
    return render_template("detail.html")

@app.route("/api/get_all", methods=["POST"])
def get_all():
    return mydb.as_json()

@app.route("/api/get_entry", methods=["POST"])
def get_entry():
    data = json.loads(request.get_data())
    if 'prefix' not in data:
        return "Entry key not found"

    prefix = data['prefix']
    return mydb.get_entry(prefix)
    
@app.route("/execute", methods=["POST"])
def execute():
    data = json.loads(request.get_data())
    if 'prefix' not in data:
        print("No prefix provided")
        return "No prefix provided"
    prefix = data['prefix']
    entry = mydb.get_entry(prefix)
    if entry is not None:
        print(f"Already exists: {prefix}")
        return f"Already exists: {prefix}"

    print(f"execute {prefix}")
    timestamp = time.time()
    mydb.reserve(prefix)
    result = subprocess.run([
        '../target/release/icfpc2007',
        '-s',
        '-e', '../../data/endo.dna',
        '--image-dir', 'static/images',
        '--target-path', '../../image/target.png',
        '-p', prefix], encoding='utf-8', stdout=subprocess.PIPE)
    result = result.stdout
    result = json.loads(result)
    mydb.insert(prefix = prefix,
                timestamp = int(timestamp),
                diff = result['diff'],
                rna_size = result['rna'],
                elapsed_time = result['dna2rna'])
    return f"OK: {prefix}"

if __name__ == "__main__":
    app.run(debug=True)
