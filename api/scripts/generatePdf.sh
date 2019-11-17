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
# ln -sf /dev/stdout stdout.pdf
# ---------
# generates pdf trough pandoc, that gets forwarded to stdout and piped into base64

pandoc --standalone --include-in-header styles/template1.css \
  --from markdown --to html \
  --metadata pagetitle=stdout.html

# pandoc -t latex --template styles/template1.tex --from markdown --to html
# mtxrun --path=./ --result=stdout.pdf --script context output.tex

# ls

# mtxrun --result=stdout.pdf --script context output.tex

# pandoc ${ARGS}

# wkhtmltopdf -q google.com /tmp/test.pdf 2>/dev/null && cat /tmp/test.pdf | base64 && rm /tmp/test.pdf

# ls /dev
# echo "test" >>/dev/stdout
