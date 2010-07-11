/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpq_poly.h"

void _fmpq_poly_divrem(fmpz * Q, fmpz_t q, fmpz * R, fmpz_t r, 
                       const fmpz * A, const fmpz_t a, ulong lenA, 
                       const fmpz * B, const fmpz_t b, ulong lenB)
{
    ulong lenQ = lenA + (lenB - 1UL);
    ulong lenR = lenB - 1UL;
    
    if (lenB == 1UL)
    {
        _fmpq_poly_scalar_div_mpq(Q, q, A, a, lenA, B, b);
        _fmpz_vec_zero(R, lenR);
        fmpz_set_ui(r, 1UL);
        return;
    }
    
    ulong d = 1UL;
    const fmpz * lead = B + (lenB - 1UL);
    _fmpz_poly_pseudo_divrem(Q, R, &d, A, lenA, B, lenB);
    
    /* Determine the actual length of R */
    for ( ; lenR != 0 && fmpz_is_zero(R + (lenR - 1UL)); lenR--);
    
    /* 1.  lead^d == +-1.  {Q, q} = {b Q, a}, {R, r} = {R, a} up to sign */
    if (d == 0UL || FLINT_ABS(*lead) == 1L)
    {
        fmpz_set_ui(q, 1UL);
        _fmpq_poly_scalar_mul_mpq(Q, q, Q, q, lenQ, b, a);
        
        fmpz_set_ui(r, 1UL);
        if (lenR > 0UL)
            _fmpq_poly_scalar_div_fmpz(R, r, R, r, lenR, a);
        
        if (*lead == -1L && d % 2UL)
        {
            _fmpz_vec_neg(Q, Q, lenQ);
            _fmpz_vec_neg(R, R, lenR);
        }
    }
    /* 2.  lead^d != +-1.  {Q, q} = {b Q, a lead^d}, {R, r} = {R, a lead^d} */
    else
    {
        // TODO:  Improve this.  Clearly we do not need to compute 
        // den = a lead^d in many cases, but can determine the GCD from 
        // lead alone already.
        fmpz_t den;
        fmpz_init(den);
        fmpz_pow_ui(den, lead, d);
        fmpz_mul(den, a, den);
        
        fmpz_set_ui(q, 1UL);
        _fmpq_poly_scalar_mul_mpq(Q, q, Q, q, lenQ, b, den);
        
        fmpz_set_ui(r, 1UL);
        if (lenR > 0UL)
            _fmpq_poly_scalar_div_fmpz(R, r, R, r, lenR, den);
        
        fmpz_clear(den);
    }
}

void fmpq_poly_divrem(fmpq_poly_t Q, fmpq_poly_t R, 
                      const fmpq_poly_t poly1, const fmpq_poly_t poly2)
{
    if (fmpq_poly_is_zero(poly2))
    {
        printf("Exception: division by zero in fmpq_poly_divrem\n");
        abort();
    }
    if (Q == R)
    {
        printf("Exception: output arguments aliased\n");
        abort();
    }
    
    /* Deal with the various other cases of aliasing. */
    if (R == poly1 | R == poly2)
    {
        if (Q == poly1 | Q == poly2)
        {
            fmpq_poly_t tempQ, tempR;
            fmpq_poly_init(tempQ);
            fmpq_poly_init(tempR);
            fmpq_poly_divrem(tempQ, tempR, poly1, poly2);
            fmpq_poly_swap(Q, tempQ);
            fmpq_poly_swap(R, tempR);
            fmpq_poly_clear(tempQ);
            fmpq_poly_clear(tempR);
            return;
        }
        else
        {
            fmpq_poly_t tempR;
            fmpq_poly_init(tempR);
            fmpq_poly_divrem(Q, tempR, poly1, poly2);
            fmpq_poly_swap(R, tempR);
            fmpq_poly_clear(tempR);
            return;
        }
    }
    else
    {
        if (Q == poly1 | Q == poly2)
        {
            fmpq_poly_t tempQ;
            fmpq_poly_init(tempQ);
            fmpq_poly_divrem(tempQ, R, poly1, poly2);
            fmpq_poly_swap(Q, tempQ);
            fmpq_poly_clear(tempQ);
            return;
        }
    }
    
    if (poly1->length < poly2->length)
    {
        fmpq_poly_zero(Q);
        fmpq_poly_set(R, poly1);
        return;
    }
    
    ulong lenA = poly1->length;
    ulong lenB = poly2->length;
    ulong lenQ = lenA + (lenB - 1UL);
    ulong lenR = lenB - 1UL;
    
    fmpq_poly_fit_length(Q, lenQ);
    fmpq_poly_fit_length(R, lenA);  /* TODO: Need at least that much space */
    
    _fmpq_poly_divrem(Q->coeffs, Q->den, R->coeffs, R->den, 
                      poly1->coeffs, poly1->den, poly1->length, 
                      poly2->coeffs, poly2->den, poly2->length);
    
    _fmpq_poly_set_length(Q, lenQ);
    _fmpq_poly_set_length(R, lenR);
    _fmpq_poly_normalise(R);
}
