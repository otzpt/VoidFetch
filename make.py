import subprocess
import sys, tty, termios

subprocess.run(["gcc", "-Wall", "main.c", "-o", "voidfetch"], check=True)
subprocess.run(["./voidfetch"], check=True)

fd = sys.stdin.fileno()
old = termios.tcgetattr(fd)

try:
    tty.setcbreak(fd)
    if sys.stdin.read(1) == "q":
        exit()
finally:
    termios.tcsetattr(fd, termios.TCSADRAIN, old)
