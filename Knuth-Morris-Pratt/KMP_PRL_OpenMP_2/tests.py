import subprocess
make = "make"

subprocess.call([make])


subprocess.call(['./server', '2>&1', '|', 'tail -2'])


print("----------------------------")


