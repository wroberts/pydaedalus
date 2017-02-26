#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Wrapper for the daedalus maze functionality.

maze = Maze(63, 63)
maze.CreateMazePerfect()
maze.Resize(31, 61)
maze.CreateMazePerfect()
maze.SaveBitmap("test.bmp")
maze.SaveText("test.txt")
'''

class Maze(object):
    '''A daedalus Maze object.'''

    cdef CMaz *_maze

    def __cinit__(self, width, height):
        '''
        Maze constructor.

        Arguments:
        - `width`: odd integer, 3 or greater
        - `height`: odd integer, 3 or greater
        '''
        self._maze = cpp_Constructor(width, height)
        if self._maze is NULL:
            raise MazeError('Could not construct Maze object.')
        self.width = width
        self.height = height

    def __dealloc__(self):
        '''Destructor.'''
        if self._maze is not NULL:
            cpp_Destructor(self._maze)

    def CreateMazePerfect(self):
        if not cpp_CreateMazePerfect(self._maze):
            raise MazeError('Could not create perfect Maze.')

    def Resize(self, width, height):
        if not cpp_Resize(self._maze, width, height):
            raise MazeError('Could not resize the Maze.')
        self.width = width
        self.height = height

    def _HandleSaveRetval(rv):
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

    def SaveBitmap(self, filename):
        retval = cpp_SaveBitmap(self._maze, filename)
        self._HandleSaveRetval(retval)

    def SaveText(self, filename):
        retval =  cpp_SaveText(self._maze, filename)
        self._HandleSaveRetval(retval)
