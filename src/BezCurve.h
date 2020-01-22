#ifndef _BEZCURVE_H_
#define _BEZCURVE_H_

#include <utility>
#include <vector>
#include <array>
#include <string>

#include "BezCoord.h"

using std::pair;
using std::vector;
using std::array;
using std::string;

namespace morph
{

    /*!
     * Store the first N=20 rows of Pascal's triangle in a linear array. To get the
     * values from row n, where n starts at 0 (and ends at N-1), you step along a number
     * given by the triangle sequence (n(n+1)/2) and then read n+1 values. The triangle
     * has a total number of elements given by N(N+1)/2, which for 20 rows is 20(21)/2 =
     * 210.
     */
    const array<unsigned int, 210> Pascal =
    {1,
     1,1,
     1,2,1,
     1,3,3,1,
     1,4,6,4,1,
     1,5,10,10,5,1,
     1,6,15,20,15,6,1,
     1,7,21,35,35,21,7,1,
     1,8,28,56,70,56,28,8,1,
     1,9,36,84,126,126,84,36,9,1,
     1,10,45,120,210,252,210,120,45,10,1,
     1,11,55,165,330,462,462,330,165,55,11,1,
     1,12,66,220,495,792,924,792,495,220,66,12,1,
     1,13,78,286,715,1287,1716,1716,1287,715,286,78,13,1,
     1,14,91,364,1001,2002,3003,3432,3003,2002,1001,364,91,14,1,
     1,15,105,455,1365,3003,5005,6435,6435,5005,3003,1365,455,105,15,1,
     1,16,120,560,1820,4368,8008,11440,12870,11440,8008,4368,1820,560,120,16,1,
     1,17,136,680,2380,6188,12376,19448,24310,24310,19448,12376,6188,2380,680,136,17,1,
     1,18,153,816,3060,8568,18564,31824,43758,48620,43758,31824,18564,8568,3060,816,153,18,1,
     1,19,171,969,3876,11628,27132,50388,75582,92378,92378,75582,50388,27132,11628,3876,969,171,19,1};

    //! How many rows in the table above.
    const unsigned int PascalRows = 20;

    /*!
     * A Bezier curve class which allows the computation of Cartesian
     * coordinates (though with x right, y down, and hence a left-hand
     * coordinate system) of points on a Bezier curve which is
     * specified using a parameter (often called t) which is in the
     * range [0, 1]
     */
    class BezCurve
    {
    public: // methods
        /*!
         * Construct a cubic Bezier curve with a specification of
         * the curve as inital and final position with two control
         * points.
         */
        BezCurve (pair<float,float> ip,
                  pair<float,float> fp,
                  pair<float,float> c1,
                  pair<float,float> c2);

        /*!
         * Construct a quadratic Bezier curve with a specification of the
         * curve as inital and final position with a single control point
         */
        BezCurve (pair<float,float> ip,
                  pair<float,float> fp,
                  pair<float,float> c1);

        /*!
         * Construct a linear Bezier curve for production of straight
         * lines.
         */
        BezCurve (pair<float,float> ip,
                  pair<float,float> fp);

        /*!
         * Construct a Bezier curve of order cp.size()+1
         */
        BezCurve (pair<float,float> ip,
                  pair<float,float> fp,
                  vector<pair<float, float>> cp);

        /*!
         * Compute n points on the curve whose parameters, t, are
         * equally spaced in parameter space. The first point will be
         * the start of the curve (t==0) and the last point will be at
         * the end of the curve (t==1).
         */
        vector<BezCoord> computePoints (unsigned int n) const;

        /*!
         * Compute points on the curve which are distance l from each
         * other in Cartesian space. This will return 1 or more points
         * in the vector. The last point in the vector will be a
         * nullCoordinate BezCoord which will contain the Euclidean
         * distance to the end of the curve.
         *
         * If firstl is set and non-zero, then the first point will be
         * a Cartesian distance firstl from the initial point of the
         * curve, rather than being a distance l from the initial
         * point.
         */
        vector<BezCoord> computePoints (float l, float firstl = 0.0f) const;

        /*!
         * Get a vector of points on the curve with horizontal spacing
         * x.
         */
        vector<BezCoord> computePointsHorz (float x) const;

        /*!
         * Compute one point on the curve, distance t along the curve
         * from the starting position with t in range [0,1]
         */
        BezCoord computePoint (float t) const;

