# Simple Example for Implementing Scheduler Module

This is a plain simple example of how to use scheduler module. Please take a
look to `main.c` file to figure out the example. If you want to try the result,
you have to build and install the scheduler first. Then check whether the
environment variable is set properly with this command:

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
cc main.c -l scheduler -o simple
```

...and run the compilation result with this command:

```bash
./simple
```
