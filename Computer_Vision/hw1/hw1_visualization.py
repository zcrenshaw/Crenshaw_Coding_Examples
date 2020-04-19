## Homework 1
##
## For this assignment, you will implement basic convolution, denoising, and
## edge detection operations.  Your implementation should be restricted to
## using low-level primitives in numpy (e.g. you may not call a Python library
## routine for convolution in the implementation of your convolution function).
##
## This notebook provides examples for testing your code.
## See hw1.py for detailed descriptions of the functions you must implement.

import numpy as np
import matplotlib.pyplot as plt
from scipy import ndimage

from util import *
from hw1 import *
import canny as c

# Problem 1 - Convolution (10 Points)
#
# Implement the conv_2d() function as described in hw1.py.
#
# The example below tests your implementation by convolving with a box filter.

# image = load_image('data/69015.jpg')
# box = np.array([[1, 1, 1, 1, 1],[1, 1, 1, 1, 1],[1, 1, 1, 1, 1],[1, 1, 1, 1, 1],[1, 1, 1, 1, 1]])
# img = conv_2d(image,box)
#
# plt.figure(); plt.imshow(image, cmap='gray'); plt.title('Original')
# plt.figure(); plt.imshow(img, cmap='gray'); plt.title('Convolved')
# plt.show()
#
# ## Problem 2
# # (a) Denoising with Gaussian filtering (5 Points)
# ##
# ## Implement denoise_gaussian() as described in hw1.py.
# ##
# ## The example below tests your implementation.
#
image = load_image('data/a.png')
# imgA  = denoise_gaussian(image, 1.0)
# imgB  = denoise_gaussian(image, 2.5)
# imgCheckA = ndimage.gaussian_filter(image, 1.0)
# imgCheckB = ndimage.gaussian_filter(image, 2.5)
# sharp = np.array([[0,-1,0],[-1,0,-1],[0,-1,0]])
# xtra = np.array([[0,-1,0],[-1,10,-1],[0,-1,0]])
vert = np.array([[0,0,0,1,0,0,0],[0,0,0,1,0,0,0],[0,0,0,1,0,0,0],[0,0,0,1,0,0,0],[0,0,0,1,0,0,0],[0,0,0,1,0,0,0],[0,0,0,1,0,0,0]])
horz = vert.T
diag = np.identity(7)
gauss1 = denoise_gaussian(image,1.0)
gauss3 = denoise_gaussian(image,3.0)
imgV = conv_2d(image,vert)
imgH = conv_2d(image,horz)
imgD = conv_2d(image,diag)
imgDO = conv_2d(image,np.flip(diag,axis=0))
diag3 = np.identity(3)
diag15 = np.identity(15)
img3 = conv_2d(image,diag3)
img15 = conv_2d(image,diag15)

#
plt.figure(); plt.imshow(img15,cmap='gray'); plt.title('Diagonal, Filter size 15')
plt.figure(); plt.imshow(img3,cmap='gray'); plt.title('Diagonal, Filter size 3')
plt.figure(); plt.imshow(imgDO,cmap='gray'); plt.title('Diagonal another way')
plt.figure(); plt.imshow(imgD,cmap='gray'); plt.title('Diagonal one way')
plt.figure(); plt.imshow(imgV,cmap='gray'); plt.title('Vertical')
plt.figure(); plt.imshow(imgH,cmap='gray'); plt.title('Horizontal')
plt.figure(); plt.imshow(gauss1, cmap='gray'); plt.title('Gaussian Blur, Sigma = 1')
plt.figure(); plt.imshow(gauss3, cmap='gray'); plt.title('Gaussian Blur, Sigma = 3')
plt.figure(); plt.imshow(image,cmap='gray'); plt.title('Original')

plt.show()

exit()
# #
# # # (b) Denoising with median filtering (5 Points)
# # ##
# # ## Implement denoise_median() as described in hw1.py.
# # ##
# # ## The example below tests your implementation.
# #
# image = load_image('data/143090_noisy.png')
# imgA  = denoise_median(image, 1)
# imgB  = denoise_median(image, 2)
#
# plt.figure(); plt.imshow(image, cmap='gray')
# plt.figure(); plt.imshow(imgA, cmap='gray')
# plt.figure(); plt.imshow(imgB, cmap='gray')
# plt.show()
#
# ## Problem 3 - Sobel gradient operator (5 Points)
# ##
# ## Implement sobel_gradients() as described in hw1.py.
# ##
# ## The example below tests your implementation.
#
# image  = load_image('data/69015.jpg')
image  = load_image('data/edge_img/easy/002.jpg')
dx, dy = sobel_gradients(image)
dx_c,dy_c = c.sobel_gradients(image)
#
plt.figure(); plt.imshow(image, cmap='gray')
plt.figure(); plt.imshow(dx, cmap='gray')
plt.figure(); plt.imshow(dx_c, cmap='gray')
plt.figure(); plt.imshow(dy, cmap='gray')
plt.figure(); plt.imshow(dy_c, cmap='gray')
plt.show()
# # #
# # Problem 4 -  (a) Nonmax suppression (10 Points)
# #               (b) Edge linking and hysteresis thresholding (10 Points)
# #               (c) Canny edge detection (5 Points)
# #
# # Implement nonmax_suppress(), hysteresis_edge_linking(), canny() as described in hw1.py
# #
# # The examples below test your implementation

image  = load_image('data/edge_img/easy/002.jpg')
#image = load_image('data/edge_img/checker.png')
#image = load_image('data/69015.jpg')
mag, nonmax, edge = canny(image)
mag_c,theta_c = c.canny_nmax(image)


# image  = load_image('data/edge_img/easy/002.jpg')
# mag, nonmax, edge = canny(image)
plt.figure; plt.imshow(theta_c, cmap='viridis'); plt.title('canny theirs')
plt.figure; plt.imshow(theta_c, cmap='gray'); plt.title('canny theirs')
plt.figure; plt.imshow(mag_c, cmap='gray'); plt.title('canny theirs')
plt.figure(); plt.imshow(edge, cmap='gray'); plt.title('canny')
plt.figure(); plt.imshow(nonmax, cmap='gray'); plt.title("nonmax")
plt.figure(); plt.imshow(mag, cmap='gray'); plt.title("mag")
# plt.figure(); plt.imshow(image, cmap='gray'); plt.title("Original")
# plt.figure(); plt.imshow(dx, cmap='gray'); plt.title("dx")
# plt.figure(); plt.imshow(dy, cmap='gray'); plt.title("dy")

print(theta_c.shape)
print(image.shape)

plt.show()

# Extra Credits:
# (a) Improve Edge detection image quality (5 Points)
# (b) Bilateral filtering (5 Points)
# You can do either one and the maximum extra credits you can get is 5.
