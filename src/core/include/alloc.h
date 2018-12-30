/*
 * This file is part of mytank-module.
 *
 * mytank-module is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mytank-module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mytank-module.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _INCLUDE_ALLOC_H_
# define _INCLUDE_ALLOC_H_

# include <stdlib.h>
# include "cond.h"

/**
 * @brief Same behavior than the standard #calloc + control memory and assert if
 * no memory available + memset to 0
 * @param [in] nmemb : nmemb elements
 * @param [in] size : size bytes for each element
 * @return a valid pointer on success, NULL on error
 */
static inline void *x_calloc(size_t nmemb, size_t size)
{
  void *ptr = calloc(nmemb, size);
  x_assert(ptr, "allocator failed '%s'\n", strerror(errno));

  memset(ptr, 0, size * nmemb);
  return ptr;
}

/**
 * @brief Check pointer before calling #free alloc function
 * @param [in] ptr : pointer to free
 */
static inline void x_free(void *ptr)
{
  x_return_if_fail(ptr);
  free(ptr);
}

/**
 * @brief Same behavior than the standard #malloc + control memory and assert if
 * no memory available + memset to 0
 * @param [in] size : size bytes to allocate
 * @return a valid pointer on success, NULL on error
 */
static inline void *x_malloc(size_t size)
{
  void *ptr = malloc(size);
  x_assert(ptr, "allocator failed '%s'\n", strerror(errno));

  memset(ptr, 0, size);
  return ptr;
}

/**
 * @brief Same behavior than the standard #realloc + control memory and assert
 * if no memory available + memset to 0
 * @param [in] ptr : initial pointer to modify
 * @param [in] size : size bytes to allocate
 * @return a valid pointer on success, NULL on error
 */
static inline void *x_realloc(void *ptr, size_t size)
{
  void *_ptr = realloc(ptr, size);
  x_assert(ptr, "allocator failed '%s'\n", strerror(errno));

  memset(_ptr, 0, size);
  return _ptr;
}

#endif /* !_INCLUDE_ALLOC_H_ */
