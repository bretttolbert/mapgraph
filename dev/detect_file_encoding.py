from typing import Optional
import chardet
from pathlib import Path


def detect_file_encoding(fpath: Path) -> Optional[str]:
    with open(fpath, "rb") as f:
        result = chardet.detect(f.read())
    return result["encoding"]
