from typing import List
import pandas as pd

from state_to_abbreviation import state_to_abbreviation


def get_geo_name_from_csv(row: pd.Series, csv_columns_geo_name: List[str]) -> str:
    """
    Get the geographic name by extracting data from
    one or more columns in the given csv row

    E.g. "Alabama" or "Madison County, AL"

    :param row: Description
    :type row: pd.Series
    :return: Description
    :rtype: str
    """

    tokens = []
    for column in csv_columns_geo_name:
        token = row[column]
        print(f"column: {column} token: {token}")
        tokens.append(token)

    if len(tokens) == 1:  # state path
        return tokens[0]
    elif len(tokens) == 2:  # county path
        state, county = tokens
        state_abbr = state_to_abbreviation(state)
        return f"{county}, {state_abbr}"

    return ", ".join(tokens)
