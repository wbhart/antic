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

    Copyright (C) 2010 William Hart

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "nmod_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;

    printf("shift_left_right....");
    fflush(stdout);

    /* Check a << shift >> shift == a */
    for (i = 0; i < 10000; i++)
    {
        nmod_poly_t a, b;
        mp_limb_t n = n_randtest_not_zero();
        long shift = n_randint(100);
  
        nmod_poly_init(a, n);
        nmod_poly_init(b, n);
        nmod_poly_randtest(a, n_randint(100));
        
        nmod_poly_shift_left(b, a, shift);
        nmod_poly_shift_right(b, b, shift);
        
        result = (nmod_poly_equal(a, b));
        if (!result)
        {
            printf("FAIL:\n");
            printf("shift = %ld, a->length = %ld, n = %lu\n", 
                shift, a->length, a->mod.n);
            nmod_poly_print(a), printf("\n\n");
            nmod_poly_print(b), printf("\n\n");
            abort();
        }

        nmod_poly_clear(a);
        nmod_poly_clear(b);
    }
    
    /* Check a << shift >> shift == a aliasing the other way */
    for (i = 0; i < 10000; i++)
    {
        nmod_poly_t a, b, c;
        mp_limb_t n = n_randtest_not_zero();
        long shift = n_randint(100);

        nmod_poly_init(a, n);
        nmod_poly_init(b, n);
        nmod_poly_init(c, n);
        nmod_poly_randtest(c, n_randint(100));
        
        nmod_poly_set(a, c);
        nmod_poly_shift_left(c, c, shift);
        nmod_poly_shift_right(b, c, shift);

        result = (nmod_poly_equal(a, b));
        if (!result)
        {
            printf("FAIL:\n");
            printf("shift = %ld, c->length = %ld, n = %lu\n", 
                shift, c->length, a->mod.n);
            nmod_poly_print(a), printf("\n\n");
            nmod_poly_print(b), printf("\n\n");
            abort();
        }

        nmod_poly_clear(a);
        nmod_poly_clear(b);
        nmod_poly_clear(c);
    }

    printf("PASS\n");
    return 0;
}
