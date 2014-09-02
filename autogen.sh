#! /bin/sh
mkdir -p icons/{48,128}
inkscape --export-png=icons/48/pg800.png icons/pg800-48x48.svg
inkscape --export-png=icons/128/pg800.png icons/pg800-128x128.svg
autoreconf -if
