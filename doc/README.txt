----------------------------------------------------------------
cprotect --- verbatim in macro arguments
E-mail: blflatex@gmail.com
Released under the LaTeX Project Public License v1.3c or later
See http://www.latex-project.org/lppl.txt
----------------------------------------------------------------

A user of the cprotect.sty package can include verbatim text (\verb,
verbatim environment, ...) in the argument of any command,
including moving arguments. Including verbatim text in footnotes,
section titles (and other titles), etc. is as simple as typing
`\cprotect\footnote` instead of `\footnote`, etc.

A similar approach is taken for environments. Nesting of the
various environments is taken into account.

This is work in progress, all comments/bug reports are welcome!

The whole package can be built from the file `cprotect.dtx' by running

pdflatex cprotect.dtx
makeindex -s gglo.ist -o cprotect.gls cprotect.glo
makeindex -s gind.ist -o cprotect.ind cprotect.idx
pdflatex cprotect.dtx
pdflatex cprotect.dtx
mv README.txt README

The first run of pdflatex produces `cprotect.ins', `cprotect.sty',
`cprotect.pdf', and this file. The other commands are necessary for
the pdf to contain its index and glossary.

