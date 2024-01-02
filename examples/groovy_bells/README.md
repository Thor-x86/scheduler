# The Groovy Bells

In this example program, it will demonstrate how scheduling could be useful for
managing a beat pattern. To try this, you have to run the program with a
terminal console which capable of ringing the bell (or beeping).

> **ATTENTION:** Windows Terminal sometimes won't ring the bell properly.

Before trying the example program, check whether the environment variable is set
properly with this command:

```bash
echo $LD_LIBRARY_PATH
```

The output must include this:

```
/usr/local/lib
```

If it's not (or empty), run this command:

```bash
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib
```

Then you can compile with this command:

```bash
cc main.c tasks.c -l scheduler -o groovy_bells
```

...and run the compilation output:

```bash
./groovy_bells
```
