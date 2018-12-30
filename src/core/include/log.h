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

#ifndef _INCLUDE_LOG_H_
# define _INCLUDE_LOG_H_

# include <stdio.h>

# define log_blue "\x1b[34m"
# define log_cian "\x1b[36m"
# define log_green "\x1b[32m"
# define log_red "\x1b[31m"
# define log_reset "\x1b[0m"
# define log_yellow "\x1b[33m"

# define x_log_stdout(format, ...) { \
    do { \
      printf(log_cian "%s" log_reset ":%d: " format, __func__, \
        __LINE__, ## __VA_ARGS__); \
    } while (0); \
  }

# define x_log_stderr(format, ...) { \
    do { \
      printf(log_cian "%s" log_reset ":%d: " format, __func__, \
        __LINE__, ## __VA_ARGS__); \
    } while (0); \
  }

# define x_log_debug(format, ...) \
  x_log_stdout(log_blue format log_reset, ## __VA_ARGS__)

# define x_log_warning(format, ...) \
  x_log_stderr(log_yellow format log_reset, ## __VA_ARGS__)

# define x_log_error(format, ...) \
  x_log_stderr(log_red format log_reset, ## __VA_ARGS__)

#endif /* !_INCLUDE_LOG_H_ */
