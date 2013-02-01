#!/bin/sh

# Check that the documentation folder exists, if not create and initialize
if [ ! -d documentation ]; then
    mkdir documentation && cd documentation
    
    git clone git@github.com:CMU-15-239/SABTSoftware.git .
    git checkout gh-pages
    
    cd ..
fi

# Get the latest changes
cd documentation
git pull origin gh-pages
cd ..

# Build docs
doxygen Doxyfile

# Push to gh-pages
cd documentation
touch .nojekyll # Necessary to fix files that begin with an underscore
git add .
git commit -m "Updated documentation"
git push origin gh-pages