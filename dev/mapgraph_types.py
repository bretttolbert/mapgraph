from typing import cast, Dict, Any, List, Union

Data = Dict[str, Any]
Metadata = List[Dict[str, str]]


class DataItemDef:
    def __init__(
        self,
        csv_column: str,
        json_data_item: str,
        json_item_description: str,
        special_last_column_with_value: bool = False,
    ):
        """
        DataItemDefinition, defining a field in the JSON and how to get it from the source CSV.

        :param csv_column: The name of the column in the source CSV file.
        :type csv_column: str
        :param json_data_item: The name of the item in the output JSON file.
        :type json_data_item: str
        :param json_item_description: The description of the item in the output JSON file.
        :type json_item_description: str
        :param special_last_column_with_valid_value: If set, csv_column is ignored and
        instead it will get the last (rightmost) column with a non-empty value
        from the CSV file
        :type special_last_column_with_valid_value: bool
        """

        self.csv_column = csv_column
        self.json_data_item = json_data_item
        self.json_data_item_description = json_item_description
        self.special_last_column_with_value = special_last_column_with_value
