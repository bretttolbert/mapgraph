import json
import codecs
from mapgraph_types import Data


def get_map_data(map_name: str) -> Data:
    # path to map.json for the specified map, used for converting state name to FIPS numeric ID
    map_data_file_path = f"../maps/{map_name}/map.json"
    with codecs.open(map_data_file_path, "r", encoding="utf-8") as mapfile:
        return json.loads(mapfile.read())


def get_fips_code_from_geo_name(geo_name: str, mapdata: Data):
    """
    Uses map svg to get state FIPS code from the geographic name

    :param geo_name:
        us-states: state name, e.g. "Alabama"
        us-counties: county name, state abbr. e.g. "Madison County, AL"

    returns FIPS code (2-digit code for states, 5-digit code for counties)

    map.json examples:

        us-states:

        "nodeMetadata": {
            "id": {
                "digits": 2,
                "dsc": "2-digit FIPS State Code"
            },
            "n": {
                "dsc:": "Neighboring State IDs"
            },
            "s": {
                "dsc": "State Name"
            }
        },

        us-counties:

        "nodeMetadata": {
            "id": {
                "digits": 5,
                "dsc": "5-digit Combined FIPS State and County Code"
            },
            "n": {
                "dsc:": "Neighboring County IDs"
            },
            "s": {
                "dsc": "County Name, State"
            }
        },

    """
    fips_code = ""
    # convert geogrpahic name to FIPS code
    found = False
    for node in mapdata["nodes"]:
        if node["s"] == geo_name:
            fips_code = node["id"]
            found = True
            break
    if not found:
        print("Error: failed to find FIPS code for geo_name {0}".format(geo_name))
    return fips_code
