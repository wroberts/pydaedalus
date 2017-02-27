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
    # enums for return values of save functions
    cdef int SAVE_NO_MAZE_ERROR
    cdef int SAVE_NO_FILENAME_GIVEN_ERROR
    cdef int SAVE_FILE_OPEN_ERROR
    cdef int SAVE_UNKNOWN_CMD_ERROR
    cdef int SAVE_SUCCESS
    # color value constants
    cdef KV kvWhite
    cdef KV kvBlack
    # enums for setting nEntrancePos
    cdef int epCorner
    cdef int epMiddle
    cdef int epBalanced
    cdef int epRandom
    # wrapping functions
    CMaz *cpp_Constructor(unsigned width, unsigned height)
    void cpp_Destructor(CMaz *maze)
    bool cpp_Get(CMaz *maze, unsigned x, unsigned y)
    bool cpp_CreateMazePerfect ( CMaz *maze,
                                 bool  fRiver,
                                 bool  fRiverEdge,
                                 bool  fRiverFlow,
                                 bool  fSection,
                                 bool  fTreeWall,
                                 int   nEntrancePos )
    bool cpp_CreateMazePerfect2 ( CMaz *maze,
                                  bool  fRiver,
                                  bool  fSection,
                                  int   nEntrancePos )
    bool cpp_CreateMazeBraid ( CMaz *maze,
                               bool  fSection,
                               int   nEntrancePos )
    bool cpp_CreateMazeBraidTilt ( CMaz *maze,
                                   bool  fSection,
                                   bool  fTiltDiamond,
                                   int   nEntrancePos )
    bool cpp_CreateMazeSpiral ( CMaz *maze,
                                int   cRandomAdd,
                                int   cSpiral,
                                int   cSpiralWall,
                                bool  fSection,
                                int   nEntrancePos )
    bool cpp_CreateMazeDiagonal ( CMaz *maze,
                                  int   cRandomAdd,
                                  bool  fSection,
                                  int   nEntrancePos )
    bool cpp_CreateMazeRecursive ( CMaz *maze,
                                   bool  fSection,
                                   int   nEntrancePos )
    bool cpp_CreateMazePrim ( CMaz *maze,
                              bool  fSection,
                              bool  fTreeWall,
                              int   nEntrancePos )
    bool cpp_CreateMazePrim2 ( CMaz *maze,
                               bool  fSection,
                               bool  fTreeRandom,
                               bool  fTreeWall,
                               int   nEntrancePos )
    bool cpp_CreateMazeKruskal ( CMaz *maze,
                                 bool  fClear,
                                 CCol *c2,
                                 CCol *c3,
                                 bool  fKruskalPic,
                                 bool  fSection,
                                 bool  fTreeWall,
                                 int   nEntrancePos )
    bool cpp_CreateMazeTree ( CMaz *maze,
                              bool  fSection,
                              bool  fTreeRandom,
                              bool  fTreeWall,
                              int   nEntrancePos,
                              int   nTreeRiver )
    bool cpp_CreateMazeForest ( CMaz *maze,
                                bool  fWall,
                                bool  fRiverFlow,
                                bool  fSection,
                                bool  fTreeRandom,
                                bool  fTreeWall,
                                int   nEntrancePos,
                                int   nForsAdd,
                                int   nForsInit,
                                int   nTreeRiver )
    bool cpp_CreateMazeAldousBroder ( CMaz *maze,
                                      bool  fSection,
                                      bool  fTreeWall,
                                      int   nEntrancePos )
    bool cpp_CreateMazeWilson ( CMaz *maze,
                                bool  fSection,
                                bool  fTreeWall,
                                int   nEntrancePos )
    bool cpp_CreateMazeEller ( CMaz *maze,
                               bool  fSection,
                               bool  fTreeWall,
                               int   nEntrancePos )
    bool cpp_CreateMazeBraidEller ( CMaz *maze,
                                    bool  fSection,
                                    int   nEntrancePos )
    bool cpp_CreateMazeDivision ( CMaz *maze,
                                  bool  fSection,
                                  int   nEntrancePos )
    bool cpp_CreateMazeBinary ( CMaz *maze,
                                int   cRandomAdd,
                                bool  fSection,
                                bool  fTreeWall,
                                int   nEntrancePos )
    bool cpp_CreateMazeSidewinder ( CMaz *maze,
                                    bool  fSection,
                                    bool  fTreeWall,
                                    int   nEntrancePos )
    bool cpp_Resize(CMaz *maze, unsigned width, unsigned height)
    int cpp_SaveBitmap(CMaz *maze,
                       const char *filename,
                       KV kvOn,
                       KV kvOff)
    int cpp_SaveText(CMaz *maze,
                     const char *filename,
                     bool fTextClip,
                     bool fLineChar,
                     bool fTextTab)

