The /docs is needed for developer docs

## How to generate doxygen docs
> run `doxygen Doxyfile` from directory `docs/`
> generated documentation will be available in `docs/out/html/index.html`

## How to add new page
For example we want to create a page "aboutus".
So, we need to `cd docs/pages`, create a file `touch aboutus.md` with following content: 
``` 
About Us {#aboutus}
=========

content is here
```
After that, re-generate doxygen docs and you'll be able to refer this page from other pages using `[Link Name](@ref aboutus)`
