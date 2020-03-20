__all__ = ["AmLEInterface"]

import os
import sys
import warnings
import platform

import numpy as np

from ctypes import *
from numpy.ctypeslib import as_ctypes


def _load_cdll(path, name):
    if sys.platform.startswith("linux"):
        ext_suffix = ".so"
    elif sys.platform.startswith("darwin"):
        ext_suffix = ".dylib"
    elif sys.platform.startswith("win"):
        ext_suffix = ".dll"
    else:
        raise RuntimeError(
            'Platform "{}" not recognized while trying to resolve shared library'.format(
                sys.platform))

    library_format = "{}{}" if platform.system() == "Windows" else "lib{}{}"
    library_path = os.path.join(path, library_format.format(name, ext_suffix))

    try:
        return cdll.LoadLibrary(library_path)
    except Exception as ex:
        raise RuntimeError(
            "Failed to load library {}. Attempted to load {}.\n{}".format(
                name, library_path, ex))



amle_lib = _load_cdll(os.path.dirname(__file__), "amle_c")


amle_lib.AmLE_new.argtypes = [c_int, POINTER(POINTER(c_char))]
amle_lib.AmLE_new.restype = c_void_p

amle_lib.AmLE_del.argtypes = [c_void_p]
amle_lib.AmLE_del.restype = None

amle_lib.loadROM.argtypes = [c_void_p, c_int, c_char_p]
amle_lib.loadROM.restype = None


def _str_as_bytes(arg):
    if isinstance(arg, str):
        return arg.encode("utf-8")
    return arg


class AmLEInterface(object):

    def __init__(self, argc, argv):
        self.obj = amle_lib.AmLE_new(int(argc), _str_as_bytes(argv)))


    def loadROM(self, game, path):
        amle_lib.loadROM(self.obj, game, path) 


    def __del__(self):
        amle_lib.AmLE_del(self.obj)