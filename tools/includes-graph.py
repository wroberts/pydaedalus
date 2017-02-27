#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
map-deps.py
(c) Will Roberts  26 February, 2017

Determine #include relationships and export these as a graphviz .dot
file.
'''

import os
import glob

full_paths = glob.glob('../daedalus/src/*.cpp') + glob.glob('../daedalus/src/*.h')

def get_includes(filename):
    with open(filename) as input_file:
        for line in input_file:
            line = line.strip()
            if line.startswith('#include'):
                line = line.replace('#include', '').strip()
                yield line.strip('"').strip('<>')

# make graph
graph = {}
for full_path in full_paths:
    filename = os.path.basename(full_path)
    graph[filename] = list(get_includes(full_path))

# write to DOT file
# https://pypi.python.org/pypi/graphviz
import graphviz

dot = graphviz.Digraph(comment='Daedalus source files')
names_to_labels = dict(('n{:05d}'.format(idx), label)
                       for idx, label in
                       enumerate(sorted(graph.keys())))
labels_to_names = dict((l,n) for (n,l) in names_to_labels.items())
for name, label in names_to_labels.items():
    # add node
    dot.node(name, label)
    # add edges for node
    for olabel in graph[label]:
        # filter out standard imports (<stdio.h>)
        if olabel in labels_to_names:
            dot.edge(name, labels_to_names[olabel])

# render the graph
dot.render('deps.dot')
