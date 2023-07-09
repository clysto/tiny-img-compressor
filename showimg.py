import numpy as np
import matplotlib.pyplot as plt

img = np.fromfile("lenna2.dat", dtype=np.uint8)
img = img.reshape((512, 512))

plt.figure()
plt.imshow(img, cmap="gray", vmin=0)
plt.show()
