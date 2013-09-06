//  loop_time_test.cpp  ----------------------------------------------------------------//

//  Copyright Beman Dawes 2013

//  Distributed under the Boost Software License, Version 1.0.
//  http://www.boost.org/LICENSE_1_0.txt

//--------------------------------------------------------------------------------------//

#define _SCL_SECURE_NO_WARNINGS

//#define BOOST_ENDIAN_NO_INTRINSICS
//#define BOOST_ENDIAN_LOG

#include <boost/endian/detail/disable_warnings.hpp>

#include <boost/endian/conversion.hpp>
#include <boost/endian/types.hpp>
#include <boost/cstdint.hpp>
#include <boost/timer/timer.hpp>
#include <iostream>
#include <cstdlib>
#include <boost/detail/lightweight_main.hpp>

using namespace boost;
using namespace boost::endian;

using std::cout;
using std::endl;

namespace
{
  typedef  boost::timer::nanosecond_type nanosecond_t;
  std::string command_args;
  uint64_t n;                 // number of test cases to run
  int places = 2;             // decimal places for times
  bool verbose (false);

  void process_command_line(int argc, char * argv[])
  {
    for (int a = 0; a < argc; ++a)
    {
      command_args += argv[a];
      if (a != argc-1)
        command_args += ' ';
    }

 //   cout << command_args << '\n';;

    if (argc >=2)
#ifndef _MSC_VER
      n = std::atoll(argv[1]);
#else
      n = _atoi64(argv[1]);
#endif

    for (; argc > 2; ++argv, --argc) 
    {
      if ( *(argv[2]+1) == 'p' )
        places = atoi( argv[2]+2 );
      else if ( *(argv[2]+1) == 'v' )
        verbose = true;
      else
      {
        cout << "Error - unknown option: " << argv[2] << "\n\n";
        argc = -1;
        break;
      }
    }

    if (argc < 2) 
    {
      cout << "Usage: loop_time_test n [Options]\n"
              "  The argument n specifies the number of test cases to run\n"
              "  Options:\n"
              "   -v       Verbose messages\n"
              "   -p#      Decimal places for times; default -p" << places << "\n";
      return std::exit(1);
    }
  }

//--------------------------------------------------------------------------------------//

  template <class T, class EndianT>
  void time()
  {
    T total = 0;
    { 
 //     cout << "*************Endian integer approach...\n";
      EndianT x(0);
      boost::timer::cpu_timer t;
      for (uint64_t i = 0; i < n; ++i)
      {
        x += static_cast<T>(i);
      }
      t.stop();
      total += x;
      cout << "<td align=\"right\">" << t.format(places, "%t") << " s</td>";
    }
    { 
//      cout << "***************Endian conversion approach...\n";
      T x(0);
      boost::timer::cpu_timer t;
      big_endian(x);
      for (uint64_t i = 0; i < n; ++i)
      {
        x += static_cast<T>(i);
      }
      big_endian(x);
      t.stop();
      big_endian(x);
      if (x != total)
        throw std::logic_error("integer approach total != conversion approach total");
      cout << "<td align=\"right\">" << t.format(places, "%t") << " s</td>";
    }
  }

 
  void test_big_align_int16()
  {
    cout << "<tr><td>16-bit aligned big endian</td>";
    time<int16_t, big_align_int16_t>();
    cout << "</tr>\n";
  }

  void test_little_align_int16()
  {
    cout << "<tr><td>16-bit aligned little endian</td>";
    time<int16_t, little_align_int16_t>();
    cout << "</tr>\n";
  }

  void test_big_int16()
  {
    cout << "<tr><td>16-bit unaligned big endian</td>";
    time<int16_t, big_int16_t>();
    cout << "</tr>\n";
  }

  void test_little_int16()
  {
    cout << "<tr><td>16-bit unaligned little endian</td>";
    time<int16_t, little_int16_t>();
    cout << "</tr>\n";
  }
 
  void test_big_align_int32()
  {
    cout << "<tr><td>32-bit aligned big endian</td>";
    time<int32_t, big_align_int32_t>();
    cout << "</tr>\n";
  }

  void test_little_align_int32()
  {
    cout << "<tr><td>32-bit aligned little endian</td>";
    time<int32_t, little_align_int32_t>();
    cout << "</tr>\n";
  }

  void test_big_int32()
  {
    cout << "<tr><td>32-bit unaligned big endian</td>";
    time<int32_t, big_int32_t>();
    cout << "</tr>\n";
  }

  void test_little_int32()
  {
    cout << "<tr><td>32-bit unaligned little endian</td>";
    time<int32_t, little_int32_t>();
    cout << "</tr>\n";
  }
 
  void test_big_align_int64()
  {
    cout << "<tr><td>64-bit aligned big endian</td>";
    time<int64_t, big_align_int64_t>();
    cout << "</tr>\n";
  }

  void test_little_align_int64()
  {
    cout << "<tr><td>64-bit aligned little endian</td>";
    time<int64_t, little_align_int64_t>();
    cout << "</tr>\n";
  }

  void test_big_int64()
  {
    cout << "<tr><td>64-bit unaligned big endian</td>";
    time<int64_t, big_int64_t>();
    cout << "</tr>\n";
  }

  void test_little_int64()
  {
    cout << "<tr><td>64-bit unaligned little endian</td>";
    time<int64_t, little_int64_t>();
    cout << "</tr>\n";
  }

}  // unnamed namespace

//--------------------------------------------------------------------------------------//

int cpp_main(int argc, char* argv[])
{
  process_command_line(argc, argv);
  
  cout
    << "<html>\n<head>\n<title>Endian Loop Time Test</title>\n</head>\n<body>\n"
    << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\""
    << "style=\"border-collapse: collapse\" bordercolor=\"#111111\">\n"
    << "<tr><td colspan=\"6\" align=\"center\"><b>"
    << BOOST_COMPILER << "</b></td></tr>\n"
    << "<tr><td colspan=\"6\" align=\"center\"><b>"
    << " Iterations: " << n
    << ", Intrinsics: " BOOST_ENDIAN_INTRINSIC_MSG
    << "</b></td></tr>\n"
    << "<tr><td><b>Test Case</b></td>\n"
       "<td align=\"center\"><b>Endian<br>type</b></td>\n"
       "<td align=\"center\"><b>Endian<br>conversion<br>function</b></td>\n"
       "</tr>\n"
    ;
  
  test_big_align_int16();
  test_little_align_int16();
  test_big_int16();
  test_little_int16();

  test_big_align_int32();
  test_little_align_int32();
  test_big_int32();
  test_little_int32();

  test_big_align_int64();
  test_little_align_int64();
  test_big_int64();
  test_little_int64();

  cout << "\n</table>\n</body>\n</html>\n";

  return 0;
}

#include <boost/endian/detail/disable_warnings_pop.hpp>