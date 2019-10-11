#! /bin/bash

OUT_DIR="../tmp"

ffmpeg -i $1 -pix_fmt monob -vf "scale=7:7, fps=15" ${OUT_DIR}/frame%04d.png -hide_banner
