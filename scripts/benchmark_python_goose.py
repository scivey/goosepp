import time
from goose import Goose

def load_jezebel():
    with open('resources/additional_html/jezebel1.txt') as f:
        data = f.read()
    return data

def bench(iterations=100):
    data = load_jezebel()
    goose = Goose()
    times = []
    for _ in xrange(iterations):
        t1 = time.time()
        goose.extract(raw_html=data)
        t2 = time.time()
        iteration_time = t2 - t1
        times.append(iteration_time)
    return (sum(times) / float(len(times)))

if __name__ == '__main__':
    start = time.time()
    print bench()
    end = time.time()
    total_len = end - start
    print "total test length: %f" % total_len
