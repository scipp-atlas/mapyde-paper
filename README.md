[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit) | [![pre-commit.ci status](https://results.pre-commit.ci/badge/github/scipp-atlas/mapyde-paper/main.svg)](https://results.pre-commit.ci/latest/github/scipp-atlas/mapyde-paper/main)

# To compile

```
$ make -C ./figures/
$ latexmk -pdf main.tex
```

## Dependencies

- [arxiv-style](https://github.com/kourgeorge/arxiv-style/tree/282153fc15470f9192c6ea1b90e5f5f98912f527)

## Pre-Commit

Run `pre-commit install` to install the pre-commit hooks.
