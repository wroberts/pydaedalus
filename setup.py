#!/usr/bin/env python
# -*- coding: utf-8 -*-

from setuptools import setup, find_packages, Extension
from codecs import open  # To use a consistent encoding
from os import path
import sys

HERE = path.abspath(path.dirname(__file__))

with open(path.join(HERE, 'daedalus', 'VERSION'), encoding='utf-8') as f:
    VERSION = f.read().strip()

# Get the long description from the relevant file
with open(path.join(HERE, 'README.rst'), encoding='utf-8') as f:
    LONG_DESCRIPTION = f.read()

with open(path.join(HERE, 'daedalus', 'pdconfig.h'), 'w') as f:
    f.write('int PYTHON_VERSION2 = {};\n'.format(int(sys.version_info[0] == 2)))

USE_CYTHON = False
try:
    from Cython.Build import cythonize
    USE_CYTHON = True
except ImportError:
    pass

PYDAEDALUS_EXTENSIONS = [Extension(
    "daedalus._maze",
    [
        'daedalus/_maze' + ('.pyx' if USE_CYTHON else '.cpp'),
        'daedalus/wrapper.cpp',
        'daedalus/src/daedalus.cpp',
        'daedalus/src/maze.cpp',
        'daedalus/src/solve.cpp',
        'daedalus/src/util.cpp',
        'daedalus/src/graphics.cpp',
        'daedalus/src/color.cpp',
        'daedalus/src/create.cpp',
        'daedalus/src/create2.cpp',
        'daedalus/src/create3.cpp',
        'daedalus/src/labyrnth.cpp',
        'daedalus/src/threed.cpp',
        'daedalus/src/draw.cpp',
        'daedalus/src/inside.cpp',
        'daedalus/src/command.cpp',
        'daedalus/src/solids.cpp',
        'daedalus/src/draw2.cpp',
    ],
    include_dirs=['daedalus/src'],
    extra_compile_args=['-std=c++11', '-Wno-error=format-security'],
    language="c++")]
if USE_CYTHON:
    PYDAEDALUS_EXTENSIONS = cythonize(PYDAEDALUS_EXTENSIONS)

setup(
    name='pydaedalus',

    # Versions should comply with PEP440.  For a discussion on single-sourcing
    # the version across setup.py and the project code, see
    # https://packaging.python.org/en/latest/single_source_version.html
    version=VERSION,

    description='Python wrapper of Daedalus monochrome maze',
    long_description=LONG_DESCRIPTION,

    # The project's main homepage.
    url='https://github.com/wroberts/pydaedalus',

    # Author details
    author='Will Roberts',
    author_email='wildwilhelm@gmail.com',

    # Choose your license
    license='GPLv2',

    # See https://pypi.python.org/pypi?%3Aaction=list_classifiers
    classifiers=[
        # How mature is this project? Common values are
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        'Development Status :: 3 - Alpha',

        # Indicate who your project is intended for
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'Topic :: Games/Entertainment :: Puzzle Games',

        # Pick your license as you wish (should match "license" above)
        'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',

        # Specify the Python versions you support here. In particular, ensure
        # that you indicate whether you support Python 2, Python 3 or both.
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.2',
        'Programming Language :: Python :: 3.3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
    ],

    # What does your project relate to?
    keywords='maze generation solving daedalus',

    # You can just specify the packages manually here if your project is
    # simple. Or you can use find_packages().
    packages=find_packages(),

    ext_modules = PYDAEDALUS_EXTENSIONS,

    # List run-time dependencies here.  These will be installed by pip when your
    # project is installed. For an analysis of "install_requires" vs pip's
    # requirements files see:
    # https://packaging.python.org/en/latest/requirements.html
    #install_requires=['peppercorn'],

    # List additional groups of dependencies here (e.g. development dependencies).
    # You can install these using the following syntax, for example:
    # $ pip install -e .[dev,test]
    #extras_require = {
    #    'dev': ['check-manifest'],
    #    'test': ['coverage'],
    #},

    # If there are data files included in your packages that need to be
    # installed, specify them here.  If using Python 2.6 or less, then these
    # have to be included in MANIFEST.in as well.
    #package_data={
    #    'sample': ['package_data.dat'],
    #},

    # Although 'package_data' is the preferred approach, in some case you may
    # need to place data files outside of your packages.
    # see http://docs.python.org/3.4/distutils/setupscript.html#installing-additional-files
    # In this case, 'data_file' will be installed into '<sys.prefix>/my_data'
    #data_files=[('my_data', ['data/data_file'])],

    # To provide executable scripts, use entry points in preference to the
    # "scripts" keyword. Entry points provide cross-platform support and allow
    # pip to create the appropriate form of executable for the target platform.
    #entry_points={
    #    'console_scripts': [
    #        'sample=sample:main',
    #    ],
    #},
    setup_requires=['pytest-runner'],
    tests_require=['pytest'],
)
