In the spirit of code reusability, MD2 and MD7 are templates for letter practice (single cell and multi-cell respectively).

The ideal solution of course would involve rewriting the SABT code in C++ to take advantage of inheritance. However, since I'm too lazy to do that this summer, go ahead and make a copy of the template and change 4 lines of code instead - specifically from line 40 to 45.

An avenue for further development would be to unify all letter practices under one MD file and simply set the variables when selecting the mode. I have no idea why I didn't think of this till the very last day of the internship. Shouldn't be too hard, so knock yourself out.

So, when duplicating the template, there are a couple of files that need to be in place in order for everything to work fine.

1. Make sure mode MP3 files are in the SD card. Ref: sd_card_files for those files.
2. Make sure language files are in place. Ref: README.common_lang_files for a list of those.
3. Set up your script under a new header file. Ref: script_hindi.h for an example of how to do this. Hindi has support for multi-cell characters.
4. Follow standard mode registration procedure for ui_handle.c. Ref: README.adding_modes