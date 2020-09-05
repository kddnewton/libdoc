# libdoc

[![Build Status](https://github.com/kddeisz/libdoc/workflows/Main/badge.svg)](https://github.com/kddeisz/libdoc/actions)

This project is a work-in-progress.

This project is a C implementation of Philip Wadler's [A prettier printer](https://homepages.inf.ed.ac.uk/wadler/papers/prettier/prettier.pdf). The source is based on the JavaScript implementation [within prettier](https://github.com/prettier/prettier/tree/master/src/document). The function of this is to have a library that can be bound from multiple contexts (i.e., remove the need to run the printer through a node process. Hopefully this will make it easier to use printer from prettier within other formatters.

So far, these are the commands [from prettier](https://github.com/prettier/prettier/blob/master/commands.md) that have been implemented:

* [x] concat
* [x] group
* [ ] conditionalGroup
* [ ] fill
* [x] ifBreak
* [ ] breakParent
* [x] join
* [x] line
* [x] softline
* [x] hardline
* [x] literalline
* [ ] lineSuffix
* [ ] lineSuffixBoundary
* [x] indent
* [x] dedent
* [ ] align
* [ ] markAsRoot
* [ ] dedentToRoot
* [ ] trim
* [ ] cursor

## JavaScript

There is additionally a JavaScript library embedded in this repository under `javascript` that links against the C library defined under `src`. It uses [n-api](https://nodejs.org/api/n-api.html) to access the node headers. Currently, you can create doc tree from prettier and then pass it into the native module to use this printer. In the future this could potentially be released as a separate package that could replace `printDocToString`.
