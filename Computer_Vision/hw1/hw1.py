import numpy as np
import sys
from math import exp,sqrt,pi,cos,sin,ceil
import statistics

"""
   Mirror an image about its border.

   Arguments:
      image - a 2D numpy array of shape (sx, sy)
      wx    - a scalar specifying width of the top/bottom border
      wy    - a scalar specifying width of the left/right border

   Returns:
      img   - a 2D numpy array of shape (sx + 2*wx, sy + 2*wy) containing
              the original image centered in its interior and a surrounding
              border of the specified width created by mirroring the interior
"""
def mirror_border(image, wx = 1, wy = 1):
   assert image.ndim == 2, 'image should be grayscale'
   sx, sy = image.shape
   # mirror top/bottom
   top    = image[:wx:,:]
   bottom = image[(sx-wx):,:]
   img = np.concatenate( \
      (top[::-1,:], image, bottom[::-1,:]), \
      axis=0 \
   )
   # mirror left/right
   left  = img[:,:wy]
   right = img[:,(sy-wy):]
   img = np.concatenate( \
      (left[:,::-1], img, right[:,::-1]), \
      axis=1 \
   )
   return img

"""
   Pad an image with zeros about its border.

   Arguments:
      image - a 2D numpy array of shape (sx, sy)
      wx    - a scalar specifying width of the top/bottom border
      wy    - a scalar specifying width of the left/right border

   Returns:
      img   - a 2D numpy array of shape (sx + 2*wx, sy + 2*wy) containing
              the original image centered in its interior and a surrounding
              border of zeros
"""
def pad_border(image, wx = 1, wy = 1):
   assert image.ndim == 2, 'image should be grayscale'
   sx, sy = image.shape
   img = np.zeros((sx+2*wx, sy+2*wy))
   img[wx:(sx+wx),wy:(sy+wy)] = image
   return img

"""
   Remove the border of an image.

   Arguments:
      image - a 2D numpy array of shape (sx, sy)
      wx    - a scalar specifying width of the top/bottom border
      wy    - a scalar specifying width of the left/right border

   Returns:
      img   - a 2D numpy array of shape (sx - 2*wx, sy - 2*wy), extracted by
              removing a border of the specified width from the sides of the
              input image
"""
def trim_border(image, wx = 1, wy = 1):
   assert image.ndim == 2, 'image should be grayscale'
   sx, sy = image.shape
   img = np.copy(image[wx:(sx-wx),wy:(sy-wy)])
   return img

"""
   Return an approximation of a 1-dimensional Gaussian filter.

   The returned filter approximates:

   g(x) = 1 / sqrt(2 * pi * sigma^2) * exp( -(x^2) / (2 * sigma^2) )

   for x in the range [-3*sigma, 3*sigma]
"""
def gaussian_1d(sigma = 1.0):
   width = np.ceil(3.0 * sigma)
   x = np.arange(-width, width + 1)
   g = np.exp(-(x * x) / (2 * sigma * sigma))
   g = g / np.sum(g)          # normalize filter to sum to 1 ( equivalent
   g = np.atleast_2d(g)       # to multiplication by 1 / sqrt(2*pi*sigma^2) )
   return g

"""
   CONVOLUTION IMPLEMENTATION (10 Points)

   Convolve a 2D image with a 2D filter.

   Requirements:

   (1) Return a result the same size as the input image.

   (2) You may assume the filter has odd dimensions.

   (3) The result at location (x,y) in the output should correspond to
       aligning the center of the filter over location (x,y) in the input
       image.

   (4) When computing a product at locations where the filter extends beyond
       the defined image, treat missing terms as zero.  (Equivalently stated,
       treat the image as being padded with zeros around its border).

   You must write the code for the nested loops of the convolutions yourself,
   using only basic loop constructs, array indexing, multiplication, and
   addition operators.  You may not call any Python library routines that
   implement convolution.

   Arguments:
      image  - a 2D numpy array
      filt   - a 1D or 2D numpy array, with odd dimensions
      mode   - 'zero': preprocess using pad_border or 'mirror': preprocess using mirror_border.

   Returns:
      result - a 2D numpy array of the same shape as image, containing the
               result of convolving the image with filt
"""
def conv_2d(image, filt, mode='zero'):
   # make sure that both image and filter are 2D arrays
   assert image.ndim == 2, 'image should be grayscale'
   # filt = np.atleast_2d(filt)
   # print("start")
   w_pad = (filt.shape[0] - 1) // 2
   h_pad = (filt.shape[1] - 1) // 2
   # print('filt: '+str(filt.shape))
   if mode == 'zero':
      padded = pad_border(image,w_pad,h_pad)
   if mode == 'mirror':
      padded = mirror_border(image,w_pad,h_pad)
   output = padded.copy()
   # print('image: ' + str(image.shape[0]) + 'paddedImage: '+str(padded.shape[0]))


   for w in range(w_pad,padded.shape[0] - w_pad):
      for h in range(h_pad,padded.shape[1] - h_pad):
         sum = 0
         for wf in range(filt.shape[0]):
            for hf in range(filt.shape[1]):
               sum += (filt[wf][hf]*padded[w-wf+w_pad][h-hf+h_pad])

         output[w][h] = sum


   return trim_border(output,w_pad,h_pad)

