/*
 * From Barr Group Design by Contract
 * https://barrgroup.com/Embedded-Systems/How-To/Design-by-Contract-for-Embedded-Software
 */

#ifndef QASSERT_H
#define QASSERT_H

/** NASSERT macro disables all contract validations
 * (assertions, preconditions, postconditions, and invariants).
 */
#ifdef NASSERT /* NASSERT defined--DbC disabled */

#define DEFINE_THIS_FILE
#define ASSERT( ignore_ ) ( (void)0 )
#define ALLEGE( test_ )   ( (void)( test_ ) )

#else /* NASSERT not defined--DbC enabled */

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Callback invoked in case of assertion failure
 * See main.c for implementation
 */

void portAssertHandler( const char *file,
                        unsigned    line,
                        const char *fmt,
                        ... );

#ifdef __cplusplus
}
#endif

#define DEFINE_THIS_FILE static char const THIS_FILE__[] = __FILE__

#define ASSERT(test_) \
        ((test_) ? (void)0 : portAssertHandler(THIS_FILE__, __LINE__, 0 ))

#define ASSERT_PRINTF(test_, fmt_, ... ) \
        ((test_) ? (void)0 : portAssertHandler(THIS_FILE__, __LINE__, fmt_, ## __VA_ARGS__ ))

#define ALLEGE( test_ ) ASSERT( test_ )

#endif /* NASSERT */

#define REQUIRE( test_ )   ASSERT( test_ ) /* Test preconditions */
#define ENSURE( test_ )    ASSERT( test_ ) /* Test post conditions */
#define INVARIANT( test_ ) ASSERT( test_ ) /* Test invariants */

#endif /* QASSERT_H */