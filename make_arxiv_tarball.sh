rm -f mapyde_arxiv.tar.gz

tar -cvf mapyde_arxiv.tar \
    main.tex \
    main.bbl \
    arxiv.sty \
    minted.sty \
    minted1.sty \
    orcid.pdf \
    figures/*.pdf \
    figures/feynman/output/* \
    figures/tikz/output/* \
    _minted-main/*

gzip mapyde_arxiv.tar