"""
   GAUSSIAN DENOISING (5 Points)

   Denoise an image by convolving it with a 2D Gaussian filter.

   Convolve the input image with a 2D filter G(x,y) defined by:

   G(x,y) = 1 / sqrt(2 * pi * sigma^2) * exp( -(x^2 + y^2) / (2 * sigma^2) )

   You may approximate the G(x,y) filter by computing it on a
   discrete grid for both x and y in the range [-3*sigma, 3*sigma].

   See the gaussian_1d function for reference.

   Note:
   (1) Remember that the Gaussian is a separable filter.
   (2) Denoising should not create artifacts along the border of the image.
       Make an appropriate assumption in order to obtain visually plausible
       results along the border.

   Arguments:
      image - a 2D numpy array
      sigma - standard deviation of the Gaussian

   Returns:
      img   - denoised image, a 2D numpy array of the same shape as the input
"""
def denoise_gaussian(image, sigma = 1.0):
   G = gaussian_1d(sigma)
   half_done = conv_2d(image,G,'mirror')
   return conv_2d(half_done,G.T,'mirror')


"""
   MEDIAN DENOISING (5 Points)

   Denoise an image by applying a median filter.

   Note:
       Denoising should not create artifacts along the border of the image.
       Make an appropriate assumption in order to obtain visually plausible
       results along the border. No padding needed in median denosing.

   Arguments:
      image - a 2D numpy array
      width - width of the median filter; compute the median over 2D patches of
              size (2*width +1) by (2*width + 1)

   Returns:
      img   - denoised image, a 2D numpy array of the same shape as the input
"""
def denoise_median(image, width = 1):
   padded = image.copy()
   output = padded.copy()

   x_max = padded.shape[0]
   y_max = padded.shape[1]



   for x in range(0, x_max):
      for y in range(0, y_max):
         vals = []
         for xf in range(-width,width+1):
            for yf in range(-width,width+1):
               try:
                  val = padded[x+xf][y+yf]
               except IndexError:
                  # mirroring around border
                  if x+xf < 0:
                     if y+yf < 0:
                        val = padded[0-xf][0-yf]
                     elif y+yf >= y_max:
                        val = padded[0 - xf][y_max - yf]
                     else:
                        val = padded[x-xf][y+yf]
                  elif x + xf >= x_max:
                     if y + yf < 0:
                        val = padded[x_max - xf][0 - yf]
                     elif y + yf >= padded.shape[1]:
                        val = padded[x_max - xf][y_max - yf]
                     else:
                        val = padded[x_max - xf][y + yf]

                  if y+yf < 0:
                     if x+xf < 0:
                        val = padded[0-xf][0-yf]
                     elif x+xf >= x_max:
                        val = padded[x_max - xf][0 - yf]
                     else:
                        val = padded[x+xf][0-yf]
                  elif y + yf >= y_max:
                     if x + xf < 0:
                        val = padded[0 - xf][y_max - yf]
                     elif x + xf >= x_max:
                        val = padded[x_max - xf][y_max - yf]
                     else:
                        val = padded[x + xf][y_max - yf]
               vals.append(val)

         output[x][y] = np.median(vals)


   return output


"""
   SOBEL GRADIENT OPERATOR (5 Points)
   Compute an estimate of the horizontal and vertical gradients of an image
   by applying the Sobel operator.
   The Sobel operator estimates gradients dx(horizontal), dy(vertical), of
   an image I as:

         [ 1  0  -1 ]
   dx =  [ 2  0  -2 ] (*) I
         [ 1  0  -1 ]

         [  1  2  1 ]
   dy =  [  0  0  0 ] (*) I
         [ -1 -2 -1 ]

   where (*) denotes convolution.
   Note:
      (1) Your implementation should be as efficient as possible.
      (2) Avoid creating artifacts along the border of the image.
   Arguments:
      image - a 2D numpy array
   Returns:
      dx    - gradient in x-direction at each point
              (a 2D numpy array, the same shape as the input image)
      dy    - gradient in y-direction at each point
              (a 2D numpy array, the same shape as the input image)
"""
def sobel_gradients(image):
   filter = np.array([[1,0,-1],[2,0,-2],[1,0,-1]])
   dx = conv_2d(image,filter,'mirror')
   dy = conv_2d(image,filter.T,'mirror')
   return dx, dy



def sign(n):
   return -1 if n < 0 else 1
