
extern int printf(const char *, ...);

/* Will output '2 1' if positional specifiers are supported */
int main(void)
{
    printf("%2$d %1$d\n", 1, 2);
    return 0;
}
