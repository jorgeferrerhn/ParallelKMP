import subprocess
import time
from threading import Thread
make = "make"

def f1():
    subprocess.call(['mpirun','-n','4','./server'])


def f2():
    subprocess.call(['./client'])


if __name__ == "__main__":

    subprocess.call([make])


    st = time.time()

    print("Server starting")
    x = Thread(target=f1)
    x.start()
    
    print("Client starting")

    y = Thread(target=f2)
    y.start()
    

    print("Waiting for server")
    x.join()
    y.join()

    et = time.time()

    elapsed_time = (et-st)/10
    print('Execution time: ',elapsed_time,'seconds')











print("----------------------------")


