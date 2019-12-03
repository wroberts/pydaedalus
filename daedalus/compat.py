#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Python 2/3 compatibility shim.

compat.py
(c) Will Roberts   3 December, 2019
"""

import sys

PY2 = int(sys.version[0]) == 2

def py3_str_to_bytes(filename):
    if not PY2 and isinstance(filename, str):
        return filename.encode('utf-8')
    return filename
