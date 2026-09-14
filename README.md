### This project is a 42 solo project. I must recode the nm command.
Should take me aroung 14 days... Start on the 29 of July 2026

#### Progress
First part is 'done'. I can print correctly the symbols in an object (.o) file. However, for a binary file, a lot of symbols are still missing. I don't parse 'every' symbols just yet.
Also I don't sort them, which nm requires to do so. I should also leave the option not to sort, take ELF order symbols, like nm do when -p flag is given.

##### Symbols I don't print yet:
r
B
D
W
w
d
R
and they are in my ft_nm library. What are they ? How to find them?

##### Symbols show by the -a options
N
a
r
t
Some debugging symbols, some filename.

I shouldn't print directly inside the terminal. I should store in a chained list (very simple list) and order it from there.

##### Bonus:
To do: -a -g -u -r -p. '-p' should be done, it's easy. '-a' should also be done it's not that complicated since I already watch ALL symbols in my loop just ignoring some.

- a : display all symbols, even debbuger-only symbols
- g : display only external symbols
- p : no sort. Do not bother to sort the symbols in any order, print them in the order encounterd
- r : reverse the order of the sort (so easy if sorting is done with chained list...)
- u : display only undefined symbols (those external to each object file). By default both defined and undefined symbors are displayed

g is external so STB_GLOBAL / STB_WEAK defined or undefined.
u is undefined only.
Overlap but not equivalent


##### Next steps:
-> watch for potential refactor.
-> create a table of pointer. Put all pointers to symbols headers inside it. Then sort it by name (st_name inside string table).

#### Sources and tips:
- https://kib.kiev.ua/x86docs/ARM/abi-aa/aaelf64.pdf
- https://elearning.intra.42.fr/notions/68/subnotions/264/videos/220
