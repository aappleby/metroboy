#include "test_utils.h"

#include "MtCursor.h"
#include "MtModLibrary.h"
#include "MtModule.h"
#include "MtSourceFile.h"

//------------------------------------------------------------------------------

bool comp_iws(const char* a, const char* b) {
  if (!a) return false;
  if (!b) return false;

  while (1) {
    if (isspace(*a) != isspace(*b)) return false;
    while (isspace(*a)) a++;
    while (isspace(*b)) b++;
    if (*a != *b) return false;
    if (*a == 0) break;
    a++;
    b++;
  }

  return true;
}

bool comp_iws(const std::string& a, const std::string& b) {
  return comp_iws(a.c_str(), b.c_str());
}

bool find_iws(const char* a, const char* b) {
  if (!a) return false;
  if (!b) return false;

  const char* c = a;
  const char* d = b;

  while (1) {
    if (*d == 0) return true;
    if (*c == 0) return false;

    if (isspace(*c) && isspace(*d)) {
      while (*c && isspace(*c)) c++;
      while (*d && isspace(*d)) d++;
      if (*d == 0) return true;
      if (*c == 0) return false;
    }

    if (*c != *d) {
      // Mismatch, restart
      c++;
      d = b;
    } else {
      // Match, continue
      c++;
      d++;
    }
  }
}

bool find_iws(const std::string& a, const std::string& b) {
  return find_iws(a.c_str(), b.c_str());
}

//------------------------------------------------------------------------------

void parse_simple(std::string src, MtModLibrary& library) {
  /*
  std::string out;

  auto source_file = new MtSourceFile("test.h", src);

  for (auto& mod : source_file->modules) mod->load_pass1();
  for (auto& mod : source_file->modules) mod->load_pass2();
  for (auto& mod : source_file->modules) mod->load_pass3();
  for (auto& mod : source_file->modules) {
    mod->check_dirty_ticks();
    mod->check_dirty_tocks();
  }
  */

  library.load_blob("test.h", src);
  library.cross_reference();
}

//------------------------------------------------------------------------------

std::string translate_simple(std::string src) {
  std::string out;

  MtModLibrary library;
  parse_simple(src, library);

  auto source_file = library.source_files[0];

  for (auto& mod : source_file->modules) {
    if (mod->dirty_check_fail) return "DCF";
  }

  MtCursor cursor(&library, source_file, &out);
  cursor.cursor = source_file->source;
  cursor.source_file = source_file;
  cursor.emit(source_file->mt_root);
  cursor.emit("\n");

  for (auto c : out) assert(c > 0);

  return out;
}

//------------------------------------------------------------------------------

static TestResults test_dummy() {
  TEST_INIT("Make sure our test framework is working.");
  LOG_INDENT_SCOPE();
  EXPECT(1 != 2, "One should not equal two.");
  TEST_DONE();
}

//------------------------------------------------------------------------------

static TestResults test_comp() {
  TEST_INIT("Text comparison should treat all whitespace as \"\\w+\"");

  EXPECT(comp_iws("foo bar", "foo\nbar"));
  EXPECT(comp_iws("foo bar", "foo\tbar"));
  EXPECT(comp_iws("foo\nbar", "foo\tbar"));
  EXPECT(comp_iws("foo\nbar", "foo bar"));
  EXPECT(comp_iws("foo\tbar", "foo\nbar"));
  EXPECT(comp_iws("foo\tbar", "foo bar"));

  EXPECT(!comp_iws("foo bar", "foobar"));

  TEST_DONE();
}

//------------------------------------------------------------------------------

static TestResults test_match() {
  TEST_INIT("Substring find should treat all whitespace as \"\\w+\"");
  LOG_INDENT_SCOPE();

  EXPECT(+find_iws("foobarbaz", "bar"));
  EXPECT(!find_iws("foobarbaz", " bar"));
  EXPECT(!find_iws("foobarbaz", "bar "));
  EXPECT(!find_iws("foobarbaz", " bar "));

  EXPECT(+find_iws("foo barbaz", "bar"));
  EXPECT(+find_iws("foo barbaz", " bar"));
  EXPECT(!find_iws("foo barbaz", "bar "));
  EXPECT(!find_iws("foo barbaz", " bar "));

  EXPECT(+find_iws("foobar baz", "bar"));
  EXPECT(!find_iws("foobar baz", " bar"));
  EXPECT(+find_iws("foobar baz", "bar "));
  EXPECT(!find_iws("foobar baz", " bar "));

  EXPECT(+find_iws("foo bar baz", "bar"));
  EXPECT(+find_iws("foo bar baz", " bar"));
  EXPECT(+find_iws("foo bar baz", "bar "));
  EXPECT(+find_iws("foo bar baz", " bar "));

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_utils() {
  TEST_INIT();
  results += test_dummy();
  results += test_comp();
  results += test_match();
  TEST_DONE();
}

//------------------------------------------------------------------------------
