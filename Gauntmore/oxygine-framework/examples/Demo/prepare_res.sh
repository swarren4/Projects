#!/bin/bash
python ../../tools/oxyresbuild.py -x xmls/res.xml --src_data data --dest_data data/ext
python ../../tools/oxyresbuild.py -x demo/res_ui.xml --src_data data --dest_data data/ext