        /*!
         * Compute one point on the curve, starting at the curve point
         * which is found for parameter value t and extending a
         * (Euclidean) distance l along the curve from the starting
         * position.
         */
        BezCoord computePoint (float t, float l) const;

        /*!
         * For debugging - output, as a string, the BezCoords of this
         * curve, choosing numPoints points evenly spaced in the
         * parameter space t=[0,1].
         */
        string output (unsigned int numPoints) const;

        /*!
         * For debugging/file use. Output, as a string, the BezCoords
         * of this curve with the step size step in Cartesian space.
         */
        string output (float step) const;

        /*!
         * A setter for the scaling factor.
         */
        void setScale (const float s);

        /*!
         * A setter for the length threshold.
         */
        void setLthresh (const float l);

        /*!
         * Getters for p0 and p1, the initial and final positions on
         * the curve, either unscaled or scaled by @scale
         */
        //@{
        pair<float,float> getInitialPointUnscaled (void) const;
        pair<float,float> getFinalPointUnscaled (void) const;
        pair<float,float> getInitialPointScaled (void) const;
        pair<float,float> getFinalPointScaled (void) const;
        //@}

    private: // methods
        /*!
         * Compute one point on the linear curve, distance t along the
         * curve from the starting position.
         */
        BezCoord computePointLinear (float t) const;

        /*!
         * Compute one point on the linear curve, starting at the
         * curve point which is found for parameter value t and
         * extending a distance l along the curve from the starting
         * position.
         *
         * The key to this is to compute a change in t from the l that
         * you want to move along the line. It's not hard to do the
         * maths for the linear case; see LinearBez1.jpg and
         * LinearBez2.jpg for the sums.
         */
        BezCoord computePointLinear (float t, float l) const;

        /*!
         * Compute one point on the quadratic curve, distance t along
         * the curve from the starting position.
         */
        BezCoord computePointQuadratic (float t) const;

        /*!
         * Compute one point on the cubic curve, distance t along the
         * curve from the starting position.
         */
        BezCoord computePointCubic (float t) const;

        /*!
         * Look up the binomial coefficient (n,k) from morph::Pascal.
         */
        static unsigned int binomial_lookup (unsigned int n, unsigned int k);

        /*!
         * Compute a Bezier curve of general order.
         */
        BezCoord computePointGeneral (float t) const;

        /*!
         * A computePoint starting from the point for parameter value
         * t and going to a point which is Euclidean distance l from
         * the starting point.
         *
         * This one uses a binary search to find the next point, and
         * works for quadratic and cubic Bezier curves for which it is
         * difficult to compute the t that would give a Euclidean
         * extension l (it would work for linear curves too).
         */
        BezCoord computePointBySearch (float t, float l) const;

        /*!
         * Like computePointsBySearch, but instead of using the
         * Euclidean distance, space points with x between them in the
         * first coordinate - the horizonal coordinate.
         */
        BezCoord computePointBySearchHorz (float t, float x) const;

        /*!
         * Test that t is in range [0,1]. Throw exception otherwise.
         */
        void checkt (float t) const;

    private: // attributes
        /*!
         * Initial and final positions
         */
        //@{
        pair<float,float> p0;
        pair<float,float> p1;
        //@}

        /*!
         * Control points
         */
        //@{
        vector<pair<float,float>> controls;
        //@}

        /*!
         * A scaling factor to convert from the SVG drawing units into
         * mm (or whatever). This is used when computing the BezCoords
         * to output.
         */
        float scale = 1.0f;

        /*!
         * How close we need to be to the target l for a given choice
         * of dt. arb. units in position space (not parameter space).
         * This is used in computeBySearch and computeBySearchHorz.
         */
        float lthresh = 0.000001;

        /*!
         * The as-the-crow-flies distance from p0 to p1. Use for for BEZLINEAR to avoid repeat
         * computations. See, especially, computePointLinear (float t, float l) const
         */
        float linlength = 0.0f;

        /*!
         * Scaled version of linlength
         */
        float linlengthscaled = 0.0f;

        /*!
         * The order of the Bezier curve. The value of the highest power of t. Thus 3 is
         * a cubic Bezier, 2 is a quadratic Bezier, etc. Note that 0th order Bezier
         * curve does not exist; so the constructor must update this number.
         */
        unsigned int order = 0;
    };

} // namespace morph

#endif // _BEZCURVE_H_
