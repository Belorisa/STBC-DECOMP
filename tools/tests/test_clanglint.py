import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import clanglint as C  # noqa: E402

IOSFWD = """template<class _E, class _Tr = char_traits<_E> >
	class basic_ios;
template<class _E> class plain;
"""


class PatchHeader(unittest.TestCase):
    def setUp(self):
        self.fwd = C.forward_declared(IOSFWD)

    def test_forward_declared_defaults(self):
        self.assertEqual(self.fwd, {"basic_ios"})

    def test_a_repeated_default_is_dropped(self):
        text = "template<class _E, class _Tr = char_traits<_E> >\n\tclass basic_ios : public ios_base {\n"
        out, changed = C.patch_header("ios", text, self.fwd)
        self.assertTrue(changed)
        self.assertIn("template<class _E, class _Tr>", out.replace(" >", ">"))
        self.assertNotIn("= char_traits", out)

    def test_iosfwd_itself_and_other_templates_keep_their_defaults(self):
        self.assertFalse(C.patch_header("iosfwd", IOSFWD, self.fwd)[1])
        text = "template<class _Ty, class _A = allocator<_Ty> >\n\tclass vector {\n"
        self.assertFalse(C.patch_header("vector", text, self.fwd)[1])

    def test_an_explicit_specialisation_gets_its_prefix(self):
        text = "class _CRTIMP ctype<char> : public ctype_base {\n"
        out, changed = C.patch_header("xlocale", text, self.fwd)
        self.assertTrue(changed)
        self.assertEqual(out, "template<> class _CRTIMP ctype<char> : public ctype_base {\n")

    def test_a_prefixed_specialisation_is_left_alone(self):
        text = "template<>\nclass ctype<char> : public ctype_base {\n"
        self.assertFalse(C.patch_header("xlocale", text, self.fwd)[1])

    def test_stream_flags_are_qualified(self):
        out, _ = C.patch_header("ostream", "{if (flags() & unitbuf)\n", self.fwd)
        self.assertIn("flags() & ios_base::unitbuf", out)


class ForScope(unittest.TestCase):
    def test_a_for_init_name_used_after_its_loop(self):
        texts = {"src/x.cpp": ["void f() {", "\tfor ( int i = 0; i < n; i++ ) {", "\t}", "\tfor ( i = 0; ; ) {"]}
        self.assertTrue(C.for_scope("src/x.cpp", 4, "use of undeclared identifier 'i'", texts))
        self.assertFalse(C.for_scope("src/x.cpp", 4, "use of undeclared identifier 'j'", texts))
        self.assertFalse(C.for_scope("src/x.cpp", 4, "unknown type name 'i'", texts))


if __name__ == "__main__":
    unittest.main()
