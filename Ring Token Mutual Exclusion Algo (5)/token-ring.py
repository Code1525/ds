import threading
import time

class TokenRingMutex:
    def __init__(self, n):
        self.tokens = [threading.Event() for _ in range(n)]
        self.tokens[0].set()
        self.n = n
        self.queue = []
        
    def request_critical_section(self):
        self.queue.append(threading.current_thread().ident)
        while True:
            token_idx = self.queue.index(threading.current_thread().ident)
            self.tokens[token_idx % self.n].wait()
            if token_idx == 0:
                return
            
    def release_critical_section(self):
        token_idx = self.queue.index(threading.current_thread().ident)
        self.tokens[(token_idx + 1) % self.n].set()
        self.queue.remove(threading.current_thread().ident)

def worker(mutex, id):
    while True:
        print(f"Worker {id} is outside the critical section")
        mutex.request_critical_section()
        print(f"Worker {id} is inside the critical section")
        time.sleep(1)
        mutex.release_critical_section()

if __name__ == "__main__":
    mutex = TokenRingMutex(3)
    workers = []
    for i in range(3):
        worker_thread = threading.Thread(target=worker, args=(mutex, i))
        workers.append(worker_thread)
        worker_thread.start()
    
    for worker_thread in workers:
        worker_thread.join()