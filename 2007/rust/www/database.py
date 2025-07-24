import hashlib
import json

class Database:
    JSON_PATH = "./database.json"

    def __init__(self):
        with open(self.JSON_PATH, 'r') as f:
            try:
                self._json = json.load(f)
            except (FileNotFoundError, json.JSONDecodeError):
                self._json = {}

    def insert(self, prefix: str, timestamp: int, diff: int, rna_size: int, elapsed_time: float):
        hash = hashlib.sha256(prefix.encode()).hexdigest()
        if hash in self._json:
            return

        entry = {
            "prefix": prefix,
            "timestamp": timestamp,  # Unix time in sec.
            "rna_size": rna_size,
            "diff": diff,
            "elapsed_time": elapsed_time,  # Time to convert DNA to RNA
        }
        self._json[hash] = entry
        self.save()

    def values(self):
        return self._json.values()

    def save(self):
        with open(self.JSON_PATH, 'w') as f:
            json.dump(self._json, f)

if __name__ == '__main__':
    db = Database()
    db.insert('ICPC', 1753371155, 123, 482, 27.0)
    db.insert('ICC', 1753334155, 23, 22482, 2.0)
