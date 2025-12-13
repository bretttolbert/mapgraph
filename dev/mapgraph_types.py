from typing import cast, Dict, Any, List, Union

Data = Dict[str, Any]
Metadata = List[Dict[str, str]]


class DataItemDef:
    def __init__(
        self, csv_column: str, json_data_item: str, json_item_description: str
    ):
        self.csv_column = csv_column
        self.json_data_item = json_data_item
        self.json_data_item_description = json_item_description
