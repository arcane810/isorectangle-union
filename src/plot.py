import matplotlib.pyplot as plt
import sys

ifile = sys.argv[1]
ofile = ifile + ".out"

with open(ifile) as f:
    n = int(f.readline())
    if n <= 10000:
        for i in range(n):
            x1, x2, y1, y2 = map(float, f.readline().split())
            x = []
            y = []
            x.append(x1)
            x.append(x1)
            x.append(x2)
            x.append(x2)
            x.append(x1)
            y.append(y1)
            y.append(y2)
            y.append(y2)
            y.append(y1)
            y.append(y1)
            plt.plot(x, y, color="black")
    plt.savefig(ifile + ".rects.png")

with open(ofile) as f:
    measure = f.readline()
    perimeter = f.readline()
    s = "Measure: " + measure + "  Contour Perimeter: " + perimeter
    plt.suptitle(s)
    n = int(f.readline())
    for i in range(n):
        x1, y1, x2, y2 = map(float, f.readline().split())
        x = []
        y = []
        x.append(x1)
        y.append(y1)
        x.append(x2)
        y.append(y2)
        plt.plot(x, y, color="cyan")
    plt.savefig(ifile + ".contours.png")
# plt.show()
