import time
import subprocess
st = time.time()

for i in range(10): 
    subprocess.call(['./server'])

et = time.time()

elapsed_time = (et-st)/10
print('Execution time: ',elapsed_time,'seconds')