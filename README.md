# Custom UNIX Shell

## The Shell Language
- The input to the shell is a sequence of lines. The shell must correctly handle lines of up to 100 characters. If a line containing more than 100 characters is submitted to the shell, it should print some kind of error message and then continue processing input at the start of the next line.

- Each line consists of tokens. Tokens are separated by one or more spaces. A line may contain as many tokens as can fit into 100 characters.

- There are two kinds of tokens: operators and words. The only operators are <, >, and | (pipe).

- Words consist of the characters A–Z, a–z, 0–9, dash, dot, forward slash, and underscore. If a word in a line of input to the shell contains any character not in this set, then the shell should print an error message and then continue processing input at the start of the next line.

- The only legal input to the shell, other than lines consisting of valid tokens, is the end-of-file.

This custom shell uses fork(), exec(), dup2(), and pipes.
