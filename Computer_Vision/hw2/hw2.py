import numpy as np
from canny import *
from sys import maxsize
import matplotlib.pyplot as plt
from math import pi

"""
   INTEREST POINT OPERATOR (12 Points Implementation + 3 Points Write-up)

   Implement an interest point operator of your choice.

   Your operator could be:

   (A) The Harris corner detector (Szeliski 4.1.1)

               OR

   (B) The Difference-of-Gaussians (DoG) operator defined in:
       Lowe, "Distinctive Image Features from Scale-Invariant Keypoints", 2004.
       https://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf

               OR

   (C) Any of the alternative interest point operators appearing in
       publications referenced in Szeliski or in lecture

              OR

   (D) A custom operator of your own design

   You implementation should return locations of the interest points in the
   form of (x,y) pixel coordinates, as well as a real-valued score for each
   interest point.  Greater scores indicate a stronger detector response.

   In addition, be sure to apply some form of spatial non-maximum suppression
   prior to returning interest points.

   Whichever of these options you choose, there is flexibility in the exact
   implementation, notably in regard to:

   (1) Scale

       At what scale (e.g. over what size of local patch) do you operate?

       You may optionally vary this according to an input scale argument.

       We will test your implementation at the default scale = 1.0, so you
       should make a reasonable choice for how to translate scale value 1.0
       into a size measured in pixels.

   (2) Nonmaximum suppression

       What strategy do you use for nonmaximum suppression?

       A simple (and sufficient) choice is to apply nonmaximum suppression
       over a local region.  In this case, over how large of a local region do
       you suppress?  How does that tie into the scale of your operator?

   For making these, and any other design choices, keep in mind a target of
   obtaining a few hundred interest points on the examples included with
   this assignment, with enough repeatability to have a large number of
   reliable matches between different views.

   If you detect more interest points than the requested maximum (given by
   the max_points argument), return only the max_points highest scoring ones.

   In addition to your implementation, include a brief write-up (in hw2.pdf)
   of your design choices.

   Arguments:
      image       - a grayscale image in the form of a 2D numpy array
      max_points  - maximum number of interest points to return
      scale       - (optional, for your use only) scale factor at which to
                    detect interest points

   Returns:
      xs          - numpy array of shape (N,) containing x-coordinates of the
                    N detected interest points (N <= max_points)
      ys          - numpy array of shape (N,) containing y-coordinates
      scores      - numpy array of shape (N,) containing a real-valued
                    measurement of the relative strength of each interest point
                    (e.g. corner detector criterion OR DoG operator magnitude)
"""
def find_interest_points(image, max_points = 200, scale = 1.0):
   # check that image is grayscale
   assert image.ndim == 2, 'image should be grayscale'

   k = 0.04 # this number produced the most and most accurate results
   sup = 2 # half side length of nonmax suppression filter
   # sup = 2 means a 5x5 grid with the interest point in the middle
   # sup = 3 means a 7x7 grid with the interest point in the middle
   # sup = 4 means a 9x9 grid with the interest point in the middle


   #did not use any windowing as it did not improve performance particularly
   # checking in a 5x5 grid around each point to nonmax suppress

   #smoothing image:
   #image = conv_2d_gaussian(image,1)

   #would be good to have some metric of spatial distance and interest
   # like take the highest interest in a single area as long as it is above the threshold


   #compute gradients
   dx,dy = sobel_gradients(image)


   #compute other gradients
   Ix2 = conv_2d_gaussian(dx**2)
   Iy2 = conv_2d_gaussian(dy**2)
   Ixy = conv_2d_gaussian(dy*dx)


   # #compute det and trace
   det = Ix2*Iy2 - Ixy**2
   trace = Ix2 + Iy2
   #
   # #compute
   H = det - k*(trace**2)
   #
   #threshold to find corners
   thresh = 0.0001

   xs = []
   ys = []
   scores = []

   points = []


   # go over all points
   for i in range(sup,H.shape[0] - sup):
      for j in range(sup,H.shape[1]-sup):
         #if a point is a corner
         if H[i][j] > thresh:
            max = 0
            mx = None
            my = None
            #check to see if it is the maximum in it's local vicinity
            for x in range(-sup,sup+1,sup):
               for y in range(-sup,sup+1,sup):
                  if H[i+x][j+y] > max:
                     max = H[i+x][j+y]
                     mx = i+x
                     my = j+y
            # preventing duplicates
            if mx == i and my == j and (mx,my) not in points:
                  xs.append(mx)
                  ys.append(my)
                  scores.append(max)
                  points.append((mx,my))

   # orders the scores in descending order
   order = np.argsort(scores)[::-1]

   # order other arrays in the same way
   xs = np.array(xs)[order]
   ys = np.array(ys)[order]
   scores = np.array(scores)[order]


   # take the top two hundred / max_points points
   return xs[0:max_points], ys[0:max_points], scores[0:max_points]

