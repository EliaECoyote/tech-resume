# Api module

This module consists in a stateless node server, that:

- transforms markdown requests to html syntax through [unified](https://github.com/unifiedjs/unified) and [remark](https://github.com/remarkjs/remark)
- forwards requests to the **pdfgen** service and returns the result
