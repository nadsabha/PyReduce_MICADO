#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "slit_func_2d_xi_zeta_bd.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define signum(a) (((a) > 0) ? 1 : ((a) < 0) ? -1 : 0)

#define CHECK_INDEX 0

// Store important sizes in global variables to make access easier
// When calculating the proper indices
// When not checking the indices just the variables directly
#if CHECK_INDEX
int _ncols = 0;
int _nrows = 0;
int _ny = 0;
int _nx = 0;
int _osample = 0;
int _n = 0;
int _nd = 0;
#else
#define _ncols ncols
#define _nrows nrows
#define _ny ny
#define _nx nx
#define _osample osample
#define _n n
#define _nd nd
#endif

// Define the sizes of each array
#define MAX_ZETA_X (_ncols)
#define MAX_ZETA_Y (_nrows)
#define MAX_ZETA_Z (3 * ((_osample) + 1))
#define MAX_ZETA (MAX_ZETA_X * MAX_ZETA_Y * MAX_ZETA_Z)
#define MAX_MZETA ((_ncols) * (_nrows))
#define MAX_XI ((_ncols) * (_ny)*4)
#define MAX_PSF_X (_ncols)
#define MAX_PSF_Y (3)
#define MAX_PSF (MAX_PSF_X * MAX_PSF_Y)
#define MAX_A ((_n) * (_nd))
#define MAX_R (_n)
#define MAX_SP (_ncols)
#define MAX_SL (_ny)
#define MAX_LAIJ_X (_ny)
#define MAX_LAIJ_Y (4 * (_osample) + 1)
#define MAX_LAIJ (MAX_LAIJ_X * MAX_LAIJ_Y)
#define MAX_PAIJ_X (_ncols)
#define MAX_PAIJ_Y (5)
#define MAX_PAIJ (MAX_PAIJ_X * MAX_PAIJ_Y)
#define MAX_LBJ (_ny)
#define MAX_PBJ (_ncols)
#define MAX_IM ((_ncols) * (_nrows))

// If we want to check the index use functions to represent the index
// Otherwise a simpler define will do, which should be faster ?
#if CHECK_INDEX
static int zeta_index(int x, int y, int z)
{
    int i = z + y * MAX_ZETA_Z + x * MAX_ZETA_Z * _nrows;
    if ((i < 0) | (i >= MAX_ZETA))
    {
        printf("INDEX OUT OF BOUNDS. Zeta[%i, %i, %i]\n", x, y, z);
        return 0;
    }
    return i;
}

static int mzeta_index(int x, int y)
{
    int i = y + x * _nrows;
    if ((i < 0) | (i >= MAX_MZETA))
    {
        printf("INDEX OUT OF BOUNDS. Mzeta[%i, %i]\n", x, y);
        return 0;
    }
    return i;
}

static int xi_index(int x, int y, int z)
{
    int i = z + 4 * y + _ny * 4 * x;
    if ((i < 0) | (i >= MAX_XI))
    {
        printf("INDEX OUT OF BOUNDS. Xi[%i, %i, %i]\n", x, y, z);
        return 0;
    }
    return i;
}

static int psf_index(int x, int y)
{
    int i = ((x)*3 + (y));
    if ((i < 0) | (i >= MAX_PSF))
    {
        printf("INDEX OUT OF BOUNDS. PSF[%i, %i]\n", x, y);
        return 0;
    }
    return i;
}

static int a_index(int x, int y)
{
    int i = _n * y + x;
    if ((i < 0) | (i >= MAX_A))
    {
        printf("INDEX OUT OF BOUNDS. a[%i, %i]\n", x, y);
        return 0;
    }
    return i;
}

static int r_index(int i)
{
    if ((i < 0) | (i >= MAX_R))
    {
        printf("INDEX OUT OF BOUNDS. r[%i]\n", i);
        return 0;
    }
    return i;
}

static int sp_index(int i)
{
    if ((i < 0) | (i >= MAX_SP))
    {
        printf("INDEX OUT OF BOUNDS. sP[%i]\n", i);
        return 0;
    }
    return i;
}

static int laij_index(int x, int y)
{
    int i = ((y)*_ny) + (x);
    if ((i < 0) | (i >= MAX_LAIJ))
    {
        printf("INDEX OUT OF BOUNDS. l_Aij[%i, %i]\n", x, y);
        return 0;
    }
    return i;
}

static int paij_index(int x, int y)
{
    int i = ((y)*_ncols) + (x);
    if ((i < 0) | (i >= MAX_PAIJ))
    {
        printf("INDEX OUT OF BOUNDS. p_Aij[%i, %i]\n", x, y);
        return 0;
    }
    return i;
}

static int lbj_index(int i)
{
    if ((i < 0) | (i >= MAX_LBJ))
    {
        printf("INDEX OUT OF BOUNDS. l_bj[%i]\n", i);
        return 0;
    }
    return i;
}

