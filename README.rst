========================
Polyval2d High Precision
========================

Now, Linux wheels are available.

Compile on MacOS
================

.. code-block:: bash

  $ brew install mpfr gmp
  $ pip install .

Usage
=====

.. code-block:: python

   >>> from polyval2d_mp  import polyval2d
   >>> import numpy as np
   >>> polyval2d([1.0, 2.0, 3.0, 4.0], [2.0, 3.0, 4.0, 5.0], np.ones((3,3)), 1000) # Use precision == 1000 digits
   array([ 21.,  91., 273., 651.])
