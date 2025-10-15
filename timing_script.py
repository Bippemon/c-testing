import subprocess
import time

start = time.time_ns()
var = "/home/simon/c-testing/client.exe"

result = subprocess.run(var)
finish = time.time_ns()

time_tot = finish - start
print(time_tot / 1_000_000, "ms")