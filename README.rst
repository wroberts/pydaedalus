============
 pydaedalus
============

.. image:: https://travis-ci.org/wroberts/pydaedalus.svg?branch=master
    :target: https://travis-ci.org/wroberts/pydaedalus

.. image:: https://coveralls.io/repos/wroberts/pydaedalus/badge.svg?branch=master
  :target: https://coveralls.io/r/wroberts/pydaedalus?branch=master
     :alt: Test code coverage

.. image:: https://img.shields.io/pypi/v/pydaedalus.svg
    :target: https://pypi.python.org/pypi/pydaedalus/
    :alt: Latest Version

Daedalus source code is copyright (c) 1998-2016 Walter D. Pullen.

Wrapper is copyright (c) 2017 Will Roberts <wildwilhelm@gmail.com>.

Licensed under the GNU General Public License version 2.

A Python wrapper of the Daedalus_ maze generator.  This project is
currently built against version 3.2 of the Daedalus software.

.. _Daedalus: http://www.astrolog.org/labyrnth/daedalus.htm

Quickstart
==========

Install::

    pip install pydaedalus

Getting started::

    >>> from daedalus import Maze
    >>> maze = Maze(63, 63)
    >>> maze.create_perfect()
    >>> maze.resize(31, 61)
    >>> maze.create_perfect()
    >>> maze.save_bitmap('test.bmp')
    >>> maze.save_text('test.txt')

A Maze can be treated as a 2D array of Bools (walls are True and
passages are False), or as a container of lists of ints (walls are 1
and passages are 0)::

    >>> maze.width, maze.height
    (31, 61)
    >>> maze[0,0]
    True
    >>> maze[1,1]
    False
    >>> maze[1,2]
    False
    >>> maze[1,3]
    False
    >>> maze[1]
    [1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1]
    >>> len(maze)
    61

Take a look at `demo.ipynb`_ to see visualisations of some of the
kinds of Mazes Daedalus can create.

.. _`demo.ipynb`: https://github.com/wroberts/pydaedalus/blob/master/demo.ipynb

Requirements
============

- Python 2.7, or 3.3+,
- A C++ compiler with C++11 support (GCC 4.7+, clang 3.3+).

Development Notes
=================

To compile the Cython extension::

    python setup.py build_ext --inplace
