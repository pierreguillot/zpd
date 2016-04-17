#!/bin/bash -e


# rm all the files through git to prevent stale files.
cd doc
git rm -rf .
cd -

# Generate the HTML documentation.
doxygen zpddoc

# Create and commit the documentation repo.
cd doc
git add .
git config user.name "Documentation Builder"
git config user.email ""
git commit -m "Automated documentation build for changeset git rev-parse --verify HEAD."
git push origin gh-pages
cd -
