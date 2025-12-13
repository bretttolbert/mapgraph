import json
from pathlib import Path

from mapgraph_types import Data


def write_json(data: Data, out_path: str, name: str, minify: bool):
    fname = f"{name}.json"
    if minify:
        fname = fname.replace(".json", ".min.json")
    fout_path = Path(out_path).joinpath(fname)
    with open(fout_path, "w") as f:
        if minify:
            f.write(json.dumps(data, separators=(",", ":")))
        else:
            f.write(json.dumps(data, sort_keys=False, indent=4, separators=(",", ": ")))
        print(f"Wrote file {fout_path}")
