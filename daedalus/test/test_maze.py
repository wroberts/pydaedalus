#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
test_maze.py
(c) Will Roberts  26 February, 2017

Tests on the daedalus._maze.Maze object.
'''

from __future__ import absolute_import
from .._maze import Maze

def test_maze():
    '''
    Simple test of Maze functionality.
    '''
    maze = Maze(63, 63)
    assert maze.width == 63
    assert maze.height == 63
    maze.create_perfect()
    maze.resize(31, 61)
    assert maze.width == 31
    assert maze.height == 61
    maze.create_perfect()
    #maze.save_bitmap("test.bmp")
    #maze.save_text("test.txt")
