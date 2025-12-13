#!/usr/bin/env python
from typing import cast, Dict, Any, List, Union
from pathlib import Path
import json
import codecs
import pandas as pd

"""
Convert CSV or tab-delimited files to mapgraph JSON dataset format 
for use with the us-states SVG map
"""

dry_run = False
map_name = "us-states"
dataset_name = "census-2020"
dataset_long_name = "US Census 2020 QuickFacts Data"
out_path = Path(f"../datasets/{map_name}/")
csv_filename = (
    "source_data/DECENNIALCD1182020.P1_2025-12-12T192416/DECENNIALCD1182020.P1-Data.csv"
)
skiprows = 1
csv_column_state_name = "Geographic Area Name"
csv_column_value = " !!Total"
data_item_1_id = "population"
data_item_1_description = "Total Population"
# path to map.json for the "us-states" map, used for converting state name to FIPS numeric ID
map_data_file_path = f"../maps/{map_name}/map.json"


Data = Dict[str, Any]
Metadata = List[Dict[str, str]]


def get_state_fips_from_state_name(state_name: str, mapdata: Data):
    """
    Uses map svg to get state FIPS code from the state name

    :param state_name: State name e.g. "Alabama"
    """
    state_fips = ""
    # convert state name to US state FIPS code
    found = False
    for node in mapdata["nodes"]:
        if node["s"] == state_name:
            state_fips = node["id"]
            found = True
            break
    if not found:
        print("Error: failed to find FIPS for state {0}".format(state_name))
    return state_fips


def write_json(data: Data, out_path: Path, name: str, min: bool):
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


with codecs.open(map_data_file_path, "r", encoding="utf-8") as mapfile:
    mapdata = json.loads(mapfile.read())
    data: Dict[str, Union[str, Metadata]] = {
        "nodeIdSource": "FIPS",
        "metadata": [{"Data_Item": "FIPS", "Item_Description": "FIPS state code"}],
        "data": [],
    }
    data["name"] = dataset_long_name
    data["sourceUrl"] = "http://www.erh.noaa.gov/cae/svrwx/tornadobystate.htm"
    # for each item:
    metadata: Metadata = cast(Metadata, data["metadata"])
    metadata.append(
        {
            "Data_Item": data_item_1_id,
            "Item_Description": data_item_1_description,
        }
    )
    data["metadata"] = metadata
    df = pd.read_csv(csv_filename, skiprows=skiprows)
    for i, row in df.iterrows():
        state_name = row[csv_column_state_name]
        state_value = row[csv_column_value]
        state_fips = get_state_fips_from_state_name(state_name, mapdata)
        if state_fips != "":
            record = {"FIPS": state_fips}
            print(f"state_name: {state_name} state_fips: {state_fips}")
            record[data_item_1_id] = float(state_value)
            d = cast(List[Data], data["data"])
            d.append(record)
            data["data"] = d
        else:
            print(f"Warning: Failed to find FIPS for State {state_name}")
    if not dry_run:
        write_json(data, out_path, dataset_name, min=False)
        write_json(data, out_path, dataset_name, min=True)
