from typing import Optional
import chardet
from pathlib import Path


def detect_file_encoding(fpath: Path) -> Optional[str]:
    """
    Warning, this is unacceptably slow for large files.
    TODO: Rewrite.

    :param fpath: Description
    :type fpath: Path
    :return: Description
    :rtype: str | None
    """

    with open(fpath, "rb") as f:
        result = chardet.detect(f.read())
    return result["encoding"]
