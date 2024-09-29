#!/bin/sh

cc -fsanitize=address,undefined -Wall -Wextra -g main.c jump.s threads.c
