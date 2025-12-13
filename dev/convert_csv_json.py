#!/usr/bin/env python
from typing import cast, Dict, List, Union
from pathlib import Path
import pandas as pd

from mapgraph_types import Data, Metadata, DataItemDef
from detect_file_encoding import detect_file_encoding
from map_utils import get_fips_code_from_geo_name, get_map_data
from csv_utils import get_geo_name_from_csv
from json_utils import write_json

"""
Convert CSV or tab-delimited files to mapgraph JSON dataset format 
for use with the us-states or us-counties SVG maps
"""


def convert_csv_json(
    csv_filename: str,
    skiprows: int,
    csv_columns_geo_name: List[str],
    data_item_defs: List[DataItemDef],
    map_name: str,
    dataset_name: str,
    dataset_long_name: str,
    dataset_source_url: str,
    dry_run: bool = False,
):
    map_data = get_map_data(map_name)
    out_path = f"../datasets/{map_name}/"
    data: Dict[str, Union[str, Metadata]] = {
        "nodeIdSource": "FIPS",
        "metadata": [{"Data_Item": "FIPS", "Item_Description": "FIPS code"}],
        "data": [],
    }
    data["name"] = dataset_long_name
    data["sourceUrl"] = dataset_source_url
    metadata: Metadata = cast(Metadata, data["metadata"])
    for did in data_item_defs:
        metadata.append(
            {
                "Data_Item": did.json_data_item,
                "Item_Description": did.json_data_item_description,
            }
        )
    data["metadata"] = metadata
    encoding_type = detect_file_encoding(Path(csv_filename))
    df = pd.read_csv(csv_filename, skiprows=skiprows, encoding=encoding_type)
    for i, row in df.iterrows():
        geo_name = get_geo_name_from_csv(row, csv_columns_geo_name)
        fips_code = get_fips_code_from_geo_name(geo_name, map_data)
        if fips_code != "":
            record = {"FIPS": fips_code}
            print(f"geo_name: {geo_name} fips_code: {fips_code}")
            for did in data_item_defs:
                csv_value = row[did.csv_column]
                record[did.json_data_item] = float(csv_value)
                d = cast(List[Data], data["data"])
                d.append(record)
                data["data"] = d
        else:
            print(f"Warning: Failed to find FIPS for geo name '{geo_name}'")
    if not dry_run:
        write_json(data, out_path, dataset_name, min=False)
        write_json(data, out_path, dataset_name, min=True)


def main():
    dry_run = False
    dataset = "2024-census-pop-est"

    if dataset == "*" or dataset == "2020-census-pop-CD118":
        convert_csv_json(
            csv_filename="source_data/DECENNIALCD1182020.P1_2025-12-12T192416/DECENNIALCD1182020.P1-Data.csv",
            skiprows=1,
            csv_columns_geo_name=["Geographic Area Name"],
            data_item_defs=[
                DataItemDef(
                    csv_column=" !!Total",
                    json_data_item="population",
                    json_item_description="Total Population",
                )
            ],
            map_name="us-states",
            dataset_name="2020-census-pop-CD118",
            dataset_long_name="U.S. Census Bureau, 2020 Census 118th Congressional District Summary File (CD118)",
            dataset_source_url="https://data.census.gov/table/DECENNIALCD1182020.P1",
            dry_run=dry_run,
        )
    if dataset == "*" or dataset == "2024-census-pop-est":
        # TODO: This particular CSV contains the FIPS codes for State and County, so we could
        # simply use the FIPS columns STATE and COUNTY directly (concatenated) instead of
        # getting the string geo names from the CSV and then looking up the FIPS codes in
        # the SVG us-counties map JSON.
        # But I didn't want to break the existing pattern which is compatible with
        # other CSV files that don't contain FIPS codes.
        convert_csv_json(
            csv_filename="source_data/co-est2024-alldata.csv",
            skiprows=0,
            csv_columns_geo_name=["STNAME", "CTYNAME"],
            data_item_defs=[
                DataItemDef(
                    csv_column="POPESTIMATE2024",
                    json_data_item="2024-population-est",
                    json_item_description="2024 Population Estimate",
                )
            ],
            map_name="us-counties",
            dataset_name="2024-census-pop-est",
            dataset_long_name="U.S. Census Bureau, 2024 populuation est, counties",
            dataset_source_url="https://www2.census.gov/programs-surveys/popest/datasets/2020-2024/counties/totals/",
            dry_run=dry_run,
        )


if __name__ == "__main__":
    main()
