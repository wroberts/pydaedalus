#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Wrapper for the daedalus monochrome maze functionality.

maze = Maze(63, 63)
maze.create_perfect()
maze.resize(31, 61)
maze.create_perfect()
maze.save_bitmap('test.bmp')
maze.save_text('test.txt')
'''

from libcpp cimport bool

cdef extern from "pdconfig.h":
    cdef int PYTHON_VERSION2

cdef extern from "wrapper.h":
    cdef cppclass CMaz:
        pass
    cdef cppclass CCol:
        pass
    ctypedef long KV
    cdef int SAVE_NO_MAZE_ERROR
    cdef int SAVE_NO_FILENAME_GIVEN_ERROR
    cdef int SAVE_FILE_OPEN_ERROR
    cdef int SAVE_UNKNOWN_CMD_ERROR
    cdef int SAVE_SUCCESS
    cdef KV kvWhite
    cdef KV kvBlack
    CMaz *cpp_Constructor(unsigned width, unsigned height)
    void cpp_Destructor(CMaz *maze)
    bool cpp_Get(CMaz *maze, unsigned x, unsigned y)
    bool cpp_CreateMazePerfect(CMaz *maze)
    bool cpp_CreateMazePerfect2(CMaz *maze)
    bool cpp_CreateMazeBraid(CMaz *maze)
    bool cpp_CreateMazeBraidTilt(CMaz *maze)
    bool cpp_CreateMazeSpiral(CMaz *maze)
    bool cpp_CreateMazeDiagonal(CMaz *maze)
    bool cpp_CreateMazeRecursive(CMaz *maze)
    bool cpp_CreateMazePrim(CMaz *maze)
    bool cpp_CreateMazePrim2(CMaz *maze)
    bool cpp_CreateMazeKruskal(CMaz *maze, bool fClear, CCol *c2, CCol *c3)
    bool cpp_CreateMazeTree(CMaz *maze)
    bool cpp_CreateMazeForest(CMaz *maze, bool fWall)
    bool cpp_CreateMazeAldousBroder(CMaz *maze)
    bool cpp_CreateMazeWilson(CMaz *maze)
    bool cpp_CreateMazeEller(CMaz *maze)
    bool cpp_CreateMazeBraidEller(CMaz *maze)
    bool cpp_CreateMazeDivision(CMaz *maze)
    bool cpp_CreateMazeBinary(CMaz *maze)
    bool cpp_CreateMazeSidewinder(CMaz *maze)
    bool cpp_Resize(CMaz *maze, unsigned width, unsigned height)
    int wrapper_FFileSave(CMaz *maze,
                          int wCmd,
                          const char *filename,
                          KV kvOn,
                          KV kvOff,
                          bool fTextClip,
                          bool fLineChar,
                          bool fTextTab)
    int cpp_SaveBitmap(CMaz *maze,
                       const char *filename,
                       KV kvOn,
                       KV kvOff)
    int cpp_SaveText(CMaz *maze,
                     const char *filename,
                     bool fTextClip,
                     bool fLineChar,
                     bool fTextTab)

class MazeError(Exception):
    '''An error while performing an action on a Maze object.'''
    pass

cdef class Maze(object):
    '''A monochrome daedalus Maze object.'''

    cdef CMaz *_maze
    cdef unsigned _width
    cdef unsigned _height

    def __cinit__(self, width, height):
        '''
        Maze constructor.

        Arguments:
        - `width`: odd integer, 3 or greater
        - `height`: odd integer, 3 or greater
        '''
        if width < 0:
            raise ValueError('negative width')
        if height < 0:
            raise ValueError('negative height')
        if width % 2 != 1:
            raise ValueError('width must be odd')
        if height % 2 != 1:
            raise ValueError('height must be odd')
        self._width = 0
        self._height = 0
        self._maze = cpp_Constructor(width, height)
        if self._maze is NULL:
            raise MazeError('Could not construct Maze object.')
        self._width = width
        self._height = height

    def __dealloc__(self):
        '''Destructor.'''
        if self._maze is not NULL:
            cpp_Destructor(self._maze)

    @property
    def width(self):
        '''
        Returns this Maze's width.
        '''
        return self._width

    @property
    def height(self):
        '''
        Returns this Maze's height.
        '''
        return self._height

    def get(self, x, y):
        '''
        Gets the monochrome pixel value at the coordinate (x,y) on this
        Maze.

        Arguments:
        - `x`:
        - `y`:
        '''
        if x < 0 or self._width <= x:
            raise IndexError('x coordinate out of range')
        if y < 0 or self._height <= y:
            raise IndexError('y coordinate out of range')
        return cpp_Get(self._maze, x, y)

    def create_perfect(self):
        '''
        Overwrites the contents of this Maze to create a perfect maze
        using the Hunt and Kill algorithm.
        '''
        if not cpp_CreateMazePerfect(self._maze):
            raise MazeError('Could not create Perfect Maze.')

    def create_perfect2(self):
        '''
        Overwrites the contents of this Maze to create a Perfect2 maze.
        '''
        if not cpp_CreateMazePerfect2(self._maze):
            raise MazeError('Could not create Perfect2 Maze.')

    def create_braid(self):
        '''
        Overwrites the contents of this Maze to create a Braid maze.
        '''
        if not cpp_CreateMazeBraid(self._maze):
            raise MazeError('Could not create Braid Maze.')

    def create_braid_tilt(self):
        '''
        Overwrites the contents of this Maze to create a BraidTilt maze.
        '''
        if not cpp_CreateMazeBraidTilt(self._maze):
            raise MazeError('Could not create BraidTilt Maze.')

    def create_spiral(self):
        '''
        Overwrites the contents of this Maze to create a Spiral maze.
        '''
        if not cpp_CreateMazeSpiral(self._maze):
            raise MazeError('Could not create Spiral Maze.')

    def create_diagonal(self):
        '''
        Overwrites the contents of this Maze to create a Diagonal maze.
        '''
        if not cpp_CreateMazeDiagonal(self._maze):
            raise MazeError('Could not create Diagonal Maze.')

    def create_recursive(self):
        '''
        Overwrites the contents of this Maze to create a Recursive maze.
        '''
        if not cpp_CreateMazeRecursive(self._maze):
            raise MazeError('Could not create Recursive Maze.')

    def create_prim(self):
        '''
        Overwrites the contents of this Maze to create a Prim maze.
        '''
        if not cpp_CreateMazePrim(self._maze):
            raise MazeError('Could not create Prim Maze.')

    def create_prim2(self):
        '''
        Overwrites the contents of this Maze to create a Prim2 maze.
        '''
        if not cpp_CreateMazePrim2(self._maze):
            raise MazeError('Could not create Prim2 Maze.')

    def create_kruskal(self, fClear, c2, c3):
        '''
        Overwrites the contents of this Maze to create a Kruskal maze.
        '''
        # TODO
        cdef CCol *cpp_c2 = NULL
        cdef CCol *cpp_c3 = NULL
        if not cpp_CreateMazeKruskal(self._maze, fClear, cpp_c2, cpp_c3):
            raise MazeError('Could not create Kruskal Maze.')

    def create_tree(self):
        '''
        Overwrites the contents of this Maze to create a Tree maze.
        '''
        if not cpp_CreateMazeTree(self._maze):
            raise MazeError('Could not create Tree Maze.')

    def create_forest(self, fWall):
        '''
        Overwrites the contents of this Maze to create a Forest maze.
        '''
        if not cpp_CreateMazeForest(self._maze, fWall):
            raise MazeError('Could not create Forest Maze.')

    def create_aldous_broder(self):
        '''
        Overwrites the contents of this Maze to create a AldousBroder maze.
        '''
        if not cpp_CreateMazeAldousBroder(self._maze):
            raise MazeError('Could not create AldousBroder Maze.')

    def create_wilson(self):
        '''
        Overwrites the contents of this Maze to create a Wilson maze.
        '''
        if not cpp_CreateMazeWilson(self._maze):
            raise MazeError('Could not create Wilson Maze.')

    def create_eller(self):
        '''
        Overwrites the contents of this Maze to create a Eller maze.
        '''
        if not cpp_CreateMazeEller(self._maze):
            raise MazeError('Could not create Eller Maze.')

    def create_braid_eller(self):
        '''
        Overwrites the contents of this Maze to create a BraidEller maze.
        '''
        if not cpp_CreateMazeBraidEller(self._maze):
            raise MazeError('Could not create BraidEller Maze.')

    def create_division(self):
        '''
        Overwrites the contents of this Maze to create a Division maze.
        '''
        if not cpp_CreateMazeDivision(self._maze):
            raise MazeError('Could not create Division Maze.')

    def create_binary(self):
        '''
        Overwrites the contents of this Maze to create a Binary maze.
        '''
        if not cpp_CreateMazeBinary(self._maze):
            raise MazeError('Could not create Binary Maze.')

    def create_sidewinder(self):
        '''
        Overwrites the contents of this Maze to create a Sidewinder maze.
        '''
        if not cpp_CreateMazeSidewinder(self._maze):
            raise MazeError('Could not create Sidewinder Maze.')

    def resize(self, width, height):
        '''
        Truncates or pads out this Maze to attain the given size.

        Arguments:
        - `width`: odd integer, 3 or greater
        - `height`: odd integer, 3 or greater
        '''
        if width < 0:
            raise ValueError('negative width')
        if height < 0:
            raise ValueError('negative height')
        if width % 2 != 1:
            raise ValueError('width must be odd')
        if height % 2 != 1:
            raise ValueError('height must be odd')
        if not cpp_Resize(self._maze, width, height):
            raise MazeError('Could not resize the Maze.')
        self._width = width
        self._height = height

    @staticmethod
    def _handle_save_retval(rv):
        '''
        Helper method to handle return values from the Save* methods.

        Arguments:
        - `rv`: return value
        '''
        if rv == SAVE_NO_MAZE_ERROR:
            raise MazeError('Maze not properly constructed.')
        elif rv == SAVE_NO_FILENAME_GIVEN_ERROR:
            raise MazeError('No filename given.')
        elif rv == SAVE_FILE_OPEN_ERROR:
            raise MazeError('Could not open file.')
        elif rv == SAVE_UNKNOWN_CMD_ERROR:
            raise MazeError('Unknown save command.')
        elif rv == SAVE_SUCCESS:
            pass
        else:
            raise MazeError('Unknown return value from Maze save function.')

    def save_bitmap(self, filename, kvOn=kvWhite, kvOff=kvBlack):
        '''
        Saves this Maze object as a bitmap to the given path.

        Arguments:
        - `filename`:
        - `kvOn`: defaults to white
        - `kvOff`: defaults to black
        '''
        retval = cpp_SaveBitmap(self._maze, filename, kvOn, kvOff)
        self._handle_save_retval(retval)

    def save_text(self, filename, fTextClip=True, fLineChar=False, fTextTab=False):
        '''
        Saves this Maze object formatted in ASCII text to the given path.

        Arguments:
        - `filename`:
        - `fTextClip`: defaults to True
        - `fLineChar`: defaults to False
        - `fTextTab`: defaults to False
        '''
        retval =  cpp_SaveText(self._maze, filename, fTextClip, fLineChar, fTextTab)
        self._handle_save_retval(retval)
