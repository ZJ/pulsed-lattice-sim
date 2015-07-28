# pulsed-lattice-sim

## Fourier Transforms
Lens stuff:
 - If we're doing f-f imaging, it'll be a true Fourier Transform.
 - Otherwise the best spot is the pupil of the lens.  We'd get the light at the focus forming the transform, plus an overall multiplicative phase.

More specifically:
A exp[i * k/(2f) (1 - d/f) (u^2 + v^2)]/(i * lambda * f)
is the leading phase in front of the Fourier Transform part.

Now we care about it, because if we're doing weird distances on the first lens,
we might need to include that phase curvature for the lattices.

## Wedged
BK7 @ 780: 1.5112
Could be any of:
 - PS814 @ 18.9 10deg
 - PS812 @ 11.22 6deg
 - PS811 @ 7.41 4deg
 - PS810 @ 3.53 2deg

Each have 1" OD with short side at 3.00 +- 0.15mm
Angular tolerance at 30 arcsec

## Flipping Beams
We generally think of things being oriented in the horizontal plane, because that's how we work.
To generate our final lattice, we'll copy and mirror a set of beams.
We'd like to mimic that behaviour here.

In memory, our FFT array is stored in row-major order.
This means in linear order, the first entire row is stored, then the next, etc. etc.
So, if we want an easy mirroring operation, we'd like to be able to copy entire pieces whole using memcpy() or similar operations.
For row-major order, that involves copying entire rows at a time, so we'd want to arrange things vertically.

That is, we'd want to take an NxN (row)x(column) array, duplicate it to the top N lines of an Nx(2N) array.
Then take the first line, and copy it to the last (2N)th line.
The second to the (2N-1)th, and so on until the (N)th into the (N+1)th line.
