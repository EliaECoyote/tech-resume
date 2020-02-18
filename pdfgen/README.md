# Pdfgen module

This module consists in a stateless node server, that receives html and returns a PDF binary data.\
The conversion is made through mounting an instance of the [puppeteer headless browser](https://github.com/puppeteer/puppeteer), rendering a page with the received html, and generating a PDF on the fly.
