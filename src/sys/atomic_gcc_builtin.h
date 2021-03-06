/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2013 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2011      Sandia National Laboratories. All rights reserved.
 * Copyright (c) 2014-2018 Los Alamos National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2016-2017 Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * Copyright (c) 2018      Triad National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2019      Intel, Inc.  All rights reserved.
 * Copyright (c) 2020      Cisco Systems, Inc.  All rights reserved
 * Copyright (c) 2021      Nanook Consulting.  All rights reserved.
 * Copyright (c) 2021      Amazon.com, Inc. or its affiliates.  All Rights
 *                         reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef PRTE_SYS_ARCH_ATOMIC_H
#define PRTE_SYS_ARCH_ATOMIC_H 1

/**********************************************************************
 *
 * Memory Barriers
 *
 *********************************************************************/
#define PRTE_HAVE_ATOMIC_MEM_BARRIER 1

#define PRTE_HAVE_ATOMIC_MATH_32             1
#define PRTE_HAVE_ATOMIC_COMPARE_EXCHANGE_32 1
#define PRTE_HAVE_ATOMIC_ADD_32              1
#define PRTE_HAVE_ATOMIC_AND_32              1
#define PRTE_HAVE_ATOMIC_OR_32               1
#define PRTE_HAVE_ATOMIC_XOR_32              1
#define PRTE_HAVE_ATOMIC_SUB_32              1
#define PRTE_HAVE_ATOMIC_SWAP_32             1
#define PRTE_HAVE_ATOMIC_MATH_64             1
#define PRTE_HAVE_ATOMIC_COMPARE_EXCHANGE_64 1
#define PRTE_HAVE_ATOMIC_ADD_64              1
#define PRTE_HAVE_ATOMIC_AND_64              1
#define PRTE_HAVE_ATOMIC_OR_64               1
#define PRTE_HAVE_ATOMIC_XOR_64              1
#define PRTE_HAVE_ATOMIC_SUB_64              1
#define PRTE_HAVE_ATOMIC_SWAP_64             1

static inline void prte_atomic_mb(void)
{
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
}

static inline void prte_atomic_rmb(void)
{
#if defined(PRTE_ATOMIC_X86_64)
    /* work around a bug in older gcc versions where ACQUIRE seems to get
     * treated as a no-op instead of being equivalent to
     * __asm__ __volatile__("": : :"memory") */
    __asm__ __volatile__("" : : : "memory");
#else
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif
}

static inline void prte_atomic_wmb(void)
{
    __atomic_thread_fence(__ATOMIC_RELEASE);
}

#define PRTEMB() prte_atomic_mb()

/**********************************************************************
 *
 * Atomic math operations
 *
 *********************************************************************/

/*
 * Suppress numerous (spurious ?) warnings from Oracle Studio compilers
 * see https://community.oracle.com/thread/3968347
 */
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#    pragma error_messages(off, E_ARG_INCOMPATIBLE_WITH_ARG_L)
#endif

static inline bool prte_atomic_compare_exchange_strong_acq_32(prte_atomic_int32_t *addr,
                                                              int32_t *oldval, int32_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_ACQUIRE,
                                       __ATOMIC_RELAXED);
}

static inline bool prte_atomic_compare_exchange_strong_rel_32(prte_atomic_int32_t *addr,
                                                              int32_t *oldval, int32_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_RELEASE,
                                       __ATOMIC_RELAXED);
}

static inline bool prte_atomic_compare_exchange_strong_32(prte_atomic_int32_t *addr,
                                                          int32_t *oldval, int32_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_ACQUIRE,
                                       __ATOMIC_RELAXED);
}

static inline int32_t prte_atomic_swap_32(prte_atomic_int32_t *addr, int32_t newval)
{
    int32_t oldval;
    __atomic_exchange(addr, &newval, &oldval, __ATOMIC_RELAXED);
    return oldval;
}

static inline int32_t prte_atomic_fetch_add_32(prte_atomic_int32_t *addr, int32_t delta)
{
    return __atomic_fetch_add(addr, delta, __ATOMIC_RELAXED);
}

static inline int32_t prte_atomic_add_fetch_32(prte_atomic_int32_t *addr, int32_t delta)
{
    return __atomic_fetch_add(addr, delta, __ATOMIC_RELAXED) + delta;
}

static inline int32_t prte_atomic_fetch_and_32(prte_atomic_int32_t *addr, int32_t value)
{
    return __atomic_fetch_and(addr, value, __ATOMIC_RELAXED);
}

static inline int32_t prte_atomic_and_fetch_32(prte_atomic_int32_t *addr, int32_t value)
{
    return __atomic_fetch_and(addr, value, __ATOMIC_RELAXED) & value;
}

static inline int32_t prte_atomic_fetch_or_32(prte_atomic_int32_t *addr, int32_t value)
{
    return __atomic_fetch_or(addr, value, __ATOMIC_RELAXED);
}

static inline int32_t prte_atomic_or_fetch_32(prte_atomic_int32_t *addr, int32_t value)
{
    return __atomic_fetch_or(addr, value, __ATOMIC_RELAXED) | value;
}

static inline int32_t prte_atomic_fetch_xor_32(prte_atomic_int32_t *addr, int32_t value)
{
    return __atomic_fetch_xor(addr, value, __ATOMIC_RELAXED);
}

static inline int32_t prte_atomic_xor_fetch_32(prte_atomic_int32_t *addr, int32_t value)
{
    return __atomic_fetch_xor(addr, value, __ATOMIC_RELAXED) ^ value;
}