# color value constants
COLOR_WHITE = kvWhite
COLOR_BLACK = kvBlack

# nEntrancePos enum constants
ENTRANCE_CORNER   = epCorner
ENTRANCE_MIDDLE   = epMiddle
ENTRANCE_BALANCED = epBalanced
ENTRANCE_RANDOM   = epRandom

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

        :param int width: Odd integer, 3 or greater
        :param int height: Odd integer, 3 or greater
        '''
        if width < 0:
            raise ValueError('negative width')
        if height < 0:
            raise ValueError('negative height')
        # if width % 2 != 1:
        #     raise ValueError('width must be odd')
        # if height % 2 != 1:
        #     raise ValueError('height must be odd')
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
            self._maze = NULL

    def __len__(self):
        return self.height

    def __getitem__(self, key):
        if isinstance(key, int):
            y = key
            return [int(self.get(x, y)) for x in range(self.width)]
        else:
            if not isinstance(key, tuple) and len(key) == 2 and list(set(map(type, key))) == [int]:
                raise ValueError('key must be an integer or a tuple of two integers')
            (x,y) = key
            return self.get(x,y)

    def __contains__(self, value):
        for v in self:
            if v == value:
                return True
        return False

    def __iter__(self):
        i = 0
        try:
            while True:
                v = self[i]
                yield v
                i += 1
        except IndexError:
            return

    def __str__(self):
        return '<Maze: {}x{}>'.format(self.width, self.height)

    def __repr__(self):
        return str(self)

    def index(self, value):
        '''
        S.index(value) -> integer -- return first index of value.  Raises
        ValueError if the value is not present.

        :param value: A value to search for in this Maze
        :return: The index of the location in this Maze where `value` is first found
        :rtype: int
        :raises ValueError: If `value` is not found in this Maze
        '''
        for i, v in enumerate(self):
            if v == value:
                return i
        raise ValueError

    def count(self, value):
        '''
        S.count(value) -> integer -- return number of occurrences of
        value.

        :param value: A value to count in this Maze
        :return: The number of times `value` is found in this Maze.
        :rtype: int
        '''
        return sum(1 for v in self if v == value)

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

        :param int x: The x-coordinate of the pixel to get
        :param int y: The y-coordinate of the pixel to get
        :return: True if the given pixel is on, False if it is off.
        :rtype: bool
        '''
        if x < 0 or self._width <= x:
            raise IndexError('x coordinate out of range')
        if y < 0 or self._height <= y:
            raise IndexError('y coordinate out of range')
        return cpp_Get(self._maze, x, y)

    def create_perfect(self,
                       fRiver=True,
                       fRiverEdge=True,
                       fRiverFlow=True,
                       fSection=False,
                       fTreeWall=False,
                       nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a perfect maze
        using the Hunt and Kill algorithm.

        Create a new perfect Maze in the bitmap using the Hunt and
        Kill algorithm, by carving passages.

        :param bool fRiver: defaults to True
        :param bool fRiverEdge: defaults to True
        :param bool fRiverFlow: defaults to True
        :param bool fSection: defaults to False
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazePerfect(self._maze,
                                     fRiver,
                                     fRiverEdge,
                                     fRiverFlow,
                                     fSection,
                                     fTreeWall,
                                     nEntrancePos):
            raise MazeError('Could not create Perfect Maze.')

    def create_perfect2(self,
                        fRiver=True,
                        fSection=False,
                        nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Perfect2 maze.

        Create a new perfect Maze in the bitmap using the Hunt and
        Kill algorithm, by adding walls.

        :param bool fRiver: defaults to True
        :param bool fSection: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazePerfect2(self._maze,
                                      fRiver,
                                      fSection,
                                      nEntrancePos):
            raise MazeError('Could not create Perfect2 Maze.')

    def create_braid(self,
                     fSection=False,
                     nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Braid maze.

        Create a new braid Maze in the bitmap, i.e. a Maze without any
        dead ends, using a wall adding algorithm.

        :param bool fSection: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeBraid(self._maze,
                                   fSection,
                                   nEntrancePos):
            raise MazeError('Could not create Braid Maze.')

    def create_braid_tilt(self,
                          fSection=False,
                          fTiltDiamond=False,
                          nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a BraidTilt maze.

        Create a new braid Maze in the bitmap using a binary
        algorithm, i.e.  starting with a template based on the Tilt
        Maze pattern.

        :param bool fSection: defaults to False
        :param bool fTiltDiamond: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeBraidTilt(self._maze,
                                       fSection,
                                       fTiltDiamond,
                                       nEntrancePos):
            raise MazeError('Could not create BraidTilt Maze.')

    def create_spiral(self,
                      cRandomAdd=0,
                      cSpiral=15,
                      cSpiralWall=15,
                      fSection=False,
                      nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Spiral maze.

        Create a new spiral Maze in the bitmap, formed of interlocking
        spirals.

        :param int cRandomAdd: defaults to 0
        :param int cSpiral: defaults to 15
        :param int cSpiralWall: defaults to 15
        :param bool fSection: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeSpiral(self._maze,
                                    cRandomAdd,
                                    cSpiral,
                                    cSpiralWall,
                                    fSection,
                                    nEntrancePos):
            raise MazeError('Could not create Spiral Maze.')

    def create_diagonal(self,
                        cRandomAdd=0,
                        fSection=False,
                        nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Diagonal maze.

        Create a Maze with a diagonal bias, where many walls look like
        stairs.

        :param int cRandomAdd: defaults to 0
        :param bool fSection: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeDiagonal(self._maze,
                                      cRandomAdd,
                                      fSection,
                                      nEntrancePos):
            raise MazeError('Could not create Diagonal Maze.')

    def create_recursive(self,
                         fSection=False,
                         nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Recursive maze.

        Create a new perfect Maze in the bitmap using the Recursive
        Backtracking algorithm. This carves passages.

        :param bool fSection: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeRecursive(self._maze,
                                       fSection,
                                       nEntrancePos):
            raise MazeError('Could not create Recursive Maze.')

    def create_prim(self,
                    fSection=False,
                    fTreeWall=False,
                    nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Prim maze.

        Create a new perfect Maze in the bitmap using a modified
        version of Prim's algorithm. This can carve passages or add
        walls.

        :param bool fSection: defaults to False
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazePrim(self._maze,
                                  fSection,
                                  fTreeWall,
                                  nEntrancePos):
            raise MazeError('Could not create Prim Maze.')

    def create_prim2(self,
                     fSection=False,
                     fTreeRandom=True,
                     fTreeWall=False,
                     nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Prim2 maze.

        Create a new perfect Maze in the bitmap using full or
        simplified versions of Prim's algorithm. This can carve
        passages or add walls.

        :param bool fSection: defaults to False
        :param bool fTreeRandom: defaults to True
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazePrim2(self._maze,
                                   fSection,
                                   fTreeRandom,
                                   fTreeWall,
                                   nEntrancePos):
            raise MazeError('Could not create Prim2 Maze.')

    def create_kruskal(self, fClear, c2, c3,
                       fKruskalPic=False,
                       fSection=False,
                       fTreeWall=False,
                       nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Kruskal maze.

        Create a new perfect Maze in the bitmap using Kruskal's
        algorithm. This can carve passages or add walls.

        :param bool fKruskalPic: defaults to False
        :param bool fSection: defaults to False
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        # TODO
        cdef CCol *cpp_c2 = NULL
        cdef CCol *cpp_c3 = NULL
        if not cpp_CreateMazeKruskal(self._maze, fClear, cpp_c2, cpp_c3,
                                     fKruskalPic,
                                     fSection,
                                     fTreeWall,
                                     nEntrancePos):
            raise MazeError('Could not create Kruskal Maze.')

    def create_tree(self,
                    fSection=False,
                    fTreeRandom=True,
                    fTreeWall=False,
                    nEntrancePos=ENTRANCE_RANDOM,
                    nTreeRiver=10):
        '''
        Overwrites the contents of this Maze to create a Tree maze.

        Create a new perfect Maze in the bitmap using the Growing Tree
        algorithm.  This can carve passages or add walls.

        :param bool fSection: defaults to False
        :param bool fTreeRandom: defaults to True
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :param int nTreeRiver: defaults to 10
        :rtype: None
        '''
        if not cpp_CreateMazeTree(self._maze,
                                  fSection,
                                  fTreeRandom,
                                  fTreeWall,
                                  nEntrancePos,
                                  nTreeRiver):
            raise MazeError('Could not create Tree Maze.')

    def create_forest(self, fWall,
                      fRiverFlow=True,
                      fSection=False,
                      fTreeRandom=True,
                      fTreeWall=False,
                      nEntrancePos=ENTRANCE_RANDOM,
                      nForsAdd=-100,
                      nForsInit=1,
                      nTreeRiver=10):
        '''
        Overwrites the contents of this Maze to create a Forest maze.

        Create a new perfect Maze in the bitmap using the Growing
        Forest algorithm.  This can carve passages or add walls.

        :param bool fRiverFlow: defaults to True
        :param bool fSection: defaults to False
        :param bool fTreeRandom: defaults to True
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :param int nForsAdd: defaults to -100
        :param int nForsInit: defaults to 1
        :param int nTreeRiver: defaults to 10
        :rtype: None
        '''
        if not cpp_CreateMazeForest(self._maze, fWall,
                                    fRiverFlow,
                                    fSection,
                                    fTreeRandom,
                                    fTreeWall,
                                    nEntrancePos,
                                    nForsAdd,
                                    nForsInit,
                                    nTreeRiver):
            raise MazeError('Could not create Forest Maze.')

    def create_aldous_broder(self,
                             fSection=False,
                             fTreeWall=False,
                             nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a AldousBroder maze.

        Create a new perfect Maze in the bitmap using the
        Aldous-Broder algorithm.  This can carve passages or add
        walls. This is the simplest unbiased algorithm for creating
        perfect Mazes.

        :param bool fSection: defaults to False
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeAldousBroder(self._maze,
                                          fSection,
                                          fTreeWall,
                                          nEntrancePos):
            raise MazeError('Could not create AldousBroder Maze.')

    def create_wilson(self,
                      fSection=False,
                      fTreeWall=False,
                      nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Wilson maze.

        Create a new perfect Maze in the bitmap using Wilson's
        algorithm. This can carve passages or add walls. Like the
        Aldous-Broder algorithm, this generates all possible Mazes
        with equal probability, however this runs about five times
        faster on average.

        :param bool fSection: defaults to False
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeWilson(self._maze,
                                    fSection,
                                    fTreeWall,
                                    nEntrancePos):
            raise MazeError('Could not create Wilson Maze.')

    def create_eller(self,
                     fSection=False,
                     fTreeWall=False,
                     nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Eller maze.

        Create a new perfect Maze in the bitmap using Eller's
        algorithm. This can carve passages or add walls. This is the
        fastest algorithm for creating general perfect Mazes, and runs
        over twice as fast as any of the others.

        :param bool fSection: defaults to False
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeEller(self._maze,
                                   fSection,
                                   fTreeWall,
                                   nEntrancePos):
            raise MazeError('Could not create Eller Maze.')

    def create_braid_eller(self,
                           fSection=False,
                           nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a BraidEller maze.

        Create a new braid Maze in a bitmap using a variation of
        Eller's Algorithm.

        :param bool fSection: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeBraidEller(self._maze,
                                        fSection,
                                        nEntrancePos):
            raise MazeError('Could not create BraidEller Maze.')

    def create_division(self,
                        fSection=False,
                        nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Division maze.

        Create a new perfect Maze in the bitmap using recursive
        division. This always adds walls, recursively dividing the
        Maze into smaller rectangles.

        :param bool fSection: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeDivision(self._maze,
                                      fSection,
                                      nEntrancePos):
            raise MazeError('Could not create Division Maze.')

    def create_binary(self,
                      cRandomAdd=0,
                      fSection=False,
                      fTreeWall=False,
                      nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Binary maze.

        Create a new perfect Maze in the bitmap using the Binary Tree
        algorithm.  This can carve passages or add walls. This is the
        simplest algorithm of any type for creating perfect
        Mazes.

        :param int cRandomAdd: defaults to 0
        :param bool fSection: defaults to False
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeBinary(self._maze,
                                    cRandomAdd,
                                    fSection,
                                    fTreeWall,
                                    nEntrancePos):
            raise MazeError('Could not create Binary Maze.')

    def create_sidewinder(self,
                          fSection=False,
                          fTreeWall=False,
                          nEntrancePos=ENTRANCE_RANDOM):
        '''
        Overwrites the contents of this Maze to create a Sidewinder maze.

        Create a new perfect Maze in the bitmap using the Sidewinder
        algorithm.  This can carve passages or add walls.

        :param bool fSection: defaults to False
        :param bool fTreeWall: defaults to False
        :param int nEntrancePos: defaults to ENTRANCE_RANDOM
        :rtype: None
        '''
        if not cpp_CreateMazeSidewinder(self._maze,
                                        fSection,
                                        fTreeWall,
                                        nEntrancePos):
            raise MazeError('Could not create Sidewinder Maze.')

    def resize(self, width, height):
        '''
        Truncates or pads out this Maze to attain the given size.

        :param int width: Odd integer, 3 or greater
        :param int height: Odd integer, 3 or greater
        :rtype: None
        '''
        if width < 0:
            raise ValueError('negative width')
        if height < 0:
            raise ValueError('negative height')
        # if width % 2 != 1:
        #     raise ValueError('width must be odd')
        # if height % 2 != 1:
        #     raise ValueError('height must be odd')
        if not cpp_Resize(self._maze, width, height):
            raise MazeError('Could not resize the Maze.')
        self._width = width
        self._height = height

    @staticmethod
    def _handle_save_retval(rv):
        '''
        Helper method to handle return values from the Save* methods.

        :param int rv: the value returned from the Save* method.
        :rtype: None
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

    def save_bitmap(self, filename, kvOn=COLOR_WHITE, kvOff=COLOR_BLACK):
        '''
        Saves this Maze object as a bitmap to the given path.

        :param str filename: the name of the file to write the bitmap to
        :param int kvOn: defaults to white (COLOR_WHITE)
        :param int kvOff: defaults to black (COLOR_BLACK)
        :rtype: None
        '''
        retval = cpp_SaveBitmap(self._maze, filename, kvOn, kvOff)
        self._handle_save_retval(retval)

    def save_text(self, filename, fTextClip=True, fLineChar=False, fTextTab=False):
        '''
        Saves this Maze object formatted in ASCII text to the given path.

        Arguments:
        :param filename: the name of the file to write the ASCII representation to
        :param bool fTextClip: defaults to True
        :param bool fLineChar: defaults to False
        :param bool fTextTab: defaults to False
        :rtype: None
        '''
        retval =  cpp_SaveText(self._maze, filename, fTextClip, fLineChar, fTextTab)
        self._handle_save_retval(retval)
