#!/usr/bin/env python

from typing import List
from pathlib import Path

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def is_year(s: str):
    try:
        value = int(s)
        return value >= 1960
    except Exception:
        return False


def read_csv_country_latest_values(name, csv_path):
    countries = []
    values = []
    df = pd.read_csv(
        csv_path,
        skiprows=4,
    )
    df = df.rename(columns={"Country Name": "Country"})
    df = df.rename(columns={"Country Code": "CountryCode"})
    df = df.rename(columns={"Indicator Name": "IndicatorName"})
    df = df.rename(columns={"Indicator Code": "IndicatorCode"})

    for i in range(len(df)):
        row_values: List[np.float64] = []
        row = df.iloc[i]
        for column_name, column_value in row.to_dict().items():
            if is_year(column_name):
                if not np.isnan(column_value):
                    row_values.append(column_value)
        latest_value = np.nan
        if len(row_values):
            latest_value = row_values[-1]
            countries.append(row.Country)
            values.append(latest_value)
        print(f"Country: {row.Country} Latest {name} Value: {latest_value}")
        print("-" * 20)
    return (countries, values)


data_path = Path("source_data/worldbank/")

# https://data.worldbank.org/indicator/SI.POV.GINI
countries, values = read_csv_country_latest_values(
    "Gini",
    data_path.joinpath(
        "API_SI.POV.GINI_DS2_en_csv_v2_1105585/API_SI.POV.GINI_DS2_en_csv_v2_1105585.csv"
    ),
)
data_gini = {
    "Country": countries,
    "Gini": values,
}
# https://data.worldbank.org/indicator/NY.GDP.PCAP.CD
countries, values = read_csv_country_latest_values(
    "GDPpc",
    data_path.joinpath(
        "API_NY.GDP.PCAP.CD_DS2_en_csv_v2_1106527/API_NY.GDP.PCAP.CD_DS2_en_csv_v2_1106527.csv"
    ),
)
data_gdp = {
    "Country": countries,
    "GDPpc": values,
}
data = pd.merge(
    pd.DataFrame(data_gini), pd.DataFrame(data_gdp), on="Country", how="inner"
)
df = pd.DataFrame(data)
plt.rcParams.update({"font.size": 6})
plt.figure(figsize=(8, 6))
plt.scatter(df["Gini"], df["GDPpc"])
for i, row in df.iterrows():
    plt.text(
        row["Gini"], row["GDPpc"], row["Country"], rotation=90, va="top", ha="left"
    )
plt.xlabel("Wealth Gini Coefficient (0 = equal, 1 = unequal)")
plt.ylabel("GDP per Capita (USD)")
plt.title("Wealth Inequality vs GDP per Capita by Country")
plt.show()
