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

Wrapper is copyright (c) 2016 Will Roberts <wildwilhelm@gmail.com>.

Licensed under the GNU General Public License version 2.

A Python wrapper of the Daedalus_ maze generator.  This project is
currently built against version 3.2 of the Daedalus software.

.. _Daedalus: http://www.astrolog.org/labyrnth/daedalus.htm

Quickstart
==========

::

    >>> from daedalus import Maze
    >>> maze = Maze(63, 63)
    >>> maze.create_perfect()
    >>> maze.resize(31, 61)
    >>> maze.create_perfect()
    >>> maze.save_bitmap('test.bmp')
    >>> maze.save_text('test.txt')

Notes
=====

To compile the Cython extension::

    python setup.py build_ext --inplace
