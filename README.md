hashnoise
=========

A custom 2D noise implementation for C++, based on the
[Improved Noise reference implementation][perlin] by Ken Perlin.

Instead of a permutation table, the gradient vectors are derived from
a hash function, based on an [algorithm][mueller] by Thomas Mueller.

To improve the noise quality for 2D processing, the 16 possible gradient
vectors are normalized and evenly distributed around the unit circle.


Advantages
----------

- Easy to seed
- No heap usage if the hash seed is hardcoded
- Pattern only repeats after 2^16 units instead of 2^8
- Smoother gradients for 2D image processing


Interface
---------

The `hashnoise::Noise` class provides two functions:

```cpp
double get(double x, double y)
```

Returns values in `[-M_SQRT1_2, M_SQRT1_2]` for positive coordinates.
The range has been [calculated][chen] by Rudi Chen.

```cpp
double getNorm(double x, double y) const
```

Returns normalized values in `[0.0, 1.0]` for positive coordinates.

To improve performance, return values are not explicitly clamped.
As a result, some values might be slightly out of range, due
to the limited accuracy of floating point representation.


Seeds
-----

The hash function uses a multiplier to shuffle bits.
This multiplier is provided as a seed to the `Noise` constructor.

It is non-trivial to find seeds that produce good noise patterns.
The included `seedsearch` tool can help with this task.
It generates pseudo-random seeds, uses them to render noise images,
and then compresses the images with [zlib][]. The images with the
worst compression ratio may be considered reasonably irregular.

In a test with a million random seeds, followed by a manual review,
the following seeds turned out most useful:

```
0x332ff52d 0x0172051a 0x33f9156d 0xb090386a
0x0060adfa 0x131b5961 0x240d6f53 0x4039b8e7
0x48987de7 0x49e61d27 0x55c765f6 0x6ea18f35
0xa317f1ca 0xa6e35365 0xdcb46265 0xf0a91ca9
```


Install
-------

This is a header-only library.
Simply add the header from the `include` directory to your project.

A test tool is available to render sample images of the noise.
It can be compiled and run with:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./test/hashnoise-test
```

In order to compile the `seedsearch` tool, install [zlib][] and use:

```
cmake -DCMAKE_BUILD_TYPE=Release -DSEEDSEARCH=ON ..
```


[perlin]: https://mrl.nyu.edu/~perlin/noise/
[mueller]: https://stackoverflow.com/a/12996028
[chen]: http://digitalfreepen.com/2017/06/20/range-perlin-noise.html
[zlib]: https://zlib.net
