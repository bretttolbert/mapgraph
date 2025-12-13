import json
from pathlib import Path

from mapgraph_types import Data


def write_json(data: Data, out_path: str, name: str, min: bool):
    fname = f"{name}.json"
    if min:
        fname = fname.replace(".json", ".min.json")
    fout_path = Path(out_path).joinpath(fname)
    with open(fout_path, "w") as f:
        if min:
            f.write(json.dumps(data, separators=(",", ":")))
        else:
            f.write(json.dumps(data, sort_keys=False, indent=4, separators=(",", ": ")))
        print(f"Wrote file {fout_path}")
