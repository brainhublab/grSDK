## GR Visualization


building:

`build grt`

`▲ grVisualization → mkdir build && cd build`

`▲ grVisualization/build → export LIBRARY_PATH=${path_to_grSDK}grSDK/externAssets/grt/lib`

`▲ grVisualization/build → export LD_LIBRARY_PATH=${path_to_grSDK}grSDK/externAssets/grt/lib`

`▲ grVisualization/build → qmake ../ && make`

`▲ grVisualization/build → bin/GRVisualization`
