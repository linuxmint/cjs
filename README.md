JavaScript bindings for Cinnamon
================================

Based on GJS: https://wiki.gnome.org/action/show/Projects/Gjs

Please do the following when reporting CJS crashes:
===================================================

If possible, provide a stack trace.

Run dmesg and provide the line related to the crash, for instance:

`[ 4947.459104] cinnamon[2868]: segfault at 7f2611ffffe8 ip **00007f2667dda305** sp 00007fffb416b9d0 error 4 in libcjs.so.0.0.0[**7f2667db1000**+c1000]`

Launch the Calculator, choose Advanced Mode and set it to Hexadecimal. Then substract the loading address (first address in brackets: 7f2667db1000) from the ip (00007f2667dda305).

In the example above:

ip: 00007f2667dda305
loading address: 7f2667db1000

`00007f2667dda305 - 7f2667db1000 = 29305`

This gives us the offset. Use addr2line to see what's under it in our shared library:

```
addr2line -e /usr/lib/libcjs.so.0.0.0 29305 -fCi
gjs_typecheck_boxed
```

When reporting the bug, along with the trace and the dmesg line, please report that function name (in this example gjs_typecheck_boxed).
