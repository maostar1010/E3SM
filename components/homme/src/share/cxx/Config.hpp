/********************************************************************************
 * HOMMEXX 1.0: Copyright of Sandia Corporation
 * This software is released under the BSD license
 * See the file 'COPYRIGHT' in the HOMMEXX/src/share/cxx directory
 *******************************************************************************/

#ifndef HOMMEXX_CONFIG_HPP
#define HOMMEXX_CONFIG_HPP

#ifdef HOMMEXX_CONFIG_IS_CMAKE
# include "Hommexx_config.h"
# ifdef HAVE_CONFIG_H
#  include "config.h.c"
# endif
#else
#endif

// Set some defaults
#if ! defined HOMMEXX_CUDA_SPACE && ! defined HOMMEXX_OPENMP_SPACE && ! defined HOMMEXX_THREADS_SPACE && ! defined HOMMEXX_SERIAL_SPACE && ! defined HOMMEXX_HIP_SPACE
# define HOMMEXX_DEFAULT_SPACE
#endif

#ifndef HOMMEXX_MPI_ON_DEVICE
# define HOMMEXX_MPI_ON_DEVICE 1
#endif

#ifdef HOMMEXX_ENABLE_GPU 
# ifndef HOMMEXX_CUDA_MIN_WARP_PER_TEAM
#  define HOMMEXX_CUDA_MIN_WARP_PER_TEAM 8
# endif
# ifndef HOMMEXX_CUDA_MAX_WARP_PER_TEAM
#  define HOMMEXX_CUDA_MAX_WARP_PER_TEAM 16
# endif
#elif !defined(HOMMEXX_CONFIG_IS_CMAKE)
# define HOMMEXX_CUDA_MIN_WARP_PER_TEAM 1
# define HOMMEXX_CUDA_MAX_WARP_PER_TEAM 1
#endif

#if defined KOKKOS_COMPILER_GNU
// See https://github.com/kokkos/kokkos-kernels/issues/129
# define ConstExceptGnu
#else
# define ConstExceptGnu const
#endif

#include <Kokkos_Core.hpp>

namespace Homme
{

// Some in-house names for Kokkos exec spaces, which are
// always defined, possibly as alias of void

#ifdef HOMMEXX_ENABLE_GPU

#ifdef KOKKOS_ENABLE_CUDA
  using HommexxGPU = Kokkos::Cuda;
#endif

#ifdef KOKKOS_ENABLE_HIP
  using HommexxGPU = Kokkos::Experimental::HIP;
#endif

#else
using HommexxGPU = void;
#endif // HOMMEXX_ENABLE_GPU

#ifdef KOKKOS_ENABLE_OPENMP
using Hommexx_OpenMP = Kokkos::OpenMP;
#else
using Hommexx_OpenMP = void;
#endif

#ifdef KOKKOS_ENABLE_PTHREADS
using Hommexx_Threads = Kokkos::Threads;
#else
using Hommexx_Threads = void;
#endif

#ifdef KOKKOS_ENABLE_SERIAL
using Hommexx_Serial = Kokkos::Serial;
#else
using Hommexx_Serial = void;
#endif

#ifdef HOMMEXX_ENABLE_GPU
# define HOMMEXX_STATIC
#else
# define HOMMEXX_STATIC static
#endif

// Selecting the execution space. If no specific request, use Kokkos default
// exec space
#ifdef HOMMEXX_ENABLE_GPU
using ExecSpace = HommexxGPU;
#elif defined(HOMMEXX_OPENMP_SPACE)
using ExecSpace = Hommexx_OpenMP;
#elif defined(HOMMEXX_THREADS_SPACE)
using ExecSpace = Hommexx_Threads;
#elif defined(HOMMEXX_SERIAL_SPACE)
using ExecSpace = Hommexx_Serial;
#elif defined(HOMMEXX_DEFAULT_SPACE)
using ExecSpace = Kokkos::DefaultExecutionSpace::execution_space;
#else
#error "No valid execution space choice"
#endif // HOMMEXX_EXEC_SPACE

static_assert (!std::is_same<ExecSpace,void>::value,
               "Error! You are trying to use an ExecutionSpace not enabled in Kokkos.\n");

} // namespace Homme

#endif // HOMMEXX_CONFIG_HPP
