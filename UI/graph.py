from matplotlib import pyplot as plt
plt.ion()
plt.show(block=False)
while True:
  # read 2d data point from stdin
  data = [float(x) for x in input().split()]
  assert len(data) == 2, "can only plot 2d data!"
  x,y = data
  # plot the data
  plt.plot([x],[y],'r.')
  plt.gcf().canvas.flush_events()