"""
   FEATURE DESCRIPTOR (12 Points Implementation + 3 Points Write-up)

   Implement a SIFT-like feature descriptor by binning orientation energy
   in spatial cells surrounding an interest point.

   Unlike SIFT, you do not need to build-in rotation or scale invariance.

   A reasonable default design is to consider a 3 x 3 spatial grid consisting
   of cell of a set width (see below) surrounding an interest point, marked
   by () in the diagram below.  Using 8 orientation bins, spaced evenly in
   [-pi,pi), yields a feature vector with 3 * 3 * 8 = 72 dimensions.

             ____ ____ ____
            |    |    |    |
            |    |    |    |
            |____|____|____|
            |    |    |    |
            |    | () |    |
            |____|____|____|
            |    |    |    |
            |    |    |    |
            |____|____|____|

                 |----|
                  width

   You will need to decide on a default spatial width.  Optionally, this can
   be a multiple of a scale factor, passed as an argument.  We will only test
   your code by calling it with scale = 1.0.

   In addition to your implementation, include a brief write-up (in hw2.pdf)
   of your design choices.

  Arguments:
      image    - a grayscale image in the form of a 2D numpy
      xs       - numpy array of shape (N,) containing x-coordinates
      ys       - numpy array of shape (N,) containing y-coordinates
      scale    - scale factor

   Returns:
      feats    - a numpy array of shape (N,K), containing K-dimensional
                 feature descriptors at each of the N input locations
                 (using the default scheme suggested above, K = 72)
"""
def extract_features(image, xs, ys, scale = 1.0):
   # check that image is grayscale
   assert image.ndim == 2, 'image should be grayscale'

   # gaussian smoothing
   #image = conv_2d_gaussian(image)

   #tried:
   #smoothing
   #normalizing
   #threshholding mag
   #thresholding after normalization

   dy,dx = sobel_gradients(image)
   mag = np.sqrt((dx * dx) + (dy * dy))
   theta = np.arctan2(dy, dx)

   N = len(xs)
   K = 3*3*8
   w = int(5*scale)
   feats = np.ndarray(shape=(N,K))

   for p in range(N):
      i = xs[p]
      j = ys[p]
      f = []
      #find bins
      for x in range(i-w,i+w+1,w):
         for y in range(j-w,j+w+1,w):
         # iterate over pnts
            o = np.zeros(shape=8) #orientation
            for a in range(x,x+w+1):
               for b in range(y,y+w+1):
                  try:
                     t = theta[a][b]
                     if t < pi/4:
                        index = 0
                     elif t < pi/2:
                        index = 1
                     elif t < 3*pi/4:
                        index = 2
                     elif t < pi:
                        index = 3
                     elif t < 5*pi/4:
                        index = 4
                     elif t < 3*pi/2:
                        index = 5
                     elif t < 7*pi/4:
                        index = 6
                     else:
                        index = 7

                     o[index] += mag[a][b]

                  except IndexError:
                     pass

            f.extend(o)

      feats[p] = f


   return np.array(feats)

