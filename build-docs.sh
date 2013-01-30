#!/bin/sh
doxygen Doxyfile
cd documentation
git add .
git commit -m "Updated documentation"
git pull origin gh-pages
git push origin gh-pages