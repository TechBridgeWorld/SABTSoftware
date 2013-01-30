#!/bin/sh

# Check that the documentation folder exists, if not create and initialize
if [ ! -d documentation ]; then
    mkdir documentation && cd documentation
    
    git clone git@github.com:CMU-15-239/SABTSoftware.git .
    git checkout gh-pages
    git branch -d master
    
    cd ..
fi

# Build docs
doxygen Doxyfile

# Push to gh-pages
cd documentation
git add .
git commit -m "Updated documentation"
git pull origin gh-pages
git push origin gh-pages