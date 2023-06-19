rm -f mapyde_arxiv.tar.gz

sed s/"finalizecache"/"frozencache"/ main.tex > main_final.tex

cp -p main.tex main_backup.tex
cp -p main_final.tex main.tex

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

cp -p main_backup.tex main.tex