static inline int32_t prte_atomic_fetch_sub_32(prte_atomic_int32_t *addr, int32_t delta)
{
    return __atomic_fetch_sub(addr, delta, __ATOMIC_RELAXED);
}

static inline int32_t prte_atomic_sub_fetch_32(prte_atomic_int32_t *addr, int32_t delta)
{
    return __atomic_fetch_sub(addr, delta, __ATOMIC_RELAXED) - delta;
}

static inline bool prte_atomic_compare_exchange_strong_acq_64(prte_atomic_int64_t *addr,
                                                              int64_t *oldval, int64_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_ACQUIRE,
                                       __ATOMIC_RELAXED);
}

static inline bool prte_atomic_compare_exchange_strong_rel_64(prte_atomic_int64_t *addr,
                                                              int64_t *oldval, int64_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_RELEASE,
                                       __ATOMIC_RELAXED);
}

static inline bool prte_atomic_compare_exchange_strong_64(prte_atomic_int64_t *addr,
                                                          int64_t *oldval, int64_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_ACQUIRE,
                                       __ATOMIC_RELAXED);
}

static inline int64_t prte_atomic_swap_64(prte_atomic_int64_t *addr, int64_t newval)
{
    int64_t oldval;
    __atomic_exchange(addr, &newval, &oldval, __ATOMIC_RELAXED);
    return oldval;
}

static inline int64_t prte_atomic_fetch_add_64(prte_atomic_int64_t *addr, int64_t delta)
{
    return __atomic_fetch_add(addr, delta, __ATOMIC_RELAXED);
}

static inline int64_t prte_atomic_add_fetch_64(prte_atomic_int64_t *addr, int64_t delta)
{
    return __atomic_fetch_add(addr, delta, __ATOMIC_RELAXED) + delta;
}

static inline int64_t prte_atomic_fetch_and_64(prte_atomic_int64_t *addr, int64_t value)
{
    return __atomic_fetch_and(addr, value, __ATOMIC_RELAXED);
}

static inline int64_t prte_atomic_and_fetch_64(prte_atomic_int64_t *addr, int64_t value)
{
    return __atomic_fetch_and(addr, value, __ATOMIC_RELAXED) & value;
}

static inline int64_t prte_atomic_fetch_or_64(prte_atomic_int64_t *addr, int64_t value)
{
    return __atomic_fetch_or(addr, value, __ATOMIC_RELAXED);
}

static inline int64_t prte_atomic_or_fetch_64(prte_atomic_int64_t *addr, int64_t value)
{
    return __atomic_fetch_or(addr, value, __ATOMIC_RELAXED) | value;
}

static inline int64_t prte_atomic_fetch_xor_64(prte_atomic_int64_t *addr, int64_t value)
{
    return __atomic_fetch_xor(addr, value, __ATOMIC_RELAXED);
}

static inline int64_t prte_atomic_xor_fetch_64(prte_atomic_int64_t *addr, int64_t value)
{
    return __atomic_fetch_xor(addr, value, __ATOMIC_RELAXED) ^ value;
}

static inline int64_t prte_atomic_fetch_sub_64(prte_atomic_int64_t *addr, int64_t delta)
{
    return __atomic_fetch_sub(addr, delta, __ATOMIC_RELAXED);
}

static inline int64_t prte_atomic_sub_fetch_64(prte_atomic_int64_t *addr, int64_t delta)
{
    return __atomic_fetch_sub(addr, delta, __ATOMIC_RELAXED) - delta;
}

static inline size_t prte_atomic_fetch_add_size_t(prte_atomic_size_t *addr, size_t delta)
{
    return __atomic_fetch_add(addr, delta, __ATOMIC_RELAXED);
}

static inline size_t prte_atomic_add_fetch_size_t(prte_atomic_size_t *addr, size_t delta)
{
    return __atomic_fetch_add(addr, delta, __ATOMIC_RELAXED) + delta;
}

static inline size_t prte_atomic_fetch_sub_size_t(prte_atomic_size_t *addr, size_t delta)
{
    return __atomic_fetch_sub(addr, delta, __ATOMIC_RELAXED);
}

static inline size_t prte_atomic_sub_fetch_size_t(prte_atomic_size_t *addr, size_t delta)
{
    return __atomic_fetch_sub(addr, delta, __ATOMIC_RELAXED) - delta;
}

static inline bool prte_atomic_compare_exchange_strong_acq_ptr(prte_atomic_intptr_t *addr,
                                                               int64_t *oldval, int64_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_ACQUIRE,
                                       __ATOMIC_RELAXED);
}

static inline bool prte_atomic_compare_exchange_strong_rel_ptr(prte_atomic_intptr_t *addr,
                                                               void *oldval, intptr_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_RELEASE,
                                       __ATOMIC_RELAXED);
}

static inline bool prte_atomic_compare_exchange_strong_ptr(prte_atomic_intptr_t *addr,
                                                           void *oldval, intptr_t newval)
{
    return __atomic_compare_exchange_n(addr, oldval, newval, false, __ATOMIC_ACQUIRE,
                                       __ATOMIC_RELAXED);
}

static inline intptr_t prte_atomic_swap_ptr(prte_atomic_intptr_t *addr, intptr_t newval)
{
    intptr_t oldval;
    __atomic_exchange(addr, &newval, &oldval, __ATOMIC_RELAXED);
    return oldval;
}

#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#    pragma error_messages(default, E_ARG_INCOMPATIBLE_WITH_ARG_L)
#endif

#endif /* ! PRTE_SYS_ARCH_ATOMIC_H */
