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

    Copyright (C) 2009 William Hart
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int result;
    printf("primitive_part....");
    fflush(stdout);

    fmpz_poly_randinit();

    // Check aliasing
    for (ulong i = 0; i < 10000UL; i++)
    {
        fmpz_poly_t f, g;

        fmpz_poly_init(f);
        fmpz_poly_init(g);
        fmpz_poly_randtest(g, n_randint(100), n_randint(200));

        fmpz_poly_primitive_part(f, g);
        fmpz_poly_primitive_part(g, g);
        
        result = (fmpz_poly_equal(f, g));
        if (!result)
        {
            printf("Error:\n");
            fmpz_poly_print(f);
            printf("\n\n");
            fmpz_poly_print(g);
            printf("\n\n");
            abort();
        }

        fmpz_poly_clear(f);
        fmpz_poly_clear(g);
    }

    fmpz_poly_randclear();

    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}