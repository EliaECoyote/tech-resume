#!/bin/bash
set -e

# ---------
# store cli args in a variable
ARGS=($@)

# ---------
# creates a symlink from the pdf file to stdout.
# this step is necessary because pandoc currently must generate a pdf and
# cannot output the pdf into base64 by itself.
#
# ln command args:
# -f => If the target file already exists, then unlink it so that the link may occur.
# -s => Create a symbolic link
ln -sf /dev/stdout pandoc_stdout.pdf

# ---------
# generates pdf trough pandoc, that gets forwarded to stdout and piped into base64
pandoc ${ARGS} | base64
