/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_API_
#define _H_API_

/*
 * Export marker for public functions. Once the library is built with hidden
 * visibility, only declarations marked SCLI_API leave the shared object.
 * PE targets ignore ELF visibility, so Windows needs the dllexport pair.
 */
#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef SCLI_BUILDING_DLL
#define SCLI_API __declspec(dllexport)
#elif defined(SCLI_USING_DLL)
#define SCLI_API __declspec(dllimport)
#else
#define SCLI_API
#endif
#elif defined(__GNUC__)
#define SCLI_API __attribute__((visibility("default")))
#else
#define SCLI_API
#endif

#endif /* _H_API_ */
