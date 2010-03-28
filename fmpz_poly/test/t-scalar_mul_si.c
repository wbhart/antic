/*============================================================================

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

===============================================================================*/
/****************************************************************************

   Copyright (C) 2009 William Hart

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "ulong_extras.h"

int main(void)
{
   int result;
   printf("scalar_mul_si....");
   fflush(stdout);
   
   fmpz_poly_randinit();
   
   // check aliasing of a and b
   for (ulong i = 0; i < 10000UL; i++) 
   {
      fmpz_poly_t a, b;
      long n = (long) n_randtest();

      fmpz_poly_init(a);
      fmpz_poly_init(b);
      fmpz_poly_randtest(a, n_randint(100), n_randint(200));
      
      fmpz_poly_scalar_mul_si(b, a, n);
      fmpz_poly_scalar_mul_si(a, a, n);
      
      result = (fmpz_poly_equal(a, b));
      if (!result)
      {
         printf("Error:\n");
         fmpz_poly_print(a); printf("\n\n");
         fmpz_poly_print(b); printf("\n\n");
         abort();
      }

      fmpz_poly_clear(a);
      fmpz_poly_clear(b);
   }

   // compare with fmpz_poly_scalar_mul_ui
   for (ulong i = 0; i < 10000UL; i++) 
   {
      fmpz_poly_t a, b, c;
      ulong n = n_randbits(FLINT_BITS - 1);

      fmpz_poly_init(a);
      fmpz_poly_init(b);
      fmpz_poly_randtest(a, n_randint(100), n_randint(200));
      
      fmpz_poly_scalar_mul_ui(b, a, n);
      fmpz_poly_scalar_mul_si(a, a, n);
      
      result = (fmpz_poly_equal(a, b));
      if (!result)
      {
         printf("Error:\n");
         fmpz_poly_print(a); printf("\n\n");
         fmpz_poly_print(b); printf("\n\n");
         abort();
      }

      fmpz_poly_clear(a);
      fmpz_poly_clear(b);
   }

   // check (a*n1)*n2 = a*(n1*n2)
   for (ulong i = 0; i < 10000UL; i++) 
   {
      fmpz_poly_t a, b, c;
      long n1 = (long) n_randbits((FLINT_BITS-2)/2);
      long n2 = (long) n_randbits((FLINT_BITS-2)/2);
      if (n_randint(2)) n1 = -n1;
      if (n_randint(2)) n2 = -n2;

      fmpz_poly_init(a);
      fmpz_poly_init(b);
      fmpz_poly_init(c);
      fmpz_poly_randtest(a, n_randint(100), n_randint(200));
      
      fmpz_poly_scalar_mul_si(b, a, n1);
      fmpz_poly_scalar_mul_si(c, b, n2);
      fmpz_poly_scalar_mul_si(b, a, n1*n2);

      result = (fmpz_poly_equal(b, c));
      if (!result)
      {
         printf("Error n1 = %lu, n2 = %lu:\n", n1, n2);
         fmpz_poly_print(a); printf("\n\n");
         fmpz_poly_print(b); printf("\n\n");
         fmpz_poly_print(c); printf("\n\n");
         abort();
      }

      fmpz_poly_clear(a);
      fmpz_poly_clear(b);
      fmpz_poly_clear(c);
   }

   fmpz_poly_randclear();
      
   _fmpz_cleanup();
   printf("PASS\n");
   return 0;
}