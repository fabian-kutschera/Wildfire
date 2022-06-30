#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

print( "Reading data from file 'wildfire.data'" )
data = np.loadtxt( "wildfire.data" )
data = data.reshape( (-1,data.shape[1],data.shape[1]) )

cmap = ListedColormap( ["lightgrey", "green", "darkred"] )
normaliser = plt.Normalize( 0, 2, clip=False )

for k in range(0,data.shape[0]):
    plt.imshow( data[k][:][:], interpolation=None, cmap=cmap,
                norm=normaliser, aspect="equal", origin="lower" )
    plt.title( f"After step #{k}" )
    plt.pause(0.1)
    plt.clf()

input( "Press key to end.\n" )
