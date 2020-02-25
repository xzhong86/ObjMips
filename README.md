# Object MIPS

  Simple test platform for Ingenic (MIPS) SOC.

## The Story

  * At beginning, I just wrote a simple program to run on chip directly (without OS) to test basic function of MIPS CPU core, like SMP cache coherence.
  * Then, added device control, interrupt control, memory management, page management, virtual memory.
  * Then, thread (task control, thread switch), signal, semaphore, console.
  * Then, command line on console, .etc.
  * Then it becomes a simple Operating System!

  It is very insteresting to implement OS features step by step. I learned from linux kernel, and wrote it into this project.


## Plan

  I don't have Ingenic development board (jz4760 4770 .etc) to run this program, so I plan to run on QEMU.