"""
   FEATURE MATCHING (7 Points Implementation + 3 Points Write-up)

   Given two sets of feature descriptors, extracted from two different images,
   compute the best matching feature in the second set for each feature in the
   first set.

   Matching need not be (and generally will not be) one-to-one or symmetric.
   Calling this function with the order of the feature sets swapped may
   result in different returned correspondences.

   For each match, also return a real-valued score indicating the quality of
   the match.  This score could be based on a distance ratio test, in order
   to quantify distinctiveness of the closest match in relation to the second
   closest match.  It could optionally also incorporate scores of the interest
   points at which the matched features were extracted.  You are free to
   design your own criterion. Note that you are required to implement the naive
   linear NN search. For 'lsh' and 'kdtree' search mode, you could do either to
   get full credits.

   In addition to your implementation, include a brief write-up (in hw2.pdf)
   of your design choices. You are required to report the efficiency comparison
   between different modes by measure the runtime (check the benchmarking related
   codes in hw2_example.py).

   Arguments:
      feats0   - a numpy array of shape (N0, K), containing N0 K-dimensional
                 feature descriptors (generated via extract_features())
      feats1   - a numpy array of shape (N1, K), containing N1 K-dimensional
                 feature descriptors (generated via extract_features())
      scores0  - a numpy array of shape (N0,) containing the scores for the
                 interest point locations at which feats0 was extracted
                 (generated via find_interest_point())
      scores1  - a numpy array of shape (N1,) containing the scores for the
                 interest point locations at which feats1 was extracted
                 (generated via find_interest_point())
      mode     - 'naive': performs a brute force NN search

               - 'lsh': Implementing the local senstive hashing (LSH) approach
                  for fast feature matching. In LSH, the high dimensional
                  feature vectors are randomly projected into low dimension
                  space which are further binarized as boolean hashcodes. As we
                  group feature vectors by hashcodes, similar vectors may end up
                  with same 'bucket' with high propabiltiy. So that we can
                  accelerate our nearest neighbour matching through hierarchy
                  searching: first search hashcode and then find best
                  matches within the bucket.
                  Advice for impl.:
                  (1) Construct a LSH class with method like
                  compute_hash_code   (handy subroutine to project feature
                                      vector and binarize)
                  generate_hash_table (constructing hash table for all input
                                      features)
                  search_hash_table   (handy subroutine to search hash table)
                  search_feat_nn      (search nearest neighbour for input
                                       feature vector)
                  (2) It's recommended to use dictionary to maintain hashcode
                  and the associated feature vectors.
                  (3) When there is no matching for queried hashcode, find the
                  nearest hashcode as matching. When there are multiple vectors
                  with same hashcode, find the cloest one based on original
                  feature similarity.
                  (4) To improve the robustness, you can construct multiple hash tables
                  with different random project matrices and find the closest one
                  among all matched queries.
                  (5) It's recommended to fix the random seed by random.seed(0)
                  or np.random.seed(0) to make the matching behave consistenly
                  across each running.

               - 'kdtree': construct a kd-tree which will be searched in a more
                  efficient way. https://en.wikipedia.org/wiki/K-d_tree
                  Advice for impl.:
                  (1) The most important concept is to construct a KDNode. kdtree
                  is represented by its root KDNode and every node represents its
                  subtree.
                  (2) Construct a KDNode class with Variables like data (to
                  store feature points), left (reference to left node), right
                  (reference of right node) index (reference of index at original
                  point sets)and Methods like search_knn.
                  In search_knn function, you may specify a distance function,
                  input two points and returning a distance value. Distance
                  values can be any comparable type.
                  (3) You may need a user-level create function which recursively
                  creates a tree from a set of feature points. You may need specify
                  a axis on which the root-node should split to left sub-tree and
                  right sub-tree.


   Returns:
      matches  - a numpy array of shape (N0,) containing, for each feature
                 in feats0, the index of the best matching feature in feats1
      scores   - a numpy array of shape (N0,) containing a real-valued score
                 for each match
"""


def distance(a,b,d=maxsize):
   d2 = 0
   for i in range(len(a)):
      d2 += (a[i] - b[i])**2
      if d2 > d:
         break
   return np.sqrt(d2)


def square_dist(a,b,d=maxsize):
   d2 = 0
   for i in range(len(a)):
      d2 += (a[i] - b[i])**2
      if d2 > d:
         break
   return d2


class KDNode:

   def __init__(self, pnt, axis, label, left, right):
      self.pnt = pnt
      self.axis = axis
      self.label = label
      self.left = left
      self.right = right

   def height(self):
      l_height = 0 if self.left is None else self.left.height()
      r_height = 0 if self.right is None else self.right.height()

      return max(l_height,r_height) + 1

   def breadth(self):
      l_breadth = 0 if self.left is None else self.left.breadth()
      r_breadth = 0 if self.right is None else self.right.breadth()

      return l_breadth + r_breadth + 1

   def print_node(self,t):
      tabs = '-'*t
      if self.right is not None:
         self.right.print_node(t+1)
      print("{}{}".format(tabs,(self.axis,self.label)))
      if self.left is not None:
         self.left.print_node(t+1)


class KDTree:

   def __init__(self, k,data=[]):

      def build(data, axis=1):
         if data is None or len(data) == 0:
            return None

         if axis == 0:
            axis = 1

         sorted = data[data[:,axis].argsort()]

         median = len(sorted) // 2

         pnt = sorted[median]
         label = pnt[0]

         next_axis = (axis + 1) % k

         return KDNode(pnt,axis,label,
                       build(sorted[0:median],next_axis),
                       build(sorted[median + 1:],next_axis))

      self.root = build(data)


   def NN(self, dst):


      def search(here,best,sec_best):

         if here is None:
            return sec_best

         pnt = here.pnt
         label = here.label
         axis = here.axis
         left = here.left
         right = here.right

         dist = square_dist(dst, pnt[1:],best[2])

         if dist < sec_best:
            if dist < best[2]:
               best[:] = pnt, label, dist
            else:
               sec_best = dist


         diff = dst[axis] - pnt[axis]

         close, away = (left, right) if diff <= 0 else (right, left)

         sec_best = search(close,best,sec_best)
         if diff ** 2 < (dst[axis] - best[0][axis])**2:
            sec_best = search(away,best,sec_best)

         return sec_best

      best = [None,None,maxsize]
      sec_best = maxsize

      sec_best = search(self.root,best,sec_best)

      return best[1],best[2],sec_best

   def get_height(self):
      return self.root.height()

   def get_breadth(self):
      return self.root.breadth()

   def print_tree(self):
      self.root.print_node(0)