"""
   NONMAXIMUM SUPPRESSION (10 Points)

   Nonmaximum suppression.

   Given an estimate of edge strength (mag) and direction (theta) at each
   pixel, suppress edge responses that are not a local maximum along the
   direction perpendicular to the edge.

   Equivalently stated, the input edge magnitude (mag) represents an edge map
   that is thick (strong response in the vicinity of an edge).  We want a
   thinned edge map as output, in which edges are only 1 pixel wide.  This is
   accomplished by suppressing (setting to 0) the strength of any pixel that
   is not a local maximum.

   Note that the local maximum check for location (x,y) should be performed
   not in a patch surrounding (x,y), but along a line through (x,y)
   perpendicular to the direction of the edge at (x,y).

   A simple, and sufficient strategy is to check if:
      ((mag[x,y] > mag[x + ox, y + oy]) and (mag[x,y] >= mag[x - ox, y - oy]))
   or
      ((mag[x,y] >= mag[x + ox, y + oy]) and (mag[x,y] > mag[x - ox, y - oy]))
   where:
      (ox, oy) is an offset vector to the neighboring pixel in the direction
      perpendicular to edge direction at location (x, y)

   Arguments:
      mag    - a 2D numpy array, containing edge strength (magnitude)
      theta  - a 2D numpy array, containing edge direction in [0, 2*pi)

   Returns:
      nonmax - a 2D numpy array, containing edge strength (magnitude), where
               pixels that are not a local maximum of strength along an
               edge have been suppressed (assigned a strength of zero)
"""
def nonmax_suppress(mag, theta):
   nonmax = mag.copy()
   max_x = mag.shape[0] - 1
   max_y = mag.shape[1] - 1
   for x in range(mag.shape[0]):
      for y in range(mag.shape[1]):
         t = theta[x][y]
         ox = sign(cos(t ))
         oy = sign(sin(t ))

         # check gradient neighbor to ensure in range
         posx = x + ox
         if (posx > max_x):
            posx = max_x
         elif posx < 0:
            posx = 0

         negx = x - ox
         if (negx > max_x):
            negx = max_x
         elif negx < 0:
            negx = 0

         posy = y + oy
         if (posy > max_y):
            posy = max_y
         elif posy < 0:
            posy = 0

         negy = y - oy
         if (negy > max_y):
            negy = max_y
         elif negy < 0:
            negy = 0

         right = (mag[x][y] > mag[posx][posy]) and (mag[x][y] >= mag[negx][negy])
         left = (mag[x][y] >= mag[posx][posy]) and (mag[x][y] > mag[negx][negy])
         if not (left or right):
            nonmax[x][y] = 0
   return nonmax


"""
   HYSTERESIS EDGE LINKING (10 Points)

   Hysteresis edge linking.

   Given an edge magnitude map (mag) which is thinned by nonmaximum suppression,
   first compute the low threshold and high threshold so that any pixel below
   low threshold will be thrown away, and any pixel above high threshold is
   a strong edge and will be preserved in the final edge map.  The pixels that
   fall in-between are considered as weak edges.  We then add weak edges to
   true edges if they connect to a strong edge along the gradient direction.

   Since the thresholds are highly dependent on the statistics of the edge
   magnitude distribution, we recommend to consider features like maximum edge
   magnitude or the edge magnitude histogram in order to compute the high
   threshold.  Heuristically, once the high threshod is fixed, you may set the
   low threshold to be propotional to the high threshold.

   Note that the thresholds critically determine the quality of the final edges.
   You need to carefully tuned your threshold strategy to get decent
   performance on real images.

   For the edge linking, the weak edges caused by true edges will connect up
   with a neighbouring strong edge pixel.  To track theses edges, we
   investigate the 8 neighbours of strong edges.  Once we find the weak edges,
   located along strong edges' gradient direction, we will mark them as strong
   edges.  You can adopt the same gradient checking strategy used in nonmaximum
   suppression.  This process repeats util we check all strong edges.

   In practice, we use a queue to implement edge linking.  In python, we could
   use a list and its fuction .append or .pop to enqueue or dequeue.

   Arguments:
     nonmax - a 2D numpy array, containing edge strength (magnitude) which is thined by nonmaximum suppression
     theta  - a 2D numpy array, containing edeg direction in [0, 2*pi)

   Returns:
     edge   - a 2D numpy array, containing edges map where the edge pixel is 1 and 0 otherwise.
"""

