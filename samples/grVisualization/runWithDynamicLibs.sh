#!/bin/bash
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"../../externAssets/grt/lib"
export LD_LIBRARY_PATH
exec "./bin/GRVisualization"