static int pbj_index(int i)
{
    if ((i < 0) | (i >= MAX_PBJ))
    {
        printf("INDEX OUT OF BOUNDS. p_bj[%i]\n", i);
        return 0;
    }
    return i;
}

static int im_index(int x, int y)
{
    int i = ((y)*_ncols) + (x);
    if ((i < 0) | (i >= MAX_IM))
    {
        printf("INDEX OUT OF BOUNDS. im[%i, %i]\n", x, y);
        return 0;
    }
    return i;
}

static int sl_index(int i)
{
    if ((i < 0) | (i >= MAX_SL))
    {
        printf("INDEX OUT OF BOUNDS. sL[%i]\n", i);
        return 0;
    }
    return i;
}
#else
#define zeta_index(x, y, z) ((z) + (y)*MAX_ZETA_Z + (x)*MAX_ZETA_Z * _nrows)
#define mzeta_index(x, y) ((y) + (x)*_nrows)
#define xi_index(x, y, z) ((z) + 4 * (y) + _ny * 4 * (x))
#define psf_index(x, y) ((x)*3 + (y))
#define a_index(x, y) ((y)*n + (x))
#define r_index(i) (i)
#define sp_index(i) (i)
#define laij_index(x, y) ((y)*ny) + (x)
#define paij_index(x, y) ((y)*ncols) + (x)
#define lbj_index(i) (i)
#define pbj_index(i) (i)
#define im_index(x, y) ((y)*ncols) + (x)
#define sl_index(i) (i)
#endif

int bandsol(double *a, double *r, int n, int nd)
{
    /*
    bandsol solves a sparse system of linear equations with band-diagonal matrix.
    Band is assumed to be symmetric relative to the main diaginal.
    
    ..math:

        A * x = r

    Parameters
    ----------
    a : double array of shape [n,nd]
        The left-hand-side of the equation system
        The main diagonal should be in a(*,nd/2),
        the first lower subdiagonal should be in a(1:n-1,nd/2-1),
        the first upper subdiagonal is in a(0:n-2,nd/2+1) etc.
        For example:
                / 0 0 X X X \
                | 0 X X X X |
                | X X X X X |
                | X X X X X |
            A = | X X X X X |
                | X X X X X |
                | X X X X X |
                | X X X X 0 |
                \ X X X 0 0 /
    r : double array of shape [n]
        the right-hand-side of the equation system
    n : int
        The number of equations
    nd : int
        The width of the band (3 for tri-diagonal system). Must be an odd number.
    
    Returns
    -------
    code : int
        0 on success, -1 on incorrect size of "a" and -4 on degenerate matrix.
    */
    double aa;
    int i, j, k;

#if CHECK_INDEX
    _n = n;
    _nd = nd;
#endif

    /* Forward sweep */
    for (i = 0; i < n - 1; i++)
    {
        aa = a[a_index(i, nd / 2)];
        r[r_index(i)] /= aa;
        for (j = 0; j < nd; j++)
            a[a_index(i, j)] /= aa;
        for (j = 1; j < min(nd / 2 + 1, n - i); j++)
        {
            aa = a[a_index(i + j, nd / 2 - j)];
            r[r_index(i + j)] -= r[r_index(i)] * aa;
            for (k = 0; k < nd - j; k++)
                a[a_index(i + j, k)] -= a[a_index(i, k + j)] * aa;
        }
    }

    /* Backward sweep */
    r[r_index(n - 1)] /= a[a_index(n - 1, nd / 2)];
    for (i = n - 1; i > 0; i--)
    {
        for (j = 1; j <= min(nd / 2, i); j++)
            r[r_index(i - j)] -= r[r_index(i)] * a[a_index(i - j, nd / 2 + j)];
        r[r_index(i - 1)] /= a[a_index(i - 1, nd / 2)];
    }

    r[r_index(0)] /= a[a_index(0, nd / 2)];
    return 0;
}