def match_features(feats0, feats1, scores0, scores1, mode='naive'):

   matches = []
   scores = []


   if mode=='naive':

      # compare all points in one image to all points in the other

      for i in range(len(feats0)):
         NN = (None,maxsize) #(index,distance)
         SNN = (None,maxsize)
         a = feats0[i]
         for j in range(len(feats1)):
            b = feats1[j]
            d = square_dist(a,b,SNN[1])
            if d < SNN[1]:
               if d < NN[1]:
                  NN = (j,d)
               else:
                  SNN = (j,d)

         matches.append(NN[0])
         if SNN[1] == 0:
            scores.append(1)
         else:
            scores.append(NN[1] / SNN[1])

   elif mode=='lsh':
      print('lsh not implemented')
      pass

   elif mode=='kdtree':

      tree_feats = [None]*len(feats1)
      for i in range(len(feats1)):
         arr = [i]
         arr.extend(feats1[i])
         tree_feats[i] = arr
      kdtree = KDTree(72,np.array(tree_feats))

      for i in range(len(feats0)):
         f = feats0[i]
         NN,d,sec_d = kdtree.NN(f)

         matches.append(int(NN))

         if sec_d == 0:
            scores.append(1)
         else:
            scores.append(d/sec_d)

   return np.array(matches), np.array(scores)

"""
   HOUGH TRANSFORM (7 Points Implementation + 3 Points Write-up)

   Assuming two images of the same scene are related primarily by
   translational motion, use a predicted feature correspondence to
   estimate the overall translation vector t = [tx ty].

   Your implementation should use a Hough transform that tallies votes for
   translation parameters.  Each pair of matched features votes with some
   weight dependant on the confidence of the match; you may want to use your
   estimated scores to determine the weight.

   In order to accumulate votes, you will need to decide how to discretize the
   translation parameter space into bins.

   In addition to your implementation, include a brief write-up (in hw2.pdf)
   of your design choices.

   Arguments:
      xs0     - numpy array of shape (N0,) containing x-coordinates of the
                interest points for features in the first image
      ys0     - numpy array of shape (N0,) containing y-coordinates of the
                interest points for features in the first image
      xs1     - numpy array of shape (N1,) containing x-coordinates of the
                interest points for features in the second image
      ys1     - numpy array of shape (N1,) containing y-coordinates of the
                interest points for features in the second image
      matches - a numpy array of shape (N0,) containing, for each feature in
                the first image, the index of the best match in the second
      scores  - a numpy array of shape (N0,) containing a real-valued score
                for each pair of matched features

   Returns:
      tx      - predicted translation in x-direction between images
      ty      - predicted translation in y-direction between images
      votes   - a matrix storing vote tallies; this output is provided for
                your own convenience and you are free to design its format
"""
def hough_votes(xs0, ys0, xs1, ys1, matches, scores):

   dxs = []
   dys = []
   votes = []

   for i in range(len(matches)):
      dX = xs0[i] - xs1[matches[i]]
      dY = ys0[i] - ys1[matches[i]]
      dxs.append(dX)
      dys.append(dY)
      votes.append(scores[i])

   max_x = np.amax(dxs)
   max_y = np.amax(dys)

   min_x = np.amin(dxs)
   min_y = np.amin(dys)

   nb = 60

   xbins = np.zeros(shape=nb)
   ybins = np.zeros(shape=nb)

   x_axis = np.linspace(min_x,max_x,nb)
   y_axis = np.linspace(min_y,max_y,nb)

   for x in range(len(dxs)):
      binned = False
      for i in range(nb-1):
         if x_axis[i] < dxs[x] <= x_axis[i + 1]:
            xbins[i] += scores[x]
            binned = True
            break
      if not binned:
         xbins[nb - 1] += scores[x]

   for y in range(len(dys)):
      binned = False
      for j in range(nb - 1):
         if y_axis[j] < dys[y] <= y_axis[j + 1]:
            ybins[j] += scores[y]
            binned = True
            break
      if not binned:
         ybins[nb - 1] += scores[y]

   xmax = np.argmax(xbins)
   ymax = np.argmax(ybins)

   tx = (x_axis[xmax] + x_axis[xmax - 1])/2
   ty = (y_axis[ymax] + y_axis[ymax - 1])/2

   return tx, ty, votes
