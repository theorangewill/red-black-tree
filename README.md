## Red-black tree
This program implements a red-black tree.

It receives a text file and prints a red-black tree with the words in the file.

input:

```
Um, dois, tres, testando.
Testando tres tres tres vezes.
Um teste final.
```

The output prints the tree and the list of words (with the line numbers which it occurs).

The tree follows the syntax:
```
L(WORD [COLOR])R
L and R = /, if the node has a Left/Right child
          *, if does not have a Left/Right child
WORD = is the word
COLOR = R, if the color is RED
        B, if the color is BLACK
```

output:
```
/(testando [B])\ 
*(dois [B])\ /(tres [B])\ 
*(final [R])* *(teste [R])* *(vezes [R])* 
dois 1
final 3
testando 1,2
teste 3
tres 1,2
vezes 2
```