int xi_zeta_tensors(int ncols,
                    int nrows,
                    int ny,
                    double *ycen,
                    int *ycen_offset,
                    int y_lower_lim,
                    int osample,
                    double *PSF_curve,
                    xi_ref *xi,
                    zeta_ref *zeta,
                    int *m_zeta)
{
    /*
    Create the Xi and Zeta tensors, that describe the contribution of each pixel to the subpixels of the image,
    Considering the curvature of the slit.

    Parameters
    ----------
    ncols : int
        Swath width in pixels
    nrows : int
        Extraction slit height in pixels
    ny : int
        Size of the slit function array: ny = osample * (nrows + 1) + 1
    ycen : double array of shape (ncols,)
        Order centre line offset from pixel row boundary
    ycen_offsets : int array of shape (ncols,)
        Order image column shift
    y_lower_lim : int
        Number of detector pixels below the pixel containing the central line ycen
    osample : int
        Subpixel ovsersampling factor
    PSF_curve : double array of shape (ncols, 3)
        Parabolic fit to the slit image curvature.
        For column d_x = PSF_curve[ncols][0] +  PSF_curve[ncols][1] *d_y + PSF_curve[ncols][2] *d_y^2,
        where d_y is the offset from the central line ycen.
        Thus central subpixel of omega[x][y'][delta_x][iy'] does not stick out of column x.
    xi : (out) xi_ref array of shape (ncols, ny, 4)
        Convolution tensor telling the coordinates of detector
        pixels on which {x, iy} element falls and the corresponding projections.
    zeta : (out) zeta_ref array of shape (ncols, nrows, 3 * (osample + 1))
        Convolution tensor telling the coordinates of subpixels {x, iy} contributing
        to detector pixel {x, y}.
    m_zeta : (out) int array of shape (ncols, nrows)
        The actual number of contributing elements in zeta for each pixel
    
    Returns
    -------
    code : int
        0 on success, -1 on failure
    */
    int x, xx, y, yy, ix, ix1, ix2, iy, iy1, iy2, m;
    double step, delta, dy, w, d1, d2;

    step = 1.e0 / osample;

    /* Clean xi */
    for (x = 0; x < ncols; x++)
    {
        for (iy = 0; iy < ny; iy++)
        {
            for (m = 0; m < 4; m++)
            {
                xi[xi_index(x, iy, m)].x = -1;
                xi[xi_index(x, iy, m)].y = -1;
                xi[xi_index(x, iy, m)].w = 0.;
            }
        }
    }

    /* Clean zeta */
    for (x = 0; x < ncols; x++)
    {
        for (y = 0; y < nrows; y++)
        {
            m_zeta[mzeta_index(x, y)] = 0;
            for (ix = 0; ix < MAX_ZETA_Z; ix++)
            {
                zeta[zeta_index(x, y, ix)].x = -1;
                zeta[zeta_index(x, y, ix)].iy = -1;
                zeta[zeta_index(x, y, ix)].w = 0.;
            }
        }
    }

    /*
    Construct the xi and zeta tensors. They contain pixel references and contribution. 
    values going from a given subpixel to other pixels (xi) and coming from other subpixels
    to a given detector pixel (zeta).
    Note, that xi and zeta are used in the equations for sL, sP and for the model but they
    do not involve the data, only the geometry. Thus it can be pre-computed once.
    */
    for (x = 0; x < ncols; x++)
    {
        /*
        I promised to reconsider the initial offset. Here it is. For the original layout
        (no column shifts and discontinuities in ycen) there is pixel y that contains the
        central line yc. There are two options here (by construction of ycen that can be 0
        but cannot be 1): (1) yc is inside pixel y and (2) yc falls at the boundary between
        pixels y and y-1. yc cannot be at the boundary of pixels y+1 and y because we would
        select y+1 to be pixel y in that case.

        Next we need to define starting and ending indices iy for sL subpixels that contribute
        to pixel y. I call them iy1 and iy2. For both cases we assume osample+1 subpixels covering
        pixel y (wierd). So for case 1 iy1 will be (y-1)*osample and iy2 == y*osample. Special
        treatment of the boundary subpixels will compensate for introducing extra subpixel in
        case 1. In case 2 things are more logical: iy1=(yc-y)*osample+(y-1)*osample;
        iy2=(y+1-yc)*osample)+(y-1)*osample. ycen is yc-y making things simpler. Note also that
        the same pattern repeates for all rows: we only need to initialize iy1 and iy2 and keep
        incrementing them by osample. 
        */

        iy2 = osample - floor(ycen[x] * osample);
        iy1 = iy2 - osample;

        /*
        Handling partial subpixels cut by detector pixel rows is again tricky. Here we have three
        cases (mostly because of the decision to assume that we always have osample+1 subpixels
        per one detector pixel). Here d1 is the fraction of the subpixel iy1 inside detector pixel y.
        d2 is then the fraction of subpixel iy2 inside detector pixel y. By definition d1+d2==step.
        Case 1: ycen falls on the top boundary of each detector pixel (ycen == 1). Here we conclude
                that the first subpixel is fully contained inside pixel y and d1 is set to step.
        Case 2: ycen falls on the bottom boundary of each detector pixel (ycen == 0). Here we conclude
                that the first subpixel is totally outside of pixel y and d1 is set to 0.
        Case 3: ycen falls inside of each pixel (0>ycen>1). In this case d1 is set to the fraction of
                the first step contained inside of each pixel.
        And BTW, this also means that central line coinsides with the upper boundary of subpixel iy2
        when the y loop reaches pixel y_lower_lim. In other words:

        dy=(iy-(y_lower_lim+ycen[x])*osample)*step-0.5*step
        */

        d1 = fmod(ycen[x], step);
        if (d1 == 0)
            d1 = step;
        d2 = step - d1;

        /*
        The final hurdle for 2D slit decomposition is to construct two 3D reference tensors. We proceed
        similar to 1D case except that now each iy subpixel can be shifted left or right following
        the curvature of the slit image on the detector. We assume for now that each subpixel is
        exactly 1 detector pixel wide. This may not be exactly true if the curvature changes accross
        the focal plane but will deal with it when the necessity will become apparent. For now we
        just assume that a shift delta the weight w assigned to subpixel iy is divided between
        ix1=int(delta) and ix2=int(delta)+signum(delta) as (1-|delta-ix1|)*w and |delta-ix1|*w.

        The curvature is given by a quadratic polynomial evaluated from an approximation for column
        x: delta = PSF_curve[x][0] + PSF_curve[x][1] * (y-yc[x]) + PSF_curve[x][2] * (y-yc[x])^2.
        It looks easy except that y and yc are set in the global detector coordinate system rather than
        in the shifted and cropped swath passed to slit_func_2d. One possible solution I will try here
        is to modify PSF_curve before the call such as:
        delta = PSF_curve'[x][0] + PSF_curve'[x][1] * (y'-ycen[x]) + PSF_curve'[x][2] * (y'-ycen[x])^2
        where y' = y - floor(yc).
        */

        /* Define initial distance from ycen       */
        /* It is given by the center of the first  */
        /* subpixel falling into pixel y_lower_lim */
        dy = ycen[x] - floor((y_lower_lim + ycen[x]) / step) * step - step;

        /*
        Now we go detector pixels x and y incrementing subpixels looking for their controibutions
        to the current and adjacent pixels. Note that the curvature/tilt of the projected slit
        image could be so large that subpixel iy may no contribute to column x at all. On the
        other hand, subpixels around ycen by definition must contribute to pixel x,y. 
        3rd index in xi refers corners of pixel xx,y: 0:LL, 1:LR, 2:UL, 3:UR.
        */

        for (y = 0; y < nrows; y++)
        {
            iy1 += osample; // Bottom subpixel falling in row y
            iy2 += osample; // Top subpixel falling in row y
            dy -= step;
            for (iy = iy1; iy <= iy2; iy++)
            {
                if (iy == iy1)
                    w = d1;
                else if (iy == iy2)
                    w = d2;
                else
                    w = step;
                dy += step;
                delta = (PSF_curve[psf_index(x, 1)] + PSF_curve[psf_index(x, 2)] * (dy - ycen[x])) * (dy - ycen[x]);
                ix1 = delta;
                ix2 = ix1 + signum(delta);

                /* Three cases: subpixel on the bottom boundary of row y, intermediate subpixels and top boundary */

                if (iy == iy1) /* Case A: Subpixel iy is entering detector row y */
                {
                    if (ix1 < ix2) /* Subpixel iy shifts to the right from column x  */
                    {
                        if (x + ix1 >= 0 && x + ix2 < ncols)
                        {
                            xx = x + ix1; /* Upper right corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 3)].x = xx;
                            xi[xi_index(x, iy, 3)].y = yy;
                            xi[xi_index(x, iy, 3)].w = w - fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 3)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 3)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                            xx = x + ix2; /* Upper left corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 2)].x = xx;
                            xi[xi_index(x, iy, 2)].y = yy;
                            xi[xi_index(x, iy, 2)].w = fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 2)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 2)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                        }
                    }
                    else if (ix1 > ix2) /* Subpixel iy shifts to the left from column x */
                    {
                        if (x + ix2 >= 0 && x + ix1 < ncols)
                        {
                            xx = x + ix2; /* Upper left corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 2)].x = xx;
                            xi[xi_index(x, iy, 2)].y = yy;
                            xi[xi_index(x, iy, 2)].w = fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 2)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 2)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                            xx = x + ix1; /* Upper right corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 3)].x = xx;
                            xi[xi_index(x, iy, 3)].y = yy;
                            xi[xi_index(x, iy, 3)].w = w - fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 3)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 3)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                        }
                    }
                    else
                    {
                        xx = x + ix1; /* Subpixel iy stays inside column x */
                        yy = y + ycen_offset[x] - ycen_offset[xx];
                        xi[xi_index(x, iy, 2)].x = xx;
                        xi[xi_index(x, iy, 2)].y = yy;
                        xi[xi_index(x, iy, 2)].w = w;
                        if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && w > 0)
                        {
                            m = m_zeta[mzeta_index(xx, yy)];
                            zeta[zeta_index(xx, yy, m)].x = x;
                            zeta[zeta_index(xx, yy, m)].iy = iy;
                            zeta[zeta_index(xx, yy, m)].w = w;
                            m_zeta[mzeta_index(xx, yy)]++;
                        }
                    }
                }
                else if (iy == iy2) /* Case C: Subpixel iy is leaving detector row y */
                {
                    if (ix1 < ix2) /* Subpixel iy shifts to the right from column x */
                    {
                        if (x + ix1 >= 0 && x + ix2 < ncols)
                        {
                            xx = x + ix1; /* Bottom right corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 1)].x = xx;
                            xi[xi_index(x, iy, 1)].y = yy;
                            xi[xi_index(x, iy, 1)].w = w - fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 1)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 1)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                            xx = x + ix2; /* Bottom left corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 0)].x = xx;
                            xi[xi_index(x, iy, 0)].y = yy;
                            xi[xi_index(x, iy, 0)].w = fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 0)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 0)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                        }
                    }
                    else if (ix1 > ix2) /* Subpixel iy shifts to the left from column x */
                    {
                        if (x + ix2 >= 0 && x + ix1 < ncols)
                        {
                            xx = x + ix2; /* Bottom left corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 0)].x = xx;
                            xi[xi_index(x, iy, 0)].y = yy;
                            xi[xi_index(x, iy, 0)].w = fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 0)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 0)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                            xx = x + ix1; /* Bottom right corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 1)].x = xx;
                            xi[xi_index(x, iy, 1)].y = yy;
                            xi[xi_index(x, iy, 1)].w = w - fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 1)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 1)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                        }
                    }
                    else /* Subpixel iy stays inside column x        */
                    {
                        xx = x + ix1;
                        yy = y + ycen_offset[x] - ycen_offset[xx];
                        xi[xi_index(x, iy, 0)].x = xx;
                        xi[xi_index(x, iy, 0)].y = yy;
                        xi[xi_index(x, iy, 0)].w = w;
                        if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && w > 0)
                        {
                            m = m_zeta[mzeta_index(xx, yy)];
                            zeta[zeta_index(xx, yy, m)].x = x;
                            zeta[zeta_index(xx, yy, m)].iy = iy;
                            zeta[zeta_index(xx, yy, m)].w = w;
                            m_zeta[mzeta_index(xx, yy)]++;
                        }
                    }
                }
                else /* CASE B: Subpixel iy is fully inside detector row y */
                {
                    if (ix1 < ix2) /* Subpixel iy shifts to the right from column x      */
                    {
                        if (x + ix1 >= 0 && x + ix2 < ncols)
                        {
                            xx = x + ix1; /* Bottom right corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 1)].x = xx;
                            xi[xi_index(x, iy, 1)].y = yy;
                            xi[xi_index(x, iy, 1)].w = w - fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 1)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 1)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                            xx = x + ix2; /* Bottom left corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 0)].x = xx;
                            xi[xi_index(x, iy, 0)].y = yy;
                            xi[xi_index(x, iy, 0)].w = fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 0)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 0)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                        }
                    }
                    else if (ix1 > ix2) /* Subpixel iy shifts to the left from column x */
                    {
                        if (x + ix2 >= 0 && x + ix1 < ncols)
                        {
                            xx = x + ix2; /* Bottom right corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 1)].x = xx;
                            xi[xi_index(x, iy, 1)].y = yy;
                            xi[xi_index(x, iy, 1)].w = fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 1)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 1)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                            xx = x + ix1; /* Bottom left corner of subpixel iy */
                            yy = y + ycen_offset[x] - ycen_offset[xx];
                            xi[xi_index(x, iy, 0)].x = xx;
                            xi[xi_index(x, iy, 0)].y = yy;
                            xi[xi_index(x, iy, 0)].w = w - fabs(delta - ix1) * w;
                            if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && xi[xi_index(x, iy, 0)].w > 0)
                            {
                                m = m_zeta[mzeta_index(xx, yy)];
                                zeta[zeta_index(xx, yy, m)].x = x;
                                zeta[zeta_index(xx, yy, m)].iy = iy;
                                zeta[zeta_index(xx, yy, m)].w = xi[xi_index(x, iy, 0)].w;
                                m_zeta[mzeta_index(xx, yy)]++;
                            }
                        }
                    }
                    else /* Subpixel iy stays inside column x */
                    {
                        xx = x + ix2;
                        yy = y + ycen_offset[x] - ycen_offset[xx];
                        xi[xi_index(x, iy, 0)].x = xx;
                        xi[xi_index(x, iy, 0)].y = yy;
                        xi[xi_index(x, iy, 0)].w = w;
                        if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows && w > 0)
                        {
                            m = m_zeta[mzeta_index(xx, yy)];
                            zeta[zeta_index(xx, yy, m)].x = x;
                            zeta[zeta_index(xx, yy, m)].iy = iy;
                            zeta[zeta_index(xx, yy, m)].w = w;
                            m_zeta[mzeta_index(xx, yy)]++;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int slit_func_curved(int ncols,
                     int nrows,
                     int nx,
                     int ny,
                     double *im,
                     double *pix_unc,
                     int *mask_orig,
                     double *ycen,
                     int *ycen_offset,
                     int y_lower_lim,
                     int osample,
                     double lambda_sP,
                     double lambda_sL,
                     double *PSF_curve,
                     double *sP,
                     double *sL,
                     double *model,
                     double *unc,
                     int *mask,
                     double *l_Aij,
                     double *l_bj,
                     double *p_Aij,
                     double *p_bj)
{
    /*
    Extract the spectrum and slit illumination function for a curved slit

    This function does not assign or free any memory,
    therefore all working arrays are passed as parameters.
    The contents of which will be overriden however

    Parameters
    ----------
    ncols : int
        Swath width in pixels
    nrows : int
        Extraction slit height in pixels
    nx : int
        Range of columns affected by PSF tilt: nx = 2 * delta_x + 1
    ny : int
        Size of the slit function array: ny = osample * (nrows + 1) + 1
    im : double array of shape (nrows, ncols)
        Image to be decomposed
    pix_unc : double array of shape (nrows, ncols)
        Individual pixel uncertainties. Set to zero if unknown.
    mask : byte array of shape (nrows, ncols)
        Initial and final mask for the swath, both in and output
    ycen : double array of shape (ncols,)
        Order centre line offset from pixel row boundary.
        Should only contain values between 0 and 1.
    ycen_offset : int array of shape (ncols,)
        Order image column shift
    y_lower_lim : int
        Number of detector pixels below the pixel containing
        the central line yc.
    osample : int
        Subpixel ovsersampling factor
    lambda_sP : double
        Smoothing parameter for the spectrum, could be zero
    lambda_sL : double
        Smoothing parameter for the slit function, usually > 0
    PSF_curve : double array of shape (ncols, 3)
        Slit curvature parameters for each point along the spectrum
    sP : (out) double array of shape (ncols,)
        Spectrum resulting from decomposition
    sL : (out) double array of shape (ny,)
        Slit function resulting from decomposition
    model : (out) double array of shape (ncols, nrows)
        Model constructed from sp and sf
    unc : (out) double array of shape (ncols,)
        Spectrum uncertainties based on data - model and pix_unc
    sP_old : double array of shape (ncols,)
        Work array to control the convergence
    l_Aij : double array of shape (ny, 4 * osample + 1,)
        Band-diagonal input matrix for sL
    l_bj : double array of shape (ny,)
        RHS input for sL
    p_Aij : double array of shape (ncols * 5,)
        Band-diagonal input matrix for sP
    p_bj : double array of shape (ncols,)
        RHS input for sP
    
    Returns
    -------
    code : int
        0 on success, -1 on failure (see also bandsol)
    */
    int x, xx, xxx, y, yy, iy, jy, n, m;
    double delta_x, sum, norm, dev, lambda, diag_tot, ww, www;
    double cost, cost_old, cost_min, tmp, ftol;
    int info, iter, isum, max_iter;

    // Maximum number of iterations in the procedure
    max_iter = 20;
    // difference fraction criterium for early abortion of the fitting
    ftol = 1e-7;
    // Minimum cost (reduced chi square) before canceling the iteration
    cost_min = 1;

#if CHECK_INDEX
    _ncols = ncols;
    _nrows = nrows;
    _nx = nx;
    _ny = ny;
    _osample = osample;
#endif

    for (int i = 0; i < MAX_IM; i++)
        mask[i] = mask_orig[i];

    xi_ref *xi = malloc(MAX_XI * sizeof(xi_ref));
    zeta_ref *zeta = malloc(MAX_ZETA * sizeof(zeta_ref));
    int *m_zeta = malloc(MAX_MZETA * sizeof(int));

    xi_zeta_tensors(ncols, nrows, ny, ycen, ycen_offset, y_lower_lim, osample, PSF_curve, xi, zeta, m_zeta);

    cost = INFINITY;
    /* Maximum horizontal shift in detector pixels due to slit image curvature */
    delta_x = nx / 2;
    /* The size of the sL array. Extra osample is because ycen can be between 0 and 1. */
    // ny = osample * (nrows + 1) + 1;

    /* Loop through sL , sP reconstruction until convergence is reached */
    iter = 0;
    do
    {
        // Save the total cost (chi-square) from the previous iteration
        cost_old = cost;
        /* Compute slit function sL */

        /* Prepare the RHS and the matrix */
        for (iy = 0; iy < MAX_LBJ; iy++)
            l_bj[lbj_index(iy)] = 0.e0; /* Clean RHS                */
        for (iy = 0; iy < MAX_LAIJ; iy++)
            l_Aij[iy] = 0;

        /* Fill in SLE arrays for slit function */
        diag_tot = 0.e0;
        for (iy = 0; iy < ny; iy++)
        {
            for (x = 0; x < ncols; x++)
            {
                for (n = 0; n < 4; n++)
                {
                    ww = xi[xi_index(x, iy, n)].w;
                    if (ww > 0)
                    {
                        xx = xi[xi_index(x, iy, n)].x;
                        yy = xi[xi_index(x, iy, n)].y;
                        if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows)
                        {
                            if (m_zeta[mzeta_index(xx, yy)] > 0)
                            {
                                for (m = 0; m < m_zeta[mzeta_index(xx, yy)]; m++)
                                {
                                    xxx = zeta[zeta_index(xx, yy, m)].x;
                                    jy = zeta[zeta_index(xx, yy, m)].iy;
                                    www = zeta[zeta_index(xx, yy, m)].w;
                                    if (jy >= 0)
                                        l_Aij[laij_index(iy, jy - iy + 2 * osample)] += sP[sp_index(xxx)] * sP[sp_index(x)] * www * ww * mask[im_index(xx, yy)];
                                }
                                l_bj[lbj_index(iy)] += im[im_index(xx, yy)] * mask[im_index(xx, yy)] * sP[sp_index(x)] * ww;
                            }
                        }
                    }
                }
            }
            diag_tot += l_Aij[laij_index(iy, 2 * osample)];
        }

        /* Scale regularization parameters */
        lambda = lambda_sL * diag_tot / ny;

        /* Add regularization parts for the SLE matrix */

        l_Aij[laij_index(0, 2 * osample)] += lambda;     /* Main diagonal  */
        l_Aij[laij_index(0, 2 * osample + 1)] -= lambda; /* Upper diagonal */
        for (iy = 1; iy < ny - 1; iy++)
        {
            l_Aij[laij_index(iy, 2 * osample - 1)] -= lambda;    /* Lower diagonal */
            l_Aij[laij_index(iy, 2 * osample)] += lambda * 2.e0; /* Main diagonal  */
            l_Aij[laij_index(iy, 2 * osample + 1)] -= lambda;    /* Upper diagonal */
        }
        l_Aij[laij_index(ny - 1, 2 * osample - 1)] -= lambda; /* Lower diagonal */
        l_Aij[laij_index(ny - 1, 2 * osample)] += lambda;     /* Main diagonal  */

        /* Solve the system of equations */

        info = bandsol(l_Aij, l_bj, ny, 4 * osample + 1);
        if (info)
            printf("info(sL)=%d\n", info);

        /* Normalize the slit function */

        norm = 0.e0;
        for (iy = 0; iy < ny; iy++)
        {
            sL[sl_index(iy)] = l_bj[lbj_index(iy)];
            norm += sL[sl_index(iy)];
        }
        norm /= osample;
        for (iy = 0; iy < ny; iy++)
            sL[sl_index(iy)] /= norm;

        /* Compute spectrum sP */
        for (x = 0; x < MAX_PBJ; x++)
            p_bj[x] = 0;
        for (x = 0; x < MAX_PAIJ; x++)
            p_Aij[x] = 0;

        for (x = 0; x < ncols; x++)
        {
            for (iy = 0; iy < ny; iy++)
            {
                for (n = 0; n < 4; n++)
                {
                    ww = xi[xi_index(x, iy, n)].w;
                    if (ww > 0)
                    {
                        xx = xi[xi_index(x, iy, n)].x;
                        yy = xi[xi_index(x, iy, n)].y;
                        if (xx >= 0 && xx < ncols && yy >= 0 && yy < nrows)
                        {
                            if (m_zeta[mzeta_index(xx, yy)] > 0)
                            {
                                for (m = 0; m < m_zeta[mzeta_index(xx, yy)]; m++)
                                {
                                    xxx = zeta[zeta_index(xx, yy, m)].x;
                                    jy = zeta[zeta_index(xx, yy, m)].iy;
                                    www = zeta[zeta_index(xx, yy, m)].w;
                                    p_Aij[paij_index(x, xxx - x + 2)] += sL[sl_index(jy)] * sL[sl_index(iy)] * www * ww * mask[im_index(xx, yy)];
                                }
                                p_bj[pbj_index(x)] += im[im_index(xx, yy)] * mask[im_index(xx, yy)] * sL[sl_index(iy)] * ww;
                            }
                        }
                    }
                }
            }
        }

        if (lambda_sP > 0.e0)
        {
            norm = 0.e0;
            for (x = 0; x < ncols; x++)
            {
                norm += sP[sp_index(x)];
            }
            norm /= ncols;
            lambda = lambda_sP * norm; /* Scale regularization parameter */

            p_Aij[paij_index(0, 2)] += lambda; /* Main diagonal  */
            p_Aij[paij_index(0, 3)] -= lambda; /* Upper diagonal */
            for (x = 1; x < ncols - 1; x++)
            {
                p_Aij[paij_index(x, 1)] -= lambda;        /* Lower diagonal */
                p_Aij[paij_index(x, 2)] += lambda * 2.e0; /* Main diagonal  */
                p_Aij[paij_index(x, 3)] -= lambda;        /* Upper diagonal */
            }
            p_Aij[paij_index(ncols - 1, 1)] -= lambda; /* Lower diagonal */
            p_Aij[paij_index(ncols - 1, 2)] += lambda; /* Main diagonal  */
        }

        /* Solve the system of equations */

        info = bandsol(p_Aij, p_bj, ncols, 5);
        if (info)
            printf("info(sP)=%d\n", info);

        for (x = 0; x < ncols; x++)
        {
            sP[sp_index(x)] = p_bj[pbj_index(x)];
        }

        /* Compute the model */
        for (x = 0; x < MAX_IM; x++)
        {
            model[x] = 0.;
        }

        for (y = 0; y < nrows; y++)
        {
            for (x = 0; x < ncols; x++)
            {
                for (m = 0; m < m_zeta[mzeta_index(x, y)]; m++)
                {
                    xx = zeta[zeta_index(x, y, m)].x;
                    iy = zeta[zeta_index(x, y, m)].iy;
                    ww = zeta[zeta_index(x, y, m)].w;
                    model[im_index(x, y)] += sP[sp_index(xx)] * sL[sl_index(iy)] * ww;
                }
            }
        }

        /* Compare model and data */

        sum = 0.e0;
        isum = 0;
        cost = 0;
        for (y = 0; y < nrows; y++)
        {
            for (x = delta_x; x < ncols - delta_x; x++)
            {
                if (mask[im_index(x, y)])
                {
                    tmp = (model[im_index(x, y)] - im[im_index(x, y)]) * (model[im_index(x, y)] - im[im_index(x, y)]);
                    sum += tmp;
                    isum++;
                    cost += tmp / max(pix_unc[im_index(x, y)] * pix_unc[im_index(x, y)], 1);
                }
            }
        }
        cost /= (isum - (ncols + ny));
        dev = sqrt(sum / isum);

        /* Adjust the mask marking outlyers */
        for (y = 0; y < nrows; y++)
        {
            for (x = delta_x; x < ncols - delta_x; x++)
            {
                if ((mask_orig[im_index(x, y)] != 0) & (fabs(model[im_index(x, y)] - im[im_index(x, y)]) < 6. * dev))
                    mask[im_index(x, y)] = 1;
                else
                    mask[im_index(x, y)] = 0;
            }
        }

        // printf("reduced chi-square: %f\n", cost);
        // printf("dev: %f\n", dev);
        // printf("sum: %f\n", sum);
        // printf("isum: %i\n", isum);
        // printf("-----------\n");

        /* Check for convergence */
        // Less than maximum iterations, no Nans, and cost either stopped improving or is less than the threshold
    } while ((iter++ < max_iter) && ((isfinite(cost) == 0) || ((isfinite(cost_old) == 0)) || ((cost_old - cost > ftol) && (cost > cost_min))));

    /* Uncertainty estimate */

    for (x = 0; x < ncols; x++)
    {
        unc[sp_index(x)] = 0.;
        p_bj[pbj_index(x)] = 0.;
    }

    for (y = 0; y < nrows; y++)
    {
        for (x = 0; x < ncols; x++)
        {
            for (m = 0; m < m_zeta[mzeta_index(x, y)]; m++) // Loop through all pixels contributing to x,y
            {
                xx = zeta[zeta_index(x, y, m)].x;
                iy = zeta[zeta_index(x, y, m)].iy;
                ww = zeta[zeta_index(x, y, m)].w;
                unc[sp_index(xx)] += (im[im_index(x, y)] - model[im_index(x, y)]) * (im[im_index(x, y)] - model[im_index(x, y)]) *
                                     ww * mask[im_index(x, y)];
                unc[sp_index(xx)] += pix_unc[im_index(x, y)] * pix_unc[im_index(x, y)] * ww * mask[im_index(x, y)];
                p_bj[pbj_index(xx)] += ww * mask[im_index(x, y)]; // Norm
            }
        }
    }

    for (x = 0; x < ncols; x++)
    {
        unc[sp_index(x)] = sqrt(unc[sp_index(x)] / p_bj[pbj_index(x)] * nrows);
    }

    free(xi);
    free(zeta);
    free(m_zeta);

    return 0;
}
