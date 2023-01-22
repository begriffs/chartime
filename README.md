### Measure delay between stdin characters

#### Overview

Outputs delay in microseconds between each single-byte call to `read(3)`.  Note
that this is most useful in measuring input streams where there is fairly large
natural delay between characters, because making a system call per byte would
naturally slow down bulk reads.

My own use case is measuring timed bytes from UART to check the clock-tree
calibration of an MCU.

#### Building

To build, simply run `make`. Requires C99 and POSIX 2001 (issue 6).
