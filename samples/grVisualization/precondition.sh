#!/bin/bash

sudo rfcomm bind  /dev/rfcom0 98:D3:32:10:AC:59 1

export LD_LIBRARY_PATH=/home/h3lgi/code/grSDK/externAssets/grt/lib:${LD_LIBRARY_PATH}
