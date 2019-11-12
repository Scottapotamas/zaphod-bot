/**
 * @file      qassert.h
 *
 * @ingroup   utility
 *
 * @brief     Custom assert handler for design by contract (DbC).
 *            See http://www.barrgroup.com/Embedded-Systems/How-To/Design-by-Contract-for-Embedded-Software
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2016 Applidyne Pty. Ltd. - All rights reserved.
 */

#ifndef QASSERT_H
#define QASSERT_H

/* -------------------------------------------------------------------------- */

/** NDEBUG macro disables all contract validations
  * (assertions, preconditions, postconditions, and invariants).
  */

#ifdef NDEBUG           /* NDEBUG defined--DbC disabled */

    #define DEFINE_THIS_FILE
    #define ASSERT(ignore_)         ((void)0)
    #define ASSERT_PRINTF(ignore_, ignore_also_) ((void)0)
    #define ALLEGE(test_)    ((void)(test_))

#else                    /* NDEBUG not defined--DbC enabled */

    #ifdef __cplusplus
        extern "C"
        {
    #endif

        /* callback invoked in case of assertion failure */
        void onAssert__( const char * file, unsigned line, const char * fmt, ... )
                        __attribute__ ((format (printf, 3, 4)));

    #ifdef __cplusplus
    }
    #endif

	#define DEFINE_THIS_FILE \
        static char const THIS_FILE__[] = __FILE__

    #define ASSERT(test_) \
        ((test_) ? (void)0 : onAssert__(THIS_FILE__, __LINE__, 0 ))

    #define ASSERT_PRINTF(test_, fmt_, ... ) \
        ((test_) ? (void)0 : onAssert__(THIS_FILE__, __LINE__, fmt_, ## __VA_ARGS__ ))

    #define ALLEGE(test_)    ASSERT(test_)

#endif                   /* NASSERT */

#define REQUIRE(test_)   ASSERT(test_) /* Test preconditions */
#define ENSURE(test_)    ASSERT(test_) /* Test post conditions */
#define INVARIANT(test_) ASSERT(test_) /* Test invariants */

/* -------------------------------------------------------------------------- */

#endif                   /* QASSERT_H */
