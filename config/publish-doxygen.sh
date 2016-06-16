#!/bin/bash

if [ $TRAVIS_BRANCH == 'master'] && [ $Documentation = 'on' ]
then
cd config
rm -rf doc
mkdir -p doc
git clone -b gh-pages git@github.com:pierreguillot/zpd.git --single-branch doc


cd doc
git rm -rf .
cd ..
doxygen xpddoc
doxygen cpddoc
cp doxygen.css doc/doxygen.css
cp index.html doc/index.html


cd doc
git add .
git config --global user.name "pierreguillot"
git config --global user.email guillotpierre6@gmail.com
git commit -m "Automated documentation build."
git push origin gh-pages
cd ..
cd ..
fi
