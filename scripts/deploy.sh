#!/bin/bash

echo '------ ssh starts here -----'
ssh evntmgr@tstfront '
ls && \
rm -fr ./*
echo "all files removed from tstfront" && \
ls
'

echo '------ DEPLOY -----'
scp frontend evntmgr@tstfront:./
scp makefile evntmgr@tstfront:./
scp -r ./resources evntmgr@tstfront:./resources