def hysteresis_edge_linking(nonmax, theta):
   strong = list()
   flat = nonmax.flatten()
   mean = np.mean(flat)
   std = np.std(flat)
   top = max(flat)

   max_x = nonmax.shape[0] - 1
   max_y = nonmax.shape[1] - 1

   edge = np.zeros(nonmax.shape)

   # finding strongest edges
   high = min(mean + std,top)
   low = high/3
   for x in range(nonmax.shape[0]):
      for y in range(nonmax.shape[1]):
         if nonmax[x][y] >= high:
            edge[x][y] = 1
            #enqueue
            strong.append((x,y))

   # finding linking:

   times = 0

   while len(strong) > 0:
      old = len(strong)
      p = strong.pop()

      x = p[0]
      y = p[1]
      t = theta[x][y]
      edge[x][y] = 1
      ox = sign(cos(t ))
      oy = sign(sin(t ))

      # check gradient neighbor to ensure in range
      posx = x + ox
      if (posx > max_x):
         posx = max_x
      elif posx < 0:
         posx = 0

      negx = x - ox
      if (negx > max_x):
         negx = max_x
      elif negx < 0:
         negx = 0

      posy = y + oy
      if (posy > max_y):
         posy = max_y
      elif posy < 0:
         posy = 0

      negy = y - oy
      if (negy > max_y):
         negy = max_y
      elif negy < 0:
         negy = 0


      if nonmax[posx][posy] > low:
         edge[posx][posy] = 1
         # prevent infinite loop
         if edge[posx][posy] != 1:
            strong.append((posx,posy))
      if nonmax[negx][negy] > low:
         edge[negx][negy] = 1
         # prevent infinite loop
         if edge[negx][negy] != 1:
            strong.append((negx, negy))

# # triggered on infinite loop
#       if len(strong) == old:
#          times += 1
#       if times == 10:
#          print([(x,y),(posx,posy),(negx,negy),(ox,oy)])
#          break



   return edge

"""
   CANNY EDGE DETECTOR (5 Points)

   Canny edge detector.

   Given an input image:

   (1) Compute gradients in x- and y-directions at every location using the
       Sobel operator.  See sobel_gradients() above.

   (2) Estimate edge strength (gradient magnitude) and direction.

   (3) Perform nonmaximum suppression of the edge strength map, thinning it
       in the direction perpendicular to that of a local edge.
       See nonmax_suppress() above.

   (4) Compute the high threshold and low threshold of edge strength map
       to classify the pixels as strong edges, weak edges and non edges.
       Then link weak edges to strong edges

   Return the original edge strength estimate (max), the edge
   strength map after nonmaximum suppression (nonmax) and the edge map
   after edge linking (edge)

   Arguments:
      image    - a 2D numpy array

   Returns:
      mag      - a 2D numpy array, same shape as input, edge strength at each pixel
      nonmax   - a 2D numpy array, same shape as input, edge strength after nonmaximum suppression
      edge     - a 2D numpy array, same shape as input, edges map where edge pixel is 1 and 0 otherwise.
"""
def canny(image):
   # get gradients
   dx,dy = sobel_gradients(image)
   # get magnitudes and make angles
   mag = np.zeros(dx.shape)
   theta = np.zeros(dx.shape)
   for x in range(dx.shape[0]):
      for y in range(dx.shape[1]):
         mag[x][y] = abs(int(dx[x][y])+int(dy[x][y]))/2
         theta[x][y] = np.arctan2(dx[x][y],dy[x][y])
   #nonmax suppression
   nonmax = nonmax_suppress(mag,theta)
   #hysterisis linking
   edge = hysteresis_edge_linking(nonmax,theta)


   return mag, nonmax, edge


# Extra Credits:
# (a) Improve Edge detection image quality (5 Points)
# (b) Bilateral filtering (5 Points)
# You can do either one and the maximum extra credits you can get is 5.
"""
    BILATERAL DENOISING (Extra Credits: 5 Points)
    Denoise an image by applying a bilateral filter
    Note:
        Performs standard bilateral filtering of an input image.
        Reference link: https://en.wikipedia.org/wiki/Bilateral_filter

        Basically, the idea is adding an additional edge term to Guassian filter
        described above.

        The weighted average pixels:

        BF[I]_p = 1/(W_p)sum_{q in S}G_s(||p-q||)G_r(|I_p-I_q|)I_q

        In which, 1/(W_p) is normalize factor, G_s(||p-q||) is spatial Guassian
        term, G_r(|I_p-I_q|) is range Guassian term.

        We only require you to implement the grayscale version, which means I_p
        and I_q is image intensity.

    Arguments:
        image       - input image
        sigma_s     - spatial param (pixels), spatial extent of the kernel,
                       size of the considered neighborhood.
        sigma_r     - range param (no normalized, a propotion of 0-255),
                       denotes minimum amplitude of an edge
    Returns:
        img   - denoised image, a 2D numpy array of the same shape as the input
"""

def denoise_bilateral(image, sigma_s=1, sigma_r=25.5):
    assert image.ndim == 2, 'image should be grayscale'
    ##########################################################################
    # TODO: YOUR CODE HERE
    raise NotImplementedError('denoise_bilateral')
    ##########################################################################
    return